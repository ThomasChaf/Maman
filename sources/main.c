#include <unistd.h>
#include <stdio.h>

#include "modulary.h"
#include "maman.h"

int             maman_from_list(char *list)
{
    char        filename[128];
    int         i = 0;
    Maman       maman = new(__Maman, list);
    Iter        it;

    if (maman == NULL)
        return (false);
    it = iter(maman);
    if (it == NULL)
        return (false);
    while (it != NULL)
    {
        sprintf(filename, "phantom-app/sites/fichier%d", i);
        maman->analyse(maman, MAMAN_WEB_FILE, filename, it->content);
        i += 1;
        it = next(it);
    }
    maman->publish_results(maman);
    delete(maman);
    return (true);
}

int             simple_maman(char *filename, char *descriptor)
{
    Maman       maman = new(__Maman, NULL);

    if (maman == NULL)
        return (false);
    if (maman->analyse(maman, MAMAN_WEB_FILE, filename, descriptor) == false)
    {
        delete(maman);
        return (false);
    }
    maman->publish_results(maman);
    delete(maman);
    return (true);
}

int             main(int argc, char **argv)
{
    if (argc == 2)
    {
        if (maman_from_list(argv[1]) == false)
            return (EXIT_FAILURE);
    }
    else if (argc == 3)
    {
        if (simple_maman(argv[1], argv[2]) == false)
            return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
