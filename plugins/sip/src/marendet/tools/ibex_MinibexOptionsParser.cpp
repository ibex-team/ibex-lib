/* ============================================================================
 * I B E X - ibex_MinibexOptionsParser.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_MinibexOptionsParser.h"

#include "ibex_UnknownFileException.h"

#include <algorithm>
#include <fstream>
#include <cctype>
#include <iterator>
#include <utility>

namespace ibex {
MinibexOptionsParser::MinibexOptionsParser(const std::vector<std::string>& accepted_options) :
		option_prefix_char('!'), accepted_options_(accepted_options) {

}

std::vector<std::string> MinibexOptionsParser::unsupported_options() const {
	//std::vector<std::string> res;
	/*for(const auto pair : values_) {
		auto it = std::find(accepted_options_.begin(), accepted_options_.end(), pair.first);
		if(it == accepted_options_.end()) {
			res.emplace_back(pair.second.name());
		}
	}*/
	//return res;
	return unsupported_options_;
}

std::vector<std::string> MinibexOptionsParser::as_argv_list() const {
	return argv_;
}

bool MinibexOptionsParser::is_valid_option(const std::string& name) const {
	return std::find(accepted_options_.begin(), accepted_options_.end(), name) != accepted_options_.end();
}

/*bool MinibexOptionsParser::option_exists(const std::string& name) const {
	return values_.find(name) != values_.end();
}*/

/*Value MinibexOptionsParser::get_option(const std::string& name) const {
	return values_.at(name);
}*/

std::string MinibexOptionsParser::parse_option_name(std::string::iterator &it) {
	std::string word = "";
	while (std::isalnum(*it) || *it == '_' || *it == '-') {
		word += *it;
		it++;
	}
	return word;
}

std::string MinibexOptionsParser::parse_option_value(std::string::iterator &it) {
	std::string value = "";
	if (*it == '\"') {
		it++;
		while (*it != '\"') {
			value += *it;
			it++;
		}
		it++;
	} else {
		value += *it;
		it++;
		while (!std::isspace(*it)) {
			value += *it;
			it++;
		}
	}
	return value;
}

void MinibexOptionsParser::parse_option(std::string::iterator &it) {
	std::string option_name = parse_option_name(it);
	if(!is_valid_option(option_name)) {
		unsupported_options_.emplace_back(option_name);
		return;
	}
	skip_spaces(it);
	if (*it != '=') {
		argv_.emplace_back(option_name);
		return;
	}
	it++;
	skip_spaces(it);
	std::string value = parse_option_value(it);
	argv_.emplace_back(option_name+"="+value);
	//values_.emplace(std::make_pair(option_name, Value(option_name, value)));
}

void MinibexOptionsParser::parse_block_comment(std::string::iterator &it) {
	bool in_comment = true;
	while (in_comment) {
		if (*it == '*') {
			it++;
			if (*it == '/') {
				in_comment = false;
			}
		} else if (*it == option_prefix_char) {
			it++;
			parse_option(it);
		} else {
			it++;
		}
	}
}

void MinibexOptionsParser::parse_line_comment(std::string::iterator &it) {
	while (*it != '\n') {
		if (*it == option_prefix_char) {
			it++;
			skip_spaces(it);
			parse_option(it);
		} else {
			it++;
		}
	}
	it++;
}

void MinibexOptionsParser::parse(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		throw UnknownFileException(filename.c_str());
	}
	std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
	file.close();
	auto it = content.begin();
	while (it != content.end()) {
		if (*it == '/') {
			it++;
			if (*it == '*') {
				it++;
				parse_block_comment(it);
			} else if (*it == '/') {
				parse_line_comment(it);
			}
		} else {
			*it++;
		}
	}
}

void MinibexOptionsParser::skip_spaces(std::string::iterator &it) {
	while (std::isspace(*it)) {
		it++;
	}
}
} // end namespace ibex
