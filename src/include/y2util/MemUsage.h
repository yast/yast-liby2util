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

   File:       MemUsage.h
   Author:     Martin Vidner <mvidner@suse.cz>

$Id$

/-*/

#ifndef MemUsage_h
#define MemUsage_h

/*
#include <string>
#include <map>
#include <typeinfo>
*/
#include <set>

/**
 * Counts instances of classes
 */
class MemUsage
{
/*
protected:
    static std::map <std::string, int> m_mu_count;

    MemUsage () {
	++ m_mu_count[typeid (this).name ()];
    }

    virtual ~MemUsage () {
	-- m_mu_count[typeid (this).name ()];
    }
*/
protected:
    typedef std::set <MemUsage *> data;
    static data m_mu_instances;

    MemUsage () {
	m_mu_instances.insert (this);
    }

    virtual ~MemUsage () {
	m_mu_instances.erase (this);
    }
public:
    static void MuDump ();
    static void MuDumpVal (const char *name);
};

// easier for gdb
void MuDump ();
void MuDumpVal (const char *name);

#endif
