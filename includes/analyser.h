// ****************************************************************************
//                                                                            *
//                                 ANALYSER H                                 *
//                                                                            *
// Created by Thomas Chafiol  -  thomaschaf@gmail.com  -  10  /  09  /  2015  *
// *****************************************************************************

#ifndef __ANALYSER_H__
#define __ANALYSER_H__

#include "modulary.h"

typedef enum    e_word_type {
    SIMPLE,
    NOUN,
    VERB,
    WORD_TYPE_LEN
}               t_word_type;

typedef struct  s_dictionary
{
    char        *name;
    t_word_type type;
    int         score;
}               t_dictionary;

typedef struct      s_analyser
{
    t_module        module;
    unsigned int    count;
    fct             analyse_sentence;
    fct             analyse_word;
    fct             analysers[WORD_TYPE_LEN];
}                   t_analyser;

typedef t_analyser *    Analyser;

int                 analyser_ctor(Analyser);
int                 analyser_dtor();

extern char         *__ANALYSER;
extern t_module     __Analyser;

#endif /* __ANALYSER_H__ */
