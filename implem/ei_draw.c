#include "ei_draw.h"
#include "hw_interface.h"
#include "ei_implementation.h"

// Cette fonction remplit une zone avec une couleur (comme si on peignait un mur !)
void ei_fill(ei_surface_t surface, const ei_color_t* couleur, const ei_rect_t* clipper)
{
    // On récupère le buffer (l'endroit où on dessine) et la taille de la surface
    uint8_t* pixel_0 = hw_surface_get_buffer(surface);
    ei_size_t taille_surface = hw_surface_get_size(surface);

    // On transforme la couleur en un format que l'écran comprend (Mac/Windows ou Linux, c'est pas pareil)
    #if defined(__APPLE__) || defined(_WIN32)
        uint32_t valeur_pixel = ei_impl_map_rgba(surface, *couleur);
    #else
        uint32_t valeur_pixel = *((uint32_t*)couleur);
    #endif

    // On définit la zone qu’on va remplir (le clipper, c’est comme une fenêtre où on a le droit de peindre)
    int clip_xmin = 0, clip_ymin = 0, clip_xmax = taille_surface.width - 1, clip_ymax = taille_surface.height - 1;
    if (clipper != NULL) {
        clip_xmin = clipper->top_left.x;
        clip_ymin = clipper->top_left.y;
        clip_xmax = clip_xmin + clipper->size.width - 1;
        clip_ymax = clip_ymin + clipper->size.height - 1;

        // On remplit juste la zone du clipper, ligne par ligne
        for (int y = clip_ymin; y <= clip_ymax; y++) {
            // On trouve le début de la ligne
            uint8_t* ptr_ligne = pixel_0 + (y * taille_surface.width * 4) + (clip_xmin * 4);
            uint32_t* ptr_pixel = (uint32_t*)ptr_ligne;
            // On colore chaque pixel de la ligne
            for (int x = clip_xmin; x <= clip_xmax; x++) {
                *ptr_pixel++ = valeur_pixel;
            }
        }
    } else {
        // Pas de clipper ? On remplit TOUT l’écran !
        uint32_t nombre_pixels = taille_surface.width * taille_surface.height;
        uint32_t* ptr_pixel = (uint32_t*)pixel_0;
        // On boucle pour colorer chaque pixel
        for (uint32_t i = 0; i < nombre_pixels; i++) {
            *ptr_pixel++ = valeur_pixel;
        }
    }
}

// Dessine une polyligne (une série de lignes connectées, comme un chemin)
void ei_draw_polyline(ei_surface_t surface, ei_point_t* points, size_t taille_points,
                      ei_color_t couleur, const ei_rect_t* clipper)
{
    // S’il n’y a pas de points, on fait rien
    if (taille_points == 0) return;

    // Juste un point ? On dessine un point (une ligne qui va nulle part)
    if (taille_points == 1) {
        draw_line(surface, points[0], points[0], couleur, clipper);
    } else {
        // Plusieurs points ? On dessine une ligne entre chaque paire
        for (size_t i = 0; i < taille_points - 1; i++) {
            draw_line(surface, points[i], points[i + 1], couleur, clipper);
        }
    }
}

// Dessine un polygone rempli (genre un octogone tout coloré !)
void ei_draw_polygon(ei_surface_t surface, ei_point_t* points, size_t taille_points,
                     ei_color_t couleur, const ei_rect_t* clipper)
{
    // S’il n’y a pas assez de points ou c’est vide, on dégage
    if (points == NULL || taille_points < 3) return;

    // On récupère la taille de l’écran
    ei_size_t taille_surface = hw_surface_get_size(surface);

    // On cherche les y minimum et maximum du polygone
    int y_min = points[0].y;
    int y_max = points[0].y;
    for (size_t i = 1; i < taille_points; i++) {
        if (points[i].y < y_min) y_min = points[i].y;
        if (points[i].y > y_max) y_max = points[i].y;
    }

    // On ajuste les limites pour pas dessiner n’importe où
    int clip_ymin = y_min, clip_ymax = y_max;
    if (clipper != NULL) {
        // On prend le max/min entre le clipper et le polygone
        clip_ymin = (clip_ymin > clipper->top_left.y) ? clip_ymin : clipper->top_left.y;
        clip_ymax = (clip_ymax < (clipper->top_left.y + clipper->size.height)) ? clip_ymax : (clipper->top_left.y + clipper->size.height);
    }
    clip_ymin = (clip_ymin > 0) ? clip_ymin : 0;
    clip_ymax = (clip_ymax < taille_surface.height) ? clip_ymax : (taille_surface.height);

    // Si la zone est vide, on sort
    if (clip_ymin > clip_ymax) return;

    // On crée une table pour stocker les côtés (avec calloc, merci <stdlib.h> !)
    edge_t** table_cotes = (edge_t**)calloc(clip_ymax - clip_ymin, sizeof(edge_t*));
    // On remplit la table avec les arêtes du polygone
    creer_table_tc(points, taille_points, table_cotes, clip_ymin, clip_ymax);

    // On commence avec une liste vide pour les arêtes actives (TCA)
    edge_t* tca = NULL;

    // On boucle sur chaque ligne (scanline) de y_min à y_max
    for (int y = clip_ymin; y <= clip_ymax; y++) {
        // On vire les arêtes qui se terminent à cette ligne
        supprimer_arete_tca(y, &tca);

        // On ajoute les nouvelles arêtes qui commencent à cette ligne
        if ((y - clip_ymin) >= 0 && (y - clip_ymin) < clip_ymax - clip_ymin + 1) {
            edge_t* cote = table_cotes[y - clip_ymin];
            while (cote != NULL) {
                edge_t* next = cote->next;
                ajoute_arete_tca(cote, &tca); // On met l’arête dans la TCA
                cote = next;
            }
            table_cotes[y - clip_ymin] = NULL; // On vide cette case
        }

        // On trie la TCA pour avoir les arêtes dans l’ordre des x
        trier_tca_x(&tca);

        // On remplit les bouts entre les arêtes (ça fait le polygone !)
        edge_t* courant = tca;
        while (courant != NULL && courant->next != NULL) {
            // On trouve les x de début et de fin pour cette ligne
            int x_debut = (int)ceilf(courant->x);
            int x_fin = (int)floorf(courant->next->x);

            // S’il y a quelque chose à dessiner, on trace une ligne horizontale
            if (x_debut <= x_fin) {
                draw_horizontal_line(surface, x_debut, x_fin, y, couleur, clipper);
            }
            courant = courant->next->next; // On passe au prochain couple
        }

        // On met à jour les x pour la ligne suivante
        courant = tca;
        while (courant != NULL) {
            courant->x += courant->inv_m;
            courant = courant->next;
        }
    }

    // On nettoie la TCA (on libère la mémoire avec free, merci <stdlib.h> !)
    while (tca != NULL) {
        edge_t* u = tca;
        tca = tca->next;
        free(u);
    }
    // On nettoie aussi la table des côtés
    for (int i = 0; i < clip_ymax - clip_ymin + 1; i++) {
        edge_t* cote = table_cotes[i];
        while (cote != NULL) {
            edge_t* u = cote;
            cote = cote->next;
            free(u);
        }
    }
    // On libère la table elle-même
    free(table_cotes);
}

