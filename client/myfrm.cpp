// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include "../program4.h"

using namespace std;

int main(int argc, char * argv[]){
    // initialize parameters
    FILE *fp;
    struct hostent *hp;
    struct sockaddr_in udp_sin, tcp_sin, server_addr;
    char *host, *username, *passwd, *board, buf[MAX_LINE], cmd[4];
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

    // user authentication
    do {
        // get username and send to server
        username = user_query( udp_s, "Username", &udp_sin );

        // get if user exists 
        my_recvfrom( udp_s, &flag, sizeof(flag), 0, &server_addr );

        // get password and send to server
        if ( !flag ) passwd = user_query( udp_s, "Create Password", &server_addr );
        else {
            // get password
            passwd = user_query( udp_s, "Password", &server_addr );

            // recv whether password is correct
            my_recvfrom( udp_s, &flag, sizeof(flag), 0, &server_addr );

            // prompt if password is incorrect
            if ( flag ) printf("Incorrect password\n");
        }
    } while ( flag );
    
    // handle user operation
    while (1) {
        // clear buffers
        bzero( buf, sizeof(buf) );
        bzero( cmd, sizeof(cmd) );

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
        scanf( "%s", cmd );

        // send command to server
        my_sendto( udp_s, cmd, sizeof(cmd), 0, &server_addr );

        // handle command
        if ( strncmp( cmd, "CRT", 3 ) == 0 ) {
            // get and send board name
            board = user_query( udp_s, "Name of board to create", &server_addr );

            // get confirmation
            my_recvfrom( udp_s, &flag, sizeof(flag), 0, &server_addr );

            // handle confirmation
            if ( flag == -1 ) cout << "Board already exists" << endl;
            else if ( flag ) cout << "Board successfully created" << endl;
            else cout << "Board was not able to be created" << endl;

        } else if ( strncmp( cmd, "LIS", 3 ) == 0 ) {
            // recv list of boards
            string_recvfrom( udp_s, buf, 0, &server_addr );

            // print list
            cout << buf << endl;

        } else if ( strncmp( cmd, "MSG", 3 ) == 0 ) {

        } else if ( strncmp( cmd, "DLT", 3 ) == 0 ) {

        } else if ( strncmp( cmd, "RDB", 3 ) == 0 ) {

        } else if ( strncmp( cmd, "EDT", 3 ) == 0 ) {

        } else if ( strncmp( cmd, "APN", 3 ) == 0 ) {

        } else if ( strncmp( cmd, "DWN", 3 ) == 0 ) {

        } else if ( strncmp( cmd, "DST", 3 ) == 0 ) {
            // get and send board name
            board = user_query( udp_s, "Name of board to destroy", &server_addr );

            // get confirmation
            my_recvfrom( udp_s, &flag, sizeof(flag), 0, &server_addr );

            // handle confirmation
            if ( flag == -2 ) {
                cout << "Permission denied: ";
                cout << "only the board's creator can destroy the board";
                cout << endl;
            } else if ( flag == -1 ) cout << "Board does not exist" << endl;
            else if ( flag == 0 ) cout << "Board could not be destroyed" << endl;
            else cout << "Board was successfully destroyed" << endl;

        } else if ( strncmp( cmd, "SHT", 3 ) == 0 ) {

        } else if ( strncmp( cmd, "XIT", 3 ) == 0 ) break;
        else cout << "Invalid operation" << endl;
    }

    close( udp_s );
    close( tcp_s );
    cout << "The session has been closed" << endl;
    return 0;
}
