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

  File:       ISOCountry.cc

  Author:     Michael Andres <ma@suse.de>
  Maintainer: Michael Andres <ma@suse.de>

  Textdomain "iso-countries"

/-*/

#define TEXTDOMAIN "iso-countries"


#include <iostream>
#include <map>
#include <libintl.h>

#include <y2util/Y2SLog.h>
#include <y2util/stringutil.h>

#include <y2util/ISOCountry.h>

using namespace std;

///////////////////////////////////////////////////////////////////
//
//	CLASS NAME : ISOCountry::_D
/**
 *
 **/
struct ISOCountry::_D : public _Body {

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
      WAR << "Malformed ISOCountry code '" << code_r << "' (size != 2)" << endl;
    }

    string lcode( stringutil::toUpper( code_r ) );
    if ( lcode != code_r ) {
      WAR << "Malformed ISOCountry code '" << code_r << "' (not upper case)" << endl;
      // but maybe we're lucky with the upper case code
      // and find a country name.
      dit = _defCodeMap.find( lcode );
      if ( dit != _defCodeMap.end() ) {
	nval.second = dit->second;
      }
    }

    if ( ! nval.second.size() ) {
      nval.second = string( "Unknown country '" ) + code_r + "'";
    }

    MIL << "Remember country code '" << code_r << "': " << nval.second << endl;
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

ISOCountry::_D::CodeMap ISOCountry::_D::_defCodeMap( defaultCodeMap() );
ISOCountry::_D::CodeMap ISOCountry::_D::_altCodeMap;

const string ISOCountry::_D::_noCode( "" );
const string ISOCountry::_D::_noName( "default" );

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOCountry::ISOCountry
//	METHOD TYPE : Constructor
//
ISOCountry::ISOCountry()
{
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOCountry::ISOCountry
//	METHOD TYPE : Constructor
//
ISOCountry::ISOCountry( const std::string & code_r )
{
  if ( code_r.size() ) {
    _d = new _D( code_r );
  }
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOCountry::~ISOCountry
//	METHOD TYPE : Destructor
//
ISOCountry::~ISOCountry()
{
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOCountry::isSet
//	METHOD TYPE : bool
//
bool ISOCountry::isSet() const
{
  return _d;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOCountry::code
//	METHOD TYPE : std::string
//
std::string ISOCountry::code() const
{
  return _d ? _d->code() : _D::_noCode;
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOCountry::name
//	METHOD TYPE : std::string
//
std::string ISOCountry::name() const
{
  return _d ? _d->name() : _D::_noName;
}

/******************************************************************
**
**
**	FUNCTION NAME : operator<<
**	FUNCTION TYPE : ostream &
*/
ostream & operator<<( ostream & str, const ISOCountry & obj )
{
  return str << obj.code();
}

///////////////////////////////////////////////////////////////////
//
//
//	METHOD NAME : ISOCountry::_D::defaultCodeMap
//	METHOD TYPE : ISOCountry::_D::CodeMap
//
ISOCountry::_D::CodeMap ISOCountry:: _D::defaultCodeMap()
{
  // Using dgettext() to avoid interfering with a previously set textdomain
#define _(MSG) dgettext(TEXTDOMAIN, (MSG))

  setlocale (LC_ALL, "de_DE.utf8");
  bindtextdomain( TEXTDOMAIN, LOCALEDIR );
  bind_textdomain_codeset( TEXTDOMAIN, "UTF-8" );
    
  CodeMap cm;
  cm["AD"] = _( "Andorra" ); 				// :AND:020:
  cm["AE"] = _( "United Arab Emirates" ); 		// :ARE:784:
  cm["AF"] = _( "Afghanistan" ); 			// :AFG:004:
  cm["AG"] = _( "Antigua and Barbuda" ); 		// :ATG:028:
  cm["AI"] = _( "Anguilla" ); 				// :AIA:660:
  cm["AL"] = _( "Albania" ); 				// :ALB:008:
  cm["AM"] = _( "Armenia" ); 				// :ARM:051:
  cm["AN"] = _( "Netherlands Antilles" ); 		// :ANT:530:
  cm["AO"] = _( "Angola" ); 				// :AGO:024:
  cm["AQ"] = _( "Antarctica" ); 			// :ATA:010:
  cm["AR"] = _( "Argentina" ); 				// :ARG:032:
  cm["AS"] = _( "American Samoa" ); 			// :ASM:016:
  cm["AT"] = _( "Austria" ); 				// :AUT:040:
  cm["AU"] = _( "Australia" ); 				// :AUS:036:
  cm["AW"] = _( "Aruba" ); 				// :ABW:533:
  cm["AX"] = _( "Aland Islands" ); 			// :ALA:248:
  cm["AZ"] = _( "Azerbaijan" ); 			// :AZE:031:
  cm["BA"] = _( "Bosnia and Herzegovina" ); 		// :BIH:070:
  cm["BB"] = _( "Barbados" ); 				// :BRB:052:
  cm["BD"] = _( "Bangladesh" ); 			// :BGD:050:
  cm["BE"] = _( "Belgium" ); 				// :BEL:056:
  cm["BF"] = _( "Burkina Faso" ); 			// :BFA:854:
  cm["BG"] = _( "Bulgaria" ); 				// :BGR:100:
  cm["BH"] = _( "Bahrain" ); 				// :BHR:048:
  cm["BI"] = _( "Burundi" ); 				// :BDI:108:
  cm["BJ"] = _( "Benin" ); 				// :BEN:204:
  cm["BM"] = _( "Bermuda" ); 				// :BMU:060:
  cm["BN"] = _( "Brunei Darussalam" ); 			// :BRN:096:
  cm["BO"] = _( "Bolivia" ); 				// :BOL:068:
  cm["BR"] = _( "Brazil" ); 				// :BRA:076:
  cm["BS"] = _( "Bahamas" ); 				// :BHS:044:
  cm["BT"] = _( "Bhutan" ); 				// :BTN:064:
  cm["BV"] = _( "Bouvet Island" ); 			// :BVT:074:
  cm["BW"] = _( "Botswana" ); 				// :BWA:072:
  cm["BY"] = _( "Belarus" ); 				// :BLR:112:
  cm["BZ"] = _( "Belize" ); 				// :BLZ:084:
  cm["CA"] = _( "Canada" ); 				// :CAN:124:
  cm["CC"] = _( "Cocos (Keeling) Islands" ); 		// :CCK:166:
  cm["CD"] = _( "Congo" ); 				// :COD:180:
  cm["CF"] = _( "Centruual African Republic" ); 	// :CAF:140:
  cm["CG"] = _( "Congo" ); 				// :COG:178:
  cm["CH"] = _( "Switzerland" ); 			// :CHE:756:
  cm["CI"] = _( "Cote D'Ivoire" ); 			// :CIV:384:
  cm["CK"] = _( "Cook Islands" ); 			// :COK:184:
  cm["CL"] = _( "Chile" ); 				// :CHL:152:
  cm["CM"] = _( "Cameroon" ); 				// :CMR:120:
  cm["CN"] = _( "China" ); 				// :CHN:156:
  cm["CO"] = _( "Colombia" ); 				// :COL:170:
  cm["CR"] = _( "Costa Rica" ); 			// :CRI:188:
  cm["CS"] = _( "Serbia and Montenegro" ); 		// :SCG:891:
  cm["CU"] = _( "Cuba" ); 				// :CUB:192:
  cm["CV"] = _( "Cape Verde" ); 			// :CPV:132:
  cm["CX"] = _( "Christmas Island" ); 			// :CXR:162:
  cm["CY"] = _( "Cyprus" ); 				// :CYP:196:
  cm["CZ"] = _( "Czech Republic" ); 			// :CZE:203:
  cm["DE"] = _( "Germany" ); 				// :DEU:276:
  cm["DJ"] = _( "Djibouti" ); 				// :DJI:262:
  cm["DK"] = _( "Denmark" ); 				// :DNK:208:
  cm["DM"] = _( "Dominica" ); 				// :DMA:212:
  cm["DO"] = _( "Dominican Republic" ); 		// :DOM:214:
  cm["DZ"] = _( "Algeria" ); 				// :DZA:012:
  cm["EC"] = _( "Ecuador" ); 				// :ECU:218:
  cm["EE"] = _( "Estonia" ); 				// :EST:233:
  cm["EG"] = _( "Egypt" ); 				// :EGY:818:
  cm["EH"] = _( "Western Sahara" ); 			// :ESH:732:
  cm["ER"] = _( "Eritrea" ); 				// :ERI:232:
  cm["ES"] = _( "Spain" ); 				// :ESP:724:
  cm["ET"] = _( "Ethiopia" ); 				// :ETH:231:
  cm["FI"] = _( "Finland" ); 				// :FIN:246:
  cm["FJ"] = _( "Fiji" ); 				// :FJI:242:
  cm["FK"] = _( "Falkland Islands (Malvinas)" ); 	// :FLK:238:
  cm["FM"] = _( "Federated States of Micronesia" ); 	// :FSM:583:
  cm["FO"] = _( "Faroe Islands" ); 			// :FRO:234:
  cm["FR"] = _( "France" ); 				// :FRA:250:
  cm["FX"] = _( "Metropolitan France" ); 		// :FXX:249:
  cm["GA"] = _( "Gabon" ); 				// :GAB:266:
  cm["GB"] = _( "United Kingdom" ); 			// :GBR:826:
  cm["GD"] = _( "Grenada" ); 				// :GRD:308:
  cm["GE"] = _( "Georgia" ); 				// :GEO:268:
  cm["GF"] = _( "French Guiana" ); 			// :GUF:254:
  cm["GH"] = _( "Ghana" ); 				// :GHA:288:
  cm["GI"] = _( "Gibraltar" ); 				// :GIB:292:
  cm["GL"] = _( "Greenland" ); 				// :GRL:304:
  cm["GM"] = _( "Gambia" ); 				// :GMB:270:
  cm["GN"] = _( "Guinea" ); 				// :GIN:324:
  cm["GP"] = _( "Guadeloupe" ); 			// :GLP:312:
  cm["GQ"] = _( "Equatorial Guinea" ); 			// :GNQ:226:
  cm["GR"] = _( "Greece" ); 				// :GRC:300:
  cm["GS"] = _( "South Georgia and the South Sandwich Islands" );	// :SGS:239:
  cm["GT"] = _( "Guatemala" ); 				// :GTM:320:
  cm["GU"] = _( "Guam" ); 				// :GUM:316:
  cm["GW"] = _( "Guinea-Bissau" ); 			// :GNB:624:
  cm["GY"] = _( "Guyana" ); 				// :GUY:328:
  cm["HK"] = _( "Hong Kong" ); 				// :HKG:344:
  cm["HM"] = _( "Heard Island and McDonald Islands" ); 	// :HMD:334:
  cm["HN"] = _( "Honduras" ); 				// :HND:340:
  cm["HR"] = _( "Croatia" ); 				// :HRV:191:
  cm["HT"] = _( "Haiti" ); 				// :HTI:332:
  cm["HU"] = _( "Hungary" ); 				// :HUN:348:
  cm["ID"] = _( "Indonesia" ); 				// :IDN:360:
  cm["IE"] = _( "Ireland" ); 				// :IRL:372:
  cm["IL"] = _( "Israel" ); 				// :ISR:376:
  cm["IN"] = _( "India" ); 				// :IND:356:
  cm["IO"] = _( "British Indian Ocean Territory" ); 	// :IOT:086:
  cm["IQ"] = _( "Iraq" ); 				// :IRQ:368:
  cm["IR"] = _( "Iran" ); 				// :IRN:364:
  cm["IS"] = _( "Iceland" ); 				// :ISL:352:
  cm["IT"] = _( "Italy" ); 				// :ITA:380:
  cm["JM"] = _( "Jamaica" ); 				// :JAM:388:
  cm["JO"] = _( "Jordan" ); 				// :JOR:400:
  cm["JP"] = _( "Japan" ); 				// :JPN:392:
  cm["KE"] = _( "Kenya" ); 				// :KEN:404:
  cm["KG"] = _( "Kyrgyzstan" ); 			// :KGZ:417:
  cm["KH"] = _( "Cambodia" ); 				// :KHM:116:
  cm["KI"] = _( "Kiribati" ); 				// :KIR:296:
  cm["KM"] = _( "Comoros" ); 				// :COM:174:
  cm["KN"] = _( "Saint Kitts and Nevis" ); 		// :KNA:659:
  cm["KP"] = _( "North Korea" ); 			// :PRK:408:
  cm["KR"] = _( "South Korea" ); 			// :KOR:410:
  cm["KW"] = _( "Kuwait" ); 				// :KWT:414:
  cm["KY"] = _( "Cayman Islands" ); 			// :CYM:136:
  cm["KZ"] = _( "Kazakhstan" ); 			// :KAZ:398:
  cm["LA"] = _( "Lao People's Democratic Republic" ); 	// :LAO:418:
  cm["LB"] = _( "Lebanon" ); 				// :LBN:422:
  cm["LC"] = _( "Saint Lucia" ); 			// :LCA:662:
  cm["LI"] = _( "Liechtenstein" ); 			// :LIE:438:
  cm["LK"] = _( "Sri Lanka" ); 				// :LKA:144:
  cm["LR"] = _( "Liberia" ); 				// :LBR:430:
  cm["LS"] = _( "Lesotho" ); 				// :LSO:426:
  cm["LT"] = _( "Lithuania" ); 				// :LTU:440:
  cm["LU"] = _( "Luxembourg" ); 			// :LUX:442:
  cm["LV"] = _( "Latvia" ); 				// :LVA:428:
  cm["LY"] = _( "Libya" ); 				// :LBY:434:
  cm["MA"] = _( "Morocco" ); 				// :MAR:504:
  cm["MC"] = _( "Monaco" ); 				// :MCO:492:
  cm["MD"] = _( "Moldova" ); 				// :MDA:498:
  cm["MG"] = _( "Madagascar" ); 			// :MDG:450:
  cm["MH"] = _( "Marshall Islands" ); 			// :MHL:584:
  cm["MK"] = _( "Macedonia" ); 				// :MKD:807:
  cm["ML"] = _( "Mali" ); 				// :MLI:466:
  cm["MM"] = _( "Myanmar" ); 				// :MMR:104:
  cm["MN"] = _( "Mongolia" ); 				// :MNG:496:
  cm["MO"] = _( "Macao" ); 				// :MAC:446:
  cm["MP"] = _( "Northern Mariana Islands" ); 		// :MNP:580:
  cm["MQ"] = _( "Martinique" ); 			// :MTQ:474:
  cm["MR"] = _( "Mauritania" ); 			// :MRT:478:
  cm["MS"] = _( "Montserrat" ); 			// :MSR:500:
  cm["MT"] = _( "Malta" ); 				// :MLT:470:
  cm["MU"] = _( "Mauritius" ); 				// :MUS:480:
  cm["MV"] = _( "Maldives" ); 				// :MDV:462:
  cm["MW"] = _( "Malawi" ); 				// :MWI:454:
  cm["MX"] = _( "Mexico" ); 				// :MEX:484:
  cm["MY"] = _( "Malaysia" ); 				// :MYS:458:
  cm["MZ"] = _( "Mozambique" ); 			// :MOZ:508:
  cm["NA"] = _( "Namibia" ); 				// :NAM:516:
  cm["NC"] = _( "New Caledonia" ); 			// :NCL:540:
  cm["NE"] = _( "Niger" ); 				// :NER:562:
  cm["NF"] = _( "Norfolk Island" ); 			// :NFK:574:
  cm["NG"] = _( "Nigeria" ); 				// :NGA:566:
  cm["NI"] = _( "Nicaragua" ); 				// :NIC:558:
  cm["NL"] = _( "Netherlands" ); 			// :NLD:528:
  cm["NO"] = _( "Norway" ); 				// :NOR:578:
  cm["NP"] = _( "Nepal" ); 				// :NPL:524:
  cm["NR"] = _( "Nauru" ); 				// :NRU:520:
  cm["NU"] = _( "Niue" ); 				// :NIU:570:
  cm["NZ"] = _( "New Zealand" ); 			// :NZL:554:
  cm["OM"] = _( "Oman" ); 				// :OMN:512:
  cm["PA"] = _( "Panama" ); 				// :PAN:591:
  cm["PE"] = _( "Peru" ); 				// :PER:604:
  cm["PF"] = _( "French Polynesia" ); 			// :PYF:258:
  cm["PG"] = _( "Papua New Guinea" ); 			// :PNG:598:
  cm["PH"] = _( "Philippines" ); 			// :PHL:608:
  cm["PK"] = _( "Pakistan" ); 				// :PAK:586:
  cm["PL"] = _( "Poland" ); 				// :POL:616:
  cm["PM"] = _( "Saint Pierre and Miquelon" ); 		// :SPM:666:
  cm["PN"] = _( "Pitcairn" ); 				// :PCN:612:
  cm["PR"] = _( "Puerto Rico" ); 			// :PRI:630:
  cm["PS"] = _( "Palestinian Territory" ); 		// :PSE:275:
  cm["PT"] = _( "Portugal" ); 				// :PRT:620:
  cm["PW"] = _( "Palau" ); 				// :PLW:585:
  cm["PY"] = _( "Paraguay" ); 				// :PRY:600:
  cm["QA"] = _( "Qatar" ); 				// :QAT:634:
  cm["RE"] = _( "Reunion" ); 				// :REU:638:
  cm["RO"] = _( "Romania" ); 				// :ROU:642:
  cm["RU"] = _( "Russian Federation" ); 		// :RUS:643:
  cm["RW"] = _( "Rwanda" ); 				// :RWA:646:
  cm["SA"] = _( "Saudi Arabia" ); 			// :SAU:682:
  cm["SB"] = _( "Solomon Islands" ); 			// :SLB:090:
  cm["SC"] = _( "Seychelles" ); 			// :SYC:690:
  cm["SD"] = _( "Sudan" ); 				// :SDN:736:
  cm["SE"] = _( "Sweden" ); 				// :SWE:752:
  cm["SG"] = _( "Singapore" ); 				// :SGP:702:
  cm["SH"] = _( "Saint Helena" ); 			// :SHN:654:
  cm["SI"] = _( "Slovenia" ); 				// :SVN:705:
  cm["SJ"] = _( "Svalbard and Jan Mayen" ); 		// :SJM:744:
  cm["SK"] = _( "Slovakia" ); 				// :SVK:703:
  cm["SL"] = _( "Sierra Leone" ); 			// :SLE:694:
  cm["SM"] = _( "San Marino" ); 			// :SMR:674:
  cm["SN"] = _( "Senegal" ); 				// :SEN:686:
  cm["SO"] = _( "Somalia" ); 				// :SOM:706:
  cm["SR"] = _( "Suriname" ); 				// :SUR:740:
  cm["ST"] = _( "Sao Tome and Principe" ); 		// :STP:678:
  cm["SV"] = _( "El Salvador" ); 			// :SLV:222:
  cm["SY"] = _( "Syria" ); 				// :SYR:760:
  cm["SZ"] = _( "Swaziland" ); 				// :SWZ:748:
  cm["TC"] = _( "Turks and Caicos Islands" ); 		// :TCA:796:
  cm["TD"] = _( "Chad" ); 				// :TCD:148:
  cm["TF"] = _( "French Southern Territories" ); 	// :ATF:260:
  cm["TG"] = _( "Togo" ); 				// :TGO:768:
  cm["TH"] = _( "Thailand" ); 				// :THA:764:
  cm["TJ"] = _( "Tajikistan" ); 			// :TJK:762:
  cm["TK"] = _( "Tokelau" ); 				// :TKL:772:
  cm["TM"] = _( "Turkmenistan" ); 			// :TKM:795:
  cm["TN"] = _( "Tunisia" ); 				// :TUN:788:
  cm["TO"] = _( "Tonga" ); 				// :TON:776:
  cm["TL"] = _( "East Timor" ); 			// :TLS:626:
  cm["TR"] = _( "Turkey" ); 				// :TUR:792:
  cm["TT"] = _( "Trinidad and Tobago" ); 		// :TTO:780:
  cm["TV"] = _( "Tuvalu" ); 				// :TUV:798:
  cm["TW"] = _( "Taiwan" ); 				// :TWN:158:
  cm["TZ"] = _( "Tanzania" ); 				// :TZA:834:
  cm["UA"] = _( "Ukraine" ); 				// :UKR:804:
  cm["UG"] = _( "Uganda" ); 				// :UGA:800:
  cm["UM"] = _( "United States Minor Outlying Islands" );	// :UMI:581:
  cm["US"] = _( "United States" ); 			// :USA:840:
  cm["UY"] = _( "Uruguay" ); 				// :URY:858:
  cm["UZ"] = _( "Uzbekistan" ); 			// :UZB:860:
  cm["VA"] = _( "Holy See (Vatican City State)" ); 	// :VAT:336:
  cm["VC"] = _( "Saint Vincent and the Grenadines" ); 	// :VCT:670:
  cm["VE"] = _( "Venezuela" ); 				// :VEN:862:
  cm["VG"] = _( "British Virgin Islands" ); 		// :VGB:092:
  cm["VI"] = _( "Virgin Islands, U.S." ); 		// :VIR:850:
  cm["VN"] = _( "Vietnam" ); 				// :VNM:704:
  cm["VU"] = _( "Vanuatu" ); 				// :VUT:548:
  cm["WF"] = _( "Wallis and Futuna" ); 			// :WLF:876:
  cm["WS"] = _( "Samoa" ); 				// :WSM:882:
  cm["YE"] = _( "Yemen" ); 				// :YEM:887:
  cm["YT"] = _( "Mayotte" ); 				// :MYT:175:
  cm["ZA"] = _( "South Africa" ); 			// :ZAF:710:
  cm["ZM"] = _( "Zambia" ); 				// :ZMB:894:
  cm["ZW"] = _( "Zimbabwe" ); 				// :ZWE:716:
  return cm;
}
