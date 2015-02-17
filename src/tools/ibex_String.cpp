//============================================================================
//                                  I B E X                                   
// File        : ibex_String.cpp
// Author      : Gilles Chabert
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 18, 2012
// Last Update : Jul 18, 2012
//============================================================================

#include "ibex_String.h"
#include <stdlib.h>

namespace ibex {

#ifdef _MSC_VER
#define SNPRINTF _snprintf
#else
#define SNPRINTF snprintf
#endif // _MSC_VER

char* append_index(const char* buff, char lbracket, char rbracket, int index) {
	assert(index<1000000);
	char number[6];
	SNPRINTF(number, 6, "%d", index);

	int k=strlen(buff);
	char* res=(char*) malloc(k+strlen(number)+3);
	strcpy(res,buff);
	res[k++]=lbracket;
	strcpy(&res[k], number);
	k+=strlen(number);
	res[k++]=rbracket;
	res[k]='\0';
	return res;
}

#define MAX_NAME_SIZE 20

#define BASE_VAR_NAME "_x_"
#define BASE_FUNC_NAME "_f_"


static char* next_generated_name(const char* base, int num) {
	static char generated_name_buff[MAX_NAME_SIZE];
	sprintf(generated_name_buff,"%s", base);
	SNPRINTF(&generated_name_buff[strlen(base)], MAX_NAME_SIZE-strlen(base), "%d", num);
	return generated_name_buff;
}

char* next_generated_var_name() {
	static int generated_var_count=0;
	return next_generated_name(BASE_VAR_NAME,generated_var_count++);
}

char* next_generated_func_name() {
	static int generated_func_count=0;
	return next_generated_name(BASE_FUNC_NAME,generated_func_count++);
}


} // end namespace ibex
