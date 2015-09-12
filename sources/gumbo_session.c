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

static const char *find_title(const GumboNode* root, int i)
{
    if (root->type != GUMBO_NODE_ELEMENT)
    {
        // printf("==============%d=%d===========\n", root->type, GUMBO_NODE_ELEMENT);
        if (root->type == GUMBO_NODE_TEXT && root->v.element.tag <= GUMBO_TAG_LAST)
            printf("%s\n", root->v.text.text);
        return (NULL);
    }

    const GumboVector* root_children = &root->v.element.children;
    GumboNode* head = NULL;
    for (int i = 0; i < root_children->length; ++i)
    {
        GumboNode* child = root_children->data[i];
        if (child->v.element.tag <= GUMBO_TAG_LAST)
            printf("%d %s\n", i, gumbo_normalized_tagname(child->v.element.tag));
        find_title(child, i + 1);
    }
    return NULL;
}


static int          __parse(GumboSession this, Buffer buffer)
{
    GumboOutput     *output = gumbo_parse(((String)buffer)->content);

    find_title(output->root, 0);

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

static void           __methods(GumboSession this)
{
    this->parse = __parse;
}

int                   gumbo_session_ctor(GumboSession this)
{

    __methods(this);
    return (true);
}

int                   gumbo_session_dtor(GumboSession this)
{
    return (true);
}
