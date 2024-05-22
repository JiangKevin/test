
#include "fm.h"
using namespace luaaa;
//
void bindToLUA( lua_State* );
//
//
void runLuaExample( lua_State* ls )
{
    bindToLUA( ls );

    // do
    // {
    LOG( "------------------------------------------\n" );
    std::stringstream buffer;
    std::ifstream     file( "main.lua" );
    if ( file )
    {
        buffer << file.rdbuf();
        file.close();
    }

    int err = luaL_loadbuffer( ls, buffer.str().c_str(), buffer.str().length(), "console" );
    if ( err == 0 )
    {
        err = lua_pcall( ls, 0, 0, 0 );
    }

    if ( err )
    {
        LOG( "lua err: %s", lua_tostring( ls, -1 ) );
        lua_pop( ls, 1 );
    }

    // } while ( std::cin.get() != 1 );
}

/**/
int main()
{
    auto ls = luaL_newstate();
    luaL_openlibs( ls );

    if ( ls != NULL )
    {
        runLuaExample( ls );

        lua_close( ls );
    }
    return 0;
}

//
void bindToLUA( lua_State* L )
{
    // bind class to lua
    LuaClass< Cat > luaCat( L, "AwesomeCat" );
    luaCat.ctor< std::string >();
    // luaCat.fun( "setName", &Cat::setName );
    // luaCat.fun( "getName", &Cat::getName );
    luaCat.fun( "setAge", &Cat::setAge );
    // luaCat.fun( "getAge", &Cat::getAge );
    // luaCat.fun( "eat", &Cat::eat );
    // luaCat.fun( "speak", &Cat::speak );
    // luaCat.fun( "test", &Cat::test );
    luaCat.fun( "testfunctor", &Cat::testfunctor );
    // luaCat.fun( std::string( "testFunctor1" ),
    //             []( int n1, int n2 ) -> int
    //             {
    //                 LOG( "testFunctor1:%d, %d\n", n1, n2 );
    //                 return n1 * n2;
    //             } );
    // luaCat.fun( "testFunctor2", std::function< void( int, int ) >(
    //                                 []( int n1, int n2 )
    //                                 {
    //                                     LOG( "testFunctor2:%d, %d\n", n1, n2 );
    //                                 } ) );
    // luaCat.fun( "__tostring", &Cat::toString );
    // luaCat.def( "tag", "Animal" );

    // luaCat.set( "say", &Cat::speak );
    // luaCat.set( "name", &Cat::setName );
    // luaCat.get( "name", &Cat::getName );
    // luaCat.set( "age", &Cat::setAge );
    // luaCat.get( "age", &Cat::getAge );

    // luaCat.set( "prop2",
    //             []( const Cat& cat, float val ) -> void
    //             {
    //                 printf( "set prop2=%f\n", val ); /*cat.setWeight(val)*/
    //                 ;
    //             } );
    // luaCat.get( "prop2",
    //             []( Cat& cat ) -> float
    //             {
    //                 printf( "get prop2\n" );
    //                 return cat.getWeight();
    //             } );
    // luaCat.set( std::string( "prop3" ),
    //             []( Cat& cat, const std::set< std::string >& val )
    //             {
    //                 printf( "set prop3\n" );
    //                 cat.prop3 = val;
    //             } );
    // luaCat.get( std::string( "prop3" ),
    //             []( const Cat& cat ) -> std::set< std::string >
    //             {
    //                 printf( "get prop3\n" );
    //                 return cat.prop3;
    //             } );
    // luaCat.set( std::string( "prop4" ),
    //             []( float val )
    //             {
    //                 printf( "set prop4=%f\n", val );
    //             } );
    // luaCat.get( std::string( "prop4" ),
    //             []() -> float
    //             {
    //                 printf( "get prop4\n" );
    //                 return 0.123f;
    //             } );
};
