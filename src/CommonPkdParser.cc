#include <y2util/CommonPkdParser.h>
#include <y2util/Y2SLog.h>
#include <iostream>

namespace CommonPkdParser
{

char* const Tag::global_defaultlocale = "default";

bool Tag::comparebeforedot(const std::string& str2)
{
    std::string::size_type dotpos1 = _name.find('.');
    std::string::size_type dotpos2 = str2.find('.');
    bool ret;
    if ( _type == ACCEPTPREFERREDLOCALE )
	ret = (_name.substr(0,dotpos1) == str2.substr(0,dotpos2));
    else if ( _type == ACCEPTLOCALEONLY )
    {
	// both tags need to have a locale
	if( dotpos1 != std::string::npos && dotpos2 != std::string::npos )
	    ret = (( _name.substr(0,dotpos1) == str2.substr(0,dotpos2)));
	else
	    ret = false;
    }
    else
    {
	ret = (_name == str2);
    }
//	    std::cerr << "compare " << _name << " == " << str2 << " -> " << (ret==true?"true":"false") << std::endl;
    return ret;
}

void Tag::setPreferredLocale(const std::string& preflocale)
{
    std::string::size_type underpos = preflocale.find('_');
    _prefmainlocale = preflocale.substr(0,underpos);
    if(underpos>0)
	_prefsublocale = preflocale.substr(underpos+1);
    else
	_prefsublocale.erase();
}

void Tag::setEndTag(const std::string& endtag, endtagtype etype )
{
    _endtagtype = etype;
    if(endtag.empty())
    {
	_datatype = SINGLE;
	_endtag.erase();
    }
    else
    {
	_datatype = MULTI;
	_endtag = endtag;
    }
}


Tag::assignstatus Tag::assign(const std::string& starttag, TagParser& parser, std::istream& istr)
{
//	    std::cerr << "assign(" << starttag << ")" << std::endl;
    if(!comparebeforedot(starttag))
	return REJECTED_NOMATCH;

    bool throwawaydata = false;

    if( _type == ACCEPTONCE && !_data.empty())
    {
//		std::cerr << "rejecting " << starttag << ", i'm full"  << std::endl;
	return REJECTED_FULL;
    }
    else if ( _type == ACCEPTPREFERREDLOCALE || _type == ACCEPTLOCALEONLY )
    {
	if( _prefmainlocale.empty() )
	    return REJECTED_NOMATCH;

	if( _type == ACCEPTLOCALEONLY && _defaultlocale.empty() )
	    return REJECTED_NOMATCH;
	
	// +1 to skip dot
	std::string locale;
	std::string::size_type dotpos = starttag.find('.');
	if(dotpos>0) // assume dot is not the first character
	    locale = starttag.substr(dotpos+1);
	if( !locale.empty() )
	{
	    std::string mainlocale;
	    std::string sublocale;
	    std::string::size_type underpos = locale.find('_');
	    mainlocale = locale.substr(0,underpos);
	    if(underpos>0)
		sublocale = locale.substr(underpos+1);

	    //std::cout << "mainlocale " << mainlocale << " sublocale " << sublocale << std::endl;
	    if ( _lastmainlocale.empty()
		&& ( mainlocale == _prefmainlocale
		    || ( _type == ACCEPTLOCALEONLY && mainlocale == _defaultlocale )))
	    {
		_lastmainlocale = _prefmainlocale;
		_lastsublocale.erase();
	    }
	    else if( mainlocale == _lastmainlocale
		&& _lastsublocale.empty()
		&& sublocale == _prefsublocale )
	    {
		// accept
		_lastsublocale = _prefsublocale;
	    }
	    else
	    {
		throwawaydata = true;
	    }
	}
	// localized version already stored
	else if ( !_lastmainlocale.empty() )
	{
	    throwawaydata = true;
	    D__ << "localized version already stored" << std::endl;
	}
    }
    if(_datatype == MULTI)
    {
	std::string real_endtag;
	std::string::size_type pos = starttag.find('.');
	// if starttag contains a dot, we need to add the locale to the
	// endtag too
	if(pos!=std::string::npos)
	{
	    if( _endtagtype == ENDTAG_COMPLETELYREVERSED)
	    {
		std::string locale = starttag.substr( pos + 1 );
		int len = locale.length();
		real_endtag.reserve(len+1);
		for (int i=0;i<len;i++)
		{
		    real_endtag[i]=locale[len-1-i];
		}
		real_endtag[len]='.';
		//FIXME
		//real_endtag[0]=real_endtag[0].upper();
#warning "broken"
		real_endtag+=_endtag;
		D__ << real_endtag << std::endl;
	    }
	    else
	    {
		// append locale of starttag including dot
		real_endtag = _endtag  + starttag.substr( pos );
	    }
	}
	else
	{
		real_endtag = _endtag;
	}
	D__ << "parse to endtag " << real_endtag << std::endl;
	if(!parser.parseData(istr,real_endtag))
	{
	    return REJECTED_NOENDTAG;
	}
    }

    std::streampos startpos = parser.posDataStart();
    std::streampos endpos = parser.posDataEnd();
    // QString <-> std::string conversion
    std::string tmp;
    parser.retrieveData(istr,startpos,endpos, tmp);
    if(!throwawaydata)
    {
	_startpos = startpos;
	_endpos = endpos;
	_data = tmp.c_str();
    }
    else
	D__ << "data thrown away" << std::endl;

    return ACCEPTED;
}

///////////////////////////////////////////////////////////////////////
// TagSet 

Tag::assignstatus TagSet::assign(const std::string& starttag, TagParser& parser, std::istream& istr)
{
    Tag* t = find(starttag.c_str());
    if(!t)
    {
	std::string::size_type dotpos = starttag.find('.');
	if( dotpos != std::string::npos )
	{
	    std::string again(starttag.substr(0,dotpos+1));
//		    std::cerr << starttag << " not found, search for " << again  << std::endl;
	    t = find(again.c_str());
	    if(!t)
	    {
//			std::cerr << again << " also not found, search for ";
		again = starttag.substr(0,dotpos);
//		std::cerr << again  << std::endl;
		t = find(again.c_str());
	    }
	}
    }

    if(!t)
    {
//		std::cerr << starttag << " not found" << std::endl;
	return Tag::REJECTED_NOMATCH;
    }

    return t->assign(starttag,parser,istr);
}


}

// vim:sw=4
