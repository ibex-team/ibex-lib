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

namespace ibex {

#ifdef _MSC_VER
#define SNPRINTF _snprintf
#else
#define SNPRINTF snprintf
#endif // _MSC_VER

int index_2_string(char* buff, char lbracket, char rbracket, int index) {
	assert(index<1000000);
	int k=0;
	char number[6];
	buff[k++]=lbracket;
	SNPRINTF(number, 6, "%d", index);
	strcpy(&buff[k], number);
	k+=strlen(number);
	buff[k++]=rbracket;
	buff[k]='\0';
	return k;
}

#define MAX_NAME_SIZE 20

#define BASE_VAR_NAME "_x_"
#define BASE_FUNC_NAME "_f_"

static char* generated_name_buff = new char[MAX_NAME_SIZE];

static int generated_var_count=0;
static int generated_func_count=0;

static char* next_generated_name(const char* base, int num) {
	sprintf(generated_name_buff, base);
	SNPRINTF(&generated_name_buff[strlen(base)], MAX_NAME_SIZE-strlen(base), "%d", num);
	return generated_name_buff;
}

char* next_generated_var_name() {
	return next_generated_name(BASE_VAR_NAME,generated_var_count++);
}

char* next_generated_func_name() {
	return next_generated_name(BASE_FUNC_NAME,generated_func_count++);
}


} // end namespace ibex
