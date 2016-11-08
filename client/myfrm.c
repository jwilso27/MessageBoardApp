// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include "../program4.h"

int main(int argc, char * argv[]){
    // initialize parameters
    FILE *fp;
    struct hostent *hp;
    struct sockaddr_in udp_sin, tcp_sin;
    char *host, *username, *passwd, buf[MAX_LINE];
    int port, udp_s, tcp_s, size, i, flag = 1;

    // check arguments
    if (argc == 3) {
        host = argv[1];
        port = atoi(argv[2]);
    } else {
        fprintf(stderr, "usage: ./myfrm <host> <port>\n");
        exit(1);
    }

    // translate host name to peer's IP address
    hp = gethostbyname(host);
    if (!hp) {
        fprintf(stderr, "unknown host: %s\n", host);
        exit(1);
    }

    // build address data structure (udp)
    bzero( (char*)&udp_sin, sizeof(udp_sin) );
    udp_sin.sin_family = AF_INET;
    bcopy( hp->h_addr, (char*)&udp_sin.sin_addr, hp->h_length );
    udp_sin.sin_port = htons( port );

    // build address data structure (tcp)
    bzero( (char*)&tcp_sin, sizeof(tcp_sin) );
    tcp_sin.sin_family = AF_INET;
    bcopy( hp->h_addr, (char*)&tcp_sin.sin_addr, hp->h_length );
    tcp_sin.sin_port = htons( port );

    // active open (udp)
    if ( ( udp_s = socket( PF_INET, SOCK_DGRAM, 0 ) ) < 0 ) {
        perror("socket error\n");
        exit(1);
    }
    
    // active open (tcp)
    if ( ( tcp_s = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 ) {   
        perror("socket error");
        exit(1);
    }   

    // connect to server (tcp)
    if ( connect( tcp_s, (struct sockaddr *)&tcp_sin, sizeof(tcp_sin) ) < 0 ) {
        perror("connection error");
        close(tcp_s);
        exit(1);
    }   

    // get acknowledgement from server
    my_recvfrom( udp_s, &flag, sizeof(flag), 0 );
    
    // if negative acknowledgement
    if ( !flag ) {
        close( udp_s );
        close( tcp_s );
        exit(1);
    }

    // get username and send to server
    username = user_query( udp_s, "Username", &udp_sin );

    // get if user exists 
    my_recvfrom( udp_s, &flag, sizeof(flag), 0 );

    // get password and send to server (new user)
    if ( !flag ) passwd = user_query( udp_s, "Create Password", &udp_sin );
    while ( flag ) {
        
    }

    while (1) {
        // clear buf
        bzero( buf, sizeof(buf) );

        // reset other parameters
        flag = 1;

        // prompt user for operation
        printf(
                "What operation would you like to execute?\n"
                "\tCRT: create board\n"
                "\tLIS: list boards\n"
                "\tMSG: leave message\n"
                "\tDLT: delete message\n"
                "\tRDB: read board\n"
                "\tEDT: edit message\n"
                "\tAPN: append file\n"
                "\tDWN: download file\n"
                "\tDST: destroy board\n"
                "\tXIT: exit\n"
                "\tSHT: shutdown server\n"
        );
        scanf( "%s", buf );

    }

}
