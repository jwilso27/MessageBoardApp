// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include "../program4.h"
#include "board.h"

#include <string>
#include <map>

using namespace std;

#define MAX_PENDING 5   

int main(int argc, char * argv[]){
    // initialize parameters
    FILE *fp;
    struct sockaddr_in udp_sin, tcp_sin, client_addr;
    char cmd[4], buf[MAX_LINE], *username, *input_passwd, *passwd, *admin_passwd;
    int port, udp_s, tcp_s, new_tcp;
    int opt = 1, size = sizeof(struct sockaddr), flag = 1;
    short int len;
    map< string, Board > boards;     
    map< string, string > users;

    // check arguments
    if (argc == 3) {
        port = atoi(argv[1]);
        admin_passwd = argv[2];
    } else {
        fprintf(stderr, "usage: ./myfrmd <port> <password>\n");
        exit(1);
    }

    // build address data structure (udp)
    bzero( (char*)&udp_sin, sizeof(udp_sin) );
    udp_sin.sin_family = AF_INET;
    udp_sin.sin_addr.s_addr = INADDR_ANY;
    udp_sin.sin_port = htons(port);

    // build address data structure (tcp)
    bzero( (char*)&tcp_sin, sizeof(tcp_sin) );
    tcp_sin.sin_family = AF_INET;
    tcp_sin.sin_addr.s_addr = INADDR_ANY;
    tcp_sin.sin_port = htons(port);

    // passive open (udp)
    if ( ( udp_s = socket( PF_INET, SOCK_DGRAM, 0 ) ) < 0 ) {
        perror("socket error");
        exit(1);
    }

    // passive open (tcp)
    if ( ( tcp_s = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 ) {   
        perror("socket error");
        close( udp_s );
        exit(1);    
    }   

    // set socket option   
    if ( ( setsockopt( tcp_s, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int) ) ) < 0 ) {   
        perror ("socket option error");  
        close( udp_s );
        close( tcp_s );
        exit(1);
    }   

    // bind socket (udp)
    if ( ( bind( udp_s, (struct sockaddr*)&udp_sin, sizeof(udp_sin) ) ) < 0 ) {
        perror("bind error");
        close( udp_s );
        close( tcp_s );
        exit(1);
    }
    
    // bind socket (tcp)
    if ( ( bind( tcp_s, (struct sockaddr *)&tcp_sin, sizeof(tcp_sin) ) ) < 0 ) {   
        perror("bind error");
        close( udp_s );
        close( tcp_s );
        exit(1);
    }

    // listen for client
    if ( ( listen( tcp_s, MAX_PENDING ) ) < 0 ) {   
        perror("listen error");
        close( udp_s );
        close( tcp_s );
        exit(1);    
    }    

    // main loop
    while (1) { // wait for tcp connection
        if ( ( new_tcp = accept( tcp_s, (struct sockaddr *)&tcp_sin, (socklen_t *)&size ) ) < 0 ) {   
            perror("accept error");   
            close( udp_s );
            close( tcp_s );
            exit(1);
        }

        // user authentication
        do {
            // get user info from client
            string_recvfrom( udp_s, buf, 0, &client_addr );
            username = strdup( buf );
            
            // check if user exists and send confirmation
            flag = users.count( username );
            my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );
            
            // recv passwd
            string_recvfrom( udp_s, buf, 0, &client_addr );
            input_passwd = strdup( buf );

            // handle password
            if ( !flag ) users[ username ] = input_passwd;
            else {
                // check if password is correct
                if ( users[ username ].compare( input_passwd ) == 0 )
                    flag = 0;

                // send confirmation to client
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );
            }
        } while ( flag );

        // wait for operation from client
        while (1) { 
            // clear buffers
            bzero( buf, sizeof(buf) ); 
            bzero( cmd, sizeof(cmd) ); 

            // reset other parameters
            flag = 1;

            // receive command from client
            my_recvfrom( udp_s, cmd, sizeof(cmd), 0, &client_addr );

            // handle command
            if ( strncmp( cmd, "CRT", 3 ) == 0 ) {

            } else if ( strncmp( cmd, "LIS", 3 ) == 0 ) {
                // get list of boards in buf
                for ( auto it = boards.begin(); it != boards.end(); it++ ) {
                    strcat( buf, it->first.c_str() );
                    strcat( buf, "\n" );
                }

                // send buf

            } else if ( strncmp( cmd, "MSG", 3 ) == 0 ) {

            } else if ( strncmp( cmd, "DLT", 3 ) == 0 ) {

            } else if ( strncmp( cmd, "RDB", 3 ) == 0 ) {

            } else if ( strncmp( cmd, "EDT", 3 ) == 0 ) {

            } else if ( strncmp( cmd, "APN", 3 ) == 0 ) {

            } else if ( strncmp( cmd, "DWN", 3 ) == 0 ) {

            } else if ( strncmp( cmd, "DST", 3 ) == 0 ) {

            } else if ( strncmp( cmd, "XIT", 3 ) == 0 ) {

            } else if ( strncmp( cmd, "SHT", 3 ) == 0 ) {

            }
        }
    }
}
