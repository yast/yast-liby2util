/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                     (C) 2002 SuSE AG |
\----------------------------------------------------------------------/

   File:       DiskSpace.h
   Purpose:    Interface to 'df'
   Author:     Ludwig Nussel <lnussel@suse.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

/-*/

#include <string>
#include <vector>
#include <y2util/FSize.h>

/**
 * Interface to 'df'. Calls 'df' to get the info.
 * */
class DiskSpace
{
    public:
	typedef struct
	{
	    std::string   device;
	    std::string   fstype;
	    FSize         total;
	    FSize         used;
	    FSize         available;
	    short         percent;
	    std::string   mountpoint;
	} DfInfo;

	typedef std::vector<DfInfo> DfVec;

	/**
	 * call df
	 *
	 * @param filter_nonlocal filter devices that do not begin with /
	 * @param filter_pseudo filter "none" filesystem types
	 * */
	static DfVec df(bool filter_nonlocal = false, bool filter_pseudo = false);
};

