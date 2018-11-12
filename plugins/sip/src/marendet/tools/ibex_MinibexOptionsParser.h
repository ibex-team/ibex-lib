/* ============================================================================
 * I B E X - ibex_MinibexOptionsParser.h
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#ifndef __SIP_IBEX_MINIBEXOPTIONSPARSER_H__
#define __SIP_IBEX_MINIBEXOPTIONSPARSER_H__

#include "ibex_Vector.h"

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

namespace ibex {
/*class NoValidConversionException: public std::exception {
public:
	NoValidConversionException(const char* what) :
			what_(what) {
	}
	const char* what() const _NOEXCEPT {
		return what_;
	}

private:
	const char* what_;
};

class Value {
public:
	Value(const std::string& name, const std::string& value) :
			name_(name), value_(value) {
	}

	std::string name() const {
		return name_;
	}

	std::string as_string() const {
		return value_;
	}

	int as_int() const {
		try {
			return std::stoi(value_);
		} catch (std::invalid_argument &e) {
			std::string what = value_ + " is not a valid int: " + e.what();
			throw NoValidConversionException(what.c_str());
		}
	}

	double as_double() const {
		try {
			return std::stod(value_);
		} catch (std::invalid_argument &e) {
			std::string what = value_ + " is not a valid double: " + e.what();
			throw NoValidConversionException(what.c_str());
		}
	}

	double as_bool() const {
		if (value_ == "false" || value_ == "0") {
			return false;
		} else if (value_ == "true" || value_ == "1") {
			return true;
		} else {
			std::string what = value_ + " is not a valid bool";
			throw NoValidConversionException(what.c_str());
		}
	}

	Vector as_vector() const {
		std::vector<double> numbers;
		if (value_[0] != '(' || value_[value_.size() - 1] != ')') {
			throw NoValidConversionException((value_ + " is not a valid vector").c_str());
		}
		size_t pos = value_.find(';', 1);
		size_t last_pos = 1;
		while (pos != std::string::npos) {
			try {
				numbers.emplace_back(std::stod(value_.substr(last_pos, pos - last_pos)));
			} catch (std::invalid_argument& e) {
				throw NoValidConversionException((value_ + " is not a valid vector:" + e.what()).c_str());
			}
			last_pos = pos + 1;
			pos = value_.find_first_of(";)", pos + 1);
		}
		Vector dest = Vector(numbers.size(), 0.0);
		for (size_t i = 0; i < numbers.size(); ++i) {
			dest[i] = numbers[i];
		}
		return dest;
	}

private:
	std::string value_;
	std::string name_;
};
*/

class MinibexOptionsParser {
public:
	MinibexOptionsParser(const std::vector<std::string>& accepted_options);
	virtual ~MinibexOptionsParser() {
	}
	void parse(const std::string& filename);

	//bool option_exists(const std::string& option_name) const;
	//Value get_option(const std::string& option_name) const;
	std::vector<std::string> unsupported_options() const;
	std::vector<std::string> as_argv_list() const;

	char option_prefix_char;
	//std::map<std::string, Value> values_;
private:
	std::vector<std::string> accepted_options_;
	std::vector<std::string> unsupported_options_;
	std::vector<std::string> argv_;

	bool is_valid_option(const std::string& name) const;
	void skip_spaces(std::string::iterator &it);
	// ([[:alnum:]]|_|.)+
	std::string parse_option_name(std::string::iterator &it);
	// (".*"|\S*)
	std::string parse_option_value(std::string::iterator &it);
	void parse_option(std::string::iterator &it);
	void parse_block_comment(std::string::iterator &it);
	void parse_line_comment(std::string::iterator &it);
};

} // end namespace ibex

#endif // __SIP_IBEX_MINIBEXOPTIONSPARSER_H__
