#include <unistd.h>

#include "modulary.h"
#include "curl_session.h"
#include "gumbo_session.h"

int main()
{
    CurlSession         curl = new(__CurlSession);
    GumboSession        gumbo = new(__GumboSession);
    Buffer              buffer = NULL;

    buffer = curl->get(curl, "http://t-chafiol.fr/en/about");
    // printf("%s\n", ((String)(buffer))->content);
    gumbo->parse(gumbo, buffer);

    delete(buffer);
    delete(curl);
    return (EXIT_SUCCESS);
}
