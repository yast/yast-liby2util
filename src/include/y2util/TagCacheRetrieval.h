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

  File:       TagCacheRetrieval.h

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Purpose: Keep data and provide functions for on-demand retrieval
	   of cache values

/-*/
#ifndef TagCacheRetrieval_h
#define TagCacheRetrieval_h

#include <iosfwd>
#include <string>
#include <fstream>

#include <y2util/Pathname.h>
#include <y2util/TagParser.h>

#include <y2util/TagCacheRetrievalPtr.h>

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : TagCacheRetrievalPos
//
class TagCacheRetrievalPos {
   private:
	std::streampos _begin;
	std::streampos _end;
   public:
	TagCacheRetrievalPos () : _begin(0), _end (0) {}
	TagCacheRetrievalPos (std::streampos begin, std::streampos end) : _begin(begin), _end (end) {}
	~TagCacheRetrievalPos() {}

	/**
	 * test if empty
	 */
	bool empty () const { return _end == (std::streampos)0; }

	/**
	 * access functions
	 */
	const std::streampos begin() const { return _begin; }
	const std::streampos end() const { return _end; }

	void set (std::streampos begin, std::streampos end) { _begin = begin; _end = end; }
};

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : TagCacheRetrieval
//
class TagCacheRetrieval : virtual public Rep {
    REP_BODY(TagCacheRetrieval);
    private:
	// the name of the file
	std::string _name;

	// hint to keep stream open
	bool _keep_open;

	// the stream to read data from
	std::ifstream _stream;

	// the parser to interprete data from the stream
	TagParser _parser;

    public:

	TagCacheRetrieval (const Pathname& filename);
	~TagCacheRetrieval();

	void startRetrieval();
	void stopRetrieval();

	/**
	 * access to stream and parser
	 * these are non-const because the caller might clobber the values
	 */
	std::string& getName (void) { return _name; }
	TagParser& getParser (void) { return _parser; }

	/**
	 * access to values
	 */
	bool retrieveData (const TagCacheRetrievalPos& pos, std::list<std::string> &data_r);
	bool retrieveData (const TagCacheRetrievalPos& pos, std::string &data_r);
};

///////////////////////////////////////////////////////////////////

#endif // TagCacheRetrieval_h
