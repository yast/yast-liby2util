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

   File:       TaggedParser.cc

   Author:     Michael Andres <ma@suse.de>
   Maintainer: Michael Andres <ma@suse.de>

   Purpose:	parse file in tagged format
		('<tag>: <data>\n')
   Parser for tagged file format as used in SuSE/UnitedLinux
   media data
/-*/

#include <iostream>
#include <ctype.h>
#include <y2util/Y2SLog.h>
#include <y2util/TaggedParser.h>

using namespace std;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : TaggedParser
//
///////////////////////////////////////////////////////////////////

const unsigned TaggedParser::bufferLen_i = 1024;
char           TaggedParser::buffer_ac[bufferLen_i];
const streampos TaggedParser::nopos = streampos(-1);

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : TaggedParser::_datareset
//	METHOD TYPE : void
//
//	DESCRIPTION : reset position pointers to 'nopos'
//			erase all buffers
//
inline void TaggedParser::_datareset()
{
    _startPos = _endPos = nopos;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : TaggedParser::_reset
//	METHOD TYPE : void
//
//	DESCRIPTION :
//
inline void TaggedParser::_reset()
{
    _tagPos = nopos;
    _currentTag.erase();
    _currentLocale.erase();
    _datareset();
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : TaggedParser::TaggedParser
//	METHOD TYPE : Constructor
//
//	DESCRIPTION :
//
TaggedParser::TaggedParser()
    : _lineNumber (0)
    , _allow_oldstyle (false)
    , _offset (1)
{
    _reset();
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : TaggedParser::~TaggedParser
//	METHOD TYPE : Destructor
//
//	DESCRIPTION :
//
TaggedParser::~TaggedParser()
{
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : TaggedParser::readLine
//	METHOD TYPE : streampos
//
//	DESCRIPTION : read a line from an istream to a string
//		      return streampos of start of line
//
inline streampos TaggedParser::readLine( istream & stream_fr, string & cline_tr )
{
    streampos lineBegin_ii = stream_fr.tellg();
    cline_tr.erase();

    do
    {
	stream_fr.clear();
	stream_fr.getline (buffer_ac, bufferLen_i); // always writes '\0' terminated
	cline_tr += buffer_ac;
    } while( stream_fr.rdstate() == ios::failbit );

    return lineBegin_ii;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : TaggedParser::tagOnLine
//	METHOD TYPE : TagType
//
//	DESCRIPTION : check if a tag exists on the line
//		      return != TagType::NONE if tag exists
//		      O: string tag_tr		tag found
//		      O: size_type delim_ir	position following tag
//		      O: string lang_tr		language code found
//
TaggedParser::TagType
TaggedParser::tagOnLine (const string & cline_tr, string & tag_tr, string::size_type & delim_ir, string & lang_tr)
{
    TagType type = NONE;

    // empty or comment line ?

    if (cline_tr.size() != 0)
    {
	switch (cline_tr[0])
	{
	    case '#':
	    case '\0':
	    case '\n':
		break;
	    case '=':
		type = SINGLE; break;
	    case '+':
		type = START; break;
	    case '-':
		type = END; break;
	    default:
		if ((_allow_oldstyle)
		    && isupper (cline_tr[0]))				// check oldstyle
		{
		    string::size_type colonpos = cline_tr.find (":");	// ":" + blank
		    if (colonpos != string::npos)
		    {
			colonpos++;
			if (cline_tr[colonpos] == ' ' || cline_tr[colonpos] == '\t')
			    type = OLDSINGLE;
			else
			    type = OLDMULTI;
		    }
		}
		break;
	}
    }

    if (type == NONE)
    {
	return type;
    }

    // find first separator

    delim_ir = cline_tr.find_first_of (":. \t", _offset);

    int taglen = delim_ir - _offset;

    // no tag or empty tag or whitespace in tag ?

    if (delim_ir == string::npos
	|| delim_ir < 2)
    {
	type = NONE;				// no delimiter found
    }
    else if (cline_tr[delim_ir] == '.' )	// language found ?!
    {
	string::size_type langpos = delim_ir+1;
	delim_ir = cline_tr.find_first_of (":", langpos);
	if ((delim_ir == string::npos)		// nope, ":" missing
	    || (delim_ir < langpos+1))		// language empty
	{
	    type = NONE;
	}
	else
	{
	    lang_tr = cline_tr.substr (langpos, delim_ir-langpos);
	}
    }
    else
    {
	lang_tr.clear();
    }

    if (type == NONE)
    {
	return type;
    }

    tag_tr = cline_tr.substr (_offset, taglen);

    return type;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : TaggedParser::lookupTag
//	METHOD TYPE : TaggedParser::TagType
//
//	DESCRIPTION : read from istream until known tag is found
//			return type of tag found
//		      if stag_tr!="" slang_tr!="", the tag must match
//		      the given values (used to find a matching end tag)
//
TaggedParser::TagType
TaggedParser::lookupTag( istream & stream_fr, const string & stag_tr, const string & slang_tr)
{
    _reset();
    if ( stream_fr.good() )
    {
	streampos         lineBegin_ii = nopos;
	string::size_type delim_ii = string::npos;
	string            maybe_ti;	// tag candidate
	string		  lang_ti;	// language
	TagType		  type;

	do
	{
	    lineBegin_ii = readLine (stream_fr, currentLine);
	    _lineNumber++;

	    // find next tag
	    // return tag type
	    // set maybe_ti == tagname, lang_ti == taglang
	    // delim_ii = data position
	    type = tagOnLine (currentLine, maybe_ti, delim_ii, lang_ti);

	    if (type == NONE )
		continue; // no tag on cline

	    if ( !stag_tr.empty() && maybe_ti != stag_tr )
		continue; // tag does not match given stag_tr

	    if ( !slang_tr.empty() && lang_ti != slang_tr )
		continue; // tag does not match given slang_tr

	    // here we've got a valid tag
	    _tagPos = lineBegin_ii;
	    _currentTag = maybe_ti;
	    _currentLocale = lang_ti;

	    // look for data on this line
	    delim_ii = currentLine.find_first_not_of( " \t", delim_ii+1 );

	    if ( delim_ii == string::npos )
	    {
		// no data on this line

		_startPos = _endPos = _tagPos + streamoff(currentLine.size());
		_bufferPos = -1;
	    }
	    else
	    {
		// delim_ii == first non-blank after tag ':'

		_bufferPos = delim_ii;
		_startPos = _tagPos + streamoff(delim_ii);

		_bufferLen = currentLine.find_last_not_of (" \t") + 1 - delim_ii;
		_endPos = _startPos + streamoff (_bufferLen);
	    }

	    return type;

	} while( stream_fr.good() );
    }

    return NONE;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : TaggedParser::lookupEndTag
//	METHOD TYPE : TaggedParser::TagType
//
//	DESCRIPTION : skip all data from istream until endtag
//
TaggedParser::TagType
TaggedParser::lookupEndTag (istream & stream_fr, const string & etag_tr, const string & elang_tr)
{
if (!etag_tr.empty()) cerr << "lookupEndTag(" << etag_tr << "." << elang_tr << ")" << endl;
    _datareset();
    if ( stream_fr.good() )
    {
	_startPos = stream_fr.tellg();

	streampos         lineBegin_ii = nopos;
	string::size_type delim_ii = string::npos;
	string            maybe_ti;
	string		  lang_ti;
	TagType		  type;

	do {
	    // read line
	    lineBegin_ii = readLine( stream_fr, currentLine );
	    _lineNumber++;

	    // find tag
	    type = tagOnLine( currentLine, maybe_ti, delim_ii, lang_ti);

	    // check tag
	    if ((type == END)			// end tag found
		&& (maybe_ti == etag_tr)	// the one we're expecting
		&& ((elang_tr.size() == 0)	// no lang given
		    || lang_ti == elang_tr))	// correct lang found
	    {
		_endPos = lineBegin_ii;

		// end found, return
		return type;
	    }

	} while( stream_fr.good() );

	// here saw no endTag
	_datareset();
    }

    return NONE;
}


///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : TaggedParser::data2string
//	METHOD TYPE : string
//
//	DESCRIPTION : concatenate list of strings to single string
//
string TaggedParser::data2string( const list<string> & data_Vtr )
{
    if ( data_Vtr.empty() )
	return "";

    string ret_ti;
    for (list<string>::const_iterator pos = data_Vtr.begin();
	 pos != data_Vtr.end(); ++pos)
    {
	if (!ret_ti.empty())
	    ret_ti += '\n';
	ret_ti += *pos;
    }

    return ret_ti;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : TaggedParser::split2words
//	METHOD TYPE : vector<string>
//
//	DESCRIPTION :
//
vector<string> TaggedParser::split2words( const string & line_tr, const string & sepchars_tr )
{
    vector<string> Ret_Vti;

    string::size_type wstart_ii = 0;
    string::size_type wend_ii   = string::npos;
    do {
	wstart_ii = line_tr.find_first_not_of( sepchars_tr, wstart_ii );
	if ( wstart_ii != string::npos )
	{
	    wend_ii = line_tr.find_first_of( sepchars_tr, wstart_ii );
	    if ( wend_ii != string::npos )
	    {
		Ret_Vti.push_back( line_tr.substr( wstart_ii, wend_ii-wstart_ii ) );
	    }
	    else
	    {
		Ret_Vti.push_back( line_tr.substr( wstart_ii ) );
	    }
	    wstart_ii = wend_ii;
	}
    } while ( wstart_ii != string::npos );

    return Ret_Vti;
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : ostream &
**
**	DESCRIPTION :
*/
ostream & operator<<( ostream & str, const TaggedParser & obj )
{
    str << "TaggedParser{"
	<< obj.currentTag();
    if (!obj.currentLocale().empty())
	str << "." << obj.currentLocale();
    str << " [" << obj.dataStartPos() << ":" << obj.dataEndPos() << "]";

    return str << '}' << endl;
}
