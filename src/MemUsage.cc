/*-----------------------------------------------------------*- c++ -*-\
|                                                                      |  
|                      __   __    ____ _____ ____                      |  
|                      \ \ / /_ _/ ___|_   _|___ \                     |  
|                       \ V / _` \___ \ | |   __) |                    |  
|                        | | (_| |___) || |  / __/                     |  
|                        |_|\__,_|____/ |_| |_____|                    |  
|                                                                      |  
|                               core system                            | 
|                                                    (C) SUSE LINUX AG |  
\----------------------------------------------------------------------/ 

   File:       MemUsage.cc
   Author:     Martin Vidner <mvidner@suse.cz>

$Id$

/-*/

#include "ycp/MemUsage.h"
#include <map>
#include <string>
#include <cstdio>
#include <typeinfo>
#include <cxxabi.h>
/*
std::map <std::string, int> MemUsage::m_mu_count;
*/
// top init priority, because all objects derived from MemUsage use it
MemUsage::data MemUsage::m_mu_instances __attribute__ ((init_priority (101)));

// http://www.codesourcery.com/cxx-abi/abi.html#demangler
static std::string demangle (const char *sym)
{
    std::string ret;
    int status;
    char *dsym = abi::__cxa_demangle (sym, NULL, NULL, &status);
    ret = status == 0? dsym: sym;
    free (dsym);
    return ret;
}

void MemUsage::MuDump ()
{
    fprintf (stderr, "MemUsage dump:\n");
    std::map <std::string, int> m_mu_count;
    // determine the type of each instance. now it is possible
    // because we are not in the constructor
    data::iterator
	ii = m_mu_instances.begin (),
	ie = m_mu_instances.end ();
    for (; ii != ie; ++ii)
    {
	const char * name = typeid (**ii).name ();
	++ m_mu_count[demangle (name)];
    }

    std::map <std::string, int>::iterator
	i = m_mu_count.begin (),
	e = m_mu_count.end ();
    for (; i != e; ++i)
    {
	fprintf (stderr, "%9d %s\n", i->second, i->first.c_str ());
    }
}

// for gdb copy and paste convenience
void MemUsage::MuDumpVal (const char *aname)
{
    data::iterator
	ii = m_mu_instances.begin (),
	ie = m_mu_instances.end ();
    for (; ii != ie; ++ii)
    {
	std::string dname = demangle (typeid (**ii).name ());
	if (dname == aname)
	{
	    fprintf (stderr, "(%s *)%p\n", aname, *ii);
	}
    }
}

void MuDump ()
{
    MemUsage::MuDump ();
}

void MuDumpVal (const char *name)
{
    MemUsage::MuDumpVal (name);
}

