// ****************************************************************************
//                                                                            *
//                             CURL SESSION H                                 *
//                                                                            *
// Created by Thomas Chafiol  -  thomaschaf@gmail.com  -  10  /  09  /  2015  *
// *****************************************************************************

#ifndef __CURL_SESSION_H__
#define __CURL_SESSION_H__

#include <curl/curl.h>

#include "modulary.h"

typedef struct      s_curl_session
{
    t_module        module;
    CURL            *curl_ptr;
    pfct            get;
}                   t_curl_session;

typedef t_curl_session *    CurlSession;

int                 curl_session_ctor(CurlSession);
int                 curl_session_dtor();

extern char         *__CURL_SESSION;
extern t_module     __CurlSession;

#endif /* __CURL_SESSION_H__ */
