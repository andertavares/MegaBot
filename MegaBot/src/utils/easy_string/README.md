Easy String In CPP
-------------------


Inspired by a conversation on IRC with [Pr0Wolf29](https://github.com/Pr0Wolf29/)


Provides functions that take strings and return strings (or as reference) in a namespace (obviously)

Set for camelcase


This repo will not sublass the std::string see [this](https://stackoverflow.com/questions/6006860/why-should-one-not-derive-from-c-std-string-class).


#Functions Implemented:


* std::string char_to_string(char c) {
* bool is_space(std::string s) {
* std::string to_lower(std::string s) {
* std::string lc(std::string s) { return to_lower(s); }
* std::string to_upper(std::string s) {
* std::string uc(std::string s) { return to_upper(s); }
* bool is_lower(std::string s) {
* bool is_upper(std::string s) {
* bool is_word_in(std::string sentence, std::string word) {
* std::string remove_html_comments(std::string html_response) {
* std::string replace(std::string s, std::string from, std::string to) {
* std::string replace_all(std::string s, std::string from, std::string to) {
* std::string replace_all_recursive(std::string s, std::string from, std::string to) {
* bool starts_with(std::string s, std::string start) {
* bool ends_with(std::string s, std::string end) {
* std::string chop(std::string s) {
* std::string rstrip(std::string s) {
* std::string chomp(std::string s){
* std::string lstrip(std::string s) {
* std::string strip(std::string s) {
* bool is_hex(std::string s) {
* std::vector<std::string> split(std::string s, std::string separator) {
* std::vector<std::string> split_lines(std::string s) {
* bool is_number(std::string s) {
* std::string remove_space(std::string s) {
* std::string remove_endlines(std::string s) {
* std::string remove_occurence(std::string s, std::string word) {
* std::string remove_occurence_recursive(std::string s, std::string word) {
* std::string remove_special_chars(std::string s) {
* std::string capitalize(std::string s) {
* std::string reverse(std::string s) {
* std::string center(std::string s, int width, std::string padding=" ") {
* int count(std::string s, std::string word, int start=-1,int end=-1) {
* std::string expand_tabs(std::string s, int tabsize=8) {
* std::string swap_case(std::string s) {
* std::string zfill(std::string s, int padding) {
* std::vector<std::string> partition(std::string s, std::string separator) {
* std::vector<std::string> rpartition(std::string s, std::string separator) {
* std::string translate(std::string s, std::string from, std::string to) {
* std::string tr(std::string s, std::string from, std::string to) {


# Nice foos already implemented in the standard library of cpp11

* 	float              std::stof(const string& str, size_t *idx = 0);
* 	double             std::stod(const string& str, size_t *idx = 0);
* 	long double        std::stold(const string& str, size_t *idx = 0);
* 	int                std::stoi(const string& str, size_t *idx = 0, int base = 10);
* 	long               std::stol(const string& str, size_t *idx = 0, int base = 10);
* 	unsigned long      std::stoul(const string& str, size_t *idx = 0, int base = 10);
* 	long long          std::stoll(const string& str, size_t *idx = 0, int base = 10);
* 	unsigned long long std::stoull(const string& str, size_t *idx = 0, int base = 10);
* 	string std::to_string(int val);
* 	string std::to_string(unsigned val);
* 	string std::to_string(long val);
* 	string std::to_string(unsigned long val);
* 	string std::to_string(long long val);
* 	string std::to_string(unsigned long long val);
* 	string std::to_string(float val);
* 	string std::to_string(double val);
* 	string std::to_string(long double val);


* 	isalnum - Check if character is alphanumeric (function )
* 	isalpha - Check if character is alphabetic (function )
* 	isblank - Check if character is blank (function )
* 	iscntrl - Check if character is a control character (function )
* 	isdigit - Check if character is decimal digit (function )
* 	isgraph - Check if character has graphical representation (function )
* 	islower - Check if character is lowercase letter (function )
* 	isprint - Check if character is printable (function )
* 	ispunct - Check if character is a punctuation character (function )
* 	isspace - Check if character is a white-space (function )
* 	isupper - Check if character is uppercase letter (function )
* 	isxdigit - Check if character is hexadecimal digit (function )
* 	Character conversion functions:
* 	tolower - Convert uppercase letter to lowercase (function )
* 	toupper - Convert lowercase letter to uppercase (function )

*	rfind - find characters in the string in reverse
*	find  - find characters in the string
*	find_first_of - find first occurrence of characters
*	find_first_not_of - find first absence of characters
*	find_last_of - find last occurrence of characters
*	find_last_not_of - find last absence of characters

*	regex_match
*	regex_replace
