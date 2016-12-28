#include "easy_string.hpp"
#include <iostream>

using namespace std;

int
main(int argc, char** argv) {
	std::string s = "Hello World";
	s = easy_string::lc(s);
	std::cout<<"is lower: "<< easy_string::is_lower(s) << std::endl;
	std::cout<<"is upper: "<< easy_string::is_upper(s) << std::endl;
	std::cout<< s <<std::endl;
	std::cout<< "check if 'world' is in the string: "<<
		easy_string::is_word_in(s,"world")<<
		std::endl;
	s += " "+std::to_string(1337);
	std::cout<< "concatenating the int 1337 to the string "<<
		s << std::endl;
	std::cout<< "relacing 'world' with 'universe': "
		<< easy_string::replace(s,"world","universe")
		<< std::endl;
	std::cout << "new string hello world hello world"<< std::endl;
	std::string s2 = "hello world hello world";
	std::cout<< "relacing all 'world' with 'universe and world': "
		<< easy_string::replace_all(s2,"world","universe")
		<< std::endl;
	std::cout<< "starting with hello?:" << easy_string::starts_with(s,"hello")
			<<std::endl;
	std::cout<< "starting with world?:" << easy_string::starts_with(s,"world")
			<<std::endl;
	std::cout<< "ending with hello?:" << easy_string::ends_with(s2,"hello")
			<<std::endl;
	std::cout<< "ending with world?:" << easy_string::ends_with(s2,"world")
			<<std::endl;
	std::cout<< "chopping two chars:" << easy_string::chop(easy_string::chop(s2))
			<<std::endl;
	std::string s3 = "\t\t\n\t\tThis needs to be stripped\t\t\n\n\n";
	std::cout<< "stripping :" << easy_string::strip(s3) <<std::endl;
	std::cout<< "checking if 023B is hex :" << easy_string::is_hex("023B") <<std::endl;
	std::cout<< "checking if 023BX is hex :" << easy_string::is_hex("023BX") <<std::endl;

	std::cout<<"splitting 'hello world blah world blah world end of string' by 'world'"<<endl;
	vector<string> the_vect = easy_string::split("hello world blah world blah world end of string","world");
	for (string i : the_vect) {
		std::cout<<easy_string::strip(i)<<std::endl;
	}

	std::cout<<"splitting 'nope\nnope\n\n\nnope' by lines "<<endl;
	the_vect.clear();
	the_vect = easy_string::split_lines("nope\nnope\n\n\nnope");
	for (string i : the_vect) {
		std::cout<<easy_string::strip(i)<<std::endl;
	}

	cout<<"checking if '-1.2345' is a number: "<<easy_string::is_number("-1.2345")<<endl;
	cout<<"checking if '-1.aa2345' is a number: "<<easy_string::is_number("-1.aa2345")<<endl;

	cout<<"removing white char from 'nope\nnope\n\n\nnope': "
		<< easy_string::remove_space("nope\nnope\n\n\nnope")<<endl;

	cout<<"removing endlines from 'nope\nnope\n\n\nnope': "
		<< easy_string::remove_endlines("nope\nnope\n\n\nnope")<<endl;

	cout<<"removing all bob from 'this is bboboban example string by bobvenam' recursively: "
		<< easy_string::remove_occurence_recursive(
			"this is bboboban example string by bobvenam",
			"bob"
			)
		<<endl;

	cout<<"removing special chars from '$)(*@#hello 0(*)world\\n':"
		<<easy_string::remove_special_chars("$)(*@#hello 0(*)world\n")<<endl;

	cout<<"capitalizing 'hello world i': "<<easy_string::capitalize("hello world i")<<endl;

	cout<<easy_string::reverse("hello world")<<endl;
	cout<<easy_string::center("hello world",4,"\t")<<endl;
	cout<<"counting number of l in hello world: "
		<< easy_string::count("hello world","l")<<endl;

	cout<<"zfilling a number: "<<easy_string::zfill("323",10)<<endl;

	the_vect.clear();
	the_vect = easy_string::partition("hello world now","r");
	for (string i : the_vect) {
		std::cout<<i<<std::endl;
	}

	the_vect.clear();
	the_vect = easy_string::rpartition("hello world now","l");
	for (string i : the_vect) {
		std::cout<<i<<std::endl;
	}

	return 0;
}
