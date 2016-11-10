// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include "../program4.h"
#include "board.h"

using namespace std;

#define MAX_PENDING 5   

int main(int argc, char * argv[]){
    // initialize parameters
    fstream fs;
    struct sockaddr_in udp_sin, tcp_sin, client_addr;
    char cmd[4], buf[MAX_LINE], *username, *input_passwd, *passwd, *admin_passwd, *name;
    int port, udp_s, tcp_s, new_tcp;
    int opt = 1, size = sizeof(struct sockaddr), tmp_size, flag = 1, len;
    Board tmp;
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
                if ( strcmp( users[ username ].c_str(), input_passwd ) == 0 )
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
            len = 0;
            tmp_size = 0;

            // receive command from client
            my_recvfrom( udp_s, cmd, sizeof(cmd), 0, &client_addr );

            // handle command
            if ( strncmp( cmd, "CRT", 3 ) == 0 ) {
                // get board name
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // check if board exists already
                if ( boards.count( buf ) ) flag = -1;
                else {
                    flag = tmp.create( username, buf );
                    boards[ buf ] = tmp;
                }

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

            } else if ( strncmp( cmd, "LIS", 3 ) == 0 ) {
                // get list of boards in buf
                for ( auto it = boards.begin(); it != boards.end(); it++ ) {
                    len += ( it->first.length() + 1 );
                    strcat( buf, it->first.c_str() );
                    strcat( buf, "\n" );
                    cout << buf << endl;
                }
                len++; // accout for null char

                // send buf len
                my_sendto( udp_s, &len, sizeof(len), 0, &client_addr );

                // send buf
                my_sendto( udp_s, buf, len, 0, &client_addr );

            } else if ( strncmp( cmd, "MSG", 3 ) == 0 ) {
                // get board name
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // check if board exists
                flag = boards.count( buf );

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

                if ( !flag ) continue;

                tmp = boards[ buf ];

                // get message
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // add message
                flag = tmp.addMsg( username, buf );

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );
            	
            } else if ( strncmp( cmd, "DLT", 3 ) == 0 ) {
                // get board name
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // check if board exists
                flag = boards.count( buf );

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

                if ( !flag ) continue;

                // get message index
                my_recvfrom( udp_s, &flag, sizeof(flag), 0, &client_addr );

                // delete message
                flag = boards[ buf ].dltMsg( username, flag );

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

            } else if ( strncmp( cmd, "EDT", 3 ) == 0 ) {
                // get board name
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // check if board exists
                flag = boards.count( buf );

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

                if ( !flag ) continue;

                tmp = boards[ buf ];

                // get message index
                my_recvfrom( udp_s, &flag, sizeof(flag), 0, &client_addr );

                // get new message
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // edit message
                flag = tmp.edtMsg( username, flag, buf );

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

            } else if ( strncmp( cmd, "RDB", 3 ) == 0 ) {
                // get board name
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // check if board exists
                if ( !boards.count( buf ) ) flag = -1;
                else flag = get_file_size( (char*)boards[ buf ].getFile().c_str() );

                // send filesize
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );
            	
                // handle file transfer
                if ( flag == -1 ) continue;
                cout << flag << endl;

                // open board file
                ifstream ifs;
                ifs.open( boards[ buf ].getFile() );

                // send contents of board file to client
                do {
                    bzero( buf, sizeof(buf) );
                    len = ifs.rdbuf()->sgetn( buf, MAX_LINE );
                    cout << buf << endl;
                    my_send( new_tcp, buf, len, 0 );
                } while ( ( tmp_size += len ) < flag );

                ifs.close();

            } else if ( strncmp( cmd, "APN", 3 ) == 0 ) {
                // get board name
                string_recvfrom( udp_s, buf, 0, &client_addr );
                name = strdup( buf );

                // check if board exists
                if ( !boards.count( name ) ) flag = 0;

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

                if ( !flag ) continue;

                tmp = boards[ name ];

                // get file info
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // check if attachment already exists
                name = strdup( tmp.checkAttachment( buf ).c_str() );
                
                if ( name != NULL ) flag = 0;

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

                if ( !flag ) continue;

                // receive file size
                my_recvfrom( udp_s, &flag, sizeof(flag), 0, &client_addr );

                if ( flag == -1 ) continue;

                // upload file
                fs.open( strcat( strcat( (char*)tmp.getName().c_str(), "-" ), buf ) );

                // receive attachment
                cout << endl;
                do {
                    bzero( buf, sizeof(buf) );
                    if ( flag - size < sizeof(buf) )
                        len = recv( tcp_s, buf, ( flag - size ), 0 );
                    else len = recv( tcp_s, buf, sizeof(buf), 0 );
                    if ( len == -1 ) {
                        perror("receive error");
                        exit(1);
                    }
                    cout << buf;
                } while ( ( size += len ) < size );
                cout << endl;

                fs.close();

                // add attachment to board object
                flag = tmp.apnFile( username, name );

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

            } else if ( strncmp( cmd, "DWN", 3 ) == 0 ) {
                // get board name
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // check if board exists
                flag = boards.count( buf );

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

                if ( !flag ) continue;

            	tmp = boards[ buf ];
            	
                // get file name
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // check if attachment exists
                name = strdup( tmp.checkAttachment( buf ).c_str() );
                
                if ( name == NULL ) flag = -1;
                else flag = get_file_size( name );

                // send filesize
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );
            	
                if ( flag < 1 ) continue;

                // send contents of board file to client
                fs.open( name );
                do {
                    bzero( buf, sizeof(buf) );
                    len = fs.rdbuf()->sgetn( buf, MAX_LINE );
                    my_send( new_tcp, buf, len, 0 );
                } while ( ( tmp_size += len ) < flag );

                fs.close();

            } else if ( strncmp( cmd, "DST", 3 ) == 0 ) {
                // get board name
                string_recvfrom( udp_s, buf, 0, &client_addr );

                // check if board exists
                if ( !boards.count( buf ) ) flag = -1;
                else {
                    tmp = boards[ buf ];
                    if ( strcmp( tmp.getCreator().c_str(), username ) == 0 ) {
                        flag = tmp.destroy();
                        boards.erase( buf );
                    } else flag = -2;
                }

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

            } else if ( strncmp( cmd, "SHT", 3 ) == 0 ) {
                // recv passwd
                string_recvfrom( udp_s, buf, 0, &client_addr );
                input_passwd = strdup( buf );

                // check if password is correct
                if ( strcmp( admin_passwd, input_passwd ) != 0 ) flag = 0;

                // send confirmation
                my_sendto( udp_s, &flag, sizeof(flag), 0, &client_addr );

                if ( flag ) {
                    // wait for client to close session
                    my_recvfrom( udp_s, &flag, sizeof(flag), 0, &client_addr );

                    // destroy boards
                    for ( auto it = boards.begin(); it != boards.end(); it++ ) 
                        it->second.destroy();

                    // close sockets
                    close( new_tcp );
                    close( tcp_s );
                    close( udp_s );

                    return 0;
                }
                    
            } else if ( strncmp( cmd, "XIT", 3 ) == 0 ) {
                close( new_tcp );
                break;
            }
        }
    }
}
