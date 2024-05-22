#pragma once

#include "add_on/scripthandle/scripthandle.h"
#include "angelscript.h"
#include <string>
#include <vector>

//
class CScriptMgr
{
public:
    CScriptMgr();
    ~CScriptMgr();
public:
    int Init();

    void CallOnThink( asIScriptObject* object );
protected:
    void              MessageCallback( const asSMessageInfo& msg );
    asIScriptContext* PrepareContextFromPool( asIScriptFunction* func );
    void              ReturnContextToPool( asIScriptContext* ctx );
    int               ExecuteCall( asIScriptContext* ctx );
private:
    asIScriptEngine*                 engine;
    std::vector< asIScriptContext* > contexts;
};

#endif