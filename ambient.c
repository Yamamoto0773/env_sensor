#include "ambient.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


#define return_with_msg(msg) { perror(msg); return -1; }


int ambient_send(const char* channel_id, const char* write_key, struct ambient_data* data) {
    const int portno = 80;
    const char* host = "ambidata.io";
    
    // create http request message
    char *body_fmt =
    "{"
        "\"writeKey\":\"%s\"," // write key
        "\"d1\":\"%.1f\","     // data 1 temp
        "\"d2\":\"%d\","       // data 2 rh
        "\"d3\":\"%.2f\","     // data 3 pressure
        "\"d4\":\"%d\""        // data 4 co2
    "}";

    char body_msg[1024];
    sprintf(body_msg, body_fmt, write_key, data->temp, (int)data->rh, data->pressure, data->co2);
    
    char *message_fmt =
        "POST /api/v2/channels/%s/data HTTP/1.1\r\n" // channnel_id
        "Host: %s\r\n"                               // host
        "User-Agent: raspberry-pi\r\n"
        "Accept: */*\r\n"        
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"                     // content-length
        "\r\n"
        "%s";                                        // body
    
    char message[1024];
    sprintf(message, message_fmt, channel_id, host, strlen(body_msg), body_msg);        

    /* create the socket */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return_with_msg("ERROR opening socket");

    /* lookup the ip address */
    struct hostent* server = gethostbyname(host);
    if (server == NULL) return_with_msg("ERROR, no such host");

    /* fill in the structure */
    struct sockaddr_in serv_addr;
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    // set timeout opetion
    struct timeval tv;
    tv.tv_sec = 5; // timeout 5 sec
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    /* connect the socket */
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
        return_with_msg("ERROR connecting");

    /* send the request */
    int total = strlen(message);
    int sent = 0;
    do {
        int bytes = write(sockfd,message+sent,total-sent);
        if (bytes < 0)
            return_with_msg("ERROR writing message to socket");
        if (bytes == 0)
            break;
        sent+=bytes;
    } while (sent < total);

    /* receive the response */
    char response[4096];
    memset(response,0,sizeof(response));
    total = sizeof(response)-1;
    int received = 0;
    do {
        int bytes = read(sockfd, response + received, total - received);
        if (bytes < 0)
            return_with_msg("ERROR reading response from socket");
        if (bytes == 0)
            break;
        received += bytes;
    } while (received < total);

    if (received == total)
        return_with_msg("ERROR storing complete response from socket");

    /* close the socket */
    close(sockfd);

    return 0;
}
