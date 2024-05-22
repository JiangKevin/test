#include "angelscript.h"
#include "autowrapper/aswrappedcall.h"
#include <assert.h>
#include <new>
#include <string.h>
//
#include "test.h"
//
static void Construct( test* mem )
{
    new ( mem ) test();
}
static void ConstructCopy( test* mem, const test& o )
{
    new ( mem ) test( o );
}

static void RegisterScripTest( asIScriptEngine* engine )
{
    //
    int r = engine->RegisterObjectType( "test", sizeof( test ), asOBJ_VALUE | asOBJ_APP_CLASS_CAK | asOBJ_POD | asGetTypeTraits< test >() );
    assert( r >= 0 );

    r = engine->RegisterObjectBehaviour( "test", asBEHAVE_CONSTRUCT, "void test()", WRAP_OBJ_LAST( Construct ), asCALL_GENERIC );
    assert( r >= 0 );
    r = engine->RegisterObjectBehaviour( "test", asBEHAVE_CONSTRUCT, "void test(const test &in)", WRAP_OBJ_FIRST( ConstructCopy ), asCALL_GENERIC );
    assert( r >= 0 );
    r = engine->RegisterObjectMethod( "test", "void PrintD()", WRAP_MFN( test, PrintD ), asCALL_GENERIC );
    assert( r >= 0 );
};