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

  Purpose:

/-*/

#include <iostream>
#include <map>

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

ISOLanguage::_D::CodeMap ISOLanguage::_D::_defCodeMap( defaultCodeMap() );
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
  CodeMap cm;
  cm["default"] = "Default";

  cm["aa"] = "Afar";
  cm["ab"] = "Abkhazian";
  cm["ae"] = "Avestan";
  cm["af"] = "Afrikaans";
  cm["am"] = "Amharic";
  cm["ar"] = "Arabic";
  cm["as"] = "Assamese";
  cm["ay"] = "Aymara";
  cm["az"] = "Azerbaijani";
  cm["ba"] = "Bashkir";
  cm["be"] = "Belarusian";
  cm["bg"] = "Bulgarian";
  cm["bh"] = "Bihari";
  cm["bi"] = "Bislama";
  cm["bn"] = "Bengali";
  cm["bo"] = "Tibetan";
  cm["br"] = "Breton";
  cm["bs"] = "Bosnian";
  cm["ca"] = "Catalan";
  cm["ce"] = "Chechen";
  cm["ch"] = "Chamorro";
  cm["co"] = "Corsican";
  cm["cs"] = "Czech";
  cm["cu"] = "Church Slavic";
  cm["cv"] = "Chuvash";
  cm["cy"] = "Welsh";
  cm["da"] = "Danish";
  cm["de"] = "German";
  cm["dz"] = "Dzongkha";
  cm["el"] = "Greek";
  cm["en"] = "English";
  cm["eo"] = "Esperanto";
  cm["es"] = "Spanish";
  cm["et"] = "Estonian";
  cm["eu"] = "Basque";
  cm["fa"] = "Persian";
  cm["fi"] = "Finnish";
  cm["fj"] = "Fijian";
  cm["fo"] = "Faeroese";
  cm["fr"] = "French";
  cm["fy"] = "Frisian";
  cm["ga"] = "Irish";
  cm["gd"] = "Gaelic (Scots)";
  cm["gl"] = "Gallegan";
  cm["gn"] = "Guarani";
  cm["gu"] = "Gujarati";
  cm["gv"] = "Manx";
  cm["ha"] = "Hausa";
  cm["he"] = "Hebrew";
  cm["hi"] = "Hindi";
  cm["ho"] = "Hiri Motu";
  cm["hr"] = "Croatian";
  cm["hu"] = "Hungarian";
  cm["hy"] = "Armenian";
  cm["hz"] = "Herero";
  cm["ia"] = "Interlingua";
  cm["id"] = "Indonesian";
  cm["ie"] = "Interlingue";
  cm["ik"] = "Inupiaq";
  cm["is"] = "Icelandic";
  cm["it"] = "Italian";
  cm["iu"] = "Inuktitut";
  cm["ja"] = "Japanese";
  cm["jw"] = "Javanese";
  cm["ka"] = "Georgian";
  cm["ki"] = "Kikuyu";
  cm["kj"] = "Kuanyama";
  cm["kk"] = "Kazakh";
  cm["kl"] = "Kalaallisut";
  cm["km"] = "Khmer";
  cm["kn"] = "Kannada";
  cm["ko"] = "Korean";
  cm["ks"] = "Kashmiri";
  cm["ku"] = "Kurdish";
  cm["kv"] = "Komi";
  cm["kw"] = "Cornish";
  cm["ky"] = "Kirghiz";
  cm["la"] = "Latin";
  cm["lb"] = "Letzeburgesch";
  cm["ln"] = "Lingala";
  cm["lo"] = "Lao";
  cm["lt"] = "Lithuanian";
  cm["lv"] = "Latvian";
  cm["mg"] = "Malagasy";
  cm["mh"] = "Marshall";
  cm["mi"] = "Maori";
  cm["mk"] = "Macedonian";
  cm["ml"] = "Malayalam";
  cm["mn"] = "Mongolian";
  cm["mo"] = "Moldavian";
  cm["mr"] = "Marathi";
  cm["ms"] = "Malay";
  cm["mt"] = "Maltese";
  cm["my"] = "Burmese";
  cm["na"] = "Nauru";
  cm["nb"] = "Norwegian Bokmal";
  cm["nd"] = "North Ndebele";
  cm["ne"] = "Nepali";
  cm["ng"] = "Ndonga";
  cm["nl"] = "Dutch";
  cm["nn"] = "Norwegian Nynorsk";
  cm["no"] = "Norwegian";
  cm["nr"] = "South Ndebele";
  cm["nv"] = "Navajo";
  cm["ny"] = "Nyanja";
  cm["oc"] = "Occitan (post 1500)";
  cm["om"] = "Oromo";
  cm["or"] = "Oriya";
  cm["os"] = "Ossetian";
  cm["pa"] = "Panjabi";
  cm["pi"] = "Pali";
  cm["pl"] = "Polish";
  cm["ps"] = "Pushto";
  cm["pt"] = "Portuguese";
  cm["qu"] = "Quechua";
  cm["rm"] = "Rhaeto-Romance";
  cm["rn"] = "Rundi";
  cm["ro"] = "Romanian";
  cm["ru"] = "Russian";
  cm["rw"] = "Kinyarwanda";
  cm["sa"] = "Sanskrit";
  cm["sc"] = "Sardinian";
  cm["sd"] = "Sindhi";
  cm["se"] = "Sami";
  cm["sg"] = "Sango";
  cm["si"] = "Sinhalese";
  cm["sk"] = "Slovak";
  cm["sl"] = "Slovenian";
  cm["sm"] = "Samoan";
  cm["sn"] = "Shona";
  cm["so"] = "Somali";
  cm["sq"] = "Albanian";
  cm["sr"] = "Serbian";
  cm["ss"] = "Swati";
  cm["st"] = "Sotho";
  cm["su"] = "Sundanese";
  cm["sv"] = "Swedish";
  cm["sw"] = "Swahili";
  cm["ta"] = "Tamil";
  cm["te"] = "Telugu";
  cm["tg"] = "Tajik";
  cm["th"] = "Thai";
  cm["ti"] = "Tigrinya";
  cm["tk"] = "Turkmen";
  cm["tl"] = "Tagalog";
  cm["tn"] = "Tswana";
  cm["to"] = "Tonga";
  cm["tr"] = "Turkish";
  cm["ts"] = "Tsonga";
  cm["tt"] = "Tatar";
  cm["tw"] = "Twi";
  cm["ug"] = "Uighur";
  cm["uk"] = "Ukrainian";
  cm["ur"] = "Urdu";
  cm["uz"] = "Uzbek";
  cm["vi"] = "Vietnamese";
  cm["vo"] = "Volapuk";
  cm["wo"] = "Wolof";
  cm["xh"] = "Xhosa";
  cm["yi"] = "Yiddish";
  cm["yo"] = "Yoruba";
  cm["za"] = "Zhuang";
  cm["zh"] = "Chinese";
  cm["zu"] = "Zulu";
  return cm;
}
