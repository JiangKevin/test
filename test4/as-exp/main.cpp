#include "gamemgr.h"
#include "scriptmgr.h"
#include <angelscript.h>
#include <iostream>  // cout
#include <stdio.h>

//
using namespace std;

CScriptMgr* scriptMgr = 0;
CGameMgr*   gameMgr   = 0;
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

    // Initialize the game manager
    gameMgr = new CGameMgr();

    // Initialize the script manager
    scriptMgr = new CScriptMgr();
    r         = scriptMgr->Init();
    //
    if ( r < 0 )
    {
        delete scriptMgr;
        delete gameMgr;
        return r;
    }

    // Start a new game
    r = gameMgr->StartGame();
    if ( r < 0 )
    {
        cout << "Failed to initialize the game. Please verify the script errors." << endl;
        cout << endl;
        cout << "Press enter to exit." << endl;
        char buf[ 10 ];
        cin.getline( buf, 10 );
        return -1;
    }

    // Let the game manager handle the game loop
    gameMgr->Run();

    // Uninitialize the game manager
    if ( gameMgr )
    {
        delete gameMgr;
        gameMgr = 0;
    }

    // Uninitialize the script manager
    if ( scriptMgr )
    {
        delete scriptMgr;
        scriptMgr = 0;
    }

    return r;
}
