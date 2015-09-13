// ****************************************************************************
//                                                                            *
//                             CURL SESSION C                                 *
//                                                                            *
// Created by Thomas Chafiol  -  thomaschaf@gmail.com  -  10  /  09  /  2015  *
// *****************************************************************************

#include <stdio.h>

#include "modulary.h"
#include "analyser.h"

// ****************************************************************************
//                                                                            *
//                                  METHODS                                   *
//                                                                            *
// ****************************************************************************

t_dictionary        g_dictionary[]= {
    { "ace", NOUN, 50 },
    { "chip", VERB, 60 },
    { "birdie", NOUN, 80 },
    { "bogey", NOUN, 80 },
    { "bunker", NOUN, 60 },
    { "course", NOUN, 90 },
    { "club", NOUN, 70 },
    { "clubhouse", NOUN, 70 },
    { "fairway", NOUN, 70 },
    { "game", NOUN, 50 },
    { "golf", NOUN, 100 },
    { "hole", NOUN, 70 },
    { "iron", NOUN, 60 },
    { "member", NOUN, 50 },
    { "par", NOUN, 60 },
    { "pin", SIMPLE, 60 },
    { "pga", SIMPLE, 70 },
    { "putter", NOUN, 70 },
    { "score", NOUN, 60 },
    { "scorecard", NOUN, 60 },
    { "swing", VERB, 70 },
    { "tee", NOUN, 60 },
    { NULL, 0, 0}
};

static int          __analyse_word(Analyser this, char *word)
{
    int             i = 0;

    while (g_dictionary[i].name != NULL)
    {
        if (m_strcmp(g_dictionary[i].name, word) == 0)
            return (g_dictionary[i].score);
        i += 1;
    }
    return (0);
}

static int          __analyse_sentence(Analyser this, Buffer buffer, int score)
{
    Array           words = ((String)buffer)->split(buffer, " ", NULL);
    Iter            it = NULL;

    if (words == NULL || words->len == 0)
    {
        delete(words);
        return (false);
    }

    it = iter(words);
    while (it)
    {
        this->count += ((score / words->len) * this->analyse_word(this, it->content));
        printf("%d %s\n", this->count, it->content);
        it = next(it);
    }
    delete(it);
    delete(words);
    return (true);
}

// ****************************************************************************
//                                                                            *
//                                    CLASS                                   *
//                                                                            *
// ****************************************************************************

char *__ANALYSER = "analyser";

t_module __Analyser =  { sizeof(t_analyser),  analyser_ctor, analyser_dtor,
    "analyser",
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

static void           __methods(Analyser this)
{
    this->analyse_sentence = __analyse_sentence;
    this->analyse_word = __analyse_word;
}

int                   analyser_ctor(Analyser this)
{
    this->count = 0;
    __methods(this);
    return (true);
}

int                   analyser_dtor(Analyser this)
{
    return (true);
}
