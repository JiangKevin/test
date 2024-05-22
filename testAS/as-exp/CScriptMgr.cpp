#include "CScriptMgr.h"
#include "add_on/scriptbuilder/scriptbuilder.h"
#include "add_on/scriptstdstring/scriptstdstring.h"
#include "add_on/weakref/weakref.h"
//
#include "bind.h"
//
CScriptMgr::CScriptMgr()
{
    as_engine_  = 0;
    as_context_ = 0;
}
//
CScriptMgr::~CScriptMgr()
{
    if ( as_context_ )
    {
        as_context_->Release();
    }
    if ( as_engine_ )
    {
        as_engine_->ShutDownAndRelease();
    }
}
//
DWORD CScriptMgr::timeGetTime()
{
    timeval time;
    gettimeofday( &time, NULL );
    return time.tv_sec * 1000 + time.tv_usec / 1000;
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
int CScriptMgr::Run( string fileName )
{
    int r;
    r = Init();
    if ( r < 0 )
    {
        as_engine_->Release();
        return -1;
    }
    //
    ConfigureEngine();
    //
    r = CompileScript( fileName );
    if ( r < 0 )
    {
        as_engine_->Release();
        return -1;
    }
    //
    r = CreateCtx();
    if ( r < 0 )
    {
        as_engine_->Release();
        return -1;
    }
    //
    r = GetAsFunction();
    {
        return -1;
    }
    // We must release the contexts when no longer using them
    as_context_->Release();

    // Shut down the engine
    as_engine_->ShutDownAndRelease();
}
//
int CScriptMgr::Init()
{
    int r;

    as_engine_ = asCreateScriptEngine();
    if ( as_engine_ == 0 )
    {
        cout << "Failed to create script engine." << endl;
        return -1;
    }

    // Set the message callback to print the human readable messages that the engine gives in case of errors
    r = as_engine_->SetMessageCallback( asMETHOD( CScriptMgr, MessageCallback ), this, asCALL_THISCALL );
    assert( r >= 0 );

    // Register the string type
    RegisterStdString( as_engine_ );

    // Register the generic handle type, called 'ref' in the script
    RegisterScriptHandle( as_engine_ );

    // Register the weak ref template type
    RegisterScriptWeakRef( as_engine_ );
    //
    return 0;
}
//
void CScriptMgr::ConfigureEngine()
{
    //
    RegisterScripTest( as_engine_ );
}
//
int CScriptMgr::CompileScript( string fileName )
{
    int r;
    // We will load the script from a file on the disk.
    FILE* f = fopen( fileName.c_str(), "rb" );
    if ( f == 0 )
    {
        cout << "Failed to open the script file 'script.as'." << endl;
        return -1;
    }
    // Determine the size of the file
    fseek( f, 0, SEEK_END );
    int len = ftell( f );
    fseek( f, 0, SEEK_SET );

    // On Win32 it is possible to do the following instead
    // int len = _filelength(_fileno(f));

    // Read the entire file
    string script;
    script.resize( len );
    size_t c = fread( &script[ 0 ], len, 1, f );
    fclose( f );

    if ( c == 0 )
    {
        cout << "Failed to load script file." << endl;
        return -1;
    }
    // Add the script sections that will be compiled into executable code.
    // If we want to combine more than one file into the same script, then
    // we can call AddScriptSection() several times for the same module and
    // the script engine will treat them all as if they were one. The script
    // section name, will allow us to localize any errors in the script code.
    asIScriptModule* mod = as_engine_->GetModule( 0, asGM_ALWAYS_CREATE );
    r                    = mod->AddScriptSection( "script", &script[ 0 ], len );
    if ( r < 0 )
    {
        cout << "AddScriptSection() failed" << endl;
        return -1;
    }

    // Compile the script. If there are any compiler messages they will
    // be written to the message stream that we set right after creating the
    // script engine. If there are no errors, and no warnings, nothing will
    // be written to the stream.
    r = mod->Build();
    if ( r < 0 )
    {
        cout << "Build() failed" << endl;
        return -1;
    }

    // The engine doesn't keep a copy of the script sections after Build() has
    // returned. So if the script needs to be recompiled, then all the script
    // sections must be added again.

    // If we want to have several scripts executing at different times but
    // that have no direct relation with each other, then we can compile them
    // into separate script modules. Each module use their own namespace and
    // scope, so function names, and global variables will not conflict with
    // each other.

    return 0;
};
//
int CScriptMgr::CreateCtx()
{
    // Create a context that will execute the script.
    as_context_ = as_engine_->CreateContext();
    if ( as_context_ == 0 )
    {
        cout << "Failed to create the context." << endl;
        as_engine_->Release();
        return -1;
    }
    else
    {
        return 0;
    }
};
//
int CScriptMgr::GetAsFunction()
{
    int r;
    // Find the function for the function we want to execute.
    asIScriptFunction* func_createScene = as_engine_->GetModule( 0 )->GetFunctionByDecl( "float createScene()" );
    if ( func_createScene == 0 )
    {
        cout << "The function 'float createScene()' was not found." << endl;
        as_context_->Release();
        as_engine_->Release();
        return -1;
    }
    // Prepare the script context with the function we wish to execute. Prepare()
    // must be called on the context before each new script function that will be
    // executed. Note, that if you intend to execute the same function several
    // times, it might be a good idea to store the function returned by
    // GetFunctionByDecl(), so that this relatively slow call can be skipped.
    r = as_context_->Prepare( func_createScene );
    if ( r < 0 )
    {
        cout << "Failed to prepare the context." << endl;
        as_context_->Release();
        as_engine_->Release();
        return -1;
    }
    // Execute the function
    cout << "Executing the script." << endl;
    cout << "---" << endl;
    r = as_context_->Execute();
    cout << "---" << endl;
    //
    if ( r != asEXECUTION_FINISHED )
    {
        // The execution didn't finish as we had planned. Determine why.
        if ( r == asEXECUTION_ABORTED )
        {
            cout << "The script was aborted before it could finish. Probably it timed out." << endl;
        }
        else if ( r == asEXECUTION_EXCEPTION )
        {
            cout << "The script ended with an exception." << endl;

            // Write some information about the script exception
            asIScriptFunction* func_createScene = as_context_->GetExceptionFunction();
            cout << "func: " << func_createScene->GetDeclaration() << endl;
            cout << "modl: " << func_createScene->GetModuleName() << endl;
            cout << "sect: " << func_createScene->GetScriptSectionName() << endl;
            cout << "line: " << as_context_->GetExceptionLineNumber() << endl;
            cout << "desc: " << as_context_->GetExceptionString() << endl;
        }
        else
            cout << "The script ended for some unforeseen reason (" << r << ")." << endl;
    }
    else
    {
        // Retrieve the return value from the context
        float returnValue = as_context_->GetReturnFloat();
        cout << "The script function returned: " << returnValue << endl;
    }
};
