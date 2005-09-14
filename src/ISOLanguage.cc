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
  static void setDefaultCodeMaps( CodeMap & iso639_1,
                                  CodeMap & iso639_2,
                                  CodeMap & others );

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
//      http://www.loc.gov/standards/iso639-2/ISO-639-2_values_8bits.txt
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

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOLanguage::_D::setDefaultCodeMaps
//	METHOD TYPE : void
//
//      http://www.loc.gov/standards/iso639-2/ISO-639-2_values_8bits.txt
//
void ISOLanguage::_D::setDefaultCodeMaps( ISOLanguage::_D::CodeMap & iso639_1,
                                          ISOLanguage::_D::CodeMap & iso639_2,
                                          ISOLanguage::_D::CodeMap & others )
{
  bindtextdomain( TEXTDOMAIN, LOCALEDIR );
  bind_textdomain_codeset( TEXTDOMAIN, "UTF-8" );

  others["default"] = __( "Default" );

  iso639_2["aar"]                   = iso639_1["aa"] = __( "Afar" );
  iso639_2["abk"]                   = iso639_1["ab"] = __( "Abkhazian" );
  iso639_2["ace"]                                    = __( "Achinese" );
  iso639_2["ach"]                                    = __( "Acoli" );
  iso639_2["ada"]                                    = __( "Adangme" );
  iso639_2["ady"]                                    = __( "Adyghe" );
  iso639_2["afa"]                                    = __( "Afro-Asiatic (Other)" );
  iso639_2["afh"]                                    = __( "Afrihili" );
  iso639_2["afr"]                   = iso639_1["af"] = __( "Afrikaans" );
  iso639_2["ain"]                                    = __( "Ainu" );
  iso639_2["aka"]                   = iso639_1["ak"] = __( "Akan" );
  iso639_2["akk"]                                    = __( "Akkadian" );
  iso639_2["alb"] = iso639_2["sqi"] = iso639_1["sq"] = __( "Albanian" );
  iso639_2["ale"]                                    = __( "Aleut" );
  iso639_2["alg"]                                    = __( "Algonquian languages" );
  iso639_2["alt"]                                    = __( "Southern Altai" );
  iso639_2["amh"]                   = iso639_1["am"] = __( "Amharic" );
  iso639_2["ang"]                                    = __( "English, Old (ca.450-1100)" );
  iso639_2["apa"]                                    = __( "Apache languages" );
  iso639_2["ara"]                   = iso639_1["ar"] = __( "Arabic" );
  iso639_2["arc"]                                    = __( "Aramaic" );
  iso639_2["arg"]                   = iso639_1["an"] = __( "Aragonese" );
  iso639_2["arm"] = iso639_2["hye"] = iso639_1["hy"] = __( "Armenian" );
  iso639_2["arn"]                                    = __( "Araucanian" );
  iso639_2["arp"]                                    = __( "Arapaho" );
  iso639_2["art"]                                    = __( "Artificial (Other)" );
  iso639_2["arw"]                                    = __( "Arawak" );
  iso639_2["asm"]                   = iso639_1["as"] = __( "Assamese" );
  iso639_2["ast"]                                    = __( "Asturian" );
  iso639_2["ath"]                                    = __( "Athapascan languages" );
  iso639_2["aus"]                                    = __( "Australian languages" );
  iso639_2["ava"]                   = iso639_1["av"] = __( "Avaric" );
  iso639_2["ave"]                   = iso639_1["ae"] = __( "Avestan" );
  iso639_2["awa"]                                    = __( "Awadhi" );
  iso639_2["aym"]                   = iso639_1["ay"] = __( "Aymara" );
  iso639_2["aze"]                   = iso639_1["az"] = __( "Azerbaijani" );
  iso639_2["bad"]                                    = __( "Banda" );
  iso639_2["bai"]                                    = __( "Bamileke languages" );
  iso639_2["bak"]                   = iso639_1["ba"] = __( "Bashkir" );
  iso639_2["bal"]                                    = __( "Baluchi" );
  iso639_2["bam"]                   = iso639_1["bm"] = __( "Bambara" );
  iso639_2["ban"]                                    = __( "Balinese" );
  iso639_2["baq"] = iso639_2["eus"] = iso639_1["eu"] = __( "Basque" );
  iso639_2["bas"]                                    = __( "Basa" );
  iso639_2["bat"]                                    = __( "Baltic (Other)" );
  iso639_2["bej"]                                    = __( "Beja" );
  iso639_2["bel"]                   = iso639_1["be"] = __( "Belarusian" );
  iso639_2["bem"]                                    = __( "Bemba" );
  iso639_2["ben"]                   = iso639_1["bn"] = __( "Bengali" );
  iso639_2["ber"]                                    = __( "Berber (Other)" );
  iso639_2["bho"]                                    = __( "Bhojpuri" );
  iso639_2["bih"]                   = iso639_1["bh"] = __( "Bihari" );
  iso639_2["bik"]                                    = __( "Bikol" );
  iso639_2["bin"]                                    = __( "Bini" );
  iso639_2["bis"]                   = iso639_1["bi"] = __( "Bislama" );
  iso639_2["bla"]                                    = __( "Siksika" );
  iso639_2["bnt"]                                    = __( "Bantu (Other)" );
  iso639_2["bos"]                   = iso639_1["bs"] = __( "Bosnian" );
  iso639_2["bra"]                                    = __( "Braj" );
  iso639_2["bre"]                   = iso639_1["br"] = __( "Breton" );
  iso639_2["btk"]                                    = __( "Batak (Indonesia)" );
  iso639_2["bua"]                                    = __( "Buriat" );
  iso639_2["bug"]                                    = __( "Buginese" );
  iso639_2["bul"]                   = iso639_1["bg"] = __( "Bulgarian" );
  iso639_2["bur"] = iso639_2["mya"] = iso639_1["my"] = __( "Burmese" );
  iso639_2["byn"]                                    = __( "Blin" );
  iso639_2["cad"]                                    = __( "Caddo" );
  iso639_2["cai"]                                    = __( "Central American Indian (Other)" );
  iso639_2["car"]                                    = __( "Carib" );
  iso639_2["cat"]                   = iso639_1["ca"] = __( "Catalan" );
  iso639_2["cau"]                                    = __( "Caucasian (Other)" );
  iso639_2["ceb"]                                    = __( "Cebuano" );
  iso639_2["cel"]                                    = __( "Celtic (Other)" );
  iso639_2["cha"]                   = iso639_1["ch"] = __( "Chamorro" );
  iso639_2["chb"]                                    = __( "Chibcha" );
  iso639_2["che"]                   = iso639_1["ce"] = __( "Chechen" );
  iso639_2["chg"]                                    = __( "Chagatai" );
  iso639_2["chi"] = iso639_2["zho"] = iso639_1["zh"] = __( "Chinese" );
  iso639_2["chk"]                                    = __( "Chuukese" );
  iso639_2["chm"]                                    = __( "Mari" );
  iso639_2["chn"]                                    = __( "Chinook jargon" );
  iso639_2["cho"]                                    = __( "Choctaw" );
  iso639_2["chp"]                                    = __( "Chipewyan" );
  iso639_2["chr"]                                    = __( "Cherokee" );
  iso639_2["chu"]                   = iso639_1["cu"] = __( "Church Slavic" );
  iso639_2["chv"]                   = iso639_1["cv"] = __( "Chuvash" );
  iso639_2["chy"]                                    = __( "Cheyenne" );
  iso639_2["cmc"]                                    = __( "Chamic languages" );
  iso639_2["cop"]                                    = __( "Coptic" );
  iso639_2["cor"]                   = iso639_1["kw"] = __( "Cornish" );
  iso639_2["cos"]                   = iso639_1["co"] = __( "Corsican" );
  iso639_2["cpe"]                                    = __( "Creoles and pidgins, English based (Other)" );
  iso639_2["cpf"]                                    = __( "Creoles and pidgins, French-based (Other)" );
  iso639_2["cpp"]                                    = __( "Creoles and pidgins, Portuguese-based (Other)" );
  iso639_2["cre"]                   = iso639_1["cr"] = __( "Cree" );
  iso639_2["crh"]                                    = __( "Crimean Tatar" );
  iso639_2["crp"]                                    = __( "Creoles and pidgins (Other)" );
  iso639_2["csb"]                                    = __( "Kashubian" );
  iso639_2["cus"]                                    = __( "Cushitic (Other)" );
  iso639_2["cze"] = iso639_2["ces"] = iso639_1["cs"] = __( "Czech" );
  iso639_2["dak"]                                    = __( "Dakota" );
  iso639_2["dan"]                   = iso639_1["da"] = __( "Danish" );
  iso639_2["dar"]                                    = __( "Dargwa" );
  iso639_2["day"]                                    = __( "Dayak" );
  iso639_2["del"]                                    = __( "Delaware" );
  iso639_2["den"]                                    = __( "Slave (Athapascan)" );
  iso639_2["dgr"]                                    = __( "Dogrib" );
  iso639_2["din"]                                    = __( "Dinka" );
  iso639_2["div"]                   = iso639_1["dv"] = __( "Divehi" );
  iso639_2["doi"]                                    = __( "Dogri" );
  iso639_2["dra"]                                    = __( "Dravidian (Other)" );
  iso639_2["dsb"]                                    = __( "Lower Sorbian" );
  iso639_2["dua"]                                    = __( "Duala" );
  iso639_2["dum"]                                    = __( "Dutch, Middle (ca.1050-1350)" );
  iso639_2["dut"] = iso639_2["nld"] = iso639_1["nl"] = __( "Dutch" );
  iso639_2["dyu"]                                    = __( "Dyula" );
  iso639_2["dzo"]                   = iso639_1["dz"] = __( "Dzongkha" );
  iso639_2["efi"]                                    = __( "Efik" );
  iso639_2["egy"]                                    = __( "Egyptian (Ancient)" );
  iso639_2["eka"]                                    = __( "Ekajuk" );
  iso639_2["elx"]                                    = __( "Elamite" );
  iso639_2["eng"]                   = iso639_1["en"] = __( "English" );
  iso639_2["enm"]                                    = __( "English, Middle (1100-1500)" );
  iso639_2["epo"]                   = iso639_1["eo"] = __( "Esperanto" );
  iso639_2["est"]                   = iso639_1["et"] = __( "Estonian" );
  iso639_2["ewe"]                   = iso639_1["ee"] = __( "Ewe" );
  iso639_2["ewo"]                                    = __( "Ewondo" );
  iso639_2["fan"]                                    = __( "Fang" );
  iso639_2["fao"]                   = iso639_1["fo"] = __( "Faroese" );
  iso639_2["fat"]                                    = __( "Fanti" );
  iso639_2["fij"]                   = iso639_1["fj"] = __( "Fijian" );
  iso639_2["fil"]                                    = __( "Filipino" );
  iso639_2["fin"]                   = iso639_1["fi"] = __( "Finnish" );
  iso639_2["fiu"]                                    = __( "Finno-Ugrian (Other)" );
  iso639_2["fon"]                                    = __( "Fon" );
  iso639_2["fre"] = iso639_2["fra"] = iso639_1["fr"] = __( "French" );
  iso639_2["frm"]                                    = __( "French, Middle (ca.1400-1600)" );
  iso639_2["fro"]                                    = __( "French, Old (842-ca.1400)" );
  iso639_2["fry"]                   = iso639_1["fy"] = __( "Frisian" );
  iso639_2["ful"]                   = iso639_1["ff"] = __( "Fulah" );
  iso639_2["fur"]                                    = __( "Friulian" );
  iso639_2["gaa"]                                    = __( "Ga" );
  iso639_2["gay"]                                    = __( "Gayo" );
  iso639_2["gba"]                                    = __( "Gbaya" );
  iso639_2["gem"]                                    = __( "Germanic (Other)" );
  iso639_2["geo"] = iso639_2["kat"] = iso639_1["ka"] = __( "Georgian" );
  iso639_2["ger"] = iso639_2["deu"] = iso639_1["de"] = __( "German" );
  iso639_2["gez"]                                    = __( "Geez" );
  iso639_2["gil"]                                    = __( "Gilbertese" );
  iso639_2["gla"]                   = iso639_1["gd"] = __( "Gaelic" );
  iso639_2["gle"]                   = iso639_1["ga"] = __( "Irish" );
  iso639_2["glg"]                   = iso639_1["gl"] = __( "Galician" );
  iso639_2["glv"]                   = iso639_1["gv"] = __( "Manx" );
  iso639_2["gmh"]                                    = __( "German, Middle High (ca.1050-1500)" );
  iso639_2["goh"]                                    = __( "German, Old High (ca.750-1050)" );
  iso639_2["gon"]                                    = __( "Gondi" );
  iso639_2["gor"]                                    = __( "Gorontalo" );
  iso639_2["got"]                                    = __( "Gothic" );
  iso639_2["grb"]                                    = __( "Grebo" );
  iso639_2["grc"]                                    = __( "Greek, Ancient (to 1453)" );
  iso639_2["gre"] = iso639_2["ell"] = iso639_1["el"] = __( "Greek, Modern (1453-)" );
  iso639_2["grn"]                   = iso639_1["gn"] = __( "Guarani" );
  iso639_2["guj"]                   = iso639_1["gu"] = __( "Gujarati" );
  iso639_2["gwi"]                                    = __( "Gwich'in" );
  iso639_2["hai"]                                    = __( "Haida" );
  iso639_2["hat"]                   = iso639_1["ht"] = __( "Haitian" );
  iso639_2["hau"]                   = iso639_1["ha"] = __( "Hausa" );
  iso639_2["haw"]                                    = __( "Hawaiian" );
  iso639_2["heb"]                   = iso639_1["he"] = __( "Hebrew" );
  iso639_2["her"]                   = iso639_1["hz"] = __( "Herero" );
  iso639_2["hil"]                                    = __( "Hiligaynon" );
  iso639_2["him"]                                    = __( "Himachali" );
  iso639_2["hin"]                   = iso639_1["hi"] = __( "Hindi" );
  iso639_2["hit"]                                    = __( "Hittite" );
  iso639_2["hmn"]                                    = __( "Hmong" );
  iso639_2["hmo"]                   = iso639_1["ho"] = __( "Hiri Motu" );
  iso639_2["hsb"]                                    = __( "Upper Sorbian" );
  iso639_2["hun"]                   = iso639_1["hu"] = __( "Hungarian" );
  iso639_2["hup"]                                    = __( "Hupa" );
  iso639_2["iba"]                                    = __( "Iban" );
  iso639_2["ibo"]                   = iso639_1["ig"] = __( "Igbo" );
  iso639_2["ice"] = iso639_2["isl"] = iso639_1["is"] = __( "Icelandic" );
  iso639_2["ido"]                   = iso639_1["io"] = __( "Ido" );
  iso639_2["iii"]                   = iso639_1["ii"] = __( "Sichuan Yi" );
  iso639_2["ijo"]                                    = __( "Ijo" );
  iso639_2["iku"]                   = iso639_1["iu"] = __( "Inuktitut" );
  iso639_2["ile"]                   = iso639_1["ie"] = __( "Interlingue" );
  iso639_2["ilo"]                                    = __( "Iloko" );
  iso639_2["ina"]                   = iso639_1["ia"] = __( "Interlingua (International Auxiliary Language Association)" );
  iso639_2["inc"]                                    = __( "Indic (Other)" );
  iso639_2["ind"]                   = iso639_1["id"] = __( "Indonesian" );
  iso639_2["ine"]                                    = __( "Indo-European (Other)" );
  iso639_2["inh"]                                    = __( "Ingush" );
  iso639_2["ipk"]                   = iso639_1["ik"] = __( "Inupiaq" );
  iso639_2["ira"]                                    = __( "Iranian (Other)" );
  iso639_2["iro"]                                    = __( "Iroquoian languages" );
  iso639_2["ita"]                   = iso639_1["it"] = __( "Italian" );
  iso639_2["jav"]                   = iso639_1["jv"] = __( "Javanese" );
  iso639_2["jbo"]                                    = __( "Lojban" );
  iso639_2["jpn"]                   = iso639_1["ja"] = __( "Japanese" );
  iso639_2["jpr"]                                    = __( "Judeo-Persian" );
  iso639_2["jrb"]                                    = __( "Judeo-Arabic" );
  iso639_2["kaa"]                                    = __( "Kara-Kalpak" );
  iso639_2["kab"]                                    = __( "Kabyle" );
  iso639_2["kac"]                                    = __( "Kachin" );
  iso639_2["kal"]                   = iso639_1["kl"] = __( "Kalaallisut" );
  iso639_2["kam"]                                    = __( "Kamba" );
  iso639_2["kan"]                   = iso639_1["kn"] = __( "Kannada" );
  iso639_2["kar"]                                    = __( "Karen" );
  iso639_2["kas"]                   = iso639_1["ks"] = __( "Kashmiri" );
  iso639_2["kau"]                   = iso639_1["kr"] = __( "Kanuri" );
  iso639_2["kaw"]                                    = __( "Kawi" );
  iso639_2["kaz"]                   = iso639_1["kk"] = __( "Kazakh" );
  iso639_2["kbd"]                                    = __( "Kabardian" );
  iso639_2["kha"]                                    = __( "Khasi" );
  iso639_2["khi"]                                    = __( "Khoisan (Other)" );
  iso639_2["khm"]                   = iso639_1["km"] = __( "Khmer" );
  iso639_2["kho"]                                    = __( "Khotanese" );
  iso639_2["kik"]                   = iso639_1["ki"] = __( "Kikuyu" );
  iso639_2["kin"]                   = iso639_1["rw"] = __( "Kinyarwanda" );
  iso639_2["kir"]                   = iso639_1["ky"] = __( "Kirghiz" );
  iso639_2["kmb"]                                    = __( "Kimbundu" );
  iso639_2["kok"]                                    = __( "Konkani" );
  iso639_2["kom"]                   = iso639_1["kv"] = __( "Komi" );
  iso639_2["kon"]                   = iso639_1["kg"] = __( "Kongo" );
  iso639_2["kor"]                   = iso639_1["ko"] = __( "Korean" );
  iso639_2["kos"]                                    = __( "Kosraean" );
  iso639_2["kpe"]                                    = __( "Kpelle" );
  iso639_2["krc"]                                    = __( "Karachay-Balkar" );
  iso639_2["kro"]                                    = __( "Kru" );
  iso639_2["kru"]                                    = __( "Kurukh" );
  iso639_2["kua"]                   = iso639_1["kj"] = __( "Kuanyama" );
  iso639_2["kum"]                                    = __( "Kumyk" );
  iso639_2["kur"]                   = iso639_1["ku"] = __( "Kurdish" );
  iso639_2["kut"]                                    = __( "Kutenai" );
  iso639_2["lad"]                                    = __( "Ladino" );
  iso639_2["lah"]                                    = __( "Lahnda" );
  iso639_2["lam"]                                    = __( "Lamba" );
  iso639_2["lao"]                   = iso639_1["lo"] = __( "Lao" );
  iso639_2["lat"]                   = iso639_1["la"] = __( "Latin" );
  iso639_2["lav"]                   = iso639_1["lv"] = __( "Latvian" );
  iso639_2["lez"]                                    = __( "Lezghian" );
  iso639_2["lim"]                   = iso639_1["li"] = __( "Limburgan" );
  iso639_2["lin"]                   = iso639_1["ln"] = __( "Lingala" );
  iso639_2["lit"]                   = iso639_1["lt"] = __( "Lithuanian" );
  iso639_2["lol"]                                    = __( "Mongo" );
  iso639_2["loz"]                                    = __( "Lozi" );
  iso639_2["ltz"]                   = iso639_1["lb"] = __( "Luxembourgish" );
  iso639_2["lua"]                                    = __( "Luba-Lulua" );
  iso639_2["lub"]                   = iso639_1["lu"] = __( "Luba-Katanga" );
  iso639_2["lug"]                   = iso639_1["lg"] = __( "Ganda" );
  iso639_2["lui"]                                    = __( "Luiseno" );
  iso639_2["lun"]                                    = __( "Lunda" );
  iso639_2["luo"]                                    = __( "Luo (Kenya and Tanzania)" );
  iso639_2["lus"]                                    = __( "lushai" );
  iso639_2["mac"] = iso639_2["mkd"] = iso639_1["mk"] = __( "Macedonian" );
  iso639_2["mad"]                                    = __( "Madurese" );
  iso639_2["mag"]                                    = __( "Magahi" );
  iso639_2["mah"]                   = iso639_1["mh"] = __( "Marshallese" );
  iso639_2["mai"]                                    = __( "Maithili" );
  iso639_2["mak"]                                    = __( "Makasar" );
  iso639_2["mal"]                   = iso639_1["ml"] = __( "Malayalam" );
  iso639_2["man"]                                    = __( "Mandingo" );
  iso639_2["mao"] = iso639_2["mri"] = iso639_1["mi"] = __( "Maori" );
  iso639_2["map"]                                    = __( "Austronesian (Other)" );
  iso639_2["mar"]                   = iso639_1["mr"] = __( "Marathi" );
  iso639_2["mas"]                                    = __( "Masai" );
  iso639_2["may"] = iso639_2["msa"] = iso639_1["ms"] = __( "Malay" );
  iso639_2["mdf"]                                    = __( "Moksha" );
  iso639_2["mdr"]                                    = __( "Mandar" );
  iso639_2["men"]                                    = __( "Mende" );
  iso639_2["mga"]                                    = __( "Irish, Middle (900-1200)" );
  iso639_2["mic"]                                    = __( "Mi'kmaq" );
  iso639_2["min"]                                    = __( "Minangkabau" );
  iso639_2["mis"]                                    = __( "Miscellaneous languages" );
  iso639_2["mkh"]                                    = __( "Mon-Khmer (Other)" );
  iso639_2["mlg"]                   = iso639_1["mg"] = __( "Malagasy" );
  iso639_2["mlt"]                   = iso639_1["mt"] = __( "Maltese" );
  iso639_2["mnc"]                                    = __( "Manchu" );
  iso639_2["mni"]                                    = __( "Manipuri" );
  iso639_2["mno"]                                    = __( "Manobo languages" );
  iso639_2["moh"]                                    = __( "Mohawk" );
  iso639_2["mol"]                   = iso639_1["mo"] = __( "Moldavian" );
  iso639_2["mon"]                   = iso639_1["mn"] = __( "Mongolian" );
  iso639_2["mos"]                                    = __( "Mossi" );
  iso639_2["mul"]                                    = __( "Multiple languages" );
  iso639_2["mun"]                                    = __( "Munda languages" );
  iso639_2["mus"]                                    = __( "Creek" );
  iso639_2["mwl"]                                    = __( "Mirandese" );
  iso639_2["mwr"]                                    = __( "Marwari" );
  iso639_2["myn"]                                    = __( "Mayan languages" );
  iso639_2["myv"]                                    = __( "Erzya" );
  iso639_2["nah"]                                    = __( "Nahuatl" );
  iso639_2["nai"]                                    = __( "North American Indian" );
  iso639_2["nap"]                                    = __( "Neapolitan" );
  iso639_2["nau"]                   = iso639_1["na"] = __( "Nauru" );
  iso639_2["nav"]                   = iso639_1["nv"] = __( "Navajo" );
  iso639_2["nbl"]                   = iso639_1["nr"] = __( "Ndebele, South" );
  iso639_2["nde"]                   = iso639_1["nd"] = __( "Ndebele, North" );
  iso639_2["ndo"]                   = iso639_1["ng"] = __( "Ndonga" );
  iso639_2["nds"]                                    = __( "Low German" );
  iso639_2["nep"]                   = iso639_1["ne"] = __( "Nepali" );
  iso639_2["new"]                                    = __( "Nepal Bhasa" );
  iso639_2["nia"]                                    = __( "Nias" );
  iso639_2["nic"]                                    = __( "Niger-Kordofanian (Other)" );
  iso639_2["niu"]                                    = __( "Niuean" );
  iso639_2["nno"]                   = iso639_1["nn"] = __( "Norwegian Nynorsk" );
  iso639_2["nob"]                   = iso639_1["nb"] = __( "Norwegian Bokmal" );
  iso639_2["nog"]                                    = __( "Nogai" );
  iso639_2["non"]                                    = __( "Norse, Old" );
  iso639_2["nor"]                   = iso639_1["no"] = __( "Norwegian" );
  iso639_2["nso"]                                    = __( "Northern Sotho" );
  iso639_2["nub"]                                    = __( "Nubian languages" );
  iso639_2["nwc"]                                    = __( "Classical Newari" );
  iso639_2["nya"]                   = iso639_1["ny"] = __( "Chichewa" );
  iso639_2["nym"]                                    = __( "Nyamwezi" );
  iso639_2["nyn"]                                    = __( "Nyankole" );
  iso639_2["nyo"]                                    = __( "Nyoro" );
  iso639_2["nzi"]                                    = __( "Nzima" );
  iso639_2["oci"]                   = iso639_1["oc"] = __( "Occitan (post 1500)" );
  iso639_2["oji"]                   = iso639_1["oj"] = __( "Ojibwa" );
  iso639_2["ori"]                   = iso639_1["or"] = __( "Oriya" );
  iso639_2["orm"]                   = iso639_1["om"] = __( "Oromo" );
  iso639_2["osa"]                                    = __( "Osage" );
  iso639_2["oss"]                   = iso639_1["os"] = __( "Ossetian" );
  iso639_2["ota"]                                    = __( "Turkish, Ottoman (1500-1928)" );
  iso639_2["oto"]                                    = __( "Otomian languages" );
  iso639_2["paa"]                                    = __( "Papuan (Other)" );
  iso639_2["pag"]                                    = __( "Pangasinan" );
  iso639_2["pal"]                                    = __( "Pahlavi" );
  iso639_2["pam"]                                    = __( "Pampanga" );
  iso639_2["pan"]                   = iso639_1["pa"] = __( "Panjabi" );
  iso639_2["pap"]                                    = __( "Papiamento" );
  iso639_2["pau"]                                    = __( "Palauan" );
  iso639_2["peo"]                                    = __( "Persian, Old (ca.600-400 B.C.)" );
  iso639_2["per"] = iso639_2["fas"] = iso639_1["fa"] = __( "Persian" );
  iso639_2["phi"]                                    = __( "Philippine (Other)" );
  iso639_2["phn"]                                    = __( "Phoenician" );
  iso639_2["pli"]                   = iso639_1["pi"] = __( "Pali" );
  iso639_2["pol"]                   = iso639_1["pl"] = __( "Polish" );
  iso639_2["pon"]                                    = __( "Pohnpeian" );
  iso639_2["por"]                   = iso639_1["pt"] = __( "Portuguese" );
  iso639_2["pra"]                                    = __( "Prakrit languages" );
  iso639_2["pro"]                                    = __( "Provencal, Old (to 1500)" );
  iso639_2["pus"]                   = iso639_1["ps"] = __( "Pushto" );
  iso639_2["que"]                   = iso639_1["qu"] = __( "Quechua" );
  iso639_2["raj"]                                    = __( "Rajasthani" );
  iso639_2["rap"]                                    = __( "Rapanui" );
  iso639_2["rar"]                                    = __( "Rarotongan" );
  iso639_2["roa"]                                    = __( "Romance (Other)" );
  iso639_2["roh"]                   = iso639_1["rm"] = __( "Raeto-Romance" );
  iso639_2["rom"]                                    = __( "Romany" );
  iso639_2["rum"] = iso639_2["ron"] = iso639_1["ro"] = __( "Romanian" );
  iso639_2["run"]                   = iso639_1["rn"] = __( "Rundi" );
  iso639_2["rus"]                   = iso639_1["ru"] = __( "Russian" );
  iso639_2["sad"]                                    = __( "Sandawe" );
  iso639_2["sag"]                   = iso639_1["sg"] = __( "Sango" );
  iso639_2["sah"]                                    = __( "Yakut" );
  iso639_2["sai"]                                    = __( "South American Indian (Other)" );
  iso639_2["sal"]                                    = __( "Salishan languages" );
  iso639_2["sam"]                                    = __( "Samaritan Aramaic" );
  iso639_2["san"]                   = iso639_1["sa"] = __( "Sanskrit" );
  iso639_2["sas"]                                    = __( "Sasak" );
  iso639_2["sat"]                                    = __( "Santali" );
  iso639_2["scc"] = iso639_2["srp"] = iso639_1["sr"] = __( "Serbian" );
  iso639_2["scn"]                                    = __( "Sicilian" );
  iso639_2["sco"]                                    = __( "Scots" );
  iso639_2["scr"] = iso639_2["hrv"] = iso639_1["hr"] = __( "Croatian" );
  iso639_2["sel"]                                    = __( "Selkup" );
  iso639_2["sem"]                                    = __( "Semitic (Other)" );
  iso639_2["sga"]                                    = __( "Irish, Old (to 900)" );
  iso639_2["sgn"]                                    = __( "Sign Languages" );
  iso639_2["shn"]                                    = __( "Shan" );
  iso639_2["sid"]                                    = __( "Sidamo" );
  iso639_2["sin"]                   = iso639_1["si"] = __( "Sinhala" );
  iso639_2["sio"]                                    = __( "Siouan languages" );
  iso639_2["sit"]                                    = __( "Sino-Tibetan (Other)" );
  iso639_2["sla"]                                    = __( "Slavic (Other)" );
  iso639_2["slo"] = iso639_2["slk"] = iso639_1["sk"] = __( "Slovak" );
  iso639_2["slv"]                   = iso639_1["sl"] = __( "Slovenian" );
  iso639_2["sma"]                                    = __( "Southern Sami" );
  iso639_2["sme"]                   = iso639_1["se"] = __( "Northern Sami" );
  iso639_2["smi"]                                    = __( "Sami languages (Other)" );
  iso639_2["smj"]                                    = __( "Lule Sami" );
  iso639_2["smn"]                                    = __( "Inari Sami" );
  iso639_2["smo"]                   = iso639_1["sm"] = __( "Samoan" );
  iso639_2["sms"]                                    = __( "Skolt Sami" );
  iso639_2["sna"]                   = iso639_1["sn"] = __( "Shona" );
  iso639_2["snd"]                   = iso639_1["sd"] = __( "Sindhi" );
  iso639_2["snk"]                                    = __( "Soninke" );
  iso639_2["sog"]                                    = __( "Sogdian" );
  iso639_2["som"]                   = iso639_1["so"] = __( "Somali" );
  iso639_2["son"]                                    = __( "Songhai" );
  iso639_2["sot"]                   = iso639_1["st"] = __( "Sotho, Southern" );
  iso639_2["spa"]                   = iso639_1["es"] = __( "Spanish" );
  iso639_2["srd"]                   = iso639_1["sc"] = __( "Sardinian" );
  iso639_2["srr"]                                    = __( "Serer" );
  iso639_2["ssa"]                                    = __( "Nilo-Saharan (Other)" );
  iso639_2["ssw"]                   = iso639_1["ss"] = __( "Swati" );
  iso639_2["suk"]                                    = __( "Sukuma" );
  iso639_2["sun"]                   = iso639_1["su"] = __( "Sundanese" );
  iso639_2["sus"]                                    = __( "Susu" );
  iso639_2["sux"]                                    = __( "Sumerian" );
  iso639_2["swa"]                   = iso639_1["sw"] = __( "Swahili" );
  iso639_2["swe"]                   = iso639_1["sv"] = __( "Swedish" );
  iso639_2["syr"]                                    = __( "Syriac" );
  iso639_2["tah"]                   = iso639_1["ty"] = __( "Tahitian" );
  iso639_2["tai"]                                    = __( "Tai (Other)" );
  iso639_2["tam"]                   = iso639_1["ta"] = __( "Tamil" );
  iso639_2["tat"]                   = iso639_1["tt"] = __( "Tatar" );
  iso639_2["tel"]                   = iso639_1["te"] = __( "Telugu" );
  iso639_2["tem"]                                    = __( "Timne" );
  iso639_2["ter"]                                    = __( "Tereno" );
  iso639_2["tet"]                                    = __( "Tetum" );
  iso639_2["tgk"]                   = iso639_1["tg"] = __( "Tajik" );
  iso639_2["tgl"]                   = iso639_1["tl"] = __( "Tagalog" );
  iso639_2["tha"]                   = iso639_1["th"] = __( "Thai" );
  iso639_2["tib"] = iso639_2["bod"] = iso639_1["bo"] = __( "Tibetan" );
  iso639_2["tig"]                                    = __( "Tigre" );
  iso639_2["tir"]                   = iso639_1["ti"] = __( "Tigrinya" );
  iso639_2["tiv"]                                    = __( "Tiv" );
  iso639_2["tkl"]                                    = __( "Tokelau" );
  iso639_2["tlh"]                                    = __( "Klingon" );
  iso639_2["tli"]                                    = __( "Tlingit" );
  iso639_2["tmh"]                                    = __( "Tamashek" );
  iso639_2["tog"]                                    = __( "Tonga (Nyasa)" );
  iso639_2["ton"]                   = iso639_1["to"] = __( "Tonga (Tonga Islands)" );
  iso639_2["tpi"]                                    = __( "Tok Pisin" );
  iso639_2["tsi"]                                    = __( "Tsimshian" );
  iso639_2["tsn"]                   = iso639_1["tn"] = __( "Tswana" );
  iso639_2["tso"]                   = iso639_1["ts"] = __( "Tsonga" );
  iso639_2["tuk"]                   = iso639_1["tk"] = __( "Turkmen" );
  iso639_2["tum"]                                    = __( "Tumbuka" );
  iso639_2["tup"]                                    = __( "Tupi languages" );
  iso639_2["tur"]                   = iso639_1["tr"] = __( "Turkish" );
  iso639_2["tut"]                                    = __( "Altaic (Other)" );
  iso639_2["tvl"]                                    = __( "Tuvalu" );
  iso639_2["twi"]                   = iso639_1["tw"] = __( "Twi" );
  iso639_2["tyv"]                                    = __( "Tuvinian" );
  iso639_2["udm"]                                    = __( "Udmurt" );
  iso639_2["uga"]                                    = __( "Ugaritic" );
  iso639_2["uig"]                   = iso639_1["ug"] = __( "Uighur" );
  iso639_2["ukr"]                   = iso639_1["uk"] = __( "Ukrainian" );
  iso639_2["umb"]                                    = __( "Umbundu" );
  iso639_2["und"]                                    = __( "Undetermined" );
  iso639_2["urd"]                   = iso639_1["ur"] = __( "Urdu" );
  iso639_2["uzb"]                   = iso639_1["uz"] = __( "Uzbek" );
  iso639_2["vai"]                                    = __( "Vai" );
  iso639_2["ven"]                   = iso639_1["ve"] = __( "Venda" );
  iso639_2["vie"]                   = iso639_1["vi"] = __( "Vietnamese" );
  iso639_2["vol"]                   = iso639_1["vo"] = __( "Volapuk" );
  iso639_2["vot"]                                    = __( "Votic" );
  iso639_2["wak"]                                    = __( "Wakashan languages" );
  iso639_2["wal"]                                    = __( "Walamo" );
  iso639_2["war"]                                    = __( "Waray" );
  iso639_2["was"]                                    = __( "Washo" );
  iso639_2["wel"] = iso639_2["cym"] = iso639_1["cy"] = __( "Welsh" );
  iso639_2["wen"]                                    = __( "Sorbian languages" );
  iso639_2["wln"]                   = iso639_1["wa"] = __( "Walloon" );
  iso639_2["wol"]                   = iso639_1["wo"] = __( "Wolof" );
  iso639_2["xal"]                                    = __( "Kalmyk" );
  iso639_2["xho"]                   = iso639_1["xh"] = __( "Xhosa" );
  iso639_2["yao"]                                    = __( "Yao" );
  iso639_2["yap"]                                    = __( "Yapese" );
  iso639_2["yid"]                   = iso639_1["yi"] = __( "Yiddish" );
  iso639_2["yor"]                   = iso639_1["yo"] = __( "Yoruba" );
  iso639_2["ypk"]                                    = __( "Yupik languages" );
  iso639_2["zap"]                                    = __( "Zapotec" );
  iso639_2["zen"]                                    = __( "Zenaga" );
  iso639_2["zha"]                   = iso639_1["za"] = __( "Zhuang" );
  iso639_2["znd"]                                    = __( "Zande" );
  iso639_2["zul"]                   = iso639_1["zu"] = __( "Zulu" );
  iso639_2["zun"]                                    = __( "Zuni" );
}
