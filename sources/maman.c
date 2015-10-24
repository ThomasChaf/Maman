// ****************************************************************************
//                                                                            *
//                                 MAMAN C                                    *
//                                                                            *
// Created by Thomas Chafiol  -  thomaschaf@gmail.com  -  10  /  09  /  2015  *
// *****************************************************************************

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "modulary.h"
#include "gumbo_session.h"
#include "maman.h"

// ****************************************************************************
//                                                                            *
//                                  METHODS                                   *
//                                                                            *
// ****************************************************************************

int                 compare_results(t_result *a, t_result *b)
{
    if (a->score < b->score)
        return (true);
    return (false);
}

static int          __publish_results(Maman this)
{
    Iter            it = NULL;
    t_result        *result;
    t_jnode         *tmp = this->dictionaries->head;
    int             i = 0;

    while (tmp != NULL)
    {
        printf("========================================================\n");
        printf("%25s\n", tmp->content);
        printf("========================================================\n");
        this->results[i]->quicksort(this->results[i], compare_results);
        it = iter(this->results[i]);
        while (it != NULL)
        {
            result = it->content;
            printf("[%d]\t%s\n", result->score, result->sub_descriptor);
            it = next(it);
        }
        i += 1;
        tmp = tmp->next;
    }
    printf("===================\n");
    return (true);
}

static int          __push_result(Maman this, int queu_index, char *descriptor, char *sub_descriptor, int score)
{
    t_result        *result = m_malloc(sizeof(t_result));

    if (result == NULL)
        return (false);
    result->descriptor = descriptor;
    result->sub_descriptor = sub_descriptor;
    result->score = score;
    return (push(this->results[queu_index], result));
}

unsigned int        *__analyse_web(Maman this, String string)
{
    GumboSession    gumbo = new(__GumboSession, this->dico_len, this->dictionaries);
    unsigned int    *score;

    if (gumbo->parse(gumbo, string) == false)
        return (NULL);
    score = ((Analyser)gumbo)->count;
    delete(gumbo);
    return (score);
}

int                 __analyse_web_file(Maman this, char *filename,  char *descriptor)
{
    String          string = NULL;
    int             fd = m_ropen(filename);
    unsigned int    *score;
    t_jnode         *tmp = this->dictionaries->head;
    int             i = 0;

    if (fd == -1)
        return (false);
    string = read_file(fd);
    if (string == NULL)
        return (false);
    score = this->analyse_web(this, string);
    if (score == NULL)
        return (false);
    while (tmp != NULL)
    {
        this->push_result(this, i, m_strdup(filename), descriptor, score[i]);
        i += 1;
        tmp = tmp->next;
    }
    close(fd);
    delete(string);
    return (true);
}

int                 __analyse(Maman this, t_type_maman type, char *filename, char *descriptor)
{
    return (this->actions[type](this, filename, descriptor));
}

// ****************************************************************************
//                                                                            *
//                            BUILT_IN METHODS                                *
//                                                                            *
// ****************************************************************************

static Iter         __next(Iter iter)
{
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, iter->current);
    if (read == -1)
    {
        free(iter);
        if (line)
            free(line);
        return (NULL);
    }
    if (read - 1 > 0 && line[read - 1] == '\n')
        line[read - 1] = 0;
    iter->content = line;
    return (iter);
}

static Iter         __iter(Maman this)
{
    void            *ptr;
    Iter            iter;

    if (this == NULL || (ptr = m_malloc(sizeof(t_iter))) == NULL)
        return (NULL);
    iter = (Iter)ptr;
    iter->objet = this;
    iter->content = NULL;
    iter->current = this->fp;
    iter->next = __next;
    return (iter);
}

// ****************************************************************************
//                                                                            *
//                                    CLASS                                   *
//                                                                            *
// ****************************************************************************

char *__MAMAN = "maman";

t_module __Maman =  { sizeof(t_maman),  maman_ctor, maman_dtor,
    "maman",
    0,
    0,
    0,
    __iter,
    0,
    0
};

// ****************************************************************************
//                                                                            *
//                                  CTOR/DTOR                                 *
//                                                                            *
// ****************************************************************************

static void         __methods(Maman this)
{
    this->publish_results = __publish_results;
    this->push_result = __push_result;
    this->analyse = __analyse;
    this->actions[MAMAN_WEB_FILE] = __analyse_web_file;
    this->analyse_web = __analyse_web;
}

int                 maman_ctor(Maman this, va_list *ap)
{
    char            *filename = va_arg(*ap, char *);
    Stream          stream;
    t_jnode         *tmp = NULL;
    int             i = 0;

    if (filename != NULL)
    {
        this->fp = fopen(filename, "r");
        if (this->fp == NULL)
        {
            perror("Error:");
            return (false);
        }
    }
    stream = new(__Stream, m_ropen(DICTIONARY_PATH));
    if (stream == NULL)
        return (false);
    this->dictionaries = new(__Json, stream);
    if (this->dictionaries == NULL)
        return (false);
    if (this->dictionaries->json_decode(this->dictionaries) == NULL)
        return (false);
    this->dico_len = 0;
    tmp = this->dictionaries->head;
    while (tmp != NULL)
    {
        tmp = tmp->next;
        this->dico_len += 1;
    }
    this->results = m_malloc(this->dico_len * sizeof(Liste *));
    if (this->results == NULL)
        return (false);
    i = 0;
    while (i < this->dico_len)
    {
        this->results[i] = new(__Liste);
        if (this->results[i] == NULL)
            return (false);
        i += 1;
    }
    delete(stream);
    __methods(this);
    return (true);
}

int                 maman_dtor(Maman this)
{
    int             i = 0;

    while (i < this->dico_len)
    {
        delete(this->results[i]);
        i += 1;
    }
    free(this->results);
    delete(this->dictionaries);
    return (true);
}
