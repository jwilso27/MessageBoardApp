// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include <string>
#include <vector>
#include <utility>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

class Board {
    public:
        Board();
        ~Board();
        string getName();
        string getFile();
        string getCreator();
        vector< pair< string, string > > getMsgs();
        int create( string, string );
        int addMsg( string, string );
        int dltMsg( string, int );
        int edtMsg( string, int, string );
        int apnFile( string, string );
        string checkAttachment( string );
        int destroy();
    private:
        int writeBoard();
        string name;
        string file;
        string creator; 
        vector< pair< string, string > > messages;
        map< string, string > attachments;
};
