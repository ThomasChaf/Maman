// ****************************************************************************
//                                                                            *
//                            GUMBO SESSION C                                 *
//                                                                            *
// Created by Thomas Chafiol  -  thomaschaf@gmail.com  -  10  /  09  /  2015  *
// *****************************************************************************

#include <stdio.h>
#include "gumbo.h"

#include "modulary.h"
#include "gumbo_session.h"

// ****************************************************************************
//                                                                            *
//                                  METHODS                                   *
//                                                                            *
// ****************************************************************************

t_element_rank      g_target_element[]= {
    {GUMBO_TAG_TITLE, 50},
    {GUMBO_TAG_A, 20},
    {GUMBO_TAG_H1, 50},
    {GUMBO_TAG_H2, 40},
    {GUMBO_TAG_H3, 30},
    {GUMBO_TAG_H4, 20},
    {GUMBO_TAG_H5, 20},
    {GUMBO_TAG_P, 40},
    {-1, 0}
};

int                     __get_text(GumboSession this, const GumboNode *element, Buffer buffer)
{
    const GumboVector   *childrens;
    int                 i;

    if (element->type == GUMBO_NODE_TEXT)
    {
        buffer->concat(buffer, element->v.text.text, m_strlen((char *)element->v.text.text, ""));
    }
    else if (element->type == GUMBO_NODE_ELEMENT)
    {
        childrens = &element->v.element.children;
        i = 0;
        while (i < childrens->length)
        {
            this->get_text(this, childrens->data[i], buffer);
            i += 1;
        }
    }
    return (true);
}

static int          __is_target_element(GumboSession this, const GumboNode *element)
{
    int             j = 0;

    while (g_target_element[j].tag != -1)
    {
        if (g_target_element[j].tag == element->v.element.tag)
            return (g_target_element[j].score);
        j += 1;
    }
    return (0);
}

static int              __analyse(GumboSession this, const GumboNode* root)
{
    Analyser            _this = (Analyser)this;
    GumboNode           *child;
    const GumboVector   *root_children = &root->v.element.children;
    int                 i = 0;
    Buffer              buffer = NULL;
    int                 score;

    if (root->type != GUMBO_NODE_ELEMENT)
        return (true);
    while (i < root_children->length)
    {
        child = root_children->data[i];
        if ((score = this->is_target_element(this, child)) > 0)
        {
            buffer = new(__Buffer);
            this->get_text(this, child, buffer);
            _this->analyse_sentence(this, buffer, score);
            delete(buffer);
        }
        else
            this->analyse(this, child);
        i += 1;
    }
    return (true);
}


static int              __parse(GumboSession this, Buffer buffer)
{
    GumboOutput         *output = gumbo_parse(((String)buffer)->content);

    this->analyse(this, output->root);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    return (true);
}

// ****************************************************************************
//                                                                            *
//                                    CLASS                                   *
//                                                                            *
// ****************************************************************************

char *__GUMBO_SESSION = "gumbo_session";

t_module __GumboSession =  { sizeof(t_gumbo_session),  gumbo_session_ctor, gumbo_session_dtor,
    "gumbo_session",
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

static void         __methods(GumboSession this)
{
    this->parse = __parse;
    this->analyse = __analyse;
    this->is_target_element = __is_target_element;
    this->get_text = __get_text;
}

int                 gumbo_session_ctor(GumboSession this, va_list *ap)
{
    if (super(this, __Analyser, va_arg(*ap, int), va_arg(*ap, Json)) == false)
        return (false);
    __methods(this);
    return (true);
}

int                 gumbo_session_dtor(GumboSession this)
{
    return (true);
}
