#pragma once

#include "add_on/scripthandle/scripthandle.h"
#include "angelscript.h"
#include <assert.h>  // assert()
#include <iostream>  // cout
#include <string>
#include <sys/time.h>
#include <vector>
//
using namespace std;
typedef unsigned int DWORD;
//
class CScriptMgr
{
public:
    CScriptMgr();
    ~CScriptMgr();
public:
    int  Run( string fileName );
    int  Init();
    void ConfigureEngine();
    int  CompileScript( string fileName );
    int  CreateCtx();
    int  GetAsFunction();
    int  ExecuteCall( asIScriptContext* ctx );
protected:
    void  MessageCallback( const asSMessageInfo& msg );
    DWORD timeGetTime();
private:
    asIScriptEngine*  as_engine_;
    asIScriptContext* as_context_;
    DWORD             as_timeOut_;
};
