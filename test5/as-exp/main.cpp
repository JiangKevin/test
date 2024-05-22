#include "scriptmgr.h"
#include <angelscript.h>
#include <iostream>  // cout
#include <stdio.h>
//
using namespace std;
#define AS_MAX_PORTABILITY AS_MAX_PORTABILITY
//
int main( int argc, char** argv )
{

    int r;

    // Make sure the game is being executed with the correct working directory
    // At the very least there should be a 'player.as' script for controlling the
    // player character.
    FILE* f = fopen( "player.as", "r" );
    if ( f == 0 )
    {
        cout << "The game is not executed in the correct location. Make sure you set the working directory to the path where the 'player.as' script is located." << endl;
        cout << endl;
        cout << "Press enter to exit." << endl;
        char buf[ 10 ];
        cin.getline( buf, 10 );
        return -1;
    }
    fclose( f );
    // Initialize the script manager
    auto scriptMgr = new CScriptMgr();
    r         = scriptMgr->Init();
    //
    if ( r < 0 )
    {
        delete scriptMgr;

        return r;
    }


    return r;
}
