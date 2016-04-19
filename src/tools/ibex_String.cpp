//============================================================================
//                                  I B E X                                   
// File        : ibex_String.cpp
// Author      : Gilles Chabert, Simon Rohou
// Copyright   : Ecole des Mines de Nantes (France)
// License     : See the LICENSE file
// Created     : Jul 18, 2012
// Last Update : April 18, 2016
//============================================================================

#include "ibex_String.h"
#include <stdlib.h>
#include <sstream>

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

const char* intv2str(const Interval& intv, int precision) {
  std::ostringstream o;

  if(intv == Interval::EMPTY_SET ||
     intv == Interval::POS_REALS ||
     intv == Interval::NEG_REALS ||
     intv == Interval::ALL_REALS)
  {
    o << intv;
  }

  else
  {
    if(precision == -1)
      o << intv;

    else
    {
      o.precision(precision);
      o << "[" << (double)intv.lb() << ", " << (double)intv.ub() << "]";
    }
  }

  return o.str().c_str();
}

const Interval str2intv(const char* str) {
  std::string s = str;

  if(s.find(intv2str(Interval::EMPTY_SET)) != std::string::npos)
    return Interval::EMPTY_SET;

  else if(s.find(intv2str(Interval::POS_REALS)) != std::string::npos)
    return Interval::POS_REALS;

  else if(s.find(intv2str(Interval::NEG_REALS)) != std::string::npos)
    return Interval::NEG_REALS;

  else if(s.find(intv2str(Interval::ALL_REALS)) != std::string::npos)
    return Interval::ALL_REALS;

  else { // string of the form "[float_lb,float_ub]"
    // Removing unwanted spaces:
    s.erase(std::remove(s.begin(), s.end(), ' '), s.end());

    std::string delimiter = ",";
    size_t pos_delimiter = s.find(',');
    std::string lb = s.substr(1, s.find(delimiter) - delimiter.length());
    std::string ub = s.substr(lb.length() + 2,
                              s.length() - lb.length() - delimiter.length()  - 2);

    return Interval(atof(lb.c_str()), atof(ub.c_str()));
  }
}

} // end namespace ibex
