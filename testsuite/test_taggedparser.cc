/**
 * test_taggedparser.cc
 *
 * test for TaggedParser
 *
 * Usage: test_taggedparser <file>
 *
 */

#include <iostream>
#include <fstream>
#include <y2util/TaggedParser.h>

using namespace std;

int
main (int argc, char **argv)
{
    if (argc != 2)
    {
	cerr << "Usage: " << argv[0] << " <filename>" << endl;
	return 1;
    }

    TaggedParser parser;
    TaggedParser::TagType type;
    std::string tagstr;

    std::ifstream input(argv[1]);
    if(!input)
    {
	std::cerr << "file not found" << std::endl;
	return 1;
    }

    while (((type = parser.lookupTag (input)) != TaggedParser::NONE))
    {
	switch (type)
	{
	   case TaggedParser::NONE: cout << "None: "; break;
	   case TaggedParser::SINGLE: cout << "Single: "; break;
	   case TaggedParser::START: cout << "Start: "; break;
	   case TaggedParser::END: cout << "End: "; break;
	   default: cout << "???: "; break;
	}
	cout << "Start <" << parser.currentTag() << ">["
		<< parser.tagPos() << "], End <"
		<< parser.dataStartPos() << ">[" << parser.dataEndPos() << "]" << endl;
    }

    return 0;
}
// vim:sw=4
