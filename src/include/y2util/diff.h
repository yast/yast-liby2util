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

   File:       diff.h
   Purpose:    generate diff's with the command diff
   Author:     Ludwig Nussel <lnussel@suse.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

/-*/

#ifndef diff_h
#define diff_h

#include <string>

/**
 * generate diff's with the command diff
 * */
class Diff
{
    public:
	/**
	 * generate unified diff between two files
	 *
	 * @param file1
	 * @param file2
	 * @param out where to store the diff
	 * @param maxlines maximum number of lines to put into out, -1 for unlimited
	 *
	 * @return 1 if files differ, 0 if not and any higher value for error
	 * */
	static unsigned differ(const std::string file1, const std::string file2, std::string& out, int maxlines = -1);
};
#endif
