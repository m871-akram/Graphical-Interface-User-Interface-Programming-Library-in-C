#include <stdio.h>
#include "ei_draw.h"
#include "ei_types.h"
#include "hw_interface.h"

int main() {
    hw_init();

    ei_size_t window_size = {320, 240};
    ei_surface_t root = hw_create_window(window_size, false);

    ei_size_t surface_size = {320, 240};
    ei_surface_t surface = hw_surface_create(root, surface_size, true);


    ei_color_t color = {0xff, 0xff, 0xff, 0xff};
    ei_point_t pts[3] = {{2, 5}, {100, 200}, {0, 102}}; // ei_point_t		pts[2]		= { {200, 200}, {600, 400} };
    size_t nb_points = sizeof(pts) / sizeof(ei_point_t);



    int i, nb_loop = 1000;
    double start = hw_now();
    for (i = 0; i < nb_loop; i++) {
        hw_surface_lock(surface);
        ei_draw_polygon(surface, pts, nb_points, color, NULL);
        hw_surface_unlock(surface);
    }
    double end = hw_now();

    double average_time = (end - start) / (double)nb_loop;
    printf("Temps d'exécution moyen de ei_draw_polyline : %.9f secondes\n", average_time);

    hw_surface_free(surface);
    hw_surface_free(root);
    hw_quit();
    return 0;
}