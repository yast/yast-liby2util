/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                     (C) 2002 SuSE AG |
\----------------------------------------------------------------------/

   File:       CommonPkdParser.cc
   Purpose:    Implements Tag and TagSet as interface to the TagParser
   Author:     Ludwig Nussel <lnussel@suse.de>
   Maintainer: Ludwig Nussel <lnussel@suse.de>

/-*/

#include <ctype.h> // for toupper

#include <y2util/CommonPkdParser.h>
#include <y2util/Y2SLog.h>
#include <iostream>

namespace CommonPkdParser
{

char* const Tag::global_defaultlocale = "default";

// compare the tag wit another, without looking at the locale
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

// set or reset locale
void Tag::setPreferredLocale(const std::string& preflocale)
{
    std::string::size_type underpos = preflocale.find('_');
    _prefmainlocale = preflocale.substr(0,underpos);
    if(underpos>0)
	_prefsublocale = preflocale.substr(underpos+1);
    else
	_prefsublocale.erase();
}

// set or unset endtag, switches type of tag
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

// make tag to parse data
Tag::assignstatus Tag::assign(const std::string& starttag, TagParser& parser, std::istream& istr)
{
    D__ << "assign(" << starttag << ")" << std::endl;

    // first, see if this tag is really responsible for the starttag
    if(!comparebeforedot(starttag))
	return REJECTED_NOMATCH;

    bool throwawaydata = false;
    
    // reject if data is already assigned
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
    // tag with endtag, proper endtag needs to be computed
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
		D__ << locale << len << std::endl;
//		real_endtag.reserve(len+2);
		for (int i=0;i<len;i++)
		{
		    real_endtag+=locale[len-1-i];
		    D__ << locale[len-1-i] << std::endl;
		}
		real_endtag+='.';
		D__ << real_endtag << std::endl;
		real_endtag+=_endtag;
		real_endtag[0]=toupper(real_endtag[0]);
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
	D__ << "parse to endtag >" << real_endtag << "<" << std::endl;
	if(!parser.parseData(istr,real_endtag))
	{
	    D__ << "Endtag not found" << std::endl;
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

// find tag responsible for starttag
Tag::assignstatus TagSet::assign(const std::string& starttag, TagParser& parser, std::istream& istr)
{
    // literal match
    tagmaptype::iterator t = _tags.find(starttag);
    if(t==_tags.end())
    {
	std::string::size_type dotpos = starttag.find('.');
	if( dotpos != std::string::npos )
	{
	    // retry without locale but with dot
	    std::string again(starttag.substr(0,dotpos+1));
	    t = _tags.find(again);
	    if(t==_tags.end())
	    {
		// retry without locale
		again = starttag.substr(0,dotpos);
		t = _tags.find(again);
	    }
	}
    }

    if(t==_tags.end())
    {
	return Tag::REJECTED_NOMATCH;
    }

    return t->second->assign(starttag,parser,istr);
}


}

// vim:sw=4
