#include <stdio.h>
#include <string.h>

#define HTTP_REQUEST_MAX_SIZE 4096
#define HOSTNAME_MAX_SIZE 512
#define PORT_MAX_SIZE 6
#define URI_MAX_SIZE 4096
#define METHOD_SIZE 32

/* 
 * Determine whether or not the HTTP request contained in the request argument
 * is complete in the sense that the request has been received in its entirety.
 * Hint: this can be as simple as seeing if the request ends with the
 * end-of-headers character sequence.
 *
 * Input: request, a string containing an HTTP request.  Note that the way this
 *        function is currently defined, request must be a null-terminated
 *        string. As such, you can use string functions (strcp(), strstr(),
 *        strlen()) on request.  Alternatively (i.e., not null-terminated), you
 *        would need to modify the function definition to additionally pass in
 *        a length.
 * Output: 1 if request is a complete HTTP request, 0 otherwise
 * */
int is_complete_request(const char *request)
{
    char *end_of_header_seq = "\r\n\r\n";

    int request_len = strlen(request);

    if (
        request[request_len - 4] == end_of_header_seq[0] &&
        request[request_len - 3] == end_of_header_seq[1] &&
        request[request_len - 2] == end_of_header_seq[2] &&
        request[request_len - 1] == end_of_header_seq[3])
    {
        return 1; //Complete request
    }

    // printf("End of header sequence (figurative): %d, %d, %d, %d\n", end_of_header_seq[0], end_of_header_seq[1], end_of_header_seq[2], end_of_header_seq[3]);
    // printf("Request: %d, %d, %d, %d\n", request[request_len - 4], request[request_len - 3], request[request_len - 2], request[request_len - 1]);

    return 0; //Incomplete request
}

/* Parse an HTTP request, and copy each parsed value into the
 * appropriate string argument as a null-terminated string (remember the null
 * terminator!). Call is_complete_request() first to make sure the client has
 * sent the full request.
 * Input: method, hostname, port, uri - strings to which the
 *        corresponding parts parsed from the request should be
 *        copied, as strings.  The uri is the "path" part of the requested URL,
 *        including query string. If no port is specified, the port string
 *        should be populated with a string specifying the default HTTP port,
 *        i.e., "80".
 * Output: 1 if request is a complete HTTP request, 0 otherwise
 * */
int parse_request(const char *request, char *method,
                  char *hostname, char *port, char *uri)
{
    // printf("parse_request() call\n");

    if (is_complete_request(request) == 0)
    {
        return 0; //If not a complete request, return 0
    }

    //Get hostname
    char *tmp = strstr(request, "www.");
    int i;
    for (i = 0; i < strlen(tmp); i++)
    {
        if (tmp[i] != '/' && tmp[i] != ':')
        {
            // printf("tmp[i] = %c\n", tmp[i]);
            hostname[i] = tmp[i];
        }
        else
        {
            // printf("BREAK!\n");
            hostname[i] = '\0';
            break;
        }
    }

    //Check port
    if (tmp[i] == ':') //Spesified port
    {
        i += 1;
        int j = 0;

        for (; i < strlen(tmp); i++)
        {
            if (tmp[i] != '/')
            {
                port[j] = tmp[i];
                j++;
            }
            else
            {
                port[j] = '\0';
                break;
            }
        }
        // printf("Non-default port\nport = %s\n", port);
    }
    else //No spesified port
    {
        port[0] = '8';
        port[1] = '0';
        port[2] = '\0';
        // printf("Default port\nport = %s\n", port);
    }

    //Get uri
    if (tmp[i] == '/')
    {
        int j = 0;
        for (; i < strlen(tmp); i++)
        {
            if (tmp[i] != ' ')
            {
                // printf("tmp[i] = %c\n", tmp[i]);
                uri[j] = tmp[i];
                j++;
                // printf("uri[i] = %c\n", uri[i]);
            }
            else
            {
                uri[j] = '\0';
                break;
            }
        }
    }

    //Get method
    for (i = 0; i < strlen(request); i++)
    {
        if (request[i] != ' ')
        {
            method[i] = request[i];
        }
        else
        {
            method[i] = '\0';
            break;
        }
    }

    //Complete request, return 0
    // printf("parse_request() end\n");
    return 1;
}

int main()
{
    char *req1;
    char *req2;
    char *req3;

    char method[METHOD_SIZE];
    char hostname[HOSTNAME_MAX_SIZE];
    char port[PORT_MAX_SIZE];
    char uri[URI_MAX_SIZE];

    req1 = "GET http://www.example.com/index.html HTTP/1.0\r\n"
           "Host: www.example.com\r\n"
           "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0\r\n"
           "Accept-Language: en-US,en;q=0.5\r\n\r\n";

    req2 = "GET http://www.example.com:8080/index.html?foo=1&bar=2 HTTP/1.0\r\n"
           "Host: www.example.com:8080\r\n"
           "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0\r\n"
           "Accept-Language: en-US,en;q=0.5\r\n\r\n";

    req3 = "GET http://www.example.com:8080/index.html HTTP/1.0\r\n";

    if (parse_request(req1, method, hostname, port, uri))
    {
        printf("req1 method: %s\n", method);
        printf("req1 hostname: %s\n", hostname);
        printf("req1 port: %s\n", port);
        printf("req1 uri: %s\n", uri);
    }
    else
    {
        printf("req1 is incomplete\n");
    }

    if (parse_request(req2, method, hostname, port, uri))
    {
        printf("req2 method: %s\n", method);
        printf("req2 hostname: %s\n", hostname);
        printf("req2 port: %s\n", port);
        printf("req2 uri: %s\n", uri);
    }
    else
    {
        printf("req2 is incomplete\n");
    }

    if (parse_request(req3, method, hostname, port, uri))
    {
        printf("req3 method: %s\n", method);
        printf("req3 hostname: %s\n", hostname);
        printf("req3 port: %s\n", port);
        printf("req4 uri: %s\n", uri);
    }
    else
    {
        printf("req3 is incomplete\n");
    }
}