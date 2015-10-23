// ****************************************************************************
//                                                                            *
//                                  ANALYSER C                                *
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

char *types[]=
{
    "SIMPLE",
    "NOUN",
    "VERB",
};

static int          __word_type(Analyser this, char *type)
{
    int             i = 0;

    while (i < WORD_TYPE_LEN)
    {
        if (m_strcmp(type, types[i]) == 0)
            return (i);
        i += 1;
    }
    return (0);
}

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

static int          __analyse_word(Analyser this, char *word, t_jnode **words)
{
    int             i = 0;
    unsigned int    len = 0;
    int             type;
    char            *_text, *_type, *_score;

    word = m_strlowercase(word);
    m_epur(word, " \n\t", NULL);
    len = m_strlen(word, "");
    while (words[i] != NULL)
    {
        _text = ARRAY_CONTENT(words[i], 0);
        _type = ARRAY_CONTENT(words[i], 1);
        _score= ARRAY_CONTENT(words[i], 2);
        type = this->word_type(this, _type);
        if (this->analysers[type](this, _text, word, len) == 0)
            return (atoi(_score));
        i += 1;
    }
    return (0);
}

static int          __analyse_sentence(Analyser this, Buffer buffer, int coeff)
{
    Array           words = ((String)buffer)->split(buffer, " .-:", NULL);
    Iter            it = NULL;
    int             i = 0;
    int             score;
    t_jnode         *tmp = NULL;

    if (words == NULL || words->len == 0)
    {
        delete(words);
        return (false);
    }

    it = iter(words);
    while (it)
    {
        i = 0;
        tmp = this->dictionaries->head;
        while (tmp != NULL)
        {
            score = this->analyse_word(this, it->content, tmp->child->content);
            this->count[i] += ((coeff / words->len) * score);
            tmp = tmp->next;
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

static void         __methods(Analyser this)
{
    this->analyse_sentence = __analyse_sentence;
    this->analyse_word = __analyse_word;
    this->word_type = __word_type;
    this->analysers[SIMPLE] = __analyse_simple;
    this->analysers[VERB] = __analyse_verb;
    this->analysers[NOUN] = __analyse_noun;
}

int                 analyser_ctor(Analyser this, va_list *ap)
{
    int             i = 0;
    int             nb_dico = va_arg(*ap, int);

    this->dictionaries = va_arg(*ap, Json);
    this->count = m_malloc(nb_dico * sizeof(unsigned int));
    if (this->count == NULL)
        return (false);
    while (i < nb_dico)
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
