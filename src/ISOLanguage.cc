/*---------------------------------------------------------------------\
|                                                                      |
|                      __   __    ____ _____ ____                      |
|                      \ \ / /_ _/ ___|_   _|___ \                     |
|                       \ V / _` \___ \ | |   __) |                    |
|                        | | (_| |___) || |  / __/                     |
|                        |_|\__,_|____/ |_| |_____|                    |
|                                                                      |
|                               core system                            |
|                                                    (C) SuSE Linux AG |
\----------------------------------------------------------------------/

  File:       ISOLanguage.cc

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Textdomain "iso-languages"

/-*/

#define TEXTDOMAIN "iso-languages"

#include <iostream>
#include <map>
#include <libintl.h>

#include <y2util/Y2SLog.h>
#include <y2util/stringutil.h>

#include <y2util/ISOLanguage.h>

using namespace std;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : ISOLanguage::_D
/**
 *
 **/
struct ISOLanguage::_D : public _Body {

  typedef map<string,string> CodeMap;

  static CodeMap _defCodeMap;
  static CodeMap _altCodeMap;

  static const string _noCode;
  static const string _noName;

  static CodeMap defaultCodeMap();

  typedef CodeMap::const_iterator Index;

  Index _index;

  Index _assert( const std::string & code_r ) {
    if ( _defCodeMap.empty() ) {
      _defCodeMap = defaultCodeMap();
    }
    CodeMap::iterator dit = _defCodeMap.find( code_r );
    if ( dit != _defCodeMap.end() ) {
      return dit;
    }
    CodeMap::iterator ait = _altCodeMap.find( code_r );
    if ( ait != _altCodeMap.end() ) {
      return ait;
    }

    // remember a new code
    CodeMap::value_type nval( code_r, string() );

    if ( code_r.size() != 2 ) {
      WAR << "Malformed ISOLanguage code '" << code_r << "' (size != 2)" << endl;
    }

    string lcode( stringutil::toLower( code_r ) );
    if ( lcode != code_r ) {
      WAR << "Malformed ISOLanguage code '" << code_r << "' (not lower case)" << endl;
      // but maybe we're lucky with the lower case code
      // and find a language name.
      dit = _defCodeMap.find( lcode );
      if ( dit != _defCodeMap.end() ) {
	nval.second = dit->second;
      }
    }

    if ( ! nval.second.size() ) {
      nval.second = string( "Unknown language '" ) + code_r + "'";
    }

    MIL << "Remember language code '" << code_r << "': " << nval.second << endl;
    return _altCodeMap.insert( nval ).first;
  }

  _D( const std::string & code_r )
    : _index( _assert(code_r) )
  {}

  string code() const {
    return _index->first;
  }

  string name() const {
    return _index->second;
  }
};

ISOLanguage::_D::CodeMap ISOLanguage::_D::_defCodeMap;
ISOLanguage::_D::CodeMap ISOLanguage::_D::_altCodeMap;

const string ISOLanguage::_D::_noCode( "" );
const string ISOLanguage::_D::_noName( "Default" );

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOLanguage::ISOLanguage
//	METHOD TYPE : Constructor
//
ISOLanguage::ISOLanguage()
{
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOLanguage::ISOLanguage
//	METHOD TYPE : Constructor
//
ISOLanguage::ISOLanguage( const std::string & code_r )
{
  if ( code_r.size() ) {
    _d = new _D( code_r );
  }
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOLanguage::~ISOLanguage
//	METHOD TYPE : Destructor
//
ISOLanguage::~ISOLanguage()
{
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOLanguage::isSet
//	METHOD TYPE : bool
//
bool ISOLanguage::isSet() const
{
  return _d;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOLanguage::code
//	METHOD TYPE : std::string
//
std::string ISOLanguage::code() const
{
  return _d ? _d->code() : _D::_noCode;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOLanguage::name
//	METHOD TYPE : std::string
//
std::string ISOLanguage::name() const
{
  return _d ? _d->name() : _D::_noName;
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : ostream &
*/
ostream & operator<<( ostream & str, const ISOLanguage & obj )
{
  return str << obj.code();
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOLanguage::_D::defaultCodeMap
//	METHOD TYPE : ISOLanguage::_D::CodeMap
//
ISOLanguage::_D::CodeMap ISOLanguage:: _D::defaultCodeMap()
{
  // Using dgettext() to avoid interfering with a previously set textdomain
  #define _(MSG) dgettext(TEXTDOMAIN, (MSG))

  bindtextdomain( TEXTDOMAIN, LOCALEDIR );
  bind_textdomain_codeset( TEXTDOMAIN, "UTF-8" );

  CodeMap cm;
  cm["default"] = _( "Default" );
  cm["aa"] = _( "Afar" );
  cm["ab"] = _( "Abkhazian" );
  cm["ae"] = _( "Avestan" );
  cm["af"] = _( "Afrikaans" );
  cm["am"] = _( "Amharic" );
  cm["ar"] = _( "Arabic" );
  cm["as"] = _( "Assamese" );
  cm["ay"] = _( "Aymara" );
  cm["az"] = _( "Azerbaijani" );
  cm["ba"] = _( "Bashkir" );
  cm["be"] = _( "Belarusian" );
  cm["bg"] = _( "Bulgarian" );
  cm["bh"] = _( "Bihari" );
  cm["bi"] = _( "Bislama" );
  cm["bn"] = _( "Bengali" );
  cm["bo"] = _( "Tibetan" );
  cm["br"] = _( "Breton" );
  cm["bs"] = _( "Bosnian" );
  cm["ca"] = _( "Catalan" );
  cm["ce"] = _( "Chechen" );
  cm["ch"] = _( "Chamorro" );
  cm["co"] = _( "Corsican" );
  cm["cs"] = _( "Czech" );
  cm["cu"] = _( "Church Slavic" );
  cm["cv"] = _( "Chuvash" );
  cm["cy"] = _( "Welsh" );
  cm["da"] = _( "Danish" );
  cm["de"] = _( "German" );
  cm["dz"] = _( "Dzongkha" );
  cm["el"] = _( "Greek" );
  cm["en"] = _( "English" );
  cm["eo"] = _( "Esperanto" );
  cm["es"] = _( "Spanish" );
  cm["et"] = _( "Estonian" );
  cm["eu"] = _( "Basque" );
  cm["fa"] = _( "Persian" );
  cm["fi"] = _( "Finnish" );
  cm["fj"] = _( "Fijian" );
  cm["fo"] = _( "Faeroese" );
  cm["fr"] = _( "French" );
  cm["fy"] = _( "Frisian" );
  cm["ga"] = _( "Irish" );
  cm["gd"] = _( "Gaelic (Scots)" );
  cm["gl"] = _( "Gallegan" );
  cm["gn"] = _( "Guarani" );
  cm["gu"] = _( "Gujarati" );
  cm["gv"] = _( "Manx" );
  cm["ha"] = _( "Hausa" );
  cm["he"] = _( "Hebrew" );
  cm["hi"] = _( "Hindi" );
  cm["ho"] = _( "Hiri Motu" );
  cm["hr"] = _( "Croatian" );
  cm["hu"] = _( "Hungarian" );
  cm["hy"] = _( "Armenian" );
  cm["hz"] = _( "Herero" );
  cm["ia"] = _( "Interlingua" );
  cm["id"] = _( "Indonesian" );
  cm["ie"] = _( "Interlingue" );
  cm["ik"] = _( "Inupiaq" );
  cm["is"] = _( "Icelandic" );
  cm["it"] = _( "Italian" );
  cm["iu"] = _( "Inuktitut" );
  cm["ja"] = _( "Japanese" );
  cm["jw"] = _( "Javanese" );
  cm["ka"] = _( "Georgian" );
  cm["ki"] = _( "Kikuyu" );
  cm["kj"] = _( "Kuanyama" );
  cm["kk"] = _( "Kazakh" );
  cm["kl"] = _( "Kalaallisut" );
  cm["km"] = _( "Khmer" );
  cm["kn"] = _( "Kannada" );
  cm["ko"] = _( "Korean" );
  cm["ks"] = _( "Kashmiri" );
  cm["ku"] = _( "Kurdish" );
  cm["kv"] = _( "Komi" );
  cm["kw"] = _( "Cornish" );
  cm["ky"] = _( "Kirghiz" );
  cm["la"] = _( "Latin" );
  cm["lb"] = _( "Letzeburgesch" );
  cm["ln"] = _( "Lingala" );
  cm["lo"] = _( "Lao" );
  cm["lt"] = _( "Lithuanian" );
  cm["lv"] = _( "Latvian" );
  cm["mg"] = _( "Malagasy" );
  cm["mh"] = _( "Marshall" );
  cm["mi"] = _( "Maori" );
  cm["mk"] = _( "Macedonian" );
  cm["ml"] = _( "Malayalam" );
  cm["mn"] = _( "Mongolian" );
  cm["mo"] = _( "Moldavian" );
  cm["mr"] = _( "Marathi" );
  cm["ms"] = _( "Malay" );
  cm["mt"] = _( "Maltese" );
  cm["my"] = _( "Burmese" );
  cm["na"] = _( "Nauru" );
  cm["nb"] = _( "Norwegian Bokmal" );
  cm["nd"] = _( "North Ndebele" );
  cm["ne"] = _( "Nepali" );
  cm["ng"] = _( "Ndonga" );
  cm["nl"] = _( "Dutch" );
  cm["nn"] = _( "Norwegian Nynorsk" );
  cm["no"] = _( "Norwegian" );
  cm["nr"] = _( "South Ndebele" );
  cm["nv"] = _( "Navajo" );
  cm["ny"] = _( "Nyanja" );
  cm["oc"] = _( "Occitan (post 1500)" );
  cm["om"] = _( "Oromo" );
  cm["or"] = _( "Oriya" );
  cm["os"] = _( "Ossetian" );
  cm["pa"] = _( "Panjabi" );
  cm["pi"] = _( "Pali" );
  cm["pl"] = _( "Polish" );
  cm["ps"] = _( "Pushto" );
  cm["pt"] = _( "Portuguese" );
  cm["qu"] = _( "Quechua" );
  cm["rm"] = _( "Rhaeto-Romance" );
  cm["rn"] = _( "Rundi" );
  cm["ro"] = _( "Romanian" );
  cm["ru"] = _( "Russian" );
  cm["rw"] = _( "Kinyarwanda" );
  cm["sa"] = _( "Sanskrit" );
  cm["sc"] = _( "Sardinian" );
  cm["sd"] = _( "Sindhi" );
  cm["se"] = _( "Sami" );
  cm["sg"] = _( "Sango" );
  cm["si"] = _( "Sinhalese" );
  cm["sk"] = _( "Slovak" );
  cm["sl"] = _( "Slovenian" );
  cm["sm"] = _( "Samoan" );
  cm["sn"] = _( "Shona" );
  cm["so"] = _( "Somali" );
  cm["sq"] = _( "Albanian" );
  cm["sr"] = _( "Serbian" );
  cm["ss"] = _( "Swati" );
  cm["st"] = _( "Sotho" );
  cm["su"] = _( "Sundanese" );
  cm["sv"] = _( "Swedish" );
  cm["sw"] = _( "Swahili" );
  cm["ta"] = _( "Tamil" );
  cm["te"] = _( "Telugu" );
  cm["tg"] = _( "Tajik" );
  cm["th"] = _( "Thai" );
  cm["ti"] = _( "Tigrinya" );
  cm["tk"] = _( "Turkmen" );
  cm["tl"] = _( "Tagalog" );
  cm["tn"] = _( "Tswana" );
  cm["to"] = _( "Tonga" );
  cm["tr"] = _( "Turkish" );
  cm["ts"] = _( "Tsonga" );
  cm["tt"] = _( "Tatar" );
  cm["tw"] = _( "Twi" );
  cm["ug"] = _( "Uighur" );
  cm["uk"] = _( "Ukrainian" );
  cm["ur"] = _( "Urdu" );
  cm["uz"] = _( "Uzbek" );
  cm["vi"] = _( "Vietnamese" );
  cm["vo"] = _( "Volapuk" );
  cm["wo"] = _( "Wolof" );
  cm["xh"] = _( "Xhosa" );
  cm["yi"] = _( "Yiddish" );
  cm["yo"] = _( "Yoruba" );
  cm["za"] = _( "Zhuang" );
  cm["zh"] = _( "Chinese" );
  cm["zu"] = _( "Zulu" );
  return cm;
}
