#include "ei_utils.h"
#include "ei_widget.h"
#include "ei_implementation.h"
#include "ei_widgetclass.h"
#include "ei_application.h"
#include "ei_placer.h"
#include "hw_interface.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "ei_widget_attributes.h"



ei_surface_t pick_surface;


ei_widget_t ei_widget_create(ei_const_string_t class_name, ei_widget_t parent, ei_user_param_t user_data, ei_widget_destructor_t destructor)  {
    assert(class_name != NULL );

    ei_widgetclass_t* wclass = ei_widgetclass_from_name(class_name);
    if (wclass == NULL) return NULL;

    // Allouer la mémoire pour le widget
    ei_widget_t widget = wclass->allocfunc();
    if (widget == NULL) {
        return NULL; // Échec de l'allocation
    }

    // Initialiser les champs de la structure
    ei_impl_widget_t* impl_widget = (ei_impl_widget_t*)widget;
    impl_widget->wclass = wclass;
    impl_widget->pick_id = (uint32_t)(intptr_t)widget; // Utiliser l'adresse comme ID unique
    impl_widget->pick_color.red   = (uint8_t)((impl_widget->pick_id & 0x00FF0000) >> 16);
    impl_widget->pick_color.green = (uint8_t)((impl_widget->pick_id & 0x0000FF00) >> 8);
    impl_widget->pick_color.blue  = (uint8_t)((impl_widget->pick_id & 0x000000FF));
    impl_widget->pick_color.alpha = 0xff; // Opaque pour le picking
    impl_widget->user_data = user_data;
    impl_widget->destructor = destructor;
    impl_widget->parent = parent;
    impl_widget->children_head = NULL;
    impl_widget->children_tail = NULL;
    impl_widget->next_sibling = NULL;
    impl_widget->placer_params = NULL;
    impl_widget->requested_size = ei_size_zero();
    impl_widget->screen_location = ei_rect_zero();
    impl_widget->content_rect = &impl_widget->screen_location; // Par défaut, pointe vers screen_location


    // Ajouter le widget comme dernier enfant du parent
    if (parent != NULL) { // Ajouter cette condition
        ei_impl_widget_t* parent_impl = (ei_impl_widget_t*)parent; // Déplacer la déclaration ici
        if (parent_impl->children_head == NULL) {
            parent_impl->children_head = widget;
            parent_impl->children_tail = widget;
        } else {
            ((ei_impl_widget_t*)parent_impl->children_tail)->next_sibling = widget;
            parent_impl->children_tail = widget;
        }

        ei_app_invalidate_rect(ei_widget_get_screen_location(parent)); // commenter si nécessaire après d'autres corrections
    }

    // Définir les valeurs par défaut de la classe
    wclass->setdefaultsfunc(widget);


    if (parent != NULL) {
        ei_app_invalidate_rect(ei_widget_get_screen_location(parent)); // Peut-être mieux ici ou après ei_place
    }


        // ...
        impl_widget->content_rect = &impl_widget->screen_location; // Par défaut
        // ...
        wclass->setdefaultsfunc(widget); // Si setdefaults (comme pour toplevel) alloue son propre content_rect, il écrasera ce pointeur.
        // ...
        return widget;
    }

void ei_widget_destroy(ei_widget_t widget) {
    assert(widget != NULL && "hmm , un widget NULL !! ");

    ei_impl_widget_t* impl_widget = widget;

    // Invalider la zone du widget avant destruction
    ei_app_invalidate_rect(&impl_widget->screen_location);

    // Retirer le widget de l'écran si affiché
    if (impl_widget->placer_params != NULL) {
        ei_placer_forget(widget);
    }

    // Détruire récursivement tous les enfants
    ei_widget_t enfant = impl_widget->children_head;
    while (enfant != NULL) {
        ei_widget_t next_child = ((ei_impl_widget_t*)enfant)->next_sibling;
        ei_widget_destroy(enfant);
        enfant = next_child;
    }

    // Retirer le widget de la liste des enfants de son parent
    if (impl_widget->parent != NULL) {
        ei_impl_widget_t* parent_impl = (ei_impl_widget_t*)impl_widget->parent;
        ei_widget_t prev_sibling = NULL;
        ei_widget_t current = parent_impl->children_head;

        while (current != NULL && current != widget) {
            prev_sibling = current;
            current = ((ei_impl_widget_t*)current)->next_sibling;
        }

        if (current == widget) {
            if (prev_sibling == NULL) {
                parent_impl->children_head = impl_widget->next_sibling;
            } else {
                ((ei_impl_widget_t*)prev_sibling)->next_sibling = impl_widget->next_sibling;
            }
            if (parent_impl->children_tail == widget) {
                parent_impl->children_tail = prev_sibling;
            }
        }

        // Invalider la zone du parent pour refléter la suppression
        ei_app_invalidate_rect(ei_widget_get_screen_location(impl_widget->parent));
    }

    // Appeler le destructeur utilisateur, si défini
    if (impl_widget->destructor != NULL) {
        impl_widget->destructor(widget);
    }

    // Appeler la fonction de libération de la classe
    if (impl_widget->wclass->releasefunc != NULL) {
        impl_widget->wclass->releasefunc(widget);
    }

    // Libérer la mémoire allouée pour content_rect si ce n'est pas screen_location ( je pense qu on peu supp cette section )
    if (impl_widget->content_rect != &impl_widget->screen_location) {
        free(impl_widget->content_rect);
    }

    // Libérer la mémoire du widget
    free(widget);
}

bool ei_widget_is_displayed(ei_widget_t widget) {
    assert(widget != NULL && "hmm , un widget NULL !! ");
    return ((ei_impl_widget_t*)widget)->placer_params != NULL;
}

ei_widget_t ei_widget_pick(ei_point_t* where) {
    assert(where != NULL && "Location cannot be NULL");

    // Obtenir la surface de picking
    if (pick_surface == NULL) {
        return NULL; // Application non initialisée
    }

    // Vérifier que le point est dans les limites de la surface
    ei_rect_t surface_rect = hw_surface_get_rect(pick_surface);
    if (where->x < surface_rect.top_left.x || where->x >= surface_rect.top_left.x + surface_rect.size.width ||
        where->y < surface_rect.top_left.y || where->y >= surface_rect.top_left.y + surface_rect.size.height) {
        return NULL;
    }

    // Verrouiller la surface
    hw_surface_lock(pick_surface);

    // Obtenir le buffer de pixels
    uint8_t* buffer = hw_surface_get_buffer(pick_surface);
    ei_size_t surface_size = hw_surface_get_size(pick_surface);

    // Obtenir les indices des canaux
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(pick_surface, &ir, &ig, &ib, &ia);

    // Calculer l'offset du pixel à la position (where->x, where->y)
    int bytes_per_pixel = 4; // Supposé 32 bits par pixel (RGBA)
    size_t pixel_offset = (where->y * surface_size.width + where->x) * bytes_per_pixel;

    // Lire les composantes du pixel
    ei_color_t pixel_color;
    pixel_color.red = buffer[pixel_offset + ir];
    pixel_color.green = buffer[pixel_offset + ig];
    pixel_color.blue = buffer[pixel_offset + ib];
    pixel_color.alpha = (ia >= 0) ? buffer[pixel_offset + ia] : 255; // Opaque si pas de canal alpha

    // Déverrouiller la surface
    hw_surface_unlock(pick_surface);

    // Si le pixel est transparent ou noir, aucun widget n'est présent
    if (pixel_color.red == 0 && pixel_color.green == 0 && pixel_color.blue == 0 && pixel_color.alpha == 0) {
        return NULL;
    }

    // Convertir la couleur en pick_id
    uint32_t pick_id = (pixel_color.red << 24) | (pixel_color.green << 16) | (pixel_color.blue << 8) | pixel_color.alpha;
    ei_widget_t widget = (ei_widget_t)(intptr_t)pick_id;

    // Vérifier que le widget existe et que sa pick_color correspond
    ei_impl_widget_t* impl_widget = (ei_impl_widget_t*)widget;
    if (impl_widget != NULL && memcmp(&impl_widget->pick_color, &pixel_color, sizeof(ei_color_t)) == 0) {
        return widget;
    }


    return NULL;
}