/**
 *  @file	widgetclass.c
 *  @brief	Definition and registration of widget classes.
 *
 *  \author 
 *
 */

#include "ei_widgetclass.h"
#include "debug.h"


/**
 *  @var	wc
 *  @brief	Variable globale contenant un chaiage de tout les widgets class
 */
ei_widgetclass_t* wc = NULL;


/**
 *********************************************************************************
 * Fonctions pour les frames
 */

void* frameAllocfunc()
{
    void* ptr;
    int size = 0;
    
    size += sizeof(ei_widget_t*);
    size += sizeof(ei_size_t*);
    size += sizeof(const ei_color_t*);
    size += sizeof(int*);
    size += sizeof(ei_relief_t*);
    size += sizeof(char**);
    size += sizeof(ei_font_t*);
    size += sizeof(ei_color_t*);
    size += sizeof(ei_anchor_t*);
    size += sizeof(ei_surface_t*);
    size += sizeof(ei_rect_t**);
    size += sizeof(ei_anchor_t*);
  
    ptr = (void*)calloc(1,size);
    
    return ptr;
}

void frameReleasefunc(struct ei_widget_t* widget)
{
    
}

void frameDrawfunc(struct ei_widget_t* widget, ei_surface_t surface, ei_surface_t pick_surface, ei_rect_t* clipper)
{
  
}
			       
void frameSetdefaultsfunc(struct ei_widget_t* widget)
{
  
}

void frameGeomnotifyfunc(struct ei_widget_t* widget, ei_rect_t rect)
{
  
}


/**
 *********************************************************************************
 * !Fonctions pour les frames
 */


void ei_widgetclass_register(ei_widgetclass_t* widgetclass)
{
    ei_widgetclass_t* tmp;
  
    // s'il n'y a pas de classe on l'a definit comme la premiere
    if(wc == NULL)
	wc = widgetclass;
    else
    {
	tmp = wc;
	for(tmp = wc; tmp->next != NULL; tmp = tmp->next);
	tmp->next = widgetclass;
    }
}


ei_widgetclass_t* ei_widgetclass_from_name(ei_widgetclass_name_t name)
{
    ei_widgetclass_t* tmp;
  
    for(tmp = wc; tmp != NULL; tmp = tmp->next)
    {
	if(!strcmp(tmp->name, name))
	    return tmp;
    }
    
    PRINT_DEBUG("ei_widgetclass_from_name : classe non trouvee");
    
    return NULL;
}


void ei_frame_register_class()
{
    ei_widgetclass_t* frame = malloc(sizeof(ei_widgetclass_t));
     
    strcpy(frame->name, "frame");
    frame->frameAllocfunc;
    frame->frameReleasefunc;
    frame->frameDrawfunc;
    frame->frameSetdefaultsfunc;
    frame->frameGeomnotifyfunc;
    frame->next = NULL;
    
    ei_widgetclass_register(frame);
}

void ei_button_register_class()
{
  
}

void ei_toplevel_register_class()
{
  
}



