#include <unistd.h>

#include "modulary.h"
#include "curl_session.h"
#include "gumbo_session.h"

int main()
{
    CurlSession         curl = new(__CurlSession);
    GumboSession        gumbo = new(__GumboSession);
    Buffer              buffer = NULL;

    buffer = curl->get(curl, "http://www.gcgd.lu/");
    // printf("%s\n", ((String)(buffer))->content);
    gumbo->parse(gumbo, buffer);
    printf("Score total: %d\n", ((Analyser)gumbo)->count);

    delete(buffer);
    delete(curl);
    return (EXIT_SUCCESS);
}
