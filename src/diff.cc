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

   File:       diff.cc
   Purpose:    generate diff's with the command diff
   Author:     Ludwig Nussel <lnussel@suse.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

/-*/

#include <y2util/diff.h>
#include <y2util/ExternalProgram.h>

#ifdef STANDALONE
#include <iostream>
#endif

using namespace std;

unsigned Diff::differ(const std::string file1, const std::string file2, std::string& out, int maxlines)
{
    const char* argv[] =
    {
	"diff",
	"-u",
	file1.c_str(),
	file2.c_str(),
	NULL
    };
    ExternalProgram* prog = new ExternalProgram(argv);

    if(!prog)
	return 2;

    string line;
    int count = 0;
    for(line = prog->receiveLine(), count=0;
	!line.empty();
	line = prog->receiveLine(), count++ )
    {
	if(maxlines<0?true:count<maxlines)
	    out+=line;
    }

    return prog->close();
}

#ifdef STANDALONE
int main(int argc, char* argv[])
{
    if(argc<3)
    {
	return 42;
    }

    string str;
    unsigned ret = Diff::differ(argv[1],argv[2], str, 20);

    cout << str;
    return ret;
}
#endif
