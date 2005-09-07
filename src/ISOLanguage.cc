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
#define _(MSG)  dgettext(TEXTDOMAIN, (MSG))
#define __(MSG) MSG

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
struct ISOLanguage::_D : public Rep {

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
    return _( _index->second.c_str() );
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
    _d = makeVarPtr( new _D( code_r ) );
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
  bindtextdomain( TEXTDOMAIN, LOCALEDIR );
  bind_textdomain_codeset( TEXTDOMAIN, "UTF-8" );

  CodeMap cm;
  cm["default"] = __( "Default" );
  cm["aa"] = __( "Afar" );
  cm["ab"] = __( "Abkhazian" );
  cm["ae"] = __( "Avestan" );
  cm["af"] = __( "Afrikaans" );
  cm["am"] = __( "Amharic" );
  cm["ar"] = __( "Arabic" );
  cm["as"] = __( "Assamese" );
  cm["ay"] = __( "Aymara" );
  cm["az"] = __( "Azerbaijani" );
  cm["ba"] = __( "Bashkir" );
  cm["be"] = __( "Belarusian" );
  cm["bg"] = __( "Bulgarian" );
  cm["bh"] = __( "Bihari" );
  cm["bi"] = __( "Bislama" );
  cm["bn"] = __( "Bengali" );
  cm["bo"] = __( "Tibetan" );
  cm["br"] = __( "Breton" );
  cm["bs"] = __( "Bosnian" );
  cm["ca"] = __( "Catalan" );
  cm["ce"] = __( "Chechen" );
  cm["ch"] = __( "Chamorro" );
  cm["co"] = __( "Corsican" );
  cm["cs"] = __( "Czech" );
  cm["cu"] = __( "Church Slavic" );
  cm["cv"] = __( "Chuvash" );
  cm["cy"] = __( "Welsh" );
  cm["da"] = __( "Danish" );
  cm["de"] = __( "German" );
  cm["dz"] = __( "Dzongkha" );
  cm["el"] = __( "Greek" );
  cm["en"] = __( "English" );
  cm["eo"] = __( "Esperanto" );
  cm["es"] = __( "Spanish" );
  cm["et"] = __( "Estonian" );
  cm["eu"] = __( "Basque" );
  cm["fa"] = __( "Persian" );
  cm["fi"] = __( "Finnish" );
  cm["fj"] = __( "Fijian" );
  cm["fo"] = __( "Faroese" );
  cm["fr"] = __( "French" );
  cm["fy"] = __( "Frisian" );
  cm["ga"] = __( "Irish" );
  cm["gd"] = __( "Gaelic; Scottish" );
  cm["gl"] = __( "Gallegan" );
  cm["gn"] = __( "Guarani" );
  cm["gu"] = __( "Gujarati" );
  cm["gv"] = __( "Manx" );
  cm["ha"] = __( "Hausa" );
  cm["he"] = __( "Hebrew" );
  cm["hi"] = __( "Hindi" );
  cm["ho"] = __( "Hiri" );
  cm["hr"] = __( "Croatian" );
  cm["hu"] = __( "Hungarian" );
  cm["hy"] = __( "Armenian" );
  cm["hz"] = __( "Herero" );
  cm["ia"] = __( "Interlingua" );
  cm["id"] = __( "Indonesian" );
  cm["ie"] = __( "Interlingue" );
  cm["ik"] = __( "Inupiaq" );
  cm["is"] = __( "Icelandic" );
  cm["it"] = __( "Italian" );
  cm["iu"] = __( "Inuktitut" );
  cm["ja"] = __( "Japanese" );
  cm["jw"] = __( "Javanese" );
  cm["ka"] = __( "Georgian" );
  cm["ki"] = __( "Kikuyu" );
  cm["kj"] = __( "Kuanyama" );
  cm["kk"] = __( "Kazakh" );
  cm["kl"] = __( "Kalaallisut" );
  cm["km"] = __( "Khmer" );
  cm["kn"] = __( "Kannada" );
  cm["ko"] = __( "Korean" );
  cm["ks"] = __( "Kashmiri" );
  cm["ku"] = __( "Kurdish" );
  cm["kv"] = __( "Komi" );
  cm["kw"] = __( "Cornish" );
  cm["ky"] = __( "Kirghiz" );
  cm["la"] = __( "Latin" );
  cm["lb"] = __( "Letzeburgesch" );
  cm["ln"] = __( "Lingala" );
  cm["lo"] = __( "Lao" );
  cm["lt"] = __( "Lithuanian" );
  cm["lv"] = __( "Latvian" );
  cm["mg"] = __( "Malagasy" );
  cm["mh"] = __( "Marshall" );
  cm["mi"] = __( "Maori" );
  cm["mk"] = __( "Macedonian" );
  cm["ml"] = __( "Malayalam" );
  cm["mn"] = __( "Mongolian" );
  cm["mo"] = __( "Moldavian" );
  cm["mr"] = __( "Marathi" );
  cm["ms"] = __( "Malay" );
  cm["mt"] = __( "Maltese" );
  cm["my"] = __( "Burmese" );
  cm["na"] = __( "Nauru" );
  cm["nb"] = __( "Norwegian Bokmal" );
  cm["nd"] = __( "Ndebele, North" );
  cm["ne"] = __( "Nepali" );
  cm["ng"] = __( "Ndonga" );
  cm["nl"] = __( "Dutch" );
  cm["nn"] = __( "Norwegian Nynorsk" );
  cm["no"] = __( "Norwegian" );
  cm["nr"] = __( "Ndebele, South" );
  cm["nv"] = __( "Navajo" );
  cm["ny"] = __( "Nyanja" );
  cm["oc"] = __( "Occitan (post 1500)" );
  cm["om"] = __( "Oromo" );
  cm["or"] = __( "Oriya" );
  cm["os"] = __( "Ossetian" );
  cm["pa"] = __( "Panjabi" );
  cm["pi"] = __( "Pali" );
  cm["pl"] = __( "Polish" );
  cm["ps"] = __( "Pushto" );
  cm["pt"] = __( "Portuguese" );
  cm["qu"] = __( "Quechua" );
  cm["rm"] = __( "Raeto-Romance" );
  cm["rn"] = __( "Rundi" );
  cm["ro"] = __( "Romanian" );
  cm["ru"] = __( "Russian" );
  cm["rw"] = __( "Kinyarwanda" );
  cm["sa"] = __( "Sanskrit" );
  cm["sc"] = __( "Sardinian" );
  cm["sd"] = __( "Sindhi" );
  cm["se"] = __( "Sami" );
  cm["sg"] = __( "Sango" );
  cm["si"] = __( "Sinhala; Sinhalese" );
  cm["sk"] = __( "Slovak" );
  cm["sl"] = __( "Slovenian" );
  cm["sm"] = __( "Samoan" );
  cm["sn"] = __( "Shona" );
  cm["so"] = __( "Somali" );
  cm["sq"] = __( "Albanian" );
  cm["sr"] = __( "Serbian" );
  cm["ss"] = __( "Swati" );
  cm["st"] = __( "Sotho" );
  cm["su"] = __( "Sundanese" );
  cm["sv"] = __( "Swedish" );
  cm["sw"] = __( "Swahili" );
  cm["ta"] = __( "Tamil" );
  cm["te"] = __( "Telugu" );
  cm["tg"] = __( "Tajik" );
  cm["th"] = __( "Thai" );
  cm["ti"] = __( "Tigrinya" );
  cm["tk"] = __( "Turkmen" );
  cm["tl"] = __( "Tagalog" );
  cm["tn"] = __( "Tswana" );
  cm["to"] = __( "Tonga" );
  cm["tr"] = __( "Turkish" );
  cm["ts"] = __( "Tsonga" );
  cm["tt"] = __( "Tatar" );
  cm["tw"] = __( "Twi" );
  cm["ug"] = __( "Uighur" );
  cm["uk"] = __( "Ukrainian" );
  cm["ur"] = __( "Urdu" );
  cm["uz"] = __( "Uzbek" );
  cm["vi"] = __( "Vietnamese" );
  cm["vo"] = __( "Volapuk" );
  cm["wo"] = __( "Wolof" );
  cm["xh"] = __( "Xhosa" );
  cm["yi"] = __( "Yiddish" );
  cm["yo"] = __( "Yoruba" );
  cm["za"] = __( "Zhuang" );
  cm["zh"] = __( "Chinese" );
  cm["zu"] = __( "Zulu" );
  return cm;
}
