// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

// include librraries
#include <stdio.h>   
#include <stdlib.h>  
#include <unistd.h>
#include <string.h>  
#include <fcntl.h>
#include <sys/types.h>   
#include <sys/socket.h>  
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <bits/stat.h>
#include <time.h>
#include <arpa/inet.h>
#include <netinet/in.h>   
#include <netdb.h>   

#define MAX_LINE 4096 

// function prototypes
void my_sendto( int, void*, size_t, int, struct sockaddr_in* );
void my_recvfrom( int, void*, size_t, int );
void my_send( int, void*, size_t, int );
void my_recv( int, void*, size_t, int );
void string_recvfrom( int, char*, int );
void string_recv( int, char*, int );
char* file_query( int, char*, char* );
char* user_query( int, char*, struct sockaddr_in* );
char* check_user( char* );

// sendto with error checking
void my_sendto( int s, void* buf, size_t size, int flag, struct sockaddr_in* sin ) {
    if ( sendto( s, buf, size, flag, (struct sockaddr*)sin, sizeof(struct sockaddr) ) == -1 ) {
        perror("send error");
        exit(1);
    }
}

// recvfrom with error checking
// (only use when return value is not used)
void my_recvfrom( int s, void* buf, size_t size, int flag ) {
    socklen_t len = sizeof(struct sockaddr);
    if ( recvfrom( s, buf, size, flag, NULL, &len ) == -1 ) {    
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
void string_recvfrom( int s, char* buf, int flag ) {
    int tmp_len, bufsize = 0;
    short int len;
    char tmp_buf[16];

    // receive string length from server
    my_recvfrom( s, &len, sizeof(len), flag );

    // receive string from server
    bzero( buf, sizeof(buf) );
    while ( bufsize < len ) {
        bzero( tmp_buf, tmp_len );
        if ( ( tmp_len = recvfrom( s, tmp_buf, sizeof(tmp_buf), flag, NULL, NULL ) ) == -1 ) {
            perror("server receive error");
            exit(1);
        } else bufsize += tmp_len;
        strcat( buf, tmp_buf );
    }
}

// get file info from user
char* file_query( int s, char* type, char* op ) {
    short int len;
    char *name, buf[256];

    // get name from user
    printf( "What %s would you like to %s?\n", type, op );
    scanf( "%s", buf );
    name = strdup( buf );
    len = strlen( buf ) + 1;

    // send file info to server
    my_send( s, &len, sizeof(short int), 0 ); 
    my_send( s, buf, len, 0 );

    return name;
}

// get user info
char* user_query( int s, char* info, struct sockaddr_in* sin ) {
    short int len;
    char *name, buf[256];

    // get name from user
    printf( "%s: ", info );
    scanf( "%s", buf );
    name = strdup( buf );
    len = strlen( buf ) + 1;

    // send file info to server
    my_sendto( s, &len, sizeof(short int), 0, sin); 
    my_sendto( s, buf, len, 0, sin );

    return name;
}

// check if a user exists
char* check_user( char* username ) {
    FILE *fp;
    char* tmp;

    fp = fopen( "users.txt", "r" );
    while ( fscanf( fp, "%s", &tmp ) ) {
        if ( strcmp( tmp, username ) == 0 ) {
            fscanf( fp, "%s", &tmp );
            return tmp;
        }
    }

    return NULL;
}
