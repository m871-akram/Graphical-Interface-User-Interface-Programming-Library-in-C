#include <stdlib.h>
#include "hw_interface.h"
#include "ei_utils.h"
#include "ei_draw.h"    // Inclus pour ei_fill
#include "ei_types.h"
#include "ei_event.h"
#include "ei_relief.h"  // Pour la déclaration de draw_button

void test_button(ei_surface_t surface) {
    ei_size_t win_size = ei_size(800, 600);
    ei_size_t button_size = ei_size(300, 150);
    float rayon = 20.0f;
    ei_color_t couleur = {100, 100, 100, 255}; // Gris, ça fait sérieux mais pas trop

    // Ajout des paramètres manquants pour l'appel à draw_button
    int border_width = 8;             // Par exemple, une bordure de 2 pixels
    const ei_rect_t* clipper = NULL;  // Pas de clipping spécifique pour ce test simple

    // Configurations des boutons
    typedef struct {
        ei_point_t top_left; // Où on place le bouton
        ei_relief_t etat; // Relevé ou enfoncé, pour l'effet 3D
    } config_button; // Une petite structure pour organiser tout ça

    config_button buttons[] = {
        {
            .top_left = {
                (win_size.width - button_size.width) / 2, // Centré en largeur
                (win_size.height / 2 - button_size.height) / 2 // En haut, bien placé
            },
            .etat = ei_relief_raised,
        },
        {
            .top_left = {
                (win_size.width - button_size.width) / 2, // Toujours centré
                win_size.height / 2 + (win_size.height / 2 - button_size.height) / 2 // En bas cette fois
            },
            .etat = ei_relief_sunken, // Bouton enfoncé, comme si t'avais cliqué
        }
    };

    for (int i = 0; i < 2; i++) { // On boucle pour les deux boutons
        ei_rect_t rect = {buttons[i].top_left, button_size};
        // Appel corrigé de draw_button avec tous les arguments
        draw_button(surface, &rect, rayon, couleur, border_width, buttons[i].etat, clipper);
    }
}


int main(int argc, char** argv) {
    ei_size_t win_size = ei_size(800, 600);
    ei_surface_t main_window = NULL;
    ei_color_t white = {0xff, 0xff, 0xff, 0xff};
    ei_event_t event; // Pour capter les clics ou touches

    hw_init();
    main_window = hw_create_window(win_size, false);

    hw_surface_lock(main_window);
    ei_fill(main_window, &white, NULL); // Tout en blanc, comme une toile neuve

    test_button(main_window); // On balance les boutons, c'est parti !

    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL); // On met à jour l'écran

    event.type = ei_ev_none; // On attend un événement, genre un clic
    while (event.type != ei_ev_close &&
           !((event.type == ei_ev_keydown) && (event.param.key_code == SDLK_ESCAPE))) { // Condition de sortie améliorée
        hw_event_wait_next(&event); // On attend gentiment le prochain événement
    }

    hw_quit(); // On ferme tout, ciao !
    return EXIT_SUCCESS;
}