/**
 *@file  ei_implementation.h
 *
 *@brief Private definitions.
 *
 */


#ifndef EI_IMPLEMENTATION_H
#define EI_IMPLEMENTATION_H

#include "hw_interface.h"
#include "ei_types.h"
#include "ei_widget.h"


/**
 * Structure des arêtes pour l'algorithme scanline utilisé dans draw_polygon.
 */
typedef struct edge_t {
    int ymax;                         // Ordonnée maximale de l'arête
    float x;                          // Abscisse au y minimum
    float inv_m;                      // Inverse de la pente : dx/dy
    struct edge_t* next;              // Pointeur vers l'arête suivante
} edge_t;

/**
 * \brief Dessine un segment de ligne entre deux points sur une surface avec l'algorithme de Bresenham.
 *
 * @param surface La surface où dessiner le segment.
 * @param point_1 Le point de départ du segment.
 * @param point_2 Le point d'arrivée du segment.
 * @param couleur La couleur du segment.
 * @param clipper Si non NULL, restreint le dessin à ce rectangle.
 */
void draw_line(ei_surface_t surface, ei_point_t point_1, ei_point_t point_2, ei_color_t couleur, const ei_rect_t* clipper);

/**
 * \brief Calcule l'inverse de la pente de la droite définie par deux points.
 *
 * @param p1 Premier point.
 * @param p2 Deuxième point.
 * @return L'inverse de la pente (dx/dy) ou 0 si dy == 0.
 */
float calcule_inverse_pente(ei_point_t p1, ei_point_t p2);

/**
 * \brief Transforme la liste de points d’un polygone en une table des arêtes, triées par y minimum.
 *
 * @param point_array Tableau de points du polygone.
 * @param point_array_size Nombre de points dans le tableau.
 * @param edge_table Table des arêtes, indexée par y - y_min.
 * @param y_min Ordonnée minimale des arêtes.
 * @param y_max Ordonnée maximale des arêtes pour le clipping.
 */
void creer_table_tc(const ei_point_t* point_array, size_t point_array_size, edge_t* edge_table[], int y_min, int y_max);

/**
 * \brief Supprime les arêtes terminées de la table des côtés actifs (TCA) à l'ordonnée y.
 *
 * @param y Ordonnée actuelle pour vérifier les arêtes terminées.
 * @param tca Pointeur vers la tête de la TCA.
 */
void supprimer_arete_tca(int y, edge_t** tca);

/**
 * \brief Trie la table des côtés actifs (TCA) par coordonnée x croissante.
 *
 * @param tca Pointeur vers la tête de la TCA.
 */
void trier_tca_x(edge_t** tca);

/**
 * \brief Ajoute une arête à la table des côtés actifs (TCA).
 *
 * @param edge Arête à ajouter.
 * @param tca Pointeur vers la tête de la TCA.
 */
void ajoute_arete_tca(edge_t* edge, edge_t** tca);

/**
 * \brief Dessine une ligne horizontale de pixels avec une couleur donnée.
 *
 * @param surface La surface où dessiner.
 * @param x1 Abscisse de début de la ligne.
 * @param x2 Abscisse de fin de la ligne.
 * @param y Ordonnée de la ligne.
 * @param couleur Couleur des pixels.
 * @param clipper Si non NULL, restreint le dessin à ce rectangle.
 */
void draw_horizontal_line(ei_surface_t surface, int x1, int x2, int y, ei_color_t couleur, const ei_rect_t* clipper);


/**
 * \brief	A structure storing the placement parameters of a widget.
 *		You have to define this structure: no suggestion provided.
 */
struct ei_impl_placer_params_t;

/**
 * \brief	Tells the placer to recompute the geometry of a widget.
 *		The widget must have been previously placed by a call to \ref ei_place.
 *		Geometry re-computation is necessary for example when the text label of
 *		a widget has changed, and thus the widget "natural" size has changed.
 *
 * @param	widget		The widget which geometry must be re-computed.
 */
void ei_impl_placer_run(ei_widget_t widget);

/**
 * \brief	Fields common to all types of widget. Every widget classes specializes this base
 *		class by adding its own fields.
 */
typedef struct ei_impl_widget_t {
    ei_widgetclass_t* wclass;        ///< The class of widget of this widget. Avoids the field name "class" which is a keyword in C++.
    uint32_t   pick_id;                ///< Id of this widget in the picking offscreen.
    ei_color_t pick_color;           ///< pick_id encoded as a color.
    void*      user_data;                 ///< Pointer provided by the programmer for private use. May be NULL.
    ei_widget_destructor_t destructor; ///< Pointer to the programmer's function to call before destroying this widget. May be NULL.

    /* Widget Hierarchy Management */
    ei_widget_t parent;              ///< Pointer to the parent of this widget.
    ei_widget_t children_head;      ///< Pointer to the first child of this widget. Children are chained with the "next_sibling" field.
    ei_widget_t children_tail;       ///< Pointer to the last child of this widget.
    ei_widget_t next_sibling;        ///< Pointer to the next child of this widget's parent widget.

    /* Geometry Management */
    struct ei_impl_placer_params_t* placer_params; ///< Pointer to the placer parameters for this widget. If NULL, the widget is not currently managed and thus, is not displayed on the screen.
    ei_size_t requested_size;        ///< See \ref ei_widget_get_requested_size.
    ei_rect_t screen_location;       ///< See \ref ei_widget_get_screen_location.
    ei_rect_t* content_rect;        ///< See ei_widget_get_content_rect. By defaults, points to the screen_location.
} ei_impl_widget_t;

/**
 * @brief	Draws the children of a widget.
 * 		The children are drawn within the limits of the clipper and
 * 		the widget's content_rect.
 *
 * @param	widget		The widget whose children are drawn.
 * @param	surface		A locked surface where to draw the widget's children.
 * @param	pick_surface	The picking offscreen.
* @param	clipper		If not NULL, the drawing is restricted within this rectangle
 * (expressed in the surface reference frame).
 */
void ei_impl_widget_draw_children(ei_widget_t widget,
                                 ei_surface_t surface,
                                 ei_surface_t pick_surface,
                                 ei_rect_t* clipper);

/**
 * \brief	Converts the red, green, blue and alpha components of a color into a 32 bits integer
 * 		than can be written directly in the memory returned by \ref hw_surface_get_buffer.
 * 		The surface parameter provides the channel order.
 *
 * @param	surface		The surface where to store this pixel, provides the channels order.
 * @param	color		The color to convert.
 *
* @return 			The 32 bit integer corresponding to the color. The alpha component
 * of the color is ignored in the case of surfaces that don't have an
 * alpha channel.
 */
uint32_t ei_impl_map_rgba(ei_surface_t surface, ei_color_t color);


/**
 * @brief Crée un tableau de points définissant un arc de cercle.
 *
 * @param centre Le centre de l'arc (ei_point_t).
 * @param rayon Le rayon de l'arc.
 * @param angle_deb L'angle de début en degrés.
 * @param angle_fin L'angle de fin en degrés.
 * @return Un tableau de points (ei_point_t*) représentant l'arc. Doit être libéré par l'appelant.
 */
ei_point_t* arc(ei_point_t centre, float rayon, float angle_deb, float angle_fin);


/**
 * @brief Crée un tableau de points définissant un cadre aux bords arrondis.
 *
 * @param rect Le rectangle définissant les dimensions du cadre.
 * @param radius Le rayon des coins arrondis.
 * @param part La partie du cadre à générer : 0 (complet), 1 (haut), 2 (bas).
 * @return Un tableau de points (ei_point_t*) représentant le cadre. Doit être libéré par l'appelant.
 */
ei_point_t* rounded_frame(ei_rect_t rect, float radius, int part);

/**
 * @brief Dessine un bouton avec un effet de relief.
 *
 * @param surface La surface où dessiner.
 * @param rect Le rectangle définissant les dimensions du bouton.
 * @param radius Le rayon des coins arrondis.
 * @param color La couleur principale du bouton.
 * @param relief Type de relief : ei_relief_raised ou ei_relief_sunken.
 * @param clipper La zone de clipping.
 */
void draw_button(ei_surface_t surface, ei_rect_t rect, float radius, ei_color_t color, ei_relief_t relief, const ei_rect_t* clipper);

#endif

