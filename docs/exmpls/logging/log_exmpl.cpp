/** @example log_exmpl.cpp
 * Logging usage examples
 * Examples */


#include "logging.h"

/*
	This file should be compiled with -DLOG flag to enable log messages
	Otherwise LOG and LEV_LOG macros won't output anything
*/
int main(){
	/*
		Output:
	
		>> This is just message
		>> This is another message

	*/

	LOG("This is just message");
	LOG("This is another message");

}