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

  File:	      YStringTree.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YStringTree_h
#define YStringTree_h

#include <string>
#include <y2util/YTransText.h>
#include <y2util/TreeItem.h>


typedef SortedTreeItem<YTransText>	YStringTreeItem;


/**
 * @short Abstract base class for filter views with hierarchical filter
 * criteria - e.g., RPM group tags, MIME types.
 **/
class YStringTree
{
public:

    /**
     * Constructor.
     **/

    YStringTree();

    /**
     * Destructor.
     **/
    virtual ~YStringTree();

    /**
     * Add a new branch with text content 'content' to the tree, beginning at
     * 'parent' (root if parent == 0).
     * This content can be a path specification delimited with character
     * 'delimiter' (if not 0), i.e. this method will split 'content' up into
     * path components and insert tree items for each level as
     * appropriate. Leading delimiters will be ignored.
     * If 'delimiter' is 0, 'content' is not split but used 'as is'.
     * Items are automatically sorted alphabetically.
     *
     * Example:
     *    addBranch( "/usr/local/bin", '/' )
     *    addBranch( "/usr/lib", '/' )
     *
     *  "usr"
     *		"lib"
     *  	"local"
     *			"bin"
     **/
    void addBranch( std::string		 content,
		    char 		 delimiter	= 0,
		    YStringTreeItem * parent 	= 0 );


    /**
     * Construct a complete original (untranslated) path for the specified tree item.
     * 'startWithDelimiter' specifies whether or not the complete path should
     * start with the delimiter character.
     **/
    std::string completePath( const YStringTreeItem * item,
			      char delimiter,
			      bool startWithDelimiter = true );
    
    /**
     * Debugging - dump the tree into the log file.
     **/
    void logTree();


    /**
     * Apply the filter criteria.
     * Derived classes should overwrite this.
     * The default implementation does nothing.
     *
     * TODO: return a useful value, e.g., a pkg list
     **/
    virtual void filter() {}


    /**
     * Returns the root of the filter view tree.
     * Note: In most cases, the root item itself will not contain any useful
     * information. Consider it the handle for the entire tree, not an actual
     * data element.
     **/
    YStringTreeItem * root() const { return _root; }
    
protected:


    /**
     * Debugging - dump one branch of the tree into the log file.
     **/
    void logBranch( YStringTreeItem * branch, std::string indentation );

    
    // Data members

    YStringTreeItem * _root;
};




#endif // YStringTree_h
