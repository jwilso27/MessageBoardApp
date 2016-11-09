// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include <unistd.h>

#include <iostream>
#include <fstream>

#include "board.h"

// constructor
Board::Board() {}

// destructor
Board::~Board() {}

// returns name of the board
string Board::getName() {
    return name;
}

// returns creator's username
string Board::getCreator() {
    return creator;
}

// returns list of messages
vector< pair< string, string > > Board::getMsgs() {
    return messages;
}

// returns list of attachments
vector< pair< string, string > > Board::getAttachments() {
    return attachments;
}

// initialize board
int Board::crtBoard( string u, string n ) {
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
    // check user of msg to be deleted
    if ( user.compare( messages[i].first ) != 0 ) return -1;
    else messages.erase( messages.begin() + i );

    // update board file
    return writeBoard();
}

// edits message on board
int Board::edtMsg( string user, int i, string msg ) {
    // check user of msg to be changed
    if ( user.compare( messages[i].first ) != 0 ) return -1;
    else messages[i].second = msg;

    // update board file
    return writeBoard();
}

// appends file to board
int Board::apnFile( string user, string file ) {
    // check if attachment already exists
    for ( int i = 0; i < attachments.size(); i++ )
        if ( file.compare( attachments[i].second ) == 0 ) return -1;

    // add attachment to vector
    attachments.push_back( make_pair( user, file ) );

    // update board file
    return writeBoard();
}

// destroys board files
int Board::destroy() {
    // delete attachments
    for ( int i = 0; i < attachments.size(); i++ )
        if ( unlink( name.append( "-" ).append( attachments[i].second ).c_str() ) != 0 ) return 0;

    // delete board file
    if ( unlink( file.c_str() ) != 0 ) return 0;

    return 1;
}

// write board file
int Board::writeBoard() {
    // open file for board and clear it
    ofstream ofs;
    ofs.open( file, ofstream::out | ofstream::trunc );
    if ( !ofs ) return 0;

    // write creator of board
    ofs << creator << endl;

    // write messages
    for ( int i = 0; i < messages.size(); i++ ) {
        ofs << messages[i].second;
        ofs << " - " << messages[i].first << endl;
    }

    // write attachments
    for ( int i = 0; i < attachments.size(); i++ ) {
        ofs << attachments[i].second;
        ofs << " (attachment) - " << attachments[i].first << endl;
    }

    ofs.close();

    return 1;
}
