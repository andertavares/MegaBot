/*
Copyright (c) 2015, Patrick Louis <patrick at iotek.org>
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    1.  The author is informed of the use of his/her code. The author does not have to consent to the use; however he/she must be informed.
    2.  If the author wishes to know when his/her code is being used, it the duty of the author to provide a current email address at the top of his/her code, above or included in the copyright statement.
    3.  The author can opt out of being contacted, by not providing a form of contact in the copyright statement.
    4.  If any portion of the author’s code is used, credit must be given.
            a. For example, if the author’s code is being modified and/or redistributed in the form of a closed-source binary program, then the end user must still be made somehow aware that the author’s work has contributed to that program.
            b. If the code is being modified and/or redistributed in the form of code to be compiled, then the author’s name in the copyright statement is sufficient.
    5.  The following copyright statement must be included at the beginning of the code, regardless of binary form or source code form.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Except as contained in this notice, the name of a copyright holder shall not
be used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization of the copyright holder.

compile with: g++ t.cpp -std=c++11 -o t
*/

#ifndef EASY_STRING_HPP_INCLUDED
#define EASY_STRING_HPP_INCLUDED


#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <string.h>
#include <regex> //use that in a smart way ψ(｀∇´)ψ
#include <iostream>
#include <unordered_map>

namespace easy_string {
	/*
	 *
	 * nice sexy body here
	 *
	 */


	/*

	//NB: Nice foos built in the standard library of cpp11
	float              std::stof(const string& str, size_t *idx = 0);
	double             std::stod(const string& str, size_t *idx = 0);
	long double        std::stold(const string& str, size_t *idx = 0);
	int                std::stoi(const string& str, size_t *idx = 0, int base = 10);
	long               std::stol(const string& str, size_t *idx = 0, int base = 10);
	unsigned long      std::stoul(const string& str, size_t *idx = 0, int base = 10);
	long long          std::stoll(const string& str, size_t *idx = 0, int base = 10);
	unsigned long long std::stoull(const string& str, size_t *idx = 0, int base = 10);
	string std::to_string(int val);
	string std::to_string(unsigned val);
	string std::to_string(long val);
	string std::to_string(unsigned long val);
	string std::to_string(long long val);
	string std::to_string(unsigned long long val);
	string std::to_string(float val);
	string std::to_string(double val);
	string std::to_string(long double val);


	isalnum - Check if character is alphanumeric (function )
	isalpha - Check if character is alphabetic (function )
	isblank - Check if character is blank (function )
	iscntrl - Check if character is a control character (function )
	isdigit - Check if character is decimal digit (function )
	isgraph - Check if character has graphical representation (function )
	islower - Check if character is lowercase letter (function )
	isprint - Check if character is printable (function )
	ispunct - Check if character is a punctuation character (function )
	isspace - Check if character is a white-space (function )
	isupper - Check if character is uppercase letter (function )
	isxdigit - Check if character is hexadecimal digit (function )
	Character conversion functions:
	tolower - Convert uppercase letter to lowercase (function )
	toupper - Convert lowercase letter to uppercase (function )

	rfind
		find characters in the string in reverse
	find
		find characters in the string
	find_first_of
		find first occurrence of characters
	find_first_not_of
		find first absence of characters
	find_last_of
		find last occurrence of characters
	find_last_not_of
		find last absence of characters

	regex_match
	regex_replace

	*/


	std::string char_to_string(char c) {
		std::string s = "";
		s += c;
		return s;
	}

	bool is_space(std::string s) {
		return (std::regex_match(s, std::regex("^\\s+$")));
	}


	std::string to_lower(std::string s) {
		transform(s.begin(), s.end(), s.begin(), (int(*)(int)) std::tolower);
		return s;
	}
	std::string lc(std::string s) { return to_lower(s); }
	std::string to_upper(std::string s) {
		std::transform(s.begin(), s.end(), s.begin(), (int(*)(int)) std::toupper);
		return s;
	}
	std::string uc(std::string s) { return to_upper(s); }

	bool is_lower(std::string s) {
		return std::all_of(
			s.begin(),
			s.end(),
			[](char t) {
				if (easy_string::is_space(char_to_string(t))) {
					return true;
				}
				else {
					return (std::islower(t)>0);
				}
			}
		);
	}
	bool is_upper(std::string s) {
		return std::all_of(
			s.begin(),
			s.end(),
			[](char t) {
				if (easy_string::is_space(char_to_string(t))) {
					return true;
				}
				else {
					return (std::isupper(t)>0);
				}
			}
		);
	}

	//check if another string is in the string
	bool is_word_in(std::string sentence, std::string word) {
		return (sentence.find(word)!=std::string::npos);
	}


	std::string remove_html_comments(std::string html_response) {
		//save the start position of the start of a comment
		unsigned int position_start = html_response.find ("<!--");
		unsigned int position_stop  = html_response.find ("-->");
		if (position_start != std::string::npos) {
			//check for the end
			//if no end then we remove all after the start of the comment
			if (position_stop != std::string::npos) {
				html_response.erase(position_start, (position_stop-position_start)+3);
			}
			else {
				html_response.erase(position_start,html_response.length()-1);
			}
		}
		//couldn't find any <!-- so no more comments in the html_response
		else {
		}
		return html_response;
	}

	//replace one string inside another
	std::string replace(std::string s, std::string from, std::string to) {
		size_t start_pos = 0;
		start_pos = s.find(from, start_pos);
		if (start_pos != std::string::npos) {
			s.replace(start_pos, from.length(), to);
		}
		return s;
	}

	//replace all occurence of a string inside another
	std::string replace_all(std::string s, std::string from, std::string to) {
		if(!from.empty()) {
			size_t start_pos = 0;
			while ((start_pos = s.find(from, start_pos)) != std::string::npos) {
				s.replace(start_pos, from.length(), to);
				// In case 'to' contains 'from', like replacing 'x' with 'yx'
				start_pos += to.length();
			}
		}
		return s;
	}

	//replace all occurence of a string inside another recursively, against injection
	std::string replace_all_recursive(std::string s, std::string from, std::string to) {
		while(is_word_in(s, from)) {
			s = replace_all(s, from, to);
		}
		return s;
	}

	bool starts_with(std::string s, std::string start) {
		//if start is bigger than the string than it fails
		if (start.length() > s.length()) {
			return false;
		}
		else if (start.length() == s.length()) {
			return start == s;
		}
		//loop through start and check if all char matches
		for (int i = 0; i< start.length(); i++) {
			if (start[i] != s[i]) {
				return false;
			}
		}
		return true;
	}

	bool ends_with(std::string s, std::string end) {
		//if end is bigger than the string than it fails
		if (end.length() > s.length()) {
			return false;
		}
		else if (end.length() == s.length()) {
			return end == s;
		}
		int diff = s.length() - end.length();
		for (int i = s.length()-1; i > diff-1; i--) {
			if (end[i-diff] != s[i]) {
				return false;
			}
		}
		return true;
	}

	//Removes the last character from a string
	std::string chop(std::string s) {
		return s.erase(s.length()-1, 1);
	}

	std::string rstrip(std::string s) {
		std::regex e ("(\\s*)$");
		return std::regex_replace(s, e, "");
	}

	//Removes line ending characters from a string
	std::string chomp(std::string s){
		return rstrip(s);
	};

	std::string lstrip(std::string s) {
		std::regex e ("^(\\s*)");
		return std::regex_replace(s, e, "");
	}

	//remove all \s at beginning and end
	std::string strip(std::string s) {
		s = rstrip(s);
		return lstrip(s);
	}

	//check if a string is a valid hexadecimal
	bool is_hex(std::string s) {
		int len=s.length();
		if ( len % 2 != 0) return false;
		const char *str = s.c_str();
		for (int i = 0; i < len; i++) {
			if (
				(str[i]<'0' || str[i]>'9') &&
				(str[i]<'A' || str[i]>'F') &&
				(str[i]<'a' || str[i]>'f') ) {
				return false;
			}
		}
		return true;
	}

	//split by a string and returns a vector, the string is not included in what is returned
	std::vector<std::string> split(std::string s, std::string separator) {
		std::vector<std::string> elems;
		size_t last_pos = 0;
		size_t pos = s.find(separator);
		while (pos != std::string::npos && last_pos != std::string::npos) {
			if (pos - last_pos > 0) {
				elems.push_back(s.substr(last_pos, pos - last_pos));
			}
			last_pos = s.find_first_not_of(separator, pos+separator.length()-1);
			pos = s.find(separator, last_pos);
		}
		if (last_pos != std::string::npos) {
			elems.push_back(s.substr(last_pos));
		}
		return elems;
	}

	//split at \n
	std::vector<std::string> split_lines(std::string s) {
		return split(s, "\n");
	}

	bool is_number(std::string s) {
		static const char ss[] = { '0','1','2','3','4','5', '6','7','8','9','.'};
		/* remove signs */
		if ( s[0] == '+' or s[0] == '-') {
			s.erase(0,1);
		}
		if (s.length() == 0) {
			return false;
		}
		for (unsigned i=0; i<s.length(); i++) {
			bool passed_test = false;
			for (unsigned e = 0; e < 11; e++) {
				if (s[i] == ss[e]) {
					passed_test = true;
					break;
				}
			}
			if(!passed_test) {
				return false;
			}
		}
		return true;
	}

	//remove all white space chars in a string
	std::string remove_space(std::string s) {
		std::regex e ("(\\s)");
		return std::regex_replace(s, e, "");
	}

	//only remove endlines
	std::string remove_endlines(std::string s) {
		return replace_all(s,"\n","");
	}

	//remove all occurence of a string inside another
	std::string remove_occurence(std::string s, std::string word) {
		return replace_all(s, word, "");
	}

	//remove all occurence of a string inside another recursively (safe)
	std::string remove_occurence_recursive(std::string s, std::string word) {
		return replace_all_recursive(s, word, "");
	}

	//keep only alphanumerics and space
	std::string remove_special_chars(std::string s) {
		std::string ns = "";
		for (char c : s) {
			if (isalnum(c) or isblank(c) or isspace(c)) {
				ns += char_to_string(c);
			}
		}
		return ns;
	}

	//only first characters of every word capitalized
	std::string capitalize(std::string s) {
		if (s.length() == 0) {
			return s;
		}
		std::string ns = "";
		for (unsigned i = 0; i< s.length()-1; i++) {
			if (i == 0 and isalnum(s[0])) {
				ns += toupper(s[0]);
			}
			else {
				ns += s[i];
				if (
						(isspace(s[i]) or isblank(s[i])) and
						isalnum(s[i+1])) {
					ns += toupper(s[i+1]);
				}
				else {
					ns += s[i+1];
				}
				i++;
			}
		}
		return ns;
	}

	//already implemented - just wrapping it
	std::string reverse(std::string s) {
		std::reverse(s.begin(), s.end());
		return s;
	}

	//Return S centered in a string of length width.
	//Padding is done using the specified fill character (default is a space)
	std::string center(std::string s, int width, std::string padding=" ") {
		if (width < 0) {
			return s;
		}
		for (unsigned i=0; i<width; i++) {
			s = padding+s;
		}
		return s;
	}

	//return the number of non-overlapping occurence of a substring inside a string
	int count(std::string s, std::string word, int start=-1,int end=-1) {
		if (start<0) {
			start = 0;
		}
		if (end<0) {
			end = s.length()-1;
		}
		int nb = 0;
		while (start <= end) {
			start = s.find(word, start);
			if (start != std::string::npos) {
				nb++;
				start += word.length();
			}
			else {
				break;
			}
		}
		return nb;
	}

	//Return a copy of S where all tab characters are expanded using spaces.
	//If tabsize is not given, a tab size of 8 characters is assumed.
	std::string expand_tabs(std::string s, int tabsize=8) {
		std::string width = "";
		for (int i=0; i< tabsize; i++) {
			width += " ";
		}
		return replace_all(s,"\t",width);
	}

	//change upper by lower and lower by upper
	std::string swap_case(std::string s) {
		for (unsigned i=0; i< s.length(); i++) {
			if (isupper(s[i])) {
				s[i] = tolower(s[i]);
			}
			else {
				s[i] = toupper(s[i]);
			}
		}
		return s;
	}

	//numeral padding with 0 on the left so it fits a width
	std::string zfill(std::string s, int padding) {
		if (!is_number(s)) {
			return s;
		}
		padding = padding - s.length();
		for (int i=0; i< padding; i++) {
			s = "0"+s;
		}
		return s;
	}

	//search for the first occurence of a separator and return 2 strings
	std::vector<std::string> partition(std::string s, std::string separator) {
		std::vector<std::string> v;
		int i = s.find(separator);
		if (i != std::string::npos) {
			v.push_back(s.substr(0,i));
			v.push_back(s.substr(i+separator.length(),s.length()-1));
		}
		else {
			v.push_back(s);
		}
		return v;
	}

	//same but start searching from the right
	std::vector<std::string> rpartition(std::string s, std::string separator) {
		std::vector<std::string> v;
		int i = s.rfind(separator);
		if (i != std::string::npos) {
			v.push_back(s.substr(0,i));
			v.push_back(s.substr(i+separator.length(),s.length()-1));
		}
		else {
			v.push_back(s);
		}
		return v;
	}

	//transliteration
	std::function<std::string(std::string)>
	maketrans(const std::string& from, const std::string& to) {
		std::unordered_map<char, char> map;
		for (std::string::size_type i = 0;
				i != std::min(from.size(), to.size()); ++i) {
			map[from[i]] = to[i];
		}
		//returns a foo that replace the char found in the map by the associated one
		return [=](std::string s) {
			for (auto& c : s) {
				const auto mapped_c = map.find(c);
				if (mapped_c != map.end()) {
					c = mapped_c->second;
				}
			}
			return s;
		};
	}

	std::string translate(std::string s, std::string from, std::string to) {
		const auto translate = maketrans(from, to);
		return translate(s);
	}

	std::string tr(std::string s, std::string from, std::string to) {
		return translate(s, from, to);
	}

	/* TODO:
	std::string translate(std::string s, std::vector<char> from, std::vector<char> to);
	std::string tr(std::string s, std::vector<char> from, std::vector<char> to);
	*/
	/* TODO: binary foos
	decode
	encode
	*/
}


#endif
