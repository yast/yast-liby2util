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
    parser.asOldstyle (true);
    TaggedParser::TagType type;
    std::string tagstr;

    std::ifstream input(argv[1]);
    if(!input)
    {
	std::cerr << "file not found" << std::endl;
	return 1;
    }

    while (!input.eof())
    {
	type = parser.lookupTag (input);

	switch (type)
	{
	   case TaggedParser::NONE: cout << "None" << endl; break;
	   case TaggedParser::SINGLE: cout << "Single: "; break;
	   case TaggedParser::START: cout << "Start: "; break;
	   case TaggedParser::END: cout << "End: "; break;
	   case TaggedParser::OLDSINGLE: cout << "Oldsingle: "; break;
	   case TaggedParser::OLDMULTI: cout << "Oldmulti: "; break;
	   default: cout << "???: "; break;
	}
	if (type == TaggedParser::NONE)
	    continue;

	cout << "Start <" << parser.currentTag();
	if (!parser.currentLocale().empty())
	    cout << "." << parser.currentLocale();
	cout << ">@"
		<< parser.tagPos() << ", ["
		<< parser.dataStartPos() << ", " << parser.dataEndPos() << "]" << endl;
    }

    return 0;
}
// vim:sw=4
