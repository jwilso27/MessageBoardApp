// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

// include c libraries
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>   
#include <sys/socket.h>  
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <bits/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h>   
#include <netdb.h>   

// include c++ libraries
#include <cstring>  
#include <cstdlib>  
#include <cstdio>   

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define MAX_LINE 4096 
#define MAX_IN 256 

// function prototypes
void my_sendto( int, void*, size_t, int, struct sockaddr_in* );
void my_recvfrom( int, void*, size_t, int, struct sockaddr_in* );
void my_send( int, void*, size_t, int );
void my_recv( int, void*, size_t, int );
void string_recvfrom( int, char*, int, struct sockaddr_in* );
void string_recv( int, char*, int );
void cmd_query( int, char*, char*, struct sockaddr_in* );
void user_query( int, char*, struct sockaddr_in* );
char* file_query( int, char*, char*, struct sockaddr_in* );
int get_file_size( char* );

// sendto with error checking
void my_sendto( int s, void* buf, size_t size, int flag, struct sockaddr_in* sin ) {
    socklen_t len = sizeof(*sin);
    if ( sendto( s, buf, size, flag, (struct sockaddr*)sin, len ) == -1 ) {
        perror("send error");
        exit(1);
    }
}

// recvfrom with error checking
// (only use when return value is not used)
void my_recvfrom( int s, void* buf, size_t size, int flag, struct sockaddr_in* sin ) {
    socklen_t len = sizeof(struct sockaddr);
    if ( recvfrom( s, buf, size, flag, (struct sockaddr*)sin, &len ) == -1 ) {    
        perror("receive error");
        exit(1);
    }
}

// send with error checking
void my_send( int s, void* buf, size_t size, int flag ) {
    if ( send( s, buf, size, flag ) == -1 ) {    
        perror("send error");
        exit(1);
    }
}

// recv with error checking
// (only use when return value is not used)
void my_recv( int s, void* buf, size_t size, int flag ) {
    if ( recv( s, buf, size, flag ) == -1 ) {    
        perror("receive error");
        exit(1);
    }
}

// recvfrom length and then string in chunks
void string_recvfrom( int s, char* buf, int flag, struct sockaddr_in* sin ) {
    char tmp_buf[16];
    int tmp_len, bufsize = 0;
    short int len;
    socklen_t size = sizeof(struct sockaddr);

    // receive string length from server
    my_recvfrom( s, &len, sizeof(len), flag, sin );

    // receive string from server
    bzero( buf, sizeof(buf) );
    while ( bufsize < len ) {
        if ( ( tmp_len = recvfrom( s, tmp_buf, sizeof(tmp_buf), flag, (struct sockaddr*)sin, &size ) ) == -1 ) {
            perror("server receive error");
            exit(1);
        } else bufsize += tmp_len;
        strcat( buf, tmp_buf );
        bzero( tmp_buf, tmp_len );
    }
}

// get board info from user
void cmd_query( int s, char* type, char* op, struct sockaddr_in* sin ) {
    short int len;
    string buf;

    // get name from user
    cout << "What " << type << " would you like to " << op << "?" << endl;
    if ( cin.get() != '\n' ) cin.unget();
    getline( cin, buf );
    len = buf.length() + 1;

    // send file info to server
    my_sendto( s, &len, sizeof(short int), 0, sin ); 
    my_sendto( s, (void*)buf.c_str(), len, 0, sin );
}

// get user info
void user_query( int s, char* info, struct sockaddr_in* sin ) {
    short int len;
    string buf;

    // get name from user
    cout << info << ": ";
    if ( cin.get() != '\n' ) cin.unget();
    getline( cin, buf );
    len = buf.length() + 1;

    // send file info to server
    my_sendto( s, &len, sizeof(short int), 0, sin ); 
    my_sendto( s, (void*)buf.c_str(), len, 0, sin );
}

// get file info from user
char* file_query( int s, char* type, char* op, struct sockaddr_in* sin ) {
    short int len;
    char buf[256], *name;

    // get name from user
    printf( "What %s would you like to %s?\n", type, op );
    scanf( "%s", buf );
    name = strdup( buf );
    len = strlen( buf ) + 1;

    // send file info to server
    my_sendto( s, &len, sizeof(short int), 0, sin ); 
    my_sendto( s, buf, len, 0, sin );

    return name;
}

// returns size of board file
int get_file_size( char* file ) {
    FILE *fp;
    int size;

    fp = fopen( file, "r" );
    if ( fp == NULL ) return 0;
    
    fseek( fp, 0L, SEEK_END );
    size = ftell( fp );
    fclose( fp );

    return size;
}
