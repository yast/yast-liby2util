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

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : TagCacheRetrieval
class TagCacheRetrieval {
    public:
	// retrieval type
	typedef struct {std::streampos begin; std::streampos end;} retrieval_t;

    private:
	// the stream to read data from
	std::ifstream _packages;

	// the parser to interprete data from the stream
	TagParser _parser;

    public:

	TagCacheRetrieval (const Pathname& packagesname);
	~TagCacheRetrieval();

	/**
	 * access to stream and parser
	 */
	std::ifstream& getStream (void) { return _packages; }
	TagParser& getParser (void) { return _parser; }

	/**
	 * access to values
	 */
	bool retrieveData (const retrieval_t& pos, std::list<std::string> &data_r);
};

///////////////////////////////////////////////////////////////////

#endif // TagCacheRetrieval_h
