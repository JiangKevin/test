#include "CScriptMgr.h"
#include "add_on/scriptstdstring/scriptstdstring.h"
#include "bind.h"
#include <angelscript.h>
#include <assert.h>  // assert()
#include <iostream>  // cout
#include <stdio.h>
#include <string.h>  // strstr()
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
//
using namespace std;
typedef unsigned int DWORD;
//
int main( int argc, char** argv )
{
    CScriptMgr cm;
    cm.Run( "script.as" );

    // Wait until the user presses a key
    cout << "Press any key to quit." << endl;
    //
    return 0;
}
