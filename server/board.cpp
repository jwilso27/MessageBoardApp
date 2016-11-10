// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include <unistd.h>

#include "board.h"

// constructor
Board::Board() {}

// destructor
Board::~Board() {}

// returns name of board
string Board::getName() {
    return name;
}

// returns board file name
string Board::getFile() {
    return file;
}

// returns creator's username
string Board::getCreator() {
    return creator;
}

// returns list of messages
vector< pair< string, string > > Board::getMsgs() {
    return messages;
}

// returns size of board file
int Board::getFilesize() {
    return filesize;
}

// initialize board
int Board::create( string u, string n ) {
    // define variables
    creator = u;
    name = n;
    file = n.append(".board");

    // create board file
    return writeBoard();
}

// adds message to board
int Board::addMsg( string user, string msg ) {
    // add message to vector
    messages.push_back( make_pair( user, msg ) );

    // update board file
    return writeBoard();
}

// deletes message from board
int Board::dltMsg( string user, int i ) {
    // check if msg index is in bounds
    if ( i < 0 || i >= messages.size() ) return -1;

    // check user of msg to be deleted
    if ( user.compare( messages[i].first ) != 0 ) return -2;
    else messages.erase( messages.begin() + i );

    // update board file
    return writeBoard();
}

// edits message on board
int Board::edtMsg( string user, int i, string msg ) {
    // check if msg index is in bounds
    if ( i < 0 || i >= messages.size() ) return -1;

    // check user of msg to be changed
    if ( user.compare( messages[i].first ) != 0 ) return -2;
    else messages[i].second = msg;

    // update board file
    return writeBoard();
}

// appends file to board
int Board::apnFile( string user, string file ) {
    // add attachment to map
    attachments[ file ] = user;

    // update board file
    return writeBoard();
}

// check if an attachment already exists
int Board::checkAttachment( string file ) {
    if ( !attachments.count( file ) ) return -1;

    fstream fs;
    fs.open( name.append("-").append(file) );
    if ( !fs ) return 0;

    int i = fs.rdbuf()->pubseekoff( 0, fs.end ); 

    fs.close();

    return i;
}

// destroys board files
int Board::destroy() {
    // delete attachments
    for ( auto it = attachments.begin(); it != attachments.end(); it++ )
        if ( unlink( name.append( "-" ).append( it->first ).c_str() ) != 0 ) return 0;

    // delete board file
    if ( unlink( file.c_str() ) != 0 ) return 0;

    return 1;
}

// write board file
int Board::writeBoard() {
    // open board file and clear it
    fstream fs;
    fs.open( file, fstream::in | fstream::out | fstream::trunc );
    if ( !fs ) return 0;

    // write creator of board
    fs << creator << endl;

    // write messages
    for ( int i = 0; i < messages.size(); i++ ) {
        fs << "[" << i << "] ";
        fs << messages[i].second;
        fs << " - " << messages[i].first << endl;
    }

    // write attachments
    for ( auto it = attachments.begin(); it != attachments.end(); it++ ) {
        fs << it->first;
        fs << " (attachment) - " << it->second << endl;
    }

    fs.close();

    // update filesize
    fs.open( file );
    // filesize = fs.rdbuf()->pubseekoff( 0, fs.end );
    fs.seekg( 0, fs.end );
    filesize = fs.tellg();
    fs.close();

    return 1;
}
