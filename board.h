// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

#include "program4.h"

using namespace std;

class Board {
    public:
        Board( string, string );
        ~Board();
        int addMsg( string, string );
        int dltMsg( string );
        int edtMsg( string );
        int apnFile( string, string );
        int destroy( string );
        string getName();
    private:
        void writeFile();
        int getMsg();
        string name;
        string creator; 
        vector< pair< string, string > > messages;
        vector< string > files;
};
