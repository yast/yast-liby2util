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

   File:       DiskSpace.cc
   Purpose:    Interface to 'df'
   Author:     Ludwig Nussel <lnussel@suse.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

/-*/


#include <y2util/DiskSpace.h>
#include <y2util/ExternalProgram.h>
#include <y2util/stringutil.h>
#include <y2util/Y2SLog.h>

using namespace std;

DiskSpace::DfVec DiskSpace::df(bool filter_nonlocal, bool filter_pseudo)
{
    DfVec infovec;
    const char* argv[] =
    {
	"/bin/df",
	"-TP",
	NULL
    };
    ExternalProgram prog( argv, ExternalProgram::Discard_Stderr, false, -1, true);

    bool firstline = true;
    for (string output = prog.receiveLine(); output.length() ;output = prog.receiveLine())
    {
	if(firstline) { firstline=false; continue;}
	vector<string> vec;
	unsigned count = stringutil::split(output,vec," \t",false);
	if(count != 7)
	    { ERR << "invalid df output: " << output; continue; }
	DfInfo info;

	info.device     = vec[0];
	info.fstype     = vec[1];
	info.total      = atoll(vec[2].c_str()) * 1024;
	info.used       = atoll(vec[3].c_str()) * 1024;
	info.available  = atoll(vec[4].c_str()) * 1024;
	info.percent    = atoll(vec[5].c_str());
	info.mountpoint = vec[6];

	string::size_type pos = info.mountpoint.find("\n");
	if( pos != string::npos)
	    info.mountpoint = info.mountpoint.substr(0,pos);

	if(filter_nonlocal && info.device.substr(0,1) != "/")
	    continue;
	
	if(filter_pseudo && info.fstype == "none")
	    continue;

	infovec.push_back(info);
    }
    
    prog.close();

    return infovec;
}
