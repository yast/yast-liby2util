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

using std::string;


/**
 * @short Abstract base class for filter views with hierarchical filter
 * criteria - e.g., RPM group tags, MIME types.
 **/
class YStringTree
{
public:

    /**
     * Constructor.
     *
     * 'textdomain' specifies the gettext textdomain to use to translate
     * pathname components as new branches are added.
     *
     * NOTE: This will NOT change the gettext environment in any way - the tree
     * uses dgettext() internally. The caller is responsible to bind that
     * textdomain to a message catalog (bindtextdomain() etc.).
     **/

    YStringTree( const char * textdomain );

    /**
     * Destructor.
     **/
    virtual ~YStringTree();

    /**
     * Add a unique new branch with text content 'content' to the tree,
     * beginning at 'parent' (root if parent == 0).  This content can be a path
     * specification delimited with character 'delimiter' (if not 0), i.e. this
     * method will split 'content' up into path components and insert tree
     * items for each level as appropriate. Leading delimiters will be ignored.
     * If 'delimiter' is 0, 'content' is not split but used 'as is'.  Items are
     * automatically sorted alphabetically. Pathname components are
     * automatically translated using the textdomain specified in the
     * constructor. 
     *
     * Returns the tree node for this branch - either newly created or the
     * existing one.
     *
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
    YStringTreeItem * addBranch( const string &		content,
				 char 			delimiter = 0,
				 YStringTreeItem *	parent 	  = 0 );


    /**
     * Construct a complete original path for the specified tree item.
     * 'startWithDelimiter' specifies whether or not the complete path should
     * start with the delimiter character.
     **/
    string origPath( const YStringTreeItem *	item,
		     char 			delimiter,
		     bool 			startWithDelimiter = true )
	{ return completePath( item, false, delimiter, startWithDelimiter ); }


    /**
     * Construct a complete original path for the specified tree item.
     * 'startWithDelimiter' specifies whether or not the complete path should
     * start with the delimiter character.
     **/
    string translatedPath( const YStringTreeItem * item,
			   char delimiter,
			   bool startWithDelimiter = true )
	{ return completePath( item, true, delimiter, startWithDelimiter ); }


    /**
     * Construct a complete path (both original and translated) for the
     * specified tree item. 'startWithDelimiter' specifies whether or not the
     * complete path should start with the delimiter character.
     *
     * Note: origPath() or translatedPath() are much cheaper if only one
     * version (original or translated) is required.
     **/
    YTransText path( const YStringTreeItem *item,
		     char delimiter,
		     bool startWithDelimiter = true );


    /**
     * Debugging - dump the tree into the log file.
     **/
    void logTree();


    /**
     * Returns the root of the filter view tree.
     * Note: In most cases, the root item itself will not contain any useful
     * information. Consider it the handle for the entire tree, not an actual
     * data element.
     **/
    YStringTreeItem * root() const { return _root; }


    /**
     * Returns the textdomain used internally for translation of pathname
     * components. 
     **/
    const char * textdomain() const { return _textdomain.c_str(); }

    
    /**
     * Set the textdomain used internally for translation of pathname
     * components.
     *
     * NOTE: This will NOT change the gettext environment in any way - the tree
     * uses dgettext() internally. The caller is responsible to bind that
     * textdomain to a message catalog (bindtextdomain() etc.).
     **/
    void setTextdomain( const char * domain )	{ _textdomain = domain; }

    /**
     * Translate message 'orig' using the internal textdomain. Returns the
     * translated text or the original if there is no translation.
     **/
    string translate( const string & orig );
    
    
protected:

    /**
     * Construct a complete original or translated path for the specified tree
     * item.  'startWithDelimiter' specifies whether or not the complete path
     * should start with the delimiter character.
     **/
    string completePath( const YStringTreeItem * item,
			 bool translated,
			 char delimiter,
			 bool startWithDelimiter );

    /**
     * Debugging - dump one branch of the tree into the log file.
     **/
    void logBranch( YStringTreeItem * branch, string indentation );


    // Data members

    YStringTreeItem *	_root;
    string		_textdomain;
};




#endif // YStringTree_h
