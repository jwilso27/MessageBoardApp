// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include "../program4.h"

#define MAX_PENDING 5   

int main(int argc, char * argv[]){
    // initialize parameters
    FILE *users_fp;
    struct sockaddr_in udp_sin, tcp_sin;
    char buf[MAX_LINE];
    char *username, *passwd, *input_passwd, *admin_passwd;
    int port, udp_s, tcp_s, new_tcp, opt = 1, i, size, flag = 1;
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
        exit(1);    
    }   

    // set socket option   
    if ( ( setsockopt( tcp_s, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(int) ) ) < 0 ) {   
        perror ("socket option error");  
        exit(1);
    }   

    // bind socket (udp)
    if ( ( bind( udp_s, (struct sockaddr*)&udp_sin, sizeof(udp_sin) ) ) < 0 ) {
        perror("bind error");
        exit(1);
    }
    
    // bind socket (tcp)
    if ( ( bind( tcp_s, (struct sockaddr *)&tcp_sin, sizeof(tcp_sin) ) ) < 0 ) {   
        perror("bind error");
        exit(1);
    }

    // listen for client
    if ( ( listen( tcp_s, MAX_PENDING ) ) < 0 ) {   
        perror("listen error");
        exit(1);    
    }    

    // main loop
    while (1) { // wait for tcp connection
        if ( ( new_tcp = accept( tcp_s, (struct sockaddr *)&tcp_sin, (socklen_t *)&size ) ) < 0 ) {   
            perror("accept error");   
            flag = 0;
        }
        
        // send acknowledgement
        my_sendto( udp_s, &flag, sizeof(flag), 0, &udp_sin );

        // if tcp connection doesn't accept
        if ( !flag ) {
            close( udp_s );           
            close( tcp_s );           
            continue;
        }

        // get user info from client
        string_recvfrom( udp_s, username, 0 );
        
        // check if user exists and send acknowledgement
        if ( ( passwd = check_user( username ) ) == NULL ) flag = 0;
        my_sendto( udp_s, &flag, sizeof(flag), 0, &udp_sin );
        
        // get password from client (new user)
        if ( !flag ) {
            string_recvfrom( udp_s, passwd, 0 );
            // add user/passwd
        }

        // get password from client (existing user)
        while ( flag ) {
            string_recvfrom( udp_s, input_passwd, 0 );
            flag = strcmp( input_passwd, passwd )
            my_sendto( udp_s, &flag, sizeof(flag), 0, &udp_sin );
        }

        while (1) { // wait for operation from client
            bzero( buf, sizeof(buf) ); // clear buf

            // receive command from client
        }
    }
}
