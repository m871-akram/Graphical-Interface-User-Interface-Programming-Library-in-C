// tests/mouse_tracker.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Pour sprintf

#include "ei_application.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_widget_attributes.h"
#include "ei_widget_configure.h"
#include "ei_placer.h"
#include "ei_types.h"
#include "ei_draw.h"       // Pour ei_draw_text
#include "hw_interface.h"  // Pour ei_default_font

// Variables globales pour stocker les informations à afficher
static ei_point_t g_mouse_position = {0, 0};
static char g_widget_class_name[50] = "None";
static ei_color_t g_picked_widget_color = {0, 0, 0, 0xff};
static uint32_t g_picked_widget_id = 0;
static ei_widget_t g_info_frame = NULL;


// Fonction pour mettre à jour les informations affichées
void update_info_display() {
    if (g_info_frame == NULL) return;

    char display_text[200];
    sprintf(display_text, "Mouse: (%d, %d) . Widget: %s . Pick ID: #%02x%02x%02x (%u)",
            g_mouse_position.x, g_mouse_position.y,
            g_widget_class_name,
            g_picked_widget_color.red, g_picked_widget_color.green, g_picked_widget_color.blue,
            g_picked_widget_id);

    // Utiliser ei_frame_set_text pour mettre à jour le texte du widget d'information
    // Note: ei_frame_set_text est un raccourci pour ei_frame_configure.
    // Si vous n'avez pas ei_frame_set_text, utilisez ei_frame_configure directement.
    ei_string_t text_ptr = display_text;
    ei_frame_configure(g_info_frame, NULL, NULL, NULL, NULL, &text_ptr, NULL, NULL, NULL, NULL, NULL, NULL);

    // Forcer un redessin du widget d'info (ou de toute l'application)
    // ei_app_invalidate_rect(&(g_info_frame->screen_location)); // Plus précis
}

// Gestionnaire d'événements par défaut
void mouse_tracker_default_handler(ei_event_t* event) {
    if (event->type == ei_ev_close ||
        (event->type == ei_ev_keydown && event->param.key_code == SDLK_ESCAPE)) {
        ei_app_quit_request();
    } else if (event->type == ei_ev_mouse_move) {
        g_mouse_position = event->param.mouse.where;

        ei_widget_t picked_widget = ei_widget_pick(&g_mouse_position);
        if (picked_widget != NULL) {
            ei_widgetclass_t* wclass = ei_widget_get_class(picked_widget);
            if (wclass != NULL) {
                strncpy(g_widget_class_name, wclass->name, sizeof(g_widget_class_name) - 1);
                g_widget_class_name[sizeof(g_widget_class_name) - 1] = '\0';
            } else {
                strcpy(g_widget_class_name, "Unknown Class");
            }
            const ei_color_t* p_color = ei_widget_get_pick_color(picked_widget);
            if (p_color) {
                g_picked_widget_color = *p_color;
            }
            // Pour obtenir l'ID exact, il faudrait accéder à widget->pick_id
            // Ceci suppose que ei_impl_widget_t est accessible ou qu'il y a un getter pour pick_id
            // Pour l'instant, on affiche la pick_color.
            // Si vous avez accès à la structure interne :
            // g_picked_widget_id = ((ei_impl_widget_t*)picked_widget)->pick_id;
            // Sinon, reconstruire à partir de la couleur (approximatif si l'alpha n'est pas toujours FF)
            g_picked_widget_id = ((uint32_t)g_picked_widget_color.red << 16) |
                                 ((uint32_t)g_picked_widget_color.green << 8) |
                                 (uint32_t)g_picked_widget_color.blue;


        } else {
            strcpy(g_widget_class_name, "NULL (Outside Root?)");
            g_picked_widget_color = (ei_color_t){0,0,0,0xff};
            g_picked_widget_id = 0;
        }
        update_info_display(); // Mettre à jour l'affichage du texte
    } else if (event->type == ei_ev_mouse_buttondown) {
        // Optionnel: afficher quel bouton a été cliqué
        printf("Mouse button down: %d at (%d, %d) on widget %s\n",
               event->param.mouse.button,
               event->param.mouse.where.x, event->param.mouse.where.y,
               g_widget_class_name);
    }
}


int main(int argc, char** argv) {
    ei_size_t screen_size = {800, 600};
    ei_color_t root_bg_color = {0x52, 0x7f, 0xb4, 0xff};
    ei_color_t frame_color = {0xcc, 0xcc, 0xcc, 0xff}; // Gris clair pour le cadre d'info
    ei_color_t text_color = {0x00, 0x00, 0x00, 0xff};  // Texte noir
    int border_width = 2;

    ei_app_create(screen_size, false);
    ei_frame_set_bg_color(ei_app_root_widget(), root_bg_color);

    // Créer un widget frame pour afficher les informations
    g_info_frame = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);

    ei_size_t info_frame_size = {250, 80}; // Ajustez la taille au besoin
    ei_anchor_t text_anchor = ei_anc_northwest; // Ancrer le texte en haut à gauche du frame

    ei_frame_configure(g_info_frame, &info_frame_size, &frame_color,
                         &border_width, &(ei_relief_t){ei_relief_sunken},
                         NULL, // Le texte sera mis à jour dynamiquement
                         NULL, // Utiliser la police par défaut (ei_default_font)
                         &text_color,
                         &text_anchor, // Ancrage du texte
                         NULL, NULL, NULL);
    ei_place_xy(g_info_frame, 10, 10);


    // Créer quelques autres widgets pour tester le picking
    ei_widget_t button1 = ei_widget_create("button", ei_app_root_widget(), NULL, NULL);
    ei_size_t button_size = {100, 50};
    ei_color_t button_color = {0x88, 0x88, 0x88, 0xff};
    int button_bw = 2;
    int button_cr = 5;
    ei_string_t button_text = "Button A";
    ei_button_configure(button1, &button_size, &button_color, &button_bw, &button_cr,
                        &(ei_relief_t){ei_relief_raised}, &button_text, NULL, NULL, NULL,
                        NULL, NULL, NULL, NULL, NULL);
    ei_place_xy(button1, 50, 150);

    ei_widget_t frame2 = ei_widget_create("frame", ei_app_root_widget(), NULL, NULL);
    ei_size_t frame2_size = {150, 100};
    ei_color_t frame2_color = {0xA0, 0xA0, 0xA0, 0xff};
    ei_frame_configure(frame2, &frame2_size, &frame2_color, &border_width, &(ei_relief_t){ei_relief_raised},
                         NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_place_xy(frame2, 200, 200);


    // Initialiser l'affichage des infos
    update_info_display();

    ei_event_set_default_handle_func(mouse_tracker_default_handler);
    ei_app_run();
    ei_app_free();

    return EXIT_SUCCESS;
}