// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include "../program4.h"

#define MAX_PENDING 5   

int main(int argc, char * argv[]){
    // initialize parameters
    FILE *fp;
    struct sockaddr_in udp_sin, tcp_sin, client_addr;
    char buf[MAX_LINE], *username, *input_passwd, *passwd, *admin_passwd;
    int port, udp_s, tcp_s, new_tcp;
    int opt = 1, i, size = sizeof(struct sockaddr), flag = 1;
    short int len;

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

    // initialize users.txt
    fp = fopen( "users.txt", "w" );
    fclose( fp );

    // main loop
    while (1) { // wait for tcp connection
        if ( ( new_tcp = accept( tcp_s, (struct sockaddr *)&tcp_sin, (socklen_t *)&size ) ) < 0 ) {   
            perror("accept error");   
            close( udp_s );
            close( tcp_s );
            exit(1);
        }

/*        
        // send acknowledgement
        my_sendto( udp_s, &flag, sizeof(flag), 0, &udp_sin );

        // if tcp connection doesn't accept
        if ( !flag ) {
            close( udp_s );           
            close( tcp_s );           
            continue;
        }
*/

        // user authentication
        do {
            // get user info from client
            string_recvfrom( udp_s, buf, 0, &client_addr );
            username = strdup( buf );
            
            // check if user exists and send confirmation
            if ( ( passwd = check_user( username ) ) == NULL ) flag = 0;
            my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );
            
            // get password from client
            if ( !flag ) {
                string_recvfrom( udp_s, buf, 0, &client_addr );
                passwd = strdup( buf );
                create_user( username, passwd );
            } else {
                // get password
                string_recvfrom( udp_s, buf, 0, &client_addr );
                input_passwd = strdup( buf );

                // check if password is correct
                flag = strcmp( input_passwd, passwd );

                // send confirmation to client
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );
            }
        } while ( flag );

        // wait for operation from client
        while (1) { 
            // clear buf
            bzero( buf, sizeof(buf) ); 

            // reset other parameters
            flag = 1;

            // receive command from client
            my_recvfrom( udp_s, buf, sizeof(buf), 0, &client_addr );
            printf("%s\n",buf);

            // handle command
            if ( strncmp( buf, "CRT", 3 ) == 0 ) {

            } else if ( strncmp( buf, "LIS", 3 ) == 0 ) {

            } else if ( strncmp( buf, "MSG", 3 ) == 0 ) {

            } else if ( strncmp( buf, "DLT", 3 ) == 0 ) {

            } else if ( strncmp( buf, "RDB", 3 ) == 0 ) {

            } else if ( strncmp( buf, "EDT", 3 ) == 0 ) {

            } else if ( strncmp( buf, "APN", 3 ) == 0 ) {

            } else if ( strncmp( buf, "DWN", 3 ) == 0 ) {

            } else if ( strncmp( buf, "DST", 3 ) == 0 ) {

            } else if ( strncmp( buf, "XIT", 3 ) == 0 ) {

            } else if ( strncmp( buf, "SHT", 3 ) == 0 ) {

            }
        }
    }
}
