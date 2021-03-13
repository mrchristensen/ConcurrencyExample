#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 4096

int main()
{
    char *promt = "The query string is: ";
    const char *Q = getenv("QUERY_STRING");

    int response_size = strlen(promt) + strlen(Q);

    char response_body[response_size + 1];

    sprintf(response_body, "%s%s", promt, Q);

    printf("Content-type: text/plain\r\nContent-length: %ld\r\n%s\r\n\r\n", sizeof(response_body), response_body);
}