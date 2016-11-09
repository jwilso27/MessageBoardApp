// Jenna Wilson (jwilso27) & Madelyn Nelson (mnelso12)
// CSE 30264
// Programming Assignment 4
// 11/14/16

#include <string>
#include <vector>
#include <utility>
#include <unordered_map>

using namespace std;

class Board {
    public:
        Board();
        ~Board();
        string getName();
        string getCreator();
        vector< pair< string, string > > getMsgs();
        vector< pair< string, string > > getAttachments();
        int crtBoard( string, string );
        int addMsg( string, string );
        int dltMsg( string, int );
        int edtMsg( string, int, string );
        int apnFile( string, string );
        int destroy();
    private:
        int writeBoard();
        string name;
        string file;
        string creator; 
        vector< pair< string, string > > messages;
        vector< pair< string, string > > attachments;
};
