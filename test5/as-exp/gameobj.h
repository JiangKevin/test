#ifndef GAMEOBJ_H
#define GAMEOBJ_H

#include "add_on/scripthandle/scripthandle.h"
#include <angelscript.h>
#include <string>

class CGameObj
{
public:
    // Class method
    void method(){};

    // Overloaded method
    void method2( int input ){};
    void method2( int input, int& output ){};

    // Const method
    int getAttr( int ) const{};
};

#endif