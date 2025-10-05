#include <stdlib.h>
#include <math.h>
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_implementation.h"
#include "ei_utils.h"


/**
 * @brief Teste la fonction arc.
 */
void test_arc(ei_surface_t surface, ei_rect_t* clipper)
{
    ei_color_t color = { 255, 0, 0, 255 }; // Rouge
    ei_point_t center = { 200, 200 };
    float radius = 50.0f;
    float start_angle = 45.0f;
    float end_angle = 315.0f;
    int num_points = 20;

    ei_point_t* arc_points = arc(center, radius, start_angle, end_angle, num_points);
    if (arc_points) {
        ei_draw_polyline(surface, arc_points, num_points, color, clipper);
        free(arc_points);
    }
}

/**
 * @brief Teste la fonction rounded_frame.
 */
void test_rounded_frame(ei_surface_t surface, ei_rect_t* clipper)
{
    ei_color_t color = { 0, 255, 0, 255 }; // Vert
    ei_rect_t rect = { { 350, 150 }, { 200, 100 } };
    float radius = 20.0f;

    // Cadre complet
    ei_point_t* frame = rounded_frame(rect, radius, 0);
    if (frame) {
        ei_draw_polygon(surface, frame, 4 * 10 + 4, color, clipper);
        free(frame);
    }

    // Moitié haute
    ei_color_t color_top = { 0, 255, 255, 255 }; // Cyan
    ei_point_t* top_half = rounded_frame(rect, radius, 1);
    if (top_half) {
        ei_draw_polygon(surface, top_half, 2 * 10 + 2, color_top, clipper);
        free(top_half);
    }

    // Moitié basse
    ei_color_t color_bottom = { 255, 255, 0, 255 }; // Jaune
    ei_point_t* bottom_half = rounded_frame(rect, radius, 2);
    if (bottom_half) {
        ei_draw_polygon(surface, bottom_half, 2 * 10 + 2, color_bottom, clipper);
        free(bottom_half);
    }
}

/**
 * @brief Teste la fonction draw_button.
 */
void test_button(ei_surface_t surface, ei_rect_t* clipper)
{
    ei_color_t color = { 100, 100, 100, 255 }; // Gris
    ei_rect_t rect_raised = { { 150, 350 }, { 150, 100 } };
    ei_rect_t rect_sunken = { { 350, 350 }, { 150, 100 } };
    float radius = 20.0f;

    // Bouton relevé
    draw_button(surface, rect_raised, radius, color, ei_relief_raised, clipper);

    // Bouton enfoncé
    draw_button(surface, rect_sunken, radius, color, ei_relief_sunken, clipper);
}

/**
 * @brief Fonction principale pour tester les dessins.
 */
int main(int argc, char** argv)
{
    ei_size_t win_size = ei_size(800, 600);
    ei_surface_t main_window = NULL;
    ei_color_t white = { 0xff, 0xff, 0xff, 0xff };
    ei_rect_t* clipper_ptr = NULL;
    ei_event_t event;

    // Initialiser l'interface matérielle
    hw_init();

    // Créer la fenêtre
    main_window = hw_create_window(win_size, false);

    // Verrouiller et remplir la surface en blanc
    hw_surface_lock(main_window);
    ei_fill(main_window, &white, clipper_ptr);

    // Tester les fonctions
    test_arc(main_window, clipper_ptr);
    test_rounded_frame(main_window, clipper_ptr);
    test_button(main_window, clipper_ptr);

    // Déverrouiller et mettre à jour la surface
    hw_surface_unlock(main_window);
    hw_surface_update_rects(main_window, NULL);

    // Attendre un événement de fermeture ou une touche
    event.type = ei_ev_none;
    while ((event.type != ei_ev_close) && (event.type != ei_ev_keydown)) {
        hw_event_wait_next(&event);
    }

    // Libérer les ressources
    hw_surface_free(main_window);
    hw_quit();
    return (EXIT_SUCCESS);
}