/*---------------------------------------------------------------------\
 |                                                                      |
 |                      __   __    ____ _____ ____                      |
 |                      \ \ / /_ _/ ___|_   _|___ \                     |
 |                       \ V / _` \___ \ | |   __) |                    |
 |                        | | (_| |___) || |  / __/                     |
 |                        |_|\__,_|____/ |_| |_____|                    |
 |                                                                      |
 |                               core system                            |
 |                                                        (C) SuSE GmbH |
 \----------------------------------------------------------------------/

 File:       TaggedParser.h

 Author:     Michael Andres <ma@suse.de>
 Maintainer: Klaus Kaempf <kkaempf@suse.de>

 Parser for tagged file format as used in SuSE/UnitedLinux
 media data

 /-*/
#ifndef TaggedParser_h
#define TaggedParser_h

#include <iosfwd>
#include <string>
#include <list>
#include <vector>
#include <map>

using std::vector;
using std::map;
using std::streampos;
using std::istream;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : TaggedParser
//
//
//	DESCRIPTION :
//		parses file in "tagged" format
//		a tag starts at the beginning of a line with
//		'=' (single line tag),
//		'+' (start of multi-line tag), or
//		'-' (end of multi line tag)
//		followed by an arbitrary string and a colon (':')
//		The tag parser 'lookupTag()' searches through an open
//		stream for such a tag
//
class TaggedParser {
  public:
    typedef enum tag_type {
	NONE=0,		// no tag
	SINGLE,		// single
	START,		// start
	END		// end
    } TagType;

  private:

    static const unsigned bufferLen_i;
    static char           buffer_ac[];

    std::string currentLine;

  private:

    streampos _tagPos;		// position of tag
    streampos _startPos;	// start postition of data
    streampos _endPos;		// end position of data

    int _bufferPos;		// position of data in buffer
    int _bufferLen;		// length of data in buffer

    int _lineNumber;

    std::string _currentTag;
    std::string _currentLocale;
    std::string _currentData;		// substr of currentLine, set by data()

  private:

    void _reset();
    void _datareset();

    // read line from stream
    static streampos  readLine (istream & stream_fr, std::string & cline_tr );

    // check line for tag
    static TagType tagOnLine (const std::string & cline_tr, std::string & tag_tr, std::string::size_type & delim_ir, std::string & lang_tr );

  public:

    TaggedParser();
    virtual ~TaggedParser();

    static const streampos nopos;

    int lineNumber () const { return _lineNumber; }

    streampos tagPos() const { return _tagPos; }
    const std::string & currentTag() const { return _currentTag; }
    const std::string & currentLocale() const { return _currentLocale; }

  public:

    streampos      dataStartPos () const { return _startPos; }
    streampos      dataEndPos ()   const { return _endPos; }
    unsigned       dataLength ()   const { return _endPos - _startPos; }

    // valid after parseData()
    const std::string& data() { return (_currentData = currentLine.substr (_bufferPos, _bufferLen)); }

  public:

    /**
     * lookup a tag
     * parse until stag_tr, leave empty to parse until any tag. On succes
     * posStartTag() and posEndTag() can be used
     * Usually used to lookup a single or a start tag
     */
    TagType lookupTag( istream & stream_fr, const std::string & stag_tr = "", const std::string & slang_tr = "" );

    /**
     * lookup end tag
     * set start and end retrieval positions
     */ 
    TagType lookupEndTag ( istream & stream_fr, const std::string & etag_tr, const std::string & elang_tr = "" );

    // helper functions
    static std::string data2string( const std::list<std::string> & data_Vtr );
    static vector<std::string> split2words( const std::string & line_tr, const std::string & sepchars_tr = " \t\n" );
};

#endif // TaggedParser_h

// vim:sw=2
