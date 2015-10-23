// ****************************************************************************
//                                                                            *
//                            GUMBO SESSION H                                 *
//                                                                            *
// Created by Thomas Chafiol  -  thomaschaf@gmail.com  -  10  /  09  /  2015  *
// *****************************************************************************

#ifndef __GUMBO_SESSION_H__
#define __GUMBO_SESSION_H__

#include <stdarg.h>

#include "modulary.h"
#include "analyser.h"

typedef struct      s_element_rank
{
    int             tag;
    int             score;
}                   t_element_rank;

typedef struct      s_gumbo_session
{
    t_analyser      parent;
    fct             parse;
    fct             analyse;
    fct             is_target_element;
    fct             get_text;
}                   t_gumbo_session;

typedef t_gumbo_session *    GumboSession;

int                 gumbo_session_ctor(GumboSession, va_list *);
int                 gumbo_session_dtor();

extern char         *__GUMBO_SESSION;
extern t_module     __GumboSession;

#endif /* __GUMBO_SESSION_H__ */
