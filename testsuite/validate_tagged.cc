/**
 * validate_tagged.cc
 *
 * validate a tagged file
 *
 * Usage: validate_tagged <file>
 * outputs errors to stderr
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

    std::ifstream input(argv[1]);
    if(!input)
    {
	std::cerr << "file not found" << std::endl;
	return 1;
    }

    bool go_on = true;
    int ret = 0;

    while (go_on)
    {
	TaggedParser::TagType type;

	// find any initial tag
	type = parser.lookupTag (input);

	switch (type)
	{
	    case TaggedParser::NONE:	// normal EOF
		go_on = false;
		break;
	    case TaggedParser::SINGLE:	// single tag
		break;
	    case TaggedParser::START:	// start found, look for matching end
	    {
		string starttag = parser.currentTag();
		string startlang = parser.currentLocale();
		int line = parser.lineNumber();
		type = parser.lookupEndTag (input, starttag, startlang);
		if (type != TaggedParser::END)
		{
		    cerr << "No matching end tag for '" << starttag << "." << startlang << "' at line " << line << " found." << endl;
		    go_on = false;
		}
		break;
	    }
	    case TaggedParser::END:		// spurious end found
	    {
		cerr << "Spuriuous end tag '" << parser.currentTag() << "." << parser.currentLocale() << "' at line " << parser.lineNumber() << " found" << endl;
	    }
	    break;
	    default:
		cerr << "Internal error, unknown tag type found";
	    break;
	}
	if (!go_on)
	{
	    ret = 1;
	}
    }

    return ret;
}
// vim:sw=4
