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

  File:	      YTransText.h

  Author:     Stefan Hundhammer <sh@suse.de>

/-*/

#ifndef YTransText_h
#define YTransText_h

#include <libintl.h>
#include <string>


/**
 * Helper class for translated strings: Stores a message in the original
 * (untranslated) version along with the translation into the current locale.
 **/
class YTransText
{
public:

    /**
     * Constructor with both original and translated message.
     **/
    YTransText( const std::string & orig,
		const std::string & translation )
	: _orig( orig ), _translation( translation ) {}

    /**
     * Constructor that automatically translates the original message.
     **/
    YTransText( const std::string & orig ) : _orig( orig )
    {
	_translation = gettext( _orig.c_str() );
    }

    /**
     * Copy constructor.
     **/
    YTransText( const YTransText & src )
    {
	_orig		= src.orig();
	_translation	= src.translation();
    }

    /**
     * Assignment operator.
     **/
    YTransText & operator= ( const YTransText & src )
    {
	_orig		= src.orig();
	_translation	= src.translation();

	return *this;
    }

    /**
     * Return the original message.
     **/
    const std::string & orig()		const { return _orig;		}

    /**
     * Return the translation.
     **/
    const std::string & translation()	const { return _translation;	}
    
    /**
     * Return the translation.
     * (alias, just as a shortcut)
     **/
    const std::string & trans()		const { return _translation;	}

    /**
     * Set the original message. Does not touch the translation, so make sure
     * you change both if you want to keep them synchronized!
     **/
    void setOrig( const std::string & newOrig ) { _orig = newOrig; }

    /**
     * Set the translation.
     **/
    void setTranslation( const std::string &newTrans ) { _translation = newTrans; }

    /**
     * operator< : Compares translations.
     **/
    bool operator< ( const YTransText & other ) const
	{ return _translation < other.translation(); }

    /**
     * operator> : Compares translations.
     **/
    bool operator> ( const YTransText & other ) const
	{ return _translation > other.translation(); }
    
    /**
     * operator== : Compares translations.
     **/
    bool operator== ( const YTransText & other ) const
	{ return _translation == other.translation(); }
    
    
private:
    
    std::string	_orig;
    std::string	_translation;
    
};



#endif // YTransText_h
