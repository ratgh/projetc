
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "debug.h"
#include "ei_widget_frame.h"
#include "ei_draw_util.h"

void* frameAllocfunc()
{
    // calloc initialise automatiquement les données à 0
    return calloc(1, sizeof(ei_widget_frame_t));
}

void frameReleasefunc(struct ei_widget_t* widget)
{
    // On convertie le widget pour effectuer les opérations
    ei_widget_frame_t* wf = (ei_widget_frame_t*)widget;

    // on libere tout les membres qui ont ete alloués
    if(wf->text != NULL)
        free(wf->text);
    if(wf->img_rect != NULL)
        free(wf->img_rect);
    //if(wf->img != NULL)
    //     hw_surface_free(wf->img);
}

void frameDrawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{
    ei_widget_frame_t* wf = (ei_widget_frame_t*)widget;
    
    if(widget->parent != NULL)
    {
    int x_rel = widget->content_rect->top_left.x - widget->parent->content_rect->top_left.x;
    int y_rel = widget->content_rect->top_left.y - widget->parent->content_rect->top_left.y;
  
    }
    
    switch (wf->relief)
    {
	case (ei_relief_none): // Pas de bordure
	case (ei_relief_raised): // Bordure plus claire
	    draw_frameButton(widget, surface, clipper, EI_FALSE, EI_TRUE, pick_surface);
	    break;
	case (ei_relief_sunken): // Bordure plus foncée
	    draw_frameButton(widget, surface, clipper, EI_TRUE, EI_TRUE, pick_surface);
	    break;
    }

    // Texte
    if(wf->text != NULL)
        drawTextWidget(surface, widget, wf, clipper);

    // Image
    if(wf->img != NULL)
      drawImgWidget(surface, widget, clipper);
}


void frameSetdefaultsfunc(struct ei_widget_t* widget)
{
    // On convertie le widget pour effectuer les opérations
    ei_widget_frame_t* wf = (ei_widget_frame_t*)widget;

    wf->border_width = 0;
    wf->color = ei_default_background_color;
    wf->relief = ei_relief_none;
    wf->text = NULL;
    wf->text_font = ei_default_font;
    wf->text_color = ei_font_default_color;
    wf->text_anchor = ei_anc_center;
    wf->img = NULL;
    wf->img_rect = NULL;
    wf->img_anchor = ei_anc_center;
}


void frameGeomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect)
{
    ei_widget_frame_t* wf = (ei_widget_frame_t*)widget;
    
    widget->content_rect->top_left.x = rect.top_left.x + wf->border_width;
    widget->content_rect->top_left.y = rect.top_left.y + wf->border_width;
    widget->content_rect->size.width = rect.size.width - 2 * wf->border_width;
    widget->content_rect->size.height = rect.size.height - 2 * wf->border_width;
}


// Callback

ei_bool_t refreshFrame(struct ei_widget_t* widget, struct ei_event_t* event, void* user_param)
{
    // on demande de rafraichir la frame
    ei_app_invalidate_rect(&(widget->parent->screen_location));
    
    return EI_FALSE;
}
