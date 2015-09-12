// ****************************************************************************
//                                                                            *
//                             CURL SESSION C                                 *
//                                                                            *
// Created by Thomas Chafiol  -  thomaschaf@gmail.com  -  10  /  09  /  2015  *
// *****************************************************************************

#include <stdio.h>

#include "modulary.h"
#include "curl_session.h"

// ****************************************************************************
//                                                                            *
//                                  METHODS                                   *
//                                                                            *
// ****************************************************************************

size_t          callback_write_func(void *ptr, size_t size, size_t nmemb, Buffer buffer)
{
    size_t      new_len = size * nmemb;

    buffer->concat(buffer, ptr, new_len);
    return new_len;
}

static void         *__get(CurlSession this, char *url)
{
    Buffer          buffer = new(__Buffer);
    CURLcode        res;

    this->curl_ptr = curl_easy_init();
    if (this->curl_ptr != NULL)
    {
        curl_easy_setopt(this->curl_ptr, CURLOPT_URL, url);
        curl_easy_setopt(this->curl_ptr, CURLOPT_WRITEFUNCTION, callback_write_func);
        curl_easy_setopt(this->curl_ptr, CURLOPT_WRITEDATA, buffer);
        curl_easy_setopt(this->curl_ptr, CURLOPT_FOLLOWLOCATION, 1L);
        res = curl_easy_perform(this->curl_ptr);
        if (CURLE_OK != res)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            return (NULL);
        }
    }
    curl_easy_cleanup(this->curl_ptr);
    this->curl_ptr = NULL;
    return (buffer);
}

// ****************************************************************************
//                                                                            *
//                                    CLASS                                   *
//                                                                            *
// ****************************************************************************

char *__CURL_SESSION = "curl_session";

t_module __CurlSession =  { sizeof(t_curl_session),  curl_session_ctor, curl_session_dtor,
    "curl_session",
    0,
    0,
    0,
    0,
    0,
    0
};

// ****************************************************************************
//                                                                            *
//                                  CTOR/DTOR                                 *
//                                                                            *
// ****************************************************************************

static void           __methods(CurlSession this)
{
    this->get = __get;
}

int                   curl_session_ctor(CurlSession this)
{
    this->curl_ptr = NULL;
    curl_global_init(CURL_GLOBAL_DEFAULT);

    __methods(this);
    return (true);
}

int                   curl_session_dtor(CurlSession this)
{
    curl_global_cleanup();
    return (true);
}
