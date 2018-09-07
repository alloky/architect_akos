/** @example lev_log_exmpl.cpp
 * LEV_LOG usage examples
 * Examples */


#include "logging.h"

/*
	This file should be compiled with -DLOG flag to enable log messages
	Otherwise LOG and LEV_LOG macros won't output anything
*/
int main(){
	/*
		Output:
	
		INFO : This is info message
		WARNING : This is warning message
		DEBUG : This is debug message
		ERROR : This is error message
		>> This is just message
	*/
	LEV_LOG(LL_INFO, "This is info message");
	LEV_LOG(LL_WARN, "This is warning message");
	LEV_LOG(LL_DEBUG, "This is debug message");
	LEV_LOG(LL_ERR, "This is error message");
}