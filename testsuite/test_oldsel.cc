/**
 * test_oldsel.cc
 *
 * test for TaggedFile and old-style selection
 *
 * Usage: test_oldsel <file>
 *
 */

#include <iostream>
#include <fstream>
#include <y2util/TaggedParser.h>
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

    enum Tags {
	VERSION, NAME, LABEL, SUGGESTS, CONFLICTS, CATEGRORY,
	VISIBLE, TOINSTALL, TODELETE
    };

    TaggedFile::TagSet tagset;

    tagset.setAllowMultipleSets (false);
    tagset.setAllowUnknownTags (true);

    tagset.addTag ("Version",	VERSION,   TaggedFile::SINGLE, TaggedFile::START);
    tagset.addTag ("Name",	NAME,	   TaggedFile::SINGLE);
    tagset.addTag ("Label",	LABEL,	   TaggedFile::SINGLE, TaggedFile::ALLOWLOCALE);
    tagset.addTag ("Suggests",	SUGGESTS,  TaggedFile::SINGLE);
    tagset.addTag ("Conflicts",	CONFLICTS, TaggedFile::SINGLE);
    tagset.addTag ("Category",	CATEGRORY, TaggedFile::SINGLE);
    tagset.addTag ("Visible",	VISIBLE,   TaggedFile::SINGLE);
    tagset.addTag ("Toinstall",	TOINSTALL, TaggedFile::MULTIOLD, TaggedFile::ALLOWLOCALE);
    tagset.addTag ("Todelete",	TODELETE,  TaggedFile::MULTIOLD, TaggedFile::ALLOWLOCALE);

    TaggedParser parser;
    parser.allowOldstyle (true);

    std::ifstream input(argv[1]);
    if(!input)
    {
	std::cerr << "file not found" << std::endl;
	return 1;
    }

    while (!input.eof())
    {
	TaggedFile::assignstatus status = tagset.assignSet (parser, input);
	if (status == TaggedFile::REJECTED_EOF)
	    break;
	else if (status == TaggedFile::ACCEPTED_FULL)
	{
	    cout << "parse complete" << endl;
	}
	else
	{
	    cerr << parser.lineNumber() << ": " << "Status " << (int)status << endl;
	    cerr << "Last tag read: " << parser.currentTag();
	    if (!parser.currentLocale().empty()) cerr << "." << parser.currentLocale();
	    cerr << endl;
	}
    }

    return 0;
}
// vim:sw=4
