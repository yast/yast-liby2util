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

  File:	      YStringTree.cc

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/


#include <stdio.h>
#include <y2util/YStringTree.h>


YStringTree::YStringTree()
    : _root( 0 )
{
    _root = new YStringTreeItem( YTransText( "<root>" ) );
}


YStringTree::~YStringTree()
{
    if ( _root )
	delete _root;
}


void
YStringTree::addBranch( std::string 		content,
			char 			delimiter,
			YStringTreeItem * 	parent )
{
    if ( ! parent )
	parent = _root;

    if ( delimiter == 0 )
    {
	// Simple case: No delimiter, simply create a new item for 'content'
	// and insert it.

	new YStringTreeItem( YTransText( content ), parent );
    }
    else
    {
	// Split 'content' into substrings and insert each subitem

	std::string::size_type start = 0;
	std::string::size_type end   = 0;

	while ( start < content.length() )
	{
	    // Skip delimiters

	    while ( start < content.length() &&
		    content[ start ] == delimiter )
	    {
		start++;
	    }


	    // Search next delimiter

	    end = start;

	    while ( end < content.length() &&
		    content[ end ] != delimiter )
	    {
		end++;
	    }


	    // Extract substring, if there is any

	    if ( end > start )
	    {
		std::string path_component = content.substr( start, end - start );
		YTransText path_component_trans( path_component );

		// Check if an entry with this text already exists
		YStringTreeItem * node = findDirectChild( parent, path_component_trans);

		if ( ! node )	// No entry with this text yet? Create one.
		    node = new YStringTreeItem( path_component_trans, parent );

		parent = node;
	    }

	    start = end;
	}
    }
}


std::string
YStringTree::completePath( const YStringTreeItem * item,
			   char delimiter,
			   bool startWithDelimiter )
{
    std::string path;
    
    if ( item )
    {
	path = item->value().orig();

	while ( item->parent() && item->parent() != _root )
	{
	    path = item->parent()->value().orig() + delimiter + path;
	    item = item->parent();
	}
	    
    }

    if ( startWithDelimiter )
	path = delimiter + path;

    return path;
}


void
YStringTree::logTree()
{
    printf( "Tree:\n" );
    logBranch( _root, "" );
    printf( " " );
}


void
YStringTree::logBranch( YStringTreeItem * branch, std::string indentation )
{
    if ( branch )
    {
	printf( "%s%s (%s)\n", indentation.c_str(),
		branch->value().translation().c_str(),
		branch->value().orig().c_str() );

	YStringTreeItem * child = branch->firstChild();
	indentation += "    ";

	while ( child )
	{
	    logBranch( child, indentation );
	    child = child->next();
	}
    }
    else
    {
	printf( "%s<NULL>\n", indentation.c_str() );
    }
}






