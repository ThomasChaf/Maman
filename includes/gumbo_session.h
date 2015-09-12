// ****************************************************************************
//                                                                            *
//                            GUMBO SESSION H                                 *
//                                                                            *
// Created by Thomas Chafiol  -  thomaschaf@gmail.com  -  10  /  09  /  2015  *
// *****************************************************************************

#ifndef __GUMBO_SESSION_H__
#define __GUMBO_SESSION_H__

#include "modulary.h"

typedef struct      s_gumbo_session
{
    t_module        module;
    fct             parse;
}                   t_gumbo_session;

typedef t_gumbo_session *    GumboSession;

int                 gumbo_session_ctor(GumboSession);
int                 gumbo_session_dtor();

extern char         *__GUMBO_SESSION;
extern t_module     __GumboSession;

#endif /* __GUMBO_SESSION_H__ */
