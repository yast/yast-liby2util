#include <iostream>
#include <string>
#include <map>
#include <vector>
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
	const std::string& Data() const
	{
	    return _data;
	}
	bool operator==(const Tag& t2 )
	{
	    return comparebeforedot(t2._name);
	}
	/** if type == ACCEPTLOCALEONLY then _defaultlocale is set to "default",
	 use setDefaultLocale to change that */
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
	/** _ is not considered here so use two letter locales or "default"/"C"
	 * only
	 */
	void setDefaultLocale(const std::string& defaultlocale)
	{
	    _defaultlocale = defaultlocale;
	}
	/** set the preferred tag locale e.g de_DE */
	void setPreferredLocale(const std::string& preflocale);
	/** define an end tag
	 * @param endtag which end tag to set or an empty string to undefine it
	 * @param etype type of endtag, default is normal which means reversed plus probably added locale
	 */
	void setEndTag(const std::string& endtag, endtagtype etype = ENDTAG_NORMAL );
	/**
	 * clears only data, not behavior nor tag names
	 */
	void clear()
	{
	    _startpos = 0;
	    _endpos = 0;
	    _lastmainlocale = _lastsublocale = _data.erase();
	}
	/** if REJECTED_NOENDTAG is returned, stream and parser are in an undefined state */
	assignstatus assign(const std::string& starttag, TagParser& parser, std::istream& istr);
	/**
	 * return start position of data in stream
	 */
	std::streampos posDataStart() { return _startpos; }
	/**
	 * return end position of data in stream
	 */
	std::streampos posDataEnd() { return _endpos; }
	void print(std::ostream& os)
	{
	    os << "Tag: " << _name;
	    os << " ( " << _startpos << "," << _endpos << " ): " << _data << std::endl;
	    if(_datatype == MULTI )
		os << "EndTag: " << _endtag << std::endl;
	}
};

/** redirects assignments to the proper Tag */
class TagSet
{
    public:
	typedef vector<CommonPkdParser::Tag*> tagvectortype;
	
    protected:
	void insert(const std::string key, Tag* tag)
	{
//doenst matter, map allown only one anyways	    if(_tags.find(key) == _tags.end())
	    {
		_tags[key]=tag;
	    }
	}
	
	std::vector<Tag*> _localetags;
	typedef std::map<std::string, Tag*> tagmaptype;
	tagmaptype _tags;
	tagvectortype _tagv;

    public:
    	TagSet()
	{
	}
	virtual ~TagSet()
	{
	    //TODO: delete all tags in map
	}
	// insert tag into the dictionary
	void addTag(Tag* tag)
	{
	    _tags[tag->Name()]=tag;
	}

	// redirect assignment to starttag
	Tag::assignstatus assign(const std::string& starttag, TagParser& parser, std::istream& istr);

	// print every contained tag
	void print(std::ostream& os)
	{
	    tagmaptype::iterator it;
	    for(it=_tags.begin();it!=_tags.end();++it)
	    {
		it->second->print(os);
	    }
	}
	/**
	 * reset all included tags
	 */
	void clear()
	{
	    tagmaptype::iterator it;
	    for(it=_tags.begin();it!=_tags.end();++it)
	    {
		it->second->clear();
	    }
	}
	/** set encoding on all tags with locale, call this *before* assigning
	 * data to it
	 * @param etype Encoding to use
	 */
	virtual void setEncoding(Tag::encodingtype etype)
	{
	    std::vector<Tag*>::iterator it;
	    for(it=_localetags.begin();it!=_localetags.end();++it)
		(*it)->setEncoding(etype);
	}

	CommonPkdParser::Tag* getTagByIndex(unsigned int idx)
	{
	    try
	    {
		// may throw out of range
		return _tagv.at(idx);
	    }
	    catch(...)
	    {
		return NULL;
	    }
	}

	void addTagByIndex(unsigned int idx, CommonPkdParser::Tag* t)
	{
	    if(_tagv.size()<=idx) { _tagv.resize(idx+2),NULL; }
	    _tagv.at(idx)=t;
	}
};

}


// vim:sw=4
