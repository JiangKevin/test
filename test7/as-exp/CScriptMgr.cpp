#include "CScriptMgr.h"
#include "add_on/scriptbuilder/scriptbuilder.h"
#include "add_on/scriptstdstring/scriptstdstring.h"
//

//
CScriptMgr::CScriptMgr()
{
    engine           = 0;
    hasCompileErrors = false;
}
//
CScriptMgr::~CScriptMgr()
{
    for ( unsigned int n = 0; n < contexts.size(); n++ )
    {
        contexts[ n ]->Release();
    }
    if ( engine )
    {
        engine->ShutDownAndRelease();
    }
}
//
void CScriptMgr::MessageCallback( const asSMessageInfo& msg )
{
    const char* type = "ERR ";
    if ( msg.type == asMSGTYPE_WARNING )
    {
        type = "WARN";
    }
    else if ( msg.type == asMSGTYPE_INFORMATION )
    {
        type = "INFO";
    }
    //
    cout << msg.section << " (" << msg.row << ", " << msg.col << ") : " << type << " : " << msg.message << endl;
}
//
int CScriptMgr::Init()
{
    int r;

    engine = asCreateScriptEngine();

    // Set the message callback to print the human readable messages that the engine gives in case of errors
    r = engine->SetMessageCallback( asMETHOD( CScriptMgr, MessageCallback ), this, asCALL_THISCALL );
    assert( r >= 0 );

    // Register the string type
    RegisterStdString( engine );

    // Register the generic handle type, called 'ref' in the script
    RegisterScriptHandle( engine );

    // Register the weak ref template type
    RegisterScriptWeakRef( engine );
    //
    return 0;
}