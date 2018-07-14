#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdint.h>

#include "test.h"

Foo::Foo( int32_t a )
{
	k = a << 1;
}

Foo::~Foo()
{
	/* don't really need to do anything here */
	/* k = 0 only for example purposes       */
	k = 0;
}


void*
getFoo( int32_t a )
{
	Foo *out( new Foo(a) );
	return( reinterpret_cast< void* >( out ) );
}

void
destroyFoo( void* foo )
{
	delete( reinterpret_cast< Foo* >( foo ) );
}

void
srintString( void *foo )
{
	std::string s = reinterpret_cast< Foo* >( foo )->toString();
	std::cout << s;
}


