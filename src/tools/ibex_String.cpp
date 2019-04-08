//============================================================================
//                                  I B E X
// File        : ibex_String.cpp
// Author      : Gilles Chabert
// Copyright   : IMT Atlantique (France)
// License     : See the LICENSE file
// Created     : Jul 18, 2012
// Last Update : Jan 17, 2018
//============================================================================

#include "ibex_String.h"
#include "ibex_SyntaxError.h"

#include <stdlib.h>
#include <cassert>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <atomic>

using namespace std;

namespace ibex {

#ifdef _MSC_VER
#define SNPRINTF _snprintf
#define __attribute__(x)
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
	stringstream s;
	s << base << num;
	return strdup(s.str().c_str());
}

char* next_generated_var_name() {
	static atomic_uint generated_var_count(0);
	return next_generated_name(BASE_VAR_NAME,generated_var_count++);
}

char* next_generated_func_name() {
	static atomic_uint generated_func_count(0);
	return next_generated_name(BASE_FUNC_NAME,generated_func_count++);
}

int parse_integer(const std::string& _str) {
	if (_str.empty()) {
		stringstream message;
		message << "number expected";
		throw SyntaxError(message.str());
	}
	const char* str=_str.c_str();
	char* last;
	int value=strtol(str,&last,10);
	if ((last-str)<((int) strlen(str))) {
		stringstream message;
		message << "\"" << str << "\" is not a valid number";
		throw SyntaxError(message.str());
	} else
		return value;
}

namespace {
// should the two following function be declared in the header?

/*
 * return <-1,-1> if "all range"
 */
pair<int,int> parse_index_range(const std::string& str, bool matlab_style) {
	int i=str.find(":");
	if (i==0) {
		if (str.size()>1) throw SyntaxError("malformed indices");
		else return make_pair(-1,-1);
	}
	if (i==-1) {
		int index=parse_integer(str);
		if (matlab_style) index--;
		return make_pair(index,index);
	} else {
		string start_index_str=str.substr(0,i);
		string end_index_str=str.substr(i+1,str.size()-i-1);
		int start_index=parse_integer(start_index_str);
		int end_index=parse_integer(end_index_str);
		if (matlab_style) {
			start_index--;
			end_index--;
		}
		return make_pair(start_index,end_index);
	}
}

DoubleIndex parse_double_index(const Dim& dim, const std::string& str, bool matlab_style) {
	int i=str.find(",");
	if (i==-1) {
		pair<int,int> rows=parse_index_range(str,matlab_style);
		if (rows.first==-1)
			return DoubleIndex::all(dim);
		else
			return DoubleIndex::rows(dim, rows.first, rows.second);
	} else {
		string rows_str=str.substr(0,i);
		string cols_str=str.substr(i+1,str.size()-i-1);
		pair<int,int> rows=parse_index_range(rows_str,matlab_style);
		pair<int,int> cols=parse_index_range(cols_str,matlab_style);
		if (rows.first==-1)
			if (cols.first==-1)
				return DoubleIndex::all(dim);
			else
				return DoubleIndex::cols(dim, cols.first, cols.second);
		else
			if (cols.first==-1)
				return DoubleIndex::rows(dim, rows.first, rows.second);
			else
				return DoubleIndex(dim,
						rows.first, rows.second,
						cols.first, cols.second);
	}
}

void throw_invalid_variable(const string& str) __attribute__ ((noreturn));

void throw_invalid_variable(const string& str) {
	stringstream s;
	s << "\"" << str << "\" is not a valid variable";
	throw SyntaxError(s.str());
}

}// end anonymous namespace

const ExprNode& parse_indexed_symbol(const SymbolMap<const ExprSymbol*>& symbols, const std::string& str) {

	int i; // substring past-the-end index

	bool matlab_style=false;
	i=str.find("[");
	if (i==-1) {
		matlab_style=true;
		i=str.find("(");
	}

	if (i==-1) {
		const char* symbol_name=str.c_str();
		if (!symbols.used(symbol_name))
			throw_invalid_variable(str);
		else
			return *symbols[symbol_name];
	}

	string symbol_name_str=str.substr(0,i); // get symbol nam
	const char* symbol_name=symbol_name_str.c_str();
	//cout << "symbol name=" << symbol_name << endl;
	if (!symbols.used(symbol_name)) throw_invalid_variable(str);

	const ExprNode& x=*symbols[symbol_name];

	int last=str.size()-1;

	if ((matlab_style && str[last]!=')') ||
		(!matlab_style && str[last]!=']') ||
		(last==i+1) /* empty index */)
		throw_invalid_variable(str);

	string double_index_str=str.substr(i+1,last-i-1); // get index
	//cout << "dbl index=" << double_index_str << " last=" << last << " " << str[last-1] << endl;
	try {
		DoubleIndex double_index=parse_double_index(x.dim, double_index_str, matlab_style);

		return x[double_index].simplify();

	} catch(DimException&) {// problem with DoubleIndex
		throw_invalid_variable(str);
	}
}

char* random_alphanum_string(int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    char* s = new char[len+1];
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
    return s;
}

} // end namespace ibex
