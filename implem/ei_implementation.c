#include "ei_implementation.h"
#include "hw_interface.h"
#include <stdlib.h>
#include <math.h>
#include "ei_draw.h"
#include <stdio.h>

// Dessine une ligne entre deux points avec l'algo de Bresenham (ça fait des lignes bien droites !)
void draw_line(ei_surface_t surface, ei_point_t point_1, ei_point_t point_2, ei_color_t couleur, const ei_rect_t* clipper)
{
    // On récupère le buffer (l'endroit où on dessine) et la taille de la surface
    uint8_t* pixel_0 = hw_surface_get_buffer(surface);
    ei_size_t taille_surface = hw_surface_get_size(surface);

    // On convertit la couleur en un format que la surface comprend (ça dépend si t'es sur Mac, Windows ou Linux)
    #if defined(__APPLE__) || defined(_WIN32)
        uint32_t valeur_pixel = ei_impl_map_rgba(surface, couleur);
    #else
        uint32_t valeur_pixel = *((uint32_t*)&couleur);
    #endif

    // On définit une zone où on a le droit de dessiner (le "clipper")
    int clip_xmin = 0, clip_ymin = 0, clip_xmax = taille_surface.width - 1, clip_ymax = taille_surface.height - 1;
    if (clipper != NULL) {
        clip_xmin = clipper->top_left.x;
        clip_ymin = clipper->top_left.y;
        clip_xmax = clip_xmin + clipper->size.width - 1;
        clip_ymax = clip_ymin + clipper->size.height - 1;
    }

    // On récupère les coordonnées des deux points
    int x1 = point_1.x, y1 = point_1.y;
    int x2 = point_2.x, y2 = point_2.y;

    // On calcule les différences entre les points (pour savoir de combien on bouge)
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x2 >= x1) ? 1 : -1; // Direction : +1 si on va à droite, -1 à gauche
    int sy = (y2 >= y1) ? 1 : -1; // Direction : +1 si on va en bas, -1 en haut

    // On vérifie si la ligne est plus large que haute
    bool dirige_par_x = dx >= dy;

    // On commence au premier point
    int x = x1, y = y1;

    // On initialise une erreur pour l'algo de Bresenham (ça aide à choisir les bons pixels)
    int E = dirige_par_x ? dy : dx;

    // On calcule où est le premier pixel dans le buffer
    uint8_t* pixel_ptr = pixel_0 + (y1 * taille_surface.width +x1 )* 4;
    int ligne_increment = sy * taille_surface.width * 4; // Pour sauter à la ligne suivante
    int pixel_increment = sx * 4; // Pour bouger d'un pixel

    // Boucle pour dessiner la ligne pixel par pixel
    while (true) {
        // On dessine seulement si le pixel est dans la zone autorisée
        if (x >= 0 && x < taille_surface.width && y >= 0 && y < taille_surface.height &&
                    x >= clip_xmin && x <= clip_xmax && y >= clip_ymin && y <= clip_ymax) {
            *(uint32_t*)pixel_ptr = valeur_pixel;
                    }

        // Si on est arrivé au point final, on arrête
        if (x == x2 && y == y2) {
            break;
        }

        // On bouge le pixel suivant selon l'axe dominant
        if (dirige_par_x) {
            // Si la ligne est plus horizontale
            x += sx;
            pixel_ptr += pixel_increment;
            E += dy;
            if (2 * E > dx) {
                y += sy;
                pixel_ptr += ligne_increment;
                E -= dx;
            }
        } else {
            // Si la ligne est plus verticale
            y += sy;
            pixel_ptr += ligne_increment;
            E += dx;
            if (2 * E > dy) {
                x += sx;
                pixel_ptr += pixel_increment;
                E -= dy;
            }
        }
    }
}

// Dessine une ligne droite horizontale (super simple !)
void draw_horizontal_line(ei_surface_t surface, int x1, int x2, int y, ei_color_t couleur, const ei_rect_t* clipper)
{
    // Si x1 est plus grand que x2, on les échange pour dessiner dans le bon sens
    if (x1 > x2) {
        int a = x1;
        x1 = x2;
        x2 = a;
    }

    // On récupère le buffer et la taille de la surface
    uint8_t* pixel_0 = hw_surface_get_buffer(surface);
    ei_size_t taille_surface = hw_surface_get_size(surface);

    // On convertit la couleur (pareil, ça dépend de la plateforme)
    #if defined(__APPLE__) || defined(_WIN32)
        uint32_t valeur_pixel = ei_impl_map_rgba(surface, couleur);
    #else
        uint32_t valeur_pixel = *((uint32_t*)&couleur);
    #endif

    // On définit la zone où on peut dessiner
    int clip_xmin = 0, clip_xmax = taille_surface.width - 1, clip_ymin = 0, clip_ymax = taille_surface.height - 1;
    if (clipper) {
        clip_xmin = clipper->top_left.x;
        clip_xmax = clipper->top_left.x + clipper->size.width - 1;
        clip_ymin = clipper->top_left.y;
        clip_ymax = clipper->top_left.y + clipper->size.height - 1;
    }

    // Si y est hors de la zone, on ne dessine rien
    if (y < clip_ymin || y > clip_ymax || y < 0 || y >= taille_surface.height) return;

    // On ajuste x1 et x2 pour qu'ils restent dans les limites
    x1 = (x1 > clip_xmin) ? x1 : clip_xmin;
    x1 = (x1 < 0) ? 0 : x1;
    x2 = (x2 < clip_xmax) ? x2 : clip_xmax;
    x2 = (x2 >= taille_surface.width) ? taille_surface.width - 1 : x2;

    // Si x1 > x2 après ajustement, rien à dessiner
    if (x1 > x2) return;

    // On trouve le premier pixel à dessiner
    uint32_t* ptr_pixel = (uint32_t*)(pixel_0 + (y * taille_surface.width + x1) * 4);
    // On boucle pour colorier chaque pixel
    for (int x = x1; x <= x2; x++) {
        *ptr_pixel++ = valeur_pixel;
    }
}

// Calcule l'inverse de la pente entre deux points (pour savoir comment une ligne bouge)
float calcule_inverse_pente(ei_point_t p1, ei_point_t p2)
{
    // Si les y sont égaux, pas de pente, on renvoie 0
    return ((p2.y - p1.y) != 0) ? (float)(p2.x - p1.x) / (p2.y - p1.y) : 0;
}

// Crée une table avec les arêtes d’un polygone (pour dessiner des formes comme des octogones)
void creer_table_tc(const ei_point_t* point_array, size_t point_array_size, edge_t* edge_table[], int y_min, int y_max)
{
    // On boucle sur tous les points du polygone
    for (size_t i = 0; i < point_array_size; i++) {
        ei_point_t p1 = point_array[i];
        ei_point_t p2 = point_array[(i + 1) % point_array_size];

        // Si les points sont à la même hauteur, on skip (pas d’arête utile)
        if (p1.y == p2.y) continue;

        // On met le point le plus bas en p_min et l’autre en p_max
        ei_point_t p_min = (p1.y < p2.y) ? p1 : p2;
        ei_point_t p_max = (p1.y < p2.y) ? p2 : p1;

        // Si l’arête est hors de la zone qu’on veut, on skip
        if (p_max.y <= y_min || p_min.y >= y_max) continue;

        // On crée une nouvelle arête
        edge_t* edge = (edge_t*)malloc(sizeof(edge_t));
        if (!edge) return; // Si ça marche pas, on arrête

        // On remplit les infos de l’arête
        edge->ymax = p_max.y;
        edge->inv_m = calcule_inverse_pente(p_min, p_max);

        // Si le point est trop bas, on ajuste x pour commencer à y_min
        if (p_min.y < y_min) {
            edge->x = p_min.x + edge->inv_m * (y_min - p_min.y);
            p_min.y = y_min;
        } else {
            edge->x = p_min.x;
        }

        // On calcule où mettre l’arête dans la table
        int index = p_min.y - y_min;
        if (index >= 0 && index < (y_max - y_min)) {
            edge->next = edge_table[index];
            edge_table[index] = edge; // On ajoute l’arête à la table
        } else {
            free(edge); // Si c’est hors limite, on libère
        }
    }
}

// Ajoute une arête à la liste des arêtes actives (TCA, c’est comme une liste de travail)
void ajoute_arete_tca(edge_t* arete, edge_t** tca)
{
    // On met l’arête au début de la liste
    arete->next = *tca;
    *tca = arete;
}

// Supprime les arêtes qui sont finies dans la liste TCA
void supprimer_arete_tca(int y, edge_t** tca)
{
    edge_t* current = *tca;
    edge_t* prev = NULL;

    // On parcourt la liste
    while (current != NULL) {
        // Si l’arête se termine à cette ligne (y), on la vire
        if (current->ymax == y) {
            edge_t* u = current;
            if (prev == NULL) { // Si c’est le début de la liste
                *tca = current->next;
                current = *tca;
            } else {
                prev->next = current->next;
                current = prev->next;
            }
            free(u); // On libère la mémoire
        } else {
            prev = current;
            current = current->next;
        }
    }
}

// Trie la liste TCA pour mettre les arêtes dans l’ordre des x (de gauche à droite)
void trier_tca_x(edge_t** tca)
{
    // Si la liste est vide ou a un seul élément, rien à faire
    if (*tca == NULL || (*tca)->next == NULL) return;

    edge_t* sorted = NULL;
    edge_t* current = *tca;

    // On parcourt chaque arête
    while (current != NULL) {
        edge_t* next = current->next;

        // On insère l’arête dans la liste triée
        if (sorted == NULL || current->x <= sorted->x) {
            current->next = sorted;
            sorted = current;
        } else {
            edge_t* u = sorted;
            while (u->next != NULL && u->next->x < current->x) {
                u = u->next;
            }
            current->next = u->next;
            u->next = current;
        }

        current = next;
    }

    // On met la liste triée à la place de l’ancienne
    *tca = sorted;
}

// Convertit une couleur en un format que la surface peut utiliser
uint32_t ei_impl_map_rgba(ei_surface_t surface, ei_color_t color)
{
    // On récupère l’ordre des couleurs (rouge, vert, bleu, alpha)
    int ir, ig, ib, ia;
    hw_surface_get_channel_indices(surface, &ir, &ig, &ib, &ia);
    uint32_t pixel = 0;
    // On met chaque composante de couleur au bon endroit
    ((uint8_t*)&pixel)[ir] = color.red;
    ((uint8_t*)&pixel)[ig] = color.green;
    ((uint8_t*)&pixel)[ib] = color.blue;
    ((uint8_t*)&pixel)[ia] = color.alpha;
    return pixel;
}


ei_point_t* arc(ei_point_t centre, float rayon, float angle_deb, float angle_fin, size_t* count )
{
    if ( rayon <= 0) return NULL;


    // Convertir les angles en radians
    float deb = angle_deb * M_PI / 180.0f;
    float fin = angle_fin * M_PI / 180.0f;
    int n_points = (int)ceil( (fin -deb)*rayon);
    float d_angle = (fin-deb) / (float)n_points;

    // Allouer le tableau de points
    ei_point_t* points = (ei_point_t*)calloc(n_points+1, sizeof(ei_point_t));
    int unic_x , unic_y , prem_fois =1 , c = 0;

    // Générer les points de l'arc
    for (int i = 0; i < n_points; i++) {
        float angle = deb + i * d_angle;
        int px= centre.x + roundf(rayon * cosf(angle));
        int py= centre.y + roundf(rayon * sinf(angle));
        if ( prem_fois || px!= unic_x || py!= unic_y) {
            points[c].x = px;
            points[c].y = py;
            unic_x = px;
            unic_y = py;
            prem_fois = 0;
            c++;
        }
    }
    *count = c;
    return points;
}

typedef enum {
    FRAME_FULL,
    FRAME_TOP,
    FRAME_BOTTOM
} ei_frame_part_t;


ei_point_t* rounded_frame(ei_rect_t* rect, float rayon, ei_frame_part_t part)
{
    // Vérifications initiales
    if (!rect || rayon <= 0) {
        return NULL;
    }

    if (rayon > rect->size.width / 2.0f || rayon > rect->size.height / 2.0f) {
        printf("le rayon ne doit pas dépasser la moitié de la largeur ou hauteur!\n");
        return NULL;
    }

    // Configuration des arcs selon le type de cadre
    typedef struct {
        ei_point_t center;
        float angle_deb, angle_fin; // En degrés
    } kaws;

    kaws arcs[4];
    size_t num_arcs = 0;
    size_t extra_points = 0;

    if (part == FRAME_FULL) {
        num_arcs = 4;
        arcs[0] = (kaws){ // Haut-gauche
            {rect->top_left.x + rayon, rect->top_left.y + rayon},
            270, 180
        };
        arcs[1] = (kaws){ // Bas-gauche
            {rect->top_left.x + rayon, rect->top_left.y + rect->size.height - rayon},
            180, 90
        };
        arcs[2] = (kaws){ // Bas-droit
            {rect->top_left.x + rect->size.width - rayon, rect->top_left.y + rect->size.height - rayon},
            90, 0
        };
        arcs[3] = (kaws){ // Haut-droit
            {rect->top_left.x + rect->size.width - rayon, rect->top_left.y + rayon},
            0, -90
        };
    } else if (part == FRAME_TOP) {
        num_arcs = 3;
        extra_points = 2;
        arcs[0] = (kaws){ // Haut-droit
            {rect->top_left.x + rect->size.width - rayon, rect->top_left.y + rayon},
            -45, -90
        };
        arcs[1] = (kaws){ // Haut-gauche
            {rect->top_left.x + rayon, rect->top_left.y + rayon},
            270, 180
        };
        arcs[2] = (kaws){ // Bas-gauche
            {rect->top_left.x + rayon, rect->top_left.y + rect->size.height - rayon},
            180, 135
        };
    } else if (part == FRAME_BOTTOM) {
        num_arcs = 3;
        extra_points = 2;
        arcs[0] = (kaws){ // Bas-gauche
            {rect->top_left.x + rayon, rect->top_left.y + rect->size.height - rayon},
            135, 90
        };
        arcs[1] = (kaws){ // Bas-droit
            {rect->top_left.x + rect->size.width - rayon, rect->top_left.y + rect->size.height - rayon},
            90, 0
        };
        arcs[2] = (kaws){ // Haut-droit
            {rect->top_left.x + rect->size.width - rayon, rect->top_left.y + rayon},
            0, -45
        };
    } else {
        return NULL;
    }

    // Estimation du nombre maximal de points
    size_t max_points_per_arc = 0;
    for (size_t i = 0; i < num_arcs; i++) {
        float delta_angle = (arcs[i].angle_fin - arcs[i].angle_deb) * M_PI / 180.0f;
        size_t n = (size_t)ceil(delta_angle * rayon) + 1;
        max_points_per_arc = n > max_points_per_arc ? n : max_points_per_arc;
    }
    size_t total_count = max_points_per_arc * num_arcs + extra_points;

    // Allocation du tableau final
    ei_point_t* frame = calloc(total_count, sizeof(ei_point_t));
    if (!frame) {
        return NULL;
    }

    // Génération des arcs et copie directe dans frame
    size_t offset = 0;
    for (size_t i = 0; i < num_arcs; i++) {
        ei_point_t* arc_points = arc(arcs[i].center, rayon, arcs[i].angle_deb, arcs[i].angle_fin);
        if (!arc_points) {
            free(frame);
            return NULL;
        }

        // Compter les points générés par arc
        size_t arc_count = 0;
        while (arc_points[arc_count].x != 0 || arc_points[arc_count].y != 0 || arc_count == 0) {
            arc_count++;
        }
        if (arc_count < 2) {
            free(arc_points);
            free(frame);
            return NULL;
        }

        // Copier les points dans frame
        for (size_t j = 0; j < arc_count; j++) {
            frame[offset + j] = arc_points[j];
        }
        offset += arc_count;
        free(arc_points);
    }

    // Ajout des points de fermeture pour FRAME_TOP ou FRAME_BOTTOM
    if (part == FRAME_TOP || part == FRAME_BOTTOM) {
        float h = fmin(rect->size.width / 2.0f, rect->size.height / 2.0f);
        ei_point_t point1 = {rect->top_left.x + h, rect->top_left.y + h};
        ei_point_t point2;

        if (rect->size.width >= rect->size.height) {
            point2.x = rect->top_left.x + rect->size.width - h;
            point2.y = rect->top_left.y + h;
        } else {
            point2.x = rect->top_left.x + h;
            point2.y = rect->top_left.y + rect->size.height - h;
        }

        if (part == FRAME_TOP) {
            frame[offset] = point1;
            frame[offset + 1] = point2;
        } else { // FRAME_BOTTOM
            frame[offset] = point2;
            frame[offset + 1] = point1;
        }
        offset += 2;
    }

    // Réallouer pour ajuster la taille exacte
    ei_point_t* resized = realloc(frame, offset * sizeof(ei_point_t));
    return resized ? resized : frame;
}


// Fonction draw_button adaptée avec ei_relief_t
void draw_button(ei_surface_t surface, ei_rect_t* rect, float rayon, ei_color_t color, ei_relief_t relief)
{
    if (!surface || !rect || rayon <= 0) return;

    // Validation des dimensions
    if (rect->size.width < 0 || rect->size.height < 0 ||
        rayon > rect->size.width / 2.0f || rayon > rect->size.height / 2.0f) {
        return;
    }

    // Ajuster les couleurs pour l'effet de relief
    ei_color_t adjust_color(ei_color_t color, int delta) {
        ei_color_t result = color;
        result.red = (unsigned char)(color.red + delta > 255 ? 255 : (color.red + delta < 0 ? 0 : color.red + delta));
        result.green = (unsigned char)(color.green + delta > 255 ? 255 : (color.green + delta < 0 ? 0 : color.green + delta));
        result.blue = (unsigned char)(color.blue + delta > 255 ? 255 : (color.blue + delta < 0 ? 0 : color.blue + delta));
        return result;
    }

    ei_color_t light_color = adjust_color(color, 50);
    ei_color_t dark_color = adjust_color(color, -50);
    ei_color_t top_color, bottom_color;

    // Déterminer les couleurs en fonction du relief
    if (relief == ei_relief_raised) {
        top_color = light_color;
        bottom_color = dark_color;
    } else if (relief == ei_relief_sunken) {
        top_color = dark_color;
        bottom_color = light_color;
    } else { // ei_relief_none
        top_color = color;
        bottom_color = color;
    }

    // Structure pour gérer les parties du bouton
    typedef struct {
        ei_frame_part_t part;
        ei_color_t color;
        ei_rect_t* clipper; // NULL pour top/bottom, non-NULL pour full
    } ButtonPart;

    // Définir les parties à dessiner
    ButtonPart parts[3];
    int num_parts = 0;

    // Partie haute
    parts[num_parts++] = (ButtonPart){FRAME_TOP, top_color, NULL};

    // Partie basse
    parts[num_parts++] = (ButtonPart){FRAME_BOTTOM, bottom_color, NULL};

    // Fond (avec clipper)
    int dist = fmin(0.05f * rect->size.width, 0.05f * rect->size.height);
    ei_rect_t clipper = {
        {rect->top_left.x + dist, rect->top_left.y + dist},
        {rect->size.width - 2 * dist, rect->size.height - 2 * dist}
    };
    parts[num_parts++] = (ButtonPart){FRAME_FULL, color, &clipper};

    // Dessiner chaque partie
    for (int i = 0; i < num_parts; i++) {
        ei_point_t* frame = rounded_frame(parts[i].part == FRAME_FULL ? &clipper : rect, rayon, parts[i].part);
        if (!frame) return;

        // Compter les points
        size_t point_count = 0;
        while (frame[point_count].x != 0 || frame[point_count].y != 0 || point_count == 0) {
            point_count++;
        }
        if (point_count < 2) {
            free(frame);
            return;
        }

        // Dessiner le polygone
        ei_draw_polygon(surface, frame, point_count, parts[i].color, parts[i].clipper);
        free(frame);
    }
}