/**
 * validate_packages_lang.cc
 *
 * validate a packages.<lang> file
 *
 * Usage: validate_packages_lang <file>
 * outputs errors to stderr
 */

#include <iostream>
#include <fstream>
#include <y2util/TaggedFile.h>

using namespace std;

int
main (int argc, char **argv)
{
    if (argc != 2)
    {
	cerr << "Usage: " << argv[0] << " <filename>" << endl;
	return 1;
    }

    std::ifstream input(argv[1]);
    if(!input)
    {
	std::cerr << "file not found" << std::endl;
	return 1;
    }

    // find initial version tag

    TaggedParser::TagType type;

    TaggedParser parser;

    // find any initial tag
    type = parser.lookupTag (input);

    if ((type != TaggedParser::SINGLE)
	|| (parser.currentTag() != "Ver")
	|| (!parser.currentLocale().empty()))
    {
	std::cerr << "Initial '=Ver:' tag missing" << endl;
	return 1;
    }

    string version = parser.data();
    if (version != "2.0")
    {
	std::cerr << "Version '" << version << "' != 2.0" << endl;
	return 1;
    }

    // now repeatedly parse complete tag sets

    TaggedFile::TagSet tagset;

    tagset.addTag ("Pkg", -1, TaggedFile::SINGLE, TaggedFile::START);
    tagset.addTag ("Sum", -1, TaggedFile::SINGLE, TaggedFile::ALLOWLOCALE);
    tagset.addTag ("Des", -1, TaggedFile::MULTI, TaggedFile::ALLOWLOCALE);
    tagset.addTag ("Ins", -1, TaggedFile::MULTI, TaggedFile::ALLOWLOCALE);
    tagset.addTag ("Del", -1, TaggedFile::MULTI, TaggedFile::ALLOWLOCALE);

    bool go_on = true;
    int count = 0;
    int ret = 0;

    while (go_on)
    {
	TaggedFile::assignstatus status = tagset.assignSet (parser, input);
	switch (status)
	{
	    case TaggedFile::ACCEPTED:
		cerr << "Incomplete set at line " << parser.lineNumber() << endl;
	    break;
	    case TaggedFile::ACCEPTED_FULL:
		count++;
	    break;
	    case TaggedFile::REJECTED_NOMATCH:
		if (input.good())
		    cerr << "Unknown tag '" << parser.currentTag() << "' at line " << parser.lineNumber() << endl;
	    break;
	    case TaggedFile::REJECTED_LOCALE:
		cerr << "Unexpected locale tag '" << parser.currentTag() << "." << parser.currentLocale() << "' at line " << parser.lineNumber() << endl;
		go_on = false;
	    break;
	    case TaggedFile::REJECTED_NOLOCALE:
		cerr << "Locale missing for '" << parser.currentTag() << "' at line " << parser.lineNumber() << endl;
		go_on = false;
	    break;
	    case TaggedFile::REJECTED_FULL:
		cerr << "Duplicate tag '" << parser.currentTag() << "' at line " << parser.lineNumber() << endl;
		go_on = false;
	    break;
	    case TaggedFile::REJECTED_NOENDTAG:
		cerr << "Missing end tag at line " << parser.lineNumber() << endl;
		go_on = false;
	    break;
	    default:
		cerr << "Internal error, unknown tag status";
		go_on = false;
	    break;
	}
	if (!input.good())
	    break;
	if (!go_on)
	{
	    ret = 1;
	}
    }

    if (ret == 0)
	cout << count << " packages ok." << endl;
    else
	cout << "Error." << endl;
    return ret;
}
// vim:sw=4
