/**
 *  @file	ei_application.c
 *  @brief	Manages the main steps of a graphical application: initialization, main window,
 *		main loop, quitting, resource freeing.
 *
 *  \author
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include "ei_application.h"
#include "debug.h"
#include "ei_widgetclass.h"
#include "ei_widget_frame.h"
#include "ei_global.h"
#include "ei_geometrymanager.h"

ei_widget_t * racine;
ei_surface_t window;
ei_surface_t windowpick;
ei_widget_t* tab_widget[256];
ei_linked_rect_t*  liste_rect = NULL;

void ei_app_run_rec(ei_widget_t* widget, ei_surface_t window, ei_surface_t windowpick, ei_rect* clipper )
{
    if(widget == NULL)
        return;
    if (clipper == NULL)
      widget->wclass->drawfunc(widget ,window, windowpick, widget->parent->content_rect);
    else 
      widget->wclass->drawfunc(widget ,window, windowpick, widget->parent->content_rect);
    ei_app_run_rec(widget->next_sibling, window, windowpick,clipper);
    ei_app_run_rec(widget->children_head, window, windowpick,clipper);
}


// TODO : Implémenter fullscreen
void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen)
{ 
    ei_color_t * coloracine;
  
    hw_init();

    ei_frame_register_class();
    ei_button_register_class();
    ei_toplevel_register_class();
    ei_register_placer_manager();
    
    racine = frameAllocfunc();
    frameSetdefaultsfunc(racine);
    
    coloracine=malloc(sizeof(ei_color_t));
    coloracine->red = 255;
    coloracine->green = 0;
    coloracine->blue = 0;
    coloracine->alpha = 255;
    
    // Identifiant
    racine->pick_color = coloracine;
    racine->wclass = ei_widgetclass_from_name("frame");
    racine->pick_id = 255;
    
    // Taille
    racine->requested_size = *main_window_size;
    racine->screen_location.top_left.x = 0;
    racine->screen_location.top_left.y = 0;
    racine->screen_location.size = *main_window_size;
    racine->content_rect = malloc(sizeof(ei_rect_t));
    *(racine->content_rect) = racine->screen_location;
    
    // Initialisation du tableau de widget 
    for ( int i =0 ; i <256 ; i++)
      tab_widget[i]=NULL;

    racine->geom_params = calloc(1, sizeof(ei_geometry_param_t));
    
    window = hw_create_window(main_window_size,fullscreen);
    windowpick = hw_create_window(main_window_size,fullscreen);
}


void ei_app_free()
{
    ei_widget_destroy(racine);
    hw_surface_free(window);
    
    // On termine le widget principale
    hw_quit();
  
    return;
}

void ei_app_run()
{
    
    frameDrawfunc(racine, window, windowpick, racine->content_rect);
    ei_app_run_rec(racine->children_head, window, windowpick);
    getchar();
}

void ei_app_invalidate_rect(ei_rect_t* rect)
{
    
    ei_linked_rect_t * new_rect = calloc(1, sizeof(ei_linked_rect_t));
    new_rect->rect = *rect;
    ei_linked_rect_t * tmp;
    ei_linked_rect_t * liste_rect = malloc(sizeof(ei_linked_rect_t));
  
    if (liste_rect == NULL)
      liste_rect=new_rect;
    
    else
    {
      tmp=liste_rect;
      for(tmp=liste_rect ;tmp->next =! NULL; tmp =tmp->next)
	{
	  tmp->next= new_rect;
	}
    }
      
}

void ei_app_quit_request()
{
    return;
}

ei_widget_t* ei_app_root_widget()
{
    return racine;
}

ei_surface_t ei_app_root_surface()
{
    return window;
}
