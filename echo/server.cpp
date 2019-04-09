#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char **argv) {
    char *file_path = "Field.txt";
    char buffer[BUFSIZ];
    char protoname[] = "tcp";
    int client_sockfd;
    int enable = 1;
    int filefd;
    int i;
    int server_sockfd;
    socklen_t client_len;
    ssize_t read_return;
    struct protoent *protoent;
    struct sockaddr_in client_address, server_address;
    unsigned short server_port = 50002;

    if (argc > 1) {
        file_path = argv[1];
        if (argc > 2) {
            server_port = strtol(argv[2], NULL, 10);
        }
    }

    /* Create a socket and listen to it.. */
    protoent = getprotobyname(protoname);
    if (protoent == NULL) {
        perror("getprotobyname");
        exit(EXIT_FAILURE);
    }
    server_sockfd = socket(
            AF_INET,
            SOCK_STREAM,
            protoent->p_proto
    );
    if (server_sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        exit(EXIT_FAILURE);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(server_port);
    if (bind(
            server_sockfd,
            (struct sockaddr*)&server_address,
            sizeof(server_address)
    ) == -1
            ) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_sockfd, 2) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "listening on port %d\n", server_port);

    while (1) {
        client_len = sizeof(client_address);
        puts("waiting for client");
        client_sockfd = accept(
                server_sockfd,
                (struct sockaddr*)&client_address,
                &client_len
        );
        filefd = open(file_path,
                      O_WRONLY | O_CREAT | O_TRUNC,
                      S_IRUSR | S_IWUSR);
        if (filefd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        do {
            read_return = read(client_sockfd, buffer, BUFSIZ);
            if (read_return == -1) {
                perror("read");
                exit(EXIT_FAILURE);
            }
            if (write(filefd, buffer, read_return) == -1) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        } while (read_return > 0);
        close(filefd);
        close(client_sockfd);
    }
    return EXIT_SUCCESS;
}



/*#include <stdio.h> // standard input and output library
#include <stdlib.h> // this includes functions regarding memory allocation
#include <string.h> // contains string functions
#include <errno.h> //It defines macros for reporting and retrieving error conditions through error codes
#include <iostream> //contains various functions for manipulating date and time
#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
#include <fstream>
#include "game_proj.h"

bool readfile(std::string& s, const char* filename) {
    std::ifstream fp(filename);
    if (!fp.is_open())
        return false;

    char nil = '\0';
    std::getline(fp, s, nil);
    fp.close();
    return (s.length() > 0);
}
int main()
{
    TwoArray<char> arr;
    std::string word;
    int n =0;
    const void *s;
    std::string p;
    //time_t clock;
    char dataSending[1024]; // Actually this is called packet in Network Communication, which contain data and send through.
    int clintListn = 0, clintConnt = 0;
    struct sockaddr_in addr;
    //FILE * f;

    clintListn = socket(AF_INET, SOCK_STREAM, 0); // creating socket
    memset(&addr, '0', sizeof(addr));
    memset(dataSending, '0', sizeof(dataSending));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(50002); 		// this is the port number of running server

    bind(clintListn, (struct sockaddr*)&addr , sizeof(addr));
    listen(clintListn , 2);

    while(1)
    {
        printf("\nHi,Iam running server.Some Client hit me\n"); // whenever a request from client came. It will be processed here.
        clintConnt = accept(clintListn, (struct sockaddr*)NULL, NULL);
        //word = 'wordword';
        //s = &word;
        //std::string news = "Field.txt";
        //std::string news = readFile("Field.txt", dataSending);
        //snprintf(dataSending, sizeof(dataSending), "You entered %s: ", s); // Printing successful message
        //send(clintConnt, dataSending, strlen(dataSending),12);
        //n = write(clintListn, dataSending, strlen(dataSending));
        //if (n < 0)
        //perror("ERROR writing to socket");
        //write(clintConnt, dataSending, strlen(dataSending));
        //s = &p;
        //if (readfile(p, "file.txt")){
            //fprintf(f, "You entered %s: \n"); // Printing successful message
        //write(clintConnt, s, sizeof(s));

        close(clintConnt);
        sleep(1);
    }
//}
    return 0;
}*/


/*#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 50002

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}*/

/*#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include<stdio.h>

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;
    listener = socket(AF_INET, SOCK_STREAM, 0);

    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(50002);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        exit(2);
    }

    listen(listener, 1);

    while(1)
    {
        sock = accept(listener, NULL, NULL);
        if(sock < 0)
        {
            perror("accept");
            exit(3);
        }

        while(1)
        {
            bytes_read = recv(sock, buf, 1024, 0);
            if(bytes_read <= 0)
                break;
            //ifstream in("map1.txt");
            int i=0;
            FILE * fd = fopen( "map1.txt", "rb" );
            for ( i = 0; i < '\0'; ++i ) {
                buf[ i ] = i;
                std::cout<<buf[i]<<std::endl;
            }
            fwrite( buf, sizeof( char ), sizeof( buf ) / sizeof( char ), fd );
            fclose( fd );
            send(sock, buf, bytes_read, 0);
        }
        close(sock);
    }

    return 0;
}*/
/*//clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if(sd > 0)
                FD_SET( sd , &readfds);

            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
                max_sd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);

        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }

  sd = client_socket[i];

            if (FD_ISSET( sd , &readfds))
            {
                //Check if it was for closing , and also read the incoming message
                if ((valread = read( sd , buffer, 1024)) == 0)
                {
                    //Somebody disconnected , get his details and print
                    getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

                    //Close the socket and mark as 0 in list for reuse
                    close( sd );
                    user_count--;
                    client_socket[i] = 0;
                }

                //Echo back the message that came in
                else
                {

                    //set the string terminating NULL byte on the end of the data read

                        buffer[valread] = '\0';
                        for (i = 0; i < max_clients; i++)
                        {
                            sd = client_socket[i];
                        send(sd , buffer , strlen(buffer) , 0 );
                        }
                        buffer[1024] = {0};
                }
            }
            void *server(void *);

 pthread_create(&threadA[0], NULL, server, NULL);

                                pthread_join(threadA[0], NULL);*/