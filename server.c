/*
 * Author:
 * File: server.c
 * Date:
 * Description: Socket Server using TCP as the transport and entry point for a client connection
 * Note:
 *  The inputs are: port number.
 *  Ex. server 5555
 *
 * Edits
 *
 * 10/01/2019 SDN - Code creation
 *
 */
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
 #include <windows.h>
 #include <winsock2.h>
 #include <ws2tcpip.h>
#else
 #include <string.h>
 #include <unistd.h>
 #include <netdb.h>
#endif

int main(int argc, char *argv[])
{

#ifdef WIN32
	WSADATA wsaData;
	SOCKET listensocket = INVALID_SOCKET;
	SOCKET acceptsocket = INVALID_SOCKET;
#else
    int listensocket;
    int acceptsocket;
#endif

    int rc = 0;
    int port = 0;
    int clientaddrlen = 0;
    int fromaddresslength = 0;
    char buffer[255] = {'\0'};

    struct sockaddr_in serveraddress, clientaddress;
    struct sockaddr recv_from;

    if (argc < 2) {
        fprintf(stdout,"Usage: server <port>\n");
        return -1;
    }

#ifdef WIN32
    /*  Set rc variable to the windows socket, checks the version of the
    Windows socket, and check for errors. WSADATA is the highest version supported by
    the Windows socket DLL.
    If rc variable is not equal to zero, print an error message and return rc */
	rc = WSAStartup(MAKEWORD(2,2), &wsaData);
	if ( rc != 0 ) {
       fprintf(stdout,"WSAStartup failed with error: %d\n",rc);
	   return rc;
	}
#endif

    /* strcmp compares two strings. points to next string in argv[1] .
        if not equal to 4444, set command line argument to an int. */
    if ( strcmp(argv[1],"") ) {
        port = 4444;
    } else {
        port = atoi(argv[2]);
    }

    /* set listensock variable. If above 0, print error message.
        otherwise print server socket created */
    listensocket = socket(AF_INET, SOCK_STREAM, 0);
    if ( listensocket < 0 ) {
        perror("Error opening socket");
        return rc;
    } else {
        fprintf(stdout, "Server socket has been created...\n");
    }

    /* Set memory block of server address */
    memset(&serveraddress,'\0',sizeof(serveraddress));

    /* perform ipv4 check and do not bind socket to specific ip address. convert to binary*/
    serveraddress.sin_family = AF_INET;
    serveraddress.sin_addr.s_addr = INADDR_ANY;
    serveraddress.sin_port = htons(port);

    /* connect to client through given port. if connection is unsuccessful,
    print error message, if successful, print bind message o*/
    rc = bind(listensocket, (struct sockaddr *) &serveraddress, sizeof(serveraddress));
    if ( rc < 0 ) {
        perror("Error binding");
        return rc;
    } else {
        fprintf(stdout, "Server socket can bind a port...\n");
    }

    /* set rc to listem and set listensocket variable at 5.
    if result is less than 5, print error message, otherwise print listening message */
    rc = listen(listensocket, 5);
    if ( rc < 0 ) {
        perror("Error Listening");
        return rc;
    } else {
        fprintf(stdout, "Server socket is listening...\n");
    }

    clientaddrlen = sizeof(clientaddress);

    while(1) {
        /* while th socket is open print connected message and listen for message from client */
        acceptsocket = accept(listensocket, (struct sockaddr *) &clientaddress, &clientaddrlen);
        if ( acceptsocket < 0 ) {
           perror("Error creating client socket");
           return rc;
       } else {
           fprintf(stdout, "----> Client socket is connected...\n");
       }

       /* accept message from ip address and binded port. if unsuccessful, print error message
       otherwise, print data message and message from client*/
       fromaddresslength = sizeof(recv_from);
       rc = recvfrom(acceptsocket, buffer, sizeof(buffer),0, (struct sockaddr *)&recv_from, &fromaddresslength);
       if ( rc < 0 ) {
           perror("Error reading client socket");
           return rc;
       } else {
           fprintf(stdout, "----> Received data from client: %s\n", buffer);
       }

#ifdef WIN32
    /* set rc to close the socket */
    rc = closesocket(acceptsocket);
#else
    /* if socket is closed unsuccessfully, print error message, otherwise print success message
    that client socket has closet. there are no further messages*/
    rc = close(acceptsocket);
#endif

       if ( rc < 0 ) {
           perror("Error closing client socket");
           return rc;
       } else {
           fprintf(stdout, "----> Client socket is closed...\n");
       }
    } // End While Loop

#ifdef WIN32
    /* Your comments */
	WSACleanup();
#endif
    return rc;
}



