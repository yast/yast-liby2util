#include <iostream>
#include <string>
#include <qintdict.h>
#include <qasciidict.h>
#include <qvaluevector.h>
#include <y2util/TagParser.h>

namespace CommonPkdParser
{

/**
* A Tag has a starttag, probably and endtag as well as data and the start and end positions in the stream
*/

class Tag
{
    public:
	enum assignstatus { ACCEPTED, REJECTED_NOMATCH, REJECTED_FULL, REJECTED_NOENDTAG };
	/**
	 * ACCEPTONCE = never overwrite existing data, tags with same name but
	 * with locale do not match returns REJECTED_FULL if data is already
	 * set<br>
	 * ACCEPTPREFERREDLOCALE = overwrite data only if tag has an additional
	 * preferred locale<br>
	 * ACCEPTLOCALEONLY = match only tags which have a locale
         * @see setDefaultLocale
	 */
	enum assigntype { ACCEPTONCE, ACCEPTPREFERREDLOCALE, ACCEPTLOCALEONLY };
	/**
	 * SINGLE = data ends at end of line<br>
	 * MULTI = data ends at special end tag
	 */
	enum datatype { SINGLE, MULTI };
	/** NORMAL = just append locale to endtag<br>
	 * COMPLETELYREVERSED = prepend reversed locale
	 */
	enum endtagtype { ENDTAG_NORMAL, ENDTAG_COMPLETELYREVERSED };
	enum encodingtype { LATIN1, LATIN2, UTF8 };
    private:
	std::string _name;
	std::string _endtag;
	std::streampos _startpos;
	std::streampos _endpos;
	std::string _data;
	std::string _extdata;
	// which locale is to be preferred
	std::string _prefmainlocale;
	std::string _prefsublocale;
	// which locale is currently stored
	std::string _lastmainlocale;
	std::string _lastsublocale;
	assigntype _type;
	// normally "default", see setDefaultLocale
	std::string _defaultlocale;
	datatype _datatype;
	endtagtype _endtagtype;
	encodingtype _encodingtype;

	static char* const global_defaultlocale;

	bool comparebeforedot(const std::string& str2);
    public:
	Tag(const std::string& name, assigntype type = ACCEPTONCE )
	    : _name(name), _type(type), _endtagtype(ENDTAG_NORMAL), _encodingtype(LATIN1)
	{
	    if ( _type == ACCEPTLOCALEONLY )
	    {
		_prefmainlocale = _defaultlocale;
	    }
	}
	const std::string& Name() const
	{
	    return _name;
	}
	bool operator==(const Tag& t2 )
	{
	    return comparebeforedot(t2._name);
	}
	// if type == ACCEPTLOCALEONLY then _defaultlocale is set to "default",
	// use setDefaultLocale to change that
	void setType(assigntype type)
	{
	    _type = type;
	    if( _type == ACCEPTLOCALEONLY )
	    {
		_defaultlocale = global_defaultlocale;
	    }
	}
	void setEncoding(encodingtype etype)
	{
	    _encodingtype = etype;
	}
	// _ is not considered here so use two letter locales or "default"/"C" only
	void setDefaultLocale(const std::string& defaultlocale)
	{
	    _defaultlocale = defaultlocale;
	}
	// set the preferred tag locale e.g de_DE
	void setPreferredLocale(const std::string& preflocale);
	// pass empty string undefine the endtag
	void setEndTag(const std::string& endtag, endtagtype etype = ENDTAG_NORMAL );
	// clears only data, not behavior nor tag names
	void clear()
	{
	    _startpos = 0;
	    _endpos = 0;
	    _lastmainlocale = _lastsublocale = _extdata = _data.erase();
	}
	// if REJECTED_NOENDTAG is returned, stream and parser are in an undefined state
	assignstatus assign(const std::string& starttag, TagParser& parser, std::istream& istr);
	std::streampos posDataStart() { return _startpos; }
	std::streampos posDataEnd() { return _endpos; }
	void print(std::ostream& os)
	{
	    os << "Tag: " << _name;
	    os << " ( " << _startpos << "," << _endpos << " ): " << _data << std::endl;
	    if(_datatype == MULTI )
		os << "EndTag: " << _endtag << std::endl;
	}
};

// redirects assignments to the proper Tag
class TagSet : protected QAsciiDict<Tag>
{
    protected:
	void insert(const char* key, const Tag* tag)
	{
	    if(!find(key))
	    {
		QAsciiDict<Tag>::insert(key,tag);
	    }
	}
	
	QValueVector<Tag*> _localetags;

    public:
    	TagSet() : QAsciiDict<Tag>()
	{
	    setAutoDelete(true);
	}
	// insert tag into the dictionary
	void addTag(Tag* tag)
	{
	    insert(tag->Name().c_str(),tag);
	}

	// redirect assignment to starttag
	Tag::assignstatus assign(const std::string& starttag, TagParser& parser, std::istream& istr);

	// print every contained tag
	void print(std::ostream& os)
	{
	    QAsciiDictIterator<Tag> it( *this );
	    while(it.current())
	    {
		it.current()->print(os);
		++it;
	    }
	}
	/**
	 * reset all included tags
	 */
	void clear()
	{
	    QAsciiDictIterator<Tag> it( *this );
	    while(it.current())
	    {
		it.current()->clear();
		++it;
	    }
	}
	virtual Tag* getTagByIndex(int idx) { return NULL; }
	/** set encoding on all tags with locale, call this *before* assigning
	 * data to it
	 * @param etype Encoding to use
	 */
	virtual void setEncoding(Tag::encodingtype etype)
	{
	    QValueVector<Tag*>::iterator it;
	    for(it=_localetags.begin();it!=_localetags.end();++it)
		(*it)->setEncoding(etype);
	}
};

}


// vim:sw=4
