/*---------------------------------------------------------------------\
|								       |
|		       __   __	  ____ _____ ____		       |
|		       \ \ / /_ _/ ___|_   _|___ \		       |
|			\ V / _` \___ \ | |   __) |		       |
|			 | | (_| |___) || |  / __/		       |
|			 |_|\__,_|____/ |_| |_____|		       |
|								       |
|				core system			       |
|							 (C) SuSE GmbH |
\----------------------------------------------------------------------/

  File:	      YRpmGroupsTree.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YRpmGroupsTree_h
#define YRpmGroupsTree_h


#include <y2util/YStringTree.h>


/**
 * @short Efficient storage for RPM group tags
 **/
class YRpmGroupsTree: public YStringTree
{
public:

    /**
     * Constructor.
     **/

    YRpmGroupsTree();

    /**
     * Destructor.
     **/
    virtual ~YRpmGroupsTree();

    
protected:
    
    /**
     * Add all RPM group tags to this filter
     **/
    void addRpmGroups();
};




#endif // YRpmGroupsTree_h
