// ****************************************************************************
//                                                                            *
//                                  MAMAN H                                   *
//                                                                            *
// Created by Thomas Chafiol  -  thomaschaf@gmail.com  -  10  /  09  /  2015  *
// *****************************************************************************

#ifndef __MAMAN_H__
#define __MAMAN_H__

#include <stdarg.h>
#include <stdio.h>

#include "modulary.h"

typedef enum        e_type_maman
{
    MAMAN_WEB_FILE,
    MAMAN_TYPE_LEN
}                   t_type_maman;

typedef struct      s_result
{
    int             score;
    char            *descriptor;
    char            *sub_descriptor;
}                   t_result;


typedef struct      s_maman
{
    t_module        parent;
    Json            dictionaries;
    int             dico_len; 
    FILE            *fp;
    Liste           *results;
    fct             publish_results;
    fct             push_result;
    fct             analyse;
    fct             actions[MAMAN_TYPE_LEN];
    unsigned int    *(*analyse_web)();
}                   t_maman;

typedef t_maman *    Maman;

int                 maman_ctor(Maman, va_list *);
int                 maman_dtor();

extern char         *__maman;
extern t_module     __Maman;

#endif /* __MAMAN_H__ */
