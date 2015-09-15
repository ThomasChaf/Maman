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

static int          __analyse_simple(Analyser this, char *word, char *to_analyse, unsigned int len)
{
    return (m_strcmp(word, to_analyse));
}

static int          __analyse_verb(Analyser this, char *word, char *to_analyse, unsigned int len)
{
    return (m_strcmp(word, to_analyse));
}

static int          __analyse_noun(Analyser this, char *word, char *to_analyse, unsigned int len)
{
    if (m_strcmp(word, to_analyse) == 0 || m_strcmp(m_strcat(word, "s"), to_analyse) == 0)
        return (0);
    return (-1);
}

static int          __analyse_word(Analyser this, char *word)
{
    int             i = 0;
    unsigned int    len = 0;

    // printf("[%s|", word);
    word = m_strlowercase(word);
    m_epur(word, " \n\t", NULL);
    len = m_strlen(word, "");
    // printf("%s]\n", word);
    while (g_dictionary[i].name != NULL)
    {
        if (this->analysers[g_dictionary[i].type](this, g_dictionary[i].name, word, len) == 0)
        {
            printf("%d [%s]\n", g_dictionary[i].score, word);
            return (g_dictionary[i].score);
        }
        i += 1;
    }
    printf("%d [%s]\n", 0, word);
    return (0);
}

static int          __analyse_sentence(Analyser this, Buffer buffer, int score)
{
    Array           words = ((String)buffer)->split(buffer, " .-:", NULL);
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
    this->analysers[SIMPLE] = __analyse_simple;
    this->analysers[VERB] = __analyse_verb;
    this->analysers[NOUN] = __analyse_noun;
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
