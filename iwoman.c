#include "headers.h"

void GetTheMan(char* search_command)
{
    struct addrinfo* res;   
    struct addrinfo hints;   

    memset(&hints, 0, sizeof(hints));

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = 0;

    int add = getaddrinfo("man.he.net", "80", &hints, &res);
    
    if(add != 0)
    {
        perror(RED"get address info"reset);
    }

    int sockfile = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(sockfile == -1)
    {
        perror(RED"socket"reset);
    }

    int conn = connect(sockfile, res->ai_addr, res->ai_addrlen);

    if(conn == -1)
    {
        perror(RED"connect"reset);
    }

    char* header = calloc(1500, 1);
    header[0] = '\0';
    char hpartone[13] = "GET /?topic=";
    char hparttwo[50] = "&section=all HTTP/1.1\r\nHost: man.he.net\r\n\r\n";
    
    strcat(header, hpartone);
    strcat(header, search_command);
    strcat(header, hparttwo);

    ssize_t sze = send(sockfile, header, strlen(header), 0);

    if(sze == -1)
    {
        perror(RED"send"reset);
    }

    char* cleaned = calloc(1501, 1);
    char* html = calloc(1501, 1);
    ssize_t szes;
    int count = 0;
    int* inside = malloc(sizeof(int));
    *inside = 1;

    while((szes = recv(sockfile, html, 1500, 0)) > 0)
    {
        // printf("szes: %ld\n", szes);
        html[szes] = '\0';

        count++;
        if(count == 1)
        {
            //check if invalid
            char* invalid = strcasestr(html, "No matches for");

            if(invalid != NULL)
            {
                fprintf(stderr, RED"No matches for '%s'\n"reset, search_command);
                return;
            }

            //remove initial data
            int b = 5;
            char* start = html;
            char* next;
            while(b--)
            {
                start = strcasestr(start + 1, search_command);
                // printf("AAAAAAA %ld\n", start - html);
            }

            long int k = start - html;
            char* new = calloc(1501, 1);
            strcpy(new, html + k);
            // printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
            // printf("html: %s\n", html);
            // printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");   
            CleanHTML(new, cleaned, search_command, inside);

            free(new);
        }
        else
        {
            // printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
            // printf("html: %s\n", html);
            // printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
            CleanHTML(html, cleaned, search_command, inside);
        }
    }

    if(szes == -1)
    {
        perror(RED"recv"reset);
    }

    free(html);
    free(header);
    free(cleaned);

}

void CleanHTML(char* html, char* cleaned, char* search_command, int* inside)
{
    // printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    int a = 0;
    // printf("inside = %d\n", *inside);
    for(int i = 0; html[i] != '\0'; i++)
    {
        if(html[i] == '<')
        {
            // printf("~0~");
            *inside = 0;
        }
        if(html[i] == '&')
        {
            // if(html[i + 1] == '\0' || html[i + 2] == '\0' || html[i + 3] == '\0')
            // {
            //     printf(RED"Error: Unexpected '&' within html"reset);
            //     return;
            // }

            if(html[i + 1] == 'l' && html[i + 2] == 't' && html[i + 3] == ';')
            {
                putchar('<');

                i = i + 3;
            }
            if(html[i + 1] == 'g' && html[i + 2] == 't' && html[i + 3] == ';')
            {
                putchar('>');

                i = i + 3;
            }
        }
        else if(*inside == 1)
        {
            putchar(html[i]);
        }
        
        if(html[i] == '>')
        {
            // printf("~1~");
            *inside = 1;
        }
    }
}