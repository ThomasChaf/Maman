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

t_dictionary_word       g_dictionary_golf[]= {
    { "ace", NOUN, 10 },
    { "chip", VERB, 30 },
    { "birdie", NOUN, 50 },
    { "bogey", NOUN, 50 },
    { "bunker", NOUN, 90 },
    { "country", NOUN, 80 },
    { "course", NOUN, 90 },
    { "club", NOUN, 80 },
    { "clubhouse", NOUN, 60 },
    { "fairway", NOUN, 70 },
    { "golf", NOUN, 100 },
    { "hole", NOUN, 90 },
    { "par", NOUN, 60 },
    { "pin", SIMPLE, 30 },
    { "pga", SIMPLE, 30 },
    { "putter", NOUN, 30 },
    { "score", NOUN, 60 },
    { "scorecard", NOUN, 60 },
    { "swing", VERB, 50 },
    { "greenfee", NOUN, 80 },
    { "tee", NOUN, 60 },
    { NULL, 0, 0}
};

t_dictionary_word       g_dictionary_porno[]= {
    { "anal", NOUN, 90 },
    { "porn", NOUN, 100 },
    { "fuck", VERB, 80 },
    { "suck", VERB, 90 },
    { "cock", NOUN, 70 },
    { "pornstar", NOUN, 70 },
    { "blowjob", NOUN, 70 },
    { "lesbian", NOUN, 60 },
    { NULL, 0, 0}
};

t_dictionary        g_dictionaries[]= {
    { "golf", g_dictionary_golf },
    { "porno", g_dictionary_porno },
    { NULL, NULL }
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

static int          __analyse_word(Analyser this, char *word, t_dictionary_word *words)
{
    int             i = 0;
    unsigned int    len = 0;

    word = m_strlowercase(word);
    m_epur(word, " \n\t", NULL);
    len = m_strlen(word, "");
    while (words[i].name != NULL)
    {
        if (this->analysers[words[i].type](this, words[i].name, word, len) == 0)
        {
	  //            printf("%d [%s]\n", words[i].score, word);
            return (words[i].score);
        }
        i += 1;
    }
    // printf("%d [%s]\n", 0, word);
    return (0);
}

static int          __analyse_sentence(Analyser this, Buffer buffer, int coeff)
{
    Array           words = ((String)buffer)->split(buffer, " .-:", NULL);
    Iter            it = NULL;
    int             i = 0;
    int             score;

    if (words == NULL || words->len == 0)
    {
        delete(words);
        return (false);
    }

    it = iter(words);
    while (it)
    {
        i = 0;
        while (g_dictionaries[i].name != NULL)
        {
            score = this->analyse_word(this, it->content, g_dictionaries[i].words);
            this->count[i] += ((coeff / words->len) * score);
            i += 1;
        }
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
    int             i = 1;

    while (g_dictionaries[i].name != NULL)
        i += 1;
    this->count = m_malloc(i * sizeof(unsigned int));
    if (this->count == NULL)
        return (false);
    i = 0;
    while (g_dictionaries[i].name != NULL)
    {
        this->count[i] = 0;
        i += 1;
    }
    __methods(this);
    return (true);
}

int                   analyser_dtor(Analyser this)
{
    free(this->count);
    return (true);
}
