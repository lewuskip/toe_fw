/*
 * test.h
 *
 *  Created on: 14 Jul 2018
 *      Author: lewy
 */

#ifndef PRODUCTS_TEKNET_TEST_H_
#define PRODUCTS_TEKNET_TEST_H_

#include <stdint.h>

#ifdef __cplusplus
#include <string>

class Foo
{
	public:
		Foo( int32_t a );
		~Foo();
		std::string toString();
	private:
		int32_t k;
};
#endif

#ifdef __cplusplus

extern "C"
{
#endif

void*  getFoo( int32_t a );
void   destroyFoo( void *foo );
void   printString( void *foo );

#ifdef __cplusplus
}
#endif

#endif /* PRODUCTS_TEKNET_TEST_H_ */
