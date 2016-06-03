
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ei_widget_toplevel.h"


#define POS_BOUTON 10
#define TAILLE_BOUTON 8
#define TAILLE_ENTETE 20
#define TAILLE_RECADR 10


void* toplevelAllocfunc()
{
    // calloc initialise automatiquement les données à 0
    return calloc(1, sizeof(ei_widget_toplevel_t));
}


void toplevelReleasefunc(struct ei_widget_t* widget)
{
    // On convertie le widget pour effectuer les opérations
    ei_widget_toplevel_t* wtl = (ei_widget_toplevel_t*)widget;

    if(wtl->title != NULL)
        free(wtl->title);
    if(wtl->min_size != NULL)
        free(wtl->min_size);
}


void toplevelDrawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{

}


void toplevelSetdefaultsfunc(struct ei_widget_t* widget)
{
    // On convertie le widget pour effectuer les opérations
    ei_widget_toplevel_t* wtl = (ei_widget_toplevel_t*)widget;

    wtl->border_width = 4;
    wtl->color = ei_default_background_color;
    wtl->title = malloc(strlen("Toplevel") + 1);
    strcpy(wtl->title, "Toplevel");
    wtl->closable = EI_TRUE;
    wtl->resizable = ei_axis_both;
    wtl->min_size = malloc(sizeof(ei_size_t));
    wtl->min_size->width = 160;
    wtl->min_size->height = 120;
}


void toplevelGeomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect)
{

}


// Obtient la distance entre deux points
inline int distPoint(ei_point_t t1, ei_point_t t2)
{
    return (int)sqrt((t1.x-t2.x)*(t1.x-t2.x)+(t1.y-t2.y)*(t1.y-t2.y))
}

inline int min(int a, int b)
{
    return a < b ? a : b;
}

inline int max(int a, int b)
{
    return a > b ? a : b;
}


/**********************************************************************
***************** Events **********************************************
**********************************************************************/


ei_bool_t pushToplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param)
{
    ei_widget_toplevel_t* wtl = (ei_widget_toplevel_t*)widget;
    ei_point_t cur = getCurrent();
    ei_point_t bPos = ei_point(widget->top_left.x + POS_BOUTON, widget->top_left.y + POS_BOUTON); // Position du bouton
    ei_rect_t recadr = ei_rect(ei_point(widget->top_left.x + widget->size.width - TAILLE_RECADR, widget->top_left.y + widget->size.height - TAILLE_RECADR), ei_rect(TAILLE_RECADR,TAILLE_RECADR));
    
    // On regarde si nous sommes sur le boutton
    if(distPoint(cur, bPos) < 8 && wtl->closable == EI_TRUE)
	ei_bind(ei_ev_mouse_buttonup, NULL, "all", releaseCloseToplevel, widget);
    else if(cur < widget->top_left.y + TAILLE_ENTETE)// On regarde si nous sommes sur l'en-tête
    {
	ei_bind(ei_ev_mouse_move, NULL, "all", moveToplevel, widget);
	ei_bind(ei_ev_mouse_buttonup, NULL, "all", releaseMoveToplevel, widget);
    }
    else if(isIn(cur, recadr) == EI_TRUE && wtl->resizable != ei_axis_none)// On regarde si nous sommes sur le carré de recadrement
    {
	ei_bind(ei_ev_mouse_move, NULL, "all", moveResizeToplevel, widget);
	ei_bind(ei_ev_mouse_buttonup, NULL, "all", releaseResizeToplevel, widget);
    }
}


ei_bool_t releaseCloseToplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param)
{
    ei_widget_toplevel_t* wtl = (ei_widget_toplevel_t*)user_param;
    ei_widget_t* widget = (ei_widget_t*)user_param;
    ei_point_t bPos = ei_point(widget->top_left.x + POS_BOUTON, widget->top_left.y + POS_BOUTON);
    
    // Si le curseur est actuellement dans le button on change coupe la fenetre
    if(distPoint(cur, bPos) < 8 && wtl->closable == EI_TRUE)
    {
	// On retire l'evenement
	ei_unbind(ei_ev_mouse_buttonup, NULL, "all", releaseCloseToplevel, widget);
	ei_app_invalidate_rect(widget->screen_location);
	ei_widget_destroy(widget);
    }
    
    // On retire juste l'evenement
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", releaseCloseToplevel, widget);
}


ei_bool_t moveToplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param)
{
    ei_widget_t* widget = (ei_widget_t*)user_param;
    ei_point_t mov = ei_point_sub(getCurrent(), getLast()); // Mouvement entre la derniere et actuelle position
    ei_rect_t update;
    
    // Rectangle à mettre a jour
    update.top_left.x = min(widget->screen_location.top_left.x, widget->screen_location.top_left.x + mov.x);
    update.top_left.y = min(widget->screen_location.top_left.y, widget->screen_location.top_left.y + mov.y);
    update.size.width = max(widget->screen_location.size.width, widget->screen_location.size.width + mov.x);
    update.size.height = max(widget->screen_location.size.height, widget->screen_location.size.height + mov.y);
    
    // Met à jour sa position
    ei_place(widget, NULL, widget->screen_location.top_left.x + mov.x, widget->screen_location.top_left.y + mov.y, NULL, NULL, NULL, NULL, NULL, NULL);
    ei_app_invalidate_rect(&update);
}

ei_bool_t releaseMoveToplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param)
{
    ei_widget_t* widget = (ei_widget_t*)user_param;
    
    // On retire les evenements
    ei_unbind(ei_ev_mouse_move, NULL, "all", moveToplevel, widget);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", releaseMoveToplevel, widget);
}


ei_bool_t moveResizeToplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param)
{
    ei_widget_toplevel_t* wtl = (ei_widget_toplevel_t*)user_param;
    ei_widget_t* widget = (ei_widget_t*)user_param;
    
    // Rectangle à mettre a jour
    update.top_left.x = min(widget->screen_location.top_left.x, widget->screen_location.top_left.x + mov.x);
    update.top_left.y = min(widget->screen_location.top_left.y, widget->screen_location.top_left.y + mov.y);
    update.size.width = max(widget->screen_location.size.width, widget->screen_location.size.width + mov.x);
    update.size.height = max(widget->screen_location.size.height, widget->screen_location.size.height + mov.y);
    
    // Met à jour sa position
    switch(wtl->resizable)
    {
      case ei_axis_both :
	ei_place(widget, NULL, NULL, NULL, widget->screen_location.size.width + mov.x, widget->screen_location.size.height + mov.y, NULL, NULL, NULL, NULL);
	break;
      case ei_axis_x :
	ei_place(widget, NULL, NULL, NULL, widget->screen_location.size.width + mov.x, NULL, NULL, NULL, NULL, NULL);
	break;
      case ei_axis_y :
	ei_place(widget, NULL, NULL, NULL, NULL, widget->screen_location.size.height + mov.y, NULL, NULL, NULL, NULL);
	break;
    }
    
    ei_app_invalidate_rect(&update);
}

ei_bool_t releaseResizeToplevel(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param)
{
    ei_widget_t* widget = (ei_widget_t*)user_param;
    
    // On retire les evenements
    ei_unbind(ei_ev_mouse_move, NULL, "all", moveResizeToplevel, widget);
    ei_unbind(ei_ev_mouse_buttonup, NULL, "all", releaseResizeToplevel, widget);
}

