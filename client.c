/*
 * Author: Harrison Pierce
 * File: client.c
 * Date: 10/20/19
 * Description: Socket Client using TCP as transport
 * Note:
 *  The inputs are: ip address, port number and a string to pass to the server.
 *  Ex. client 127.0.0.1 5555 "Welcome to CSCI 345!"
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
	SOCKET clientsock = INVALID_SOCKET;
#else
    int clientsock;
#endif

    int rc = 0;
    int port = 0;
    char inputbuffer[512] = {'\0'};
    /* int addresslength = 0; */
    /* char outputbuffer[512] = {'\0'}; */

    struct sockaddr_in send_to;
    /* struct sockaddr recv_from; */

    if (argc < 4) {
        fprintf(stdout,"Usage: %s <ip address> <port> <string>\n", argv[0]);
        return -1;
    }

#ifdef WIN32
    /* Set rc variable to the windows socket, checks the version of the
    Windows socket, and check for errors. WSADATA is the highest version supported by
    the Windows socket DLL.
    If rc variable is not equal to zero, print an error message and return rc*/

	rc = WSAStartup(MAKEWORD(2,2), &wsaData);
	if ( rc != 0 ) {
       fprintf(stdout,"WSAStartup failed with error: %d\n",rc);
	   return rc;
	}
#endif

    /*
        strcmp compares two strings. points to next string in argv.
        if not equal to 4444, set command line argument to an int.
    */
    if ( strcmp(argv[2],"") ) {
        port = 4444;
    } else {
        port = atoi(argv[2]);
    }

    /* set clientsock variable. If above 0, print error message.
        otherwise print client socket created */
    clientsock = socket(AF_INET, SOCK_STREAM, 0);
    if ( clientsock < 0 ) {
        perror("Error opening socket");
        return rc;
    } else {
        fprintf(stdout, "Client socket created...\n");
    }

    /* Set memory block and send message on a socket to the size of the socket  */
    memset(&send_to,'\0',sizeof(send_to));

    /* send message on socket to an ipv4 address in the 1st variable in the argv array
     and convert address to binary.
       convert port betweek host and network byte */
    send_to.sin_addr.s_addr = inet_addr(argv[1]);
    send_to.sin_family = AF_INET;
    send_to.sin_port = htons(port);

    /* set rc to connect between client socket address
    if the rc variable is less than 0, print an error message.
    if not, print client socket connected*/
    rc = connect(clientsock, (struct sockaddr *)&send_to, sizeof(send_to));
    if ( rc < 0 ) {
        perror("Error connecting");
#ifdef WIN32
        fprintf(stdout,"Error from winsock: %d\n",WSAGetLastError());
#endif
        return rc;
    } else {
        fprintf(stdout, "Client socket connected...\n");
    }

    /* if strings in argv [3] equal to 0, then copy string. create input buffer where argv is 3 */
    if ( strcmp(argv[3], "") == 0 ) {
        strcpy(inputbuffer, "");
    } else {
        strcpy(inputbuffer, argv[3]);
    }

    /* set rc variable to send client socket, inputbuffer to
        the socket of a user defined socket address. if less than zero, print error message
        if not, print write message with inputbuffer variable*/
    rc = sendto(clientsock, inputbuffer, sizeof(inputbuffer), 0, (struct sockaddr *)&send_to, sizeof(send_to) );
    if ( rc < 0 ) {
        perror("Error writing socket");
#ifdef WIN32
        fprintf(stdout,"Error from Winsock: %d\n",WSAGetLastError());
#endif
        return rc;
    } else {
        fprintf(stdout, "----> Client socket write to server: %s\n", inputbuffer);
    }

     /*
    addresslength = sizeof(recv_from);
    rc = recvfrom(clientsock, outputbuffer, sizeof(outputbuffer), 0, &recv_from, &addresslength);
    if ( rc < 0 ) {
        perror("Error reading socket");
#ifdef WIN32
        fprintf(stdout,"Error from Winsock: %d\n",WSAGetLastError());
#endif
        return rc;
    } else {
        fprintf(stdout, "----> Client socket read: %s\n", outputbuffer);
    }
    */

#ifdef WIN32
    /* close the existing socket*/
    rc = closesocket(clientsock);
#else
    /* set rs variable to the closesocket which should be equal to 0 if
    socket was closed properly. print error if rc less than zero. if not,
    print socket close message. Finally, return rc */
    rc = close(clientsock);
#endif

    if ( rc < 0 ) {
        perror("Error closing socket");
        return rc;
    } else {
        fprintf(stdout, "Client socket closed...\n");
    }

#ifdef WIN32
	WSACleanup();
#endif

    return rc;
}



