#include "add_on/scriptbuilder/scriptbuilder.h"
#include "add_on/scriptstdstring/scriptstdstring.h"
// #include "add_on/autowrapper/aswrappedcall.h"
#include "add_on/weakref/weakref.h"
#include "gameobj.h"
#include "scriptmgr.h"
#include <assert.h>
#include <iostream>  // cout
#include <stdio.h>   // fopen, fclose
#include <string.h>  // strcmp
// #include "RegistrationMacros.h"

//
using namespace std;
//
CScriptMgr::CScriptMgr()
{
    engine           = 0;
    hasCompileErrors = false;
}

CScriptMgr::~CScriptMgr()
{
    for ( unsigned int n = 0; n < controllers.size(); n++ )
    {
        delete controllers[ n ];
    }
    for ( unsigned int n = 0; n < contexts.size(); n++ )
    {
        contexts[ n ]->Release();
    }
    if ( engine )
    {
        engine->ShutDownAndRelease();
    }
}

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
    if ( ! strstr( asGetLibraryOptions(), "AS_MAX_PORTABILITY" ) )
    {
        cout << "--------------------------------" << endl;

        // // Register the game object. The scripts cannot create these directly, so there is no factory function.
        // r = engine->RegisterObjectType( "CGameObj", sizeof( CGameObj ), asOBJ_VALUE );
        // assert( r >= 0 );
        // r = engine->RegisterObjectMethod( "CGameObj", "void Kill()", AS_METHODPR( CGameObj, Kill, (), void ), asCALL_THISCALL );
        // // r = engine->RegisterObjectMethod( "CGameObj", "int get_x() const property", asMETHOD( CGameObj, GetX ), asCALL_THISCALL );
        // assert( r >= 0 );
    }
    else
    {
        cout << "------+++++-------------" << endl;
        r = engine->RegisterObjectType( "CGameObj", sizeof( CGameObj ), asOBJ_VALUE );
        assert( r >= 0 );
        // r = engine->RegisterObjectMethod( "CGameObj",  "bool Move(int dx, int dy)", WRAP_MFN_PR( CGameObj, Move,(int dx, int dy),bool ), asCALL_THISCALL );
        r = engine->RegisterObjectMethod("CGameObj", "int getAttr(int) const", asMETHODPR(CGameObj, getAttr, (int) const, int), asCALL_THISCALL); assert( r >= 0 );

        assert( r >= 0 );
        // r = engine->RegisterObjectBehaviour( "CGameObj", asBEHAVE_RELEASE, "void f()", asMETHOD( CGameObj, Release ), asCALL_THISCALL );
        // assert( r >= 0 );
        // r = engine->RegisterObjectBehaviour( "CGameObj", asBEHAVE_GET_WEAKREF_FLAG, "int &f()", asMETHOD( CGameObj, GetWeakRefFlag ), asCALL_THISCALL );
        // assert( r >= 0 );

        // // The object's position is read-only to the script. The position is updated with the Move method
        // r = engine->RegisterObjectMethod( "CGameObj", "int get_x() const property", asMETHOD( CGameObj, GetX ), asCALL_THISCALL );
        // assert( r >= 0 );
        // r = engine->RegisterObjectMethod( "CGameObj", "int get_y() const property", asMETHOD( CGameObj, GetY ), asCALL_THISCALL );
        // assert( r >= 0 );
        // r = engine->RegisterObjectMethod( "CGameObj", "bool Move(int dx, int dy)", asMETHOD( CGameObj, Move ), asCALL_THISCALL );
        assert( r >= 0 );
    }
    return 0;
}

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
    cout << msg.section << " (" << msg.row << ", " << msg.col << ") : " << type << " : " << msg.message << endl;

    if ( msg.type == asMSGTYPE_ERROR )
    {
        hasCompileErrors = true;
    }
}

CScriptMgr::SController* CScriptMgr::GetControllerScript( const string& script )
{
    int r;

    // Find the cached controller
    for ( unsigned int n = 0; n < controllers.size(); n++ )
    {
        if ( controllers[ n ]->module == script )
        {
            return controllers[ n ];
        }
    }

    // No controller, check if the script has already been loaded
    asIScriptModule* mod = engine->GetModule( script.c_str(), asGM_ONLY_IF_EXISTS );
    if ( mod )
    {
        // We've already attempted loading the script before, but there is no controller
        return 0;
    }

    // Compile the script into the module
    CScriptBuilder builder;
    r = builder.StartNewModule( engine, script.c_str() );
    if ( r < 0 )
    {
        return 0;
    }
    // If the script file doesn't exist, then there is no script controller for this type
    FILE* f;
    if ( ( f = fopen( ( script + ".as" ).c_str(), "r" ) ) == 0 )
    {
        return 0;
    }
    fclose( f );

    // Let the builder load the script, and do the necessary pre-processing (include files, etc)
    r = builder.AddSectionFromFile( ( script + ".as" ).c_str() );
    if ( r < 0 )
    {
        return 0;
    }
    r = builder.BuildModule();
    if ( r < 0 )
    {
        return 0;
    }
    // Cache the functions and methods that will be used
    SController* ctrl = new SController;
    controllers.push_back( ctrl );
    ctrl->module = script;

    // Find the class that implements the IController interface
    mod               = engine->GetModule( script.c_str(), asGM_ONLY_IF_EXISTS );
    asITypeInfo* type = 0;
    int          tc   = mod->GetObjectTypeCount();
    for ( int n = 0; n < tc; n++ )
    {
        bool found = false;
        type       = mod->GetObjectTypeByIndex( n );
        int ic     = type->GetInterfaceCount();
        for ( int i = 0; i < ic; i++ )
        {
            if ( strcmp( type->GetInterface( i )->GetName(), "IController" ) == 0 )
            {
                found = true;
                break;
            }
        }

        if ( found == true )
        {
            ctrl->type = type;
            break;
        }
    }

    if ( ctrl->type == 0 )
    {
        cout << "Couldn't find the controller class for the type '" << script << "'" << endl;
        controllers.pop_back();
        delete ctrl;
        return 0;
    }

    // Find the factory function
    // The game engine will pass in the owning CGameObj to the controller for storage
    string s          = string( type->GetName() ) + "@ " + string( type->GetName() ) + "(CGameObj @)";
    ctrl->factoryFunc = type->GetFactoryByDecl( s.c_str() );
    if ( ctrl->factoryFunc == 0 )
    {
        cout << "Couldn't find the appropriate factory for the type '" << script << "'" << endl;
        controllers.pop_back();
        delete ctrl;
        return 0;
    }

    // Find the optional event handlers
    ctrl->onThinkMethod   = type->GetMethodByDecl( "void OnThink()" );
    ctrl->onMessageMethod = type->GetMethodByDecl( "void OnMessage(ref @msg, const CGameObj @sender)" );

    // Add the cache as user data to the type for quick access
    type->SetUserData( ctrl );

    return ctrl;
}

asIScriptObject* CScriptMgr::CreateController( const string& script, CGameObj* gameObj )
{
    int              r;
    asIScriptObject* obj = 0;

    SController* ctrl = GetControllerScript( script );
    if ( ctrl == 0 )
    {
        return 0;
    }
    // Create the object using the factory function
    asIScriptContext* ctx = PrepareContextFromPool( ctrl->factoryFunc );

    // Pass the object pointer to the script function. With this call the
    // context will automatically increase the reference count for the object.
    ctx->SetArgObject( 0, gameObj );

    // Make the call and take care of any errors that may happen
    r = ExecuteCall( ctx );
    if ( r == asEXECUTION_FINISHED )
    {
        // Get the newly created object
        obj = *( ( asIScriptObject** )ctx->GetAddressOfReturnValue() );

        // Since a reference will be kept to this object
        // it is necessary to increase the ref count
        obj->AddRef();
    }

    // Return the context to the pool so it can be reused
    ReturnContextToPool( ctx );

    return obj;
}

void CScriptMgr::CallOnThink( asIScriptObject* object )
{
    // Find the cached onThink method id
    SController* ctrl = reinterpret_cast< SController* >( object->GetObjectType()->GetUserData() );

    // Call the method using the shared context
    if ( ctrl->onThinkMethod != 0 )
    {
        asIScriptContext* ctx = PrepareContextFromPool( ctrl->onThinkMethod );
        ctx->SetObject( object );
        ExecuteCall( ctx );
        ReturnContextToPool( ctx );
    }
}

void CScriptMgr::CallOnMessage( asIScriptObject* object, CScriptHandle& msg, CGameObj* caller )
{
    // Find the cached onMessage method id
    SController* ctrl = reinterpret_cast< SController* >( object->GetObjectType()->GetUserData() );

    // Call the method using the shared context
    if ( ctrl->onMessageMethod != 0 )
    {
        asIScriptContext* ctx = PrepareContextFromPool( ctrl->onMessageMethod );
        ctx->SetObject( object );
        ctx->SetArgObject( 0, &msg );
        ctx->SetArgObject( 1, caller );
        ExecuteCall( ctx );
        ReturnContextToPool( ctx );
    }
}

int CScriptMgr::ExecuteCall( asIScriptContext* ctx )
{
    int r = ctx->Execute();
    if ( r != asEXECUTION_FINISHED )
    {
        if ( r == asEXECUTION_EXCEPTION )
        {
            cout << "Exception: " << ctx->GetExceptionString() << endl;
            cout << "Function: " << ctx->GetExceptionFunction()->GetDeclaration() << endl;
            cout << "Line: " << ctx->GetExceptionLineNumber() << endl;

            // It is possible to print more information about the location of the
            // exception, for example the call stack, values of variables, etc if
            // that is of interest.
        }
    }

    return r;
}

asIScriptContext* CScriptMgr::PrepareContextFromPool( asIScriptFunction* func )
{
    asIScriptContext* ctx = 0;
    if ( contexts.size() )
    {
        ctx = *contexts.rbegin();
        contexts.pop_back();
    }
    else
    {
        ctx = engine->CreateContext();
    }
    int r = ctx->Prepare( func );
    assert( r >= 0 );

    return ctx;
}

void CScriptMgr::ReturnContextToPool( asIScriptContext* ctx )
{
    contexts.push_back( ctx );

    // Unprepare the context to free any objects that might be held
    // as we don't know when the context will be used again.
    ctx->Unprepare();
}
