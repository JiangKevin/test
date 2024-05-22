#include "luaaa.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define LOG printf
/**/
class Cat
{
public:
    Cat() : m_age( 1 ), m_weight( 1.0f )
    {
        LOG( "Cat: a cat spawn at %p.\n", this );
    }

    Cat( const std::string& name ) : m_name( name ), m_age( 1 ), m_weight( 1.0f )
    {
        LOG( "Cat: %s spawn at %p\n", m_name.c_str(), this );
    }

    ~Cat()
    {
        LOG( "Cat: cat[%p] %s is free.\n", this, m_name.c_str() );
    }

    const std::string& getName() const
    {
        LOG( "Cat:get name\n" );
        return m_name;
    }

    const std::string& setName( const std::string& name )
    {
        LOG( "Cat:set name to %s\n", name.c_str() );
        m_name = name;
        return m_name;
    }

    int setAge( const int age )
    {
        LOG( "Cat:set age to %d\n", age );
        m_age = age;
        return m_age;
    }

    int getAge() const
    {
        LOG( "Cat:get age\n" );
        return m_age;
    }

    float setWeight( float w )
    {
        LOG( "Cat:set weight to %f\n", w );
        m_weight = w;
        return m_weight;
    }

    float getWeight() const
    {
        LOG( "Cat:get weight\n" );
        return m_weight;
    }

    void eat( const std::list< std::string >& foods )
    {
        for ( auto& it : foods )
        {
            LOG( "%s eat %s.\n", m_name.c_str(), it.c_str() );
            m_weight += 0.1f;
        }
        LOG( "%s is getting fatter.\n", m_name.c_str() );
    }

    void test( int a, const std::string& b, float c, const std::string& d, const std::string& e )
    {
        LOG( "cat test: got params from lua: [0: %d, 1:%s, 2:%f, 3:%s, 4:%s]\n", a, b.c_str(), c, d.c_str(), e.c_str() );
    }

    std::string toString() const
    {
        std::stringstream result;
        result << m_name << " is a cat, he is " << m_age << " years old, has a weight of " << m_weight << " kg.";
        return result.str();
    }

    static void speak( const std::string& w )
    {
        LOG( "%s, miaow~~\n", w.c_str() );
    }

    void testfunctor( std::function< int( int param ) > callback )
    {
        int result = callback( 42 );
        LOG( "Callback with argument 42 leads to %d.\n", result );
    }
public:
    std::string             prop1;
    std::set< std::string > prop3;
private:
    std::string m_name;
    int         m_age;
    float       m_weight;
};