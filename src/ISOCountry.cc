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

  Purpose:

/-*/

#include <iostream>
#include <map>

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
  CodeMap cm;
  cm["AD"] = "Andorra"; 	// :AND:020:
  cm["AE"] = "United Arab Emirates"; 	// :ARE:784:
  cm["AF"] = "Afghanistan"; 	// :AFG:004:
  cm["AG"] = "Antigua and Barbuda"; 	// :ATG:028:
  cm["AI"] = "Anguilla"; 	// :AIA:660:
  cm["AL"] = "Albania"; 	// :ALB:008:
  cm["AM"] = "Armenia"; 	// :ARM:051:
  cm["AN"] = "Netherlands Antilles"; 	// :ANT:530:
  cm["AO"] = "Angola"; 		// :AGO:024:
  cm["AQ"] = "Antarctica"; 	// :ATA:010:
  cm["AR"] = "Argentina"; 	// :ARG:032:
  cm["AS"] = "American Samoa"; 	// :ASM:016:
  cm["AT"] = "Austria"; 	// :AUT:040:
  cm["AU"] = "Australia"; 	// :AUS:036:
  cm["AW"] = "Aruba"; 		// :ABW:533:
  cm["AX"] = "Aland Islands"; 	// :ALA:248:
  cm["AZ"] = "Azerbaijan"; 	// :AZE:031:
  cm["BA"] = "Bosnia and Herzegovina"; 	// :BIH:070:
  cm["BB"] = "Barbados"; 	// :BRB:052:
  cm["BD"] = "Bangladesh"; 	// :BGD:050:
  cm["BE"] = "Belgium"; 	// :BEL:056:
  cm["BF"] = "Burkina Faso"; 	// :BFA:854:
  cm["BG"] = "Bulgaria"; 	// :BGR:100:
  cm["BH"] = "Bahrain"; 	// :BHR:048:
  cm["BI"] = "Burundi"; 	// :BDI:108:
  cm["BJ"] = "Benin"; 		// :BEN:204:
  cm["BM"] = "Bermuda"; 	// :BMU:060:
  cm["BN"] = "Brunei Darussalam"; 	// :BRN:096:
  cm["BO"] = "Bolivia"; 	// :BOL:068:
  cm["BR"] = "Brazil"; 		// :BRA:076:
  cm["BS"] = "Bahamas"; 	// :BHS:044:
  cm["BT"] = "Bhutan"; 		// :BTN:064:
  cm["BV"] = "Bouvet Island"; 	// :BVT:074:
  cm["BW"] = "Botswana"; 	// :BWA:072:
  cm["BY"] = "Belarus"; 	// :BLR:112:
  cm["BZ"] = "Belize"; 		// :BLZ:084:
  cm["CA"] = "Canada"; 		// :CAN:124:
  cm["CC"] = "Cocos (Keeling) Islands"; 	// :CCK:166:
  cm["CD"] = "Congo"; 		// :COD:180:
  cm["CF"] = "Centruual African Republic"; 	// :CAF:140:
  cm["CG"] = "Congo"; 		// :COG:178:
  cm["CH"] = "Switzerland"; 	// :CHE:756:
  cm["CI"] = "Cote D'Ivoire"; 	// :CIV:384:
  cm["CK"] = "Cook Islands"; 	// :COK:184:
  cm["CL"] = "Chile"; 		// :CHL:152:
  cm["CM"] = "Cameroon"; 	// :CMR:120:
  cm["CN"] = "China"; 		// :CHN:156:
  cm["CO"] = "Colombia"; 	// :COL:170:
  cm["CR"] = "Costa Rica"; 	// :CRI:188:
  cm["CS"] = "Serbia and Montenegro"; 	// :SCG:891:
  cm["CU"] = "Cuba"; 		// :CUB:192:
  cm["CV"] = "Cape Verde"; 	// :CPV:132:
  cm["CX"] = "Christmas Island"; 	// :CXR:162:
  cm["CY"] = "Cyprus"; 		// :CYP:196:
  cm["CZ"] = "Czech Republic"; 	// :CZE:203:
  cm["DE"] = "Germany"; 	// :DEU:276:
  cm["DJ"] = "Djibouti"; 	// :DJI:262:
  cm["DK"] = "Denmark"; 	// :DNK:208:
  cm["DM"] = "Dominica"; 	// :DMA:212:
  cm["DO"] = "Dominican Republic"; 	// :DOM:214:
  cm["DZ"] = "Algeria"; 	// :DZA:012:
  cm["EC"] = "Ecuador"; 	// :ECU:218:
  cm["EE"] = "Estonia"; 	// :EST:233:
  cm["EG"] = "Egypt"; 		// :EGY:818:
  cm["EH"] = "Western Sahara"; 	// :ESH:732:
  cm["ER"] = "Eritrea"; 	// :ERI:232:
  cm["ES"] = "Spain"; 		// :ESP:724:
  cm["ET"] = "Ethiopia"; 	// :ETH:231:
  cm["FI"] = "Finland"; 	// :FIN:246:
  cm["FJ"] = "Fiji"; 		// :FJI:242:
  cm["FK"] = "Falkland Islands (Malvinas)"; 	// :FLK:238:
  cm["FM"] = "Federated States of Micronesia"; 	// :FSM:583:
  cm["FO"] = "Faroe Islands"; 	// :FRO:234:
  cm["FR"] = "France"; 		// :FRA:250:
  cm["FX"] = "Metropolitan France"; 	// :FXX:249:
  cm["GA"] = "Gabon"; 		// :GAB:266:
  cm["GB"] = "United Kingdom"; 	// :GBR:826:
  cm["GD"] = "Grenada"; 	// :GRD:308:
  cm["GE"] = "Georgia"; 	// :GEO:268:
  cm["GF"] = "French Guiana"; 	// :GUF:254:
  cm["GH"] = "Ghana"; 		// :GHA:288:
  cm["GI"] = "Gibraltar"; 	// :GIB:292:
  cm["GL"] = "Greenland"; 	// :GRL:304:
  cm["GM"] = "Gambia"; 		// :GMB:270:
  cm["GN"] = "Guinea"; 		// :GIN:324:
  cm["GP"] = "Guadeloupe"; 	// :GLP:312:
  cm["GQ"] = "Equatorial Guinea"; 	// :GNQ:226:
  cm["GR"] = "Greece"; 		// :GRC:300:
  cm["GS"] = "South Georgia and the South Sandwich Islands"; 	// :SGS:239:
  cm["GT"] = "Guatemala"; 	// :GTM:320:
  cm["GU"] = "Guam"; 		// :GUM:316:
  cm["GW"] = "Guinea-Bissau"; 	// :GNB:624:
  cm["GY"] = "Guyana"; 		// :GUY:328:
  cm["HK"] = "Hong Kong"; 	// :HKG:344:
  cm["HM"] = "Heard Island and McDonald Islands"; 	// :HMD:334:
  cm["HN"] = "Honduras"; 	// :HND:340:
  cm["HR"] = "Croatia"; 	// :HRV:191:
  cm["HT"] = "Haiti"; 		// :HTI:332:
  cm["HU"] = "Hungary"; 	// :HUN:348:
  cm["ID"] = "Indonesia"; 	// :IDN:360:
  cm["IE"] = "Ireland"; 	// :IRL:372:
  cm["IL"] = "Israel"; 		// :ISR:376:
  cm["IN"] = "India"; 		// :IND:356:
  cm["IO"] = "British Indian Ocean Territory"; 	// :IOT:086:
  cm["IQ"] = "Iraq"; 		// :IRQ:368:
  cm["IR"] = "Iran"; 		// :IRN:364:
  cm["IS"] = "Iceland"; 	// :ISL:352:
  cm["IT"] = "Italy"; 		// :ITA:380:
  cm["JM"] = "Jamaica"; 	// :JAM:388:
  cm["JO"] = "Jordan"; 		// :JOR:400:
  cm["JP"] = "Japan"; 		// :JPN:392:
  cm["KE"] = "Kenya"; 		// :KEN:404:
  cm["KG"] = "Kyrgyzstan"; 	// :KGZ:417:
  cm["KH"] = "Cambodia"; 	// :KHM:116:
  cm["KI"] = "Kiribati"; 	// :KIR:296:
  cm["KM"] = "Comoros"; 	// :COM:174:
  cm["KN"] = "Saint Kitts and Nevis"; 	// :KNA:659:
  cm["KP"] = "North Korea"; 	// :PRK:408:
  cm["KR"] = "South Korea"; 	// :KOR:410:
  cm["KW"] = "Kuwait"; 		// :KWT:414:
  cm["KY"] = "Cayman Islands"; 	// :CYM:136:
  cm["KZ"] = "Kazakhstan"; 	// :KAZ:398:
  cm["LA"] = "Lao People's Democratic Republic"; 	// :LAO:418:
  cm["LB"] = "Lebanon"; 	// :LBN:422:
  cm["LC"] = "Saint Lucia"; 	// :LCA:662:
  cm["LI"] = "Liechtenstein"; 	// :LIE:438:
  cm["LK"] = "Sri Lanka"; 	// :LKA:144:
  cm["LR"] = "Liberia"; 	// :LBR:430:
  cm["LS"] = "Lesotho"; 	// :LSO:426:
  cm["LT"] = "Lithuania"; 	// :LTU:440:
  cm["LU"] = "Luxembourg"; 	// :LUX:442:
  cm["LV"] = "Latvia"; 		// :LVA:428:
  cm["LY"] = "Libya"; 		// :LBY:434:
  cm["MA"] = "Morocco"; 	// :MAR:504:
  cm["MC"] = "Monaco"; 		// :MCO:492:
  cm["MD"] = "Moldova"; 	// :MDA:498:
  cm["MG"] = "Madagascar"; 	// :MDG:450:
  cm["MH"] = "Marshall Islands"; 	// :MHL:584:
  cm["MK"] = "Macedonia"; 	// :MKD:807:
  cm["ML"] = "Mali"; 		// :MLI:466:
  cm["MM"] = "Myanmar"; 	// :MMR:104:
  cm["MN"] = "Mongolia"; 	// :MNG:496:
  cm["MO"] = "Macao"; 		// :MAC:446:
  cm["MP"] = "Northern Mariana Islands"; 	// :MNP:580:
  cm["MQ"] = "Martinique"; 	// :MTQ:474:
  cm["MR"] = "Mauritania"; 	// :MRT:478:
  cm["MS"] = "Montserrat"; 	// :MSR:500:
  cm["MT"] = "Malta"; 		// :MLT:470:
  cm["MU"] = "Mauritius"; 	// :MUS:480:
  cm["MV"] = "Maldives"; 	// :MDV:462:
  cm["MW"] = "Malawi"; 		// :MWI:454:
  cm["MX"] = "Mexico"; 		// :MEX:484:
  cm["MY"] = "Malaysia"; 	// :MYS:458:
  cm["MZ"] = "Mozambique"; 	// :MOZ:508:
  cm["NA"] = "Namibia"; 	// :NAM:516:
  cm["NC"] = "New Caledonia"; 	// :NCL:540:
  cm["NE"] = "Niger"; 		// :NER:562:
  cm["NF"] = "Norfolk Island"; 	// :NFK:574:
  cm["NG"] = "Nigeria"; 	// :NGA:566:
  cm["NI"] = "Nicaragua"; 	// :NIC:558:
  cm["NL"] = "Netherlands"; 	// :NLD:528:
  cm["NO"] = "Norway"; 		// :NOR:578:
  cm["NP"] = "Nepal"; 		// :NPL:524:
  cm["NR"] = "Nauru"; 		// :NRU:520:
  cm["NU"] = "Niue"; 		// :NIU:570:
  cm["NZ"] = "New Zealand"; 	// :NZL:554:
  cm["OM"] = "Oman"; 		// :OMN:512:
  cm["PA"] = "Panama"; 		// :PAN:591:
  cm["PE"] = "Peru"; 		// :PER:604:
  cm["PF"] = "French Polynesia"; 	// :PYF:258:
  cm["PG"] = "Papua New Guinea"; 	// :PNG:598:
  cm["PH"] = "Philippines"; 	// :PHL:608:
  cm["PK"] = "Pakistan"; 	// :PAK:586:
  cm["PL"] = "Poland"; 		// :POL:616:
  cm["PM"] = "Saint Pierre and Miquelon"; 	// :SPM:666:
  cm["PN"] = "Pitcairn"; 	// :PCN:612:
  cm["PR"] = "Puerto Rico"; 	// :PRI:630:
  cm["PS"] = "Palestinian Territory"; 	// :PSE:275:
  cm["PT"] = "Portugal"; 	// :PRT:620:
  cm["PW"] = "Palau"; 		// :PLW:585:
  cm["PY"] = "Paraguay"; 	// :PRY:600:
  cm["QA"] = "Qatar"; 		// :QAT:634:
  cm["RE"] = "Reunion"; 	// :REU:638:
  cm["RO"] = "Romania"; 	// :ROU:642:
  cm["RU"] = "Russian Federation"; 	// :RUS:643:
  cm["RW"] = "Rwanda"; 		// :RWA:646:
  cm["SA"] = "Saudi Arabia"; 	// :SAU:682:
  cm["SB"] = "Solomon Islands"; 	// :SLB:090:
  cm["SC"] = "Seychelles"; 	// :SYC:690:
  cm["SD"] = "Sudan"; 		// :SDN:736:
  cm["SE"] = "Sweden"; 		// :SWE:752:
  cm["SG"] = "Singapore"; 	// :SGP:702:
  cm["SH"] = "Saint Helena"; 	// :SHN:654:
  cm["SI"] = "Slovenia"; 	// :SVN:705:
  cm["SJ"] = "Svalbard and Jan Mayen"; 	// :SJM:744:
  cm["SK"] = "Slovakia"; 	// :SVK:703:
  cm["SL"] = "Sierra Leone"; 	// :SLE:694:
  cm["SM"] = "San Marino"; 	// :SMR:674:
  cm["SN"] = "Senegal"; 	// :SEN:686:
  cm["SO"] = "Somalia"; 	// :SOM:706:
  cm["SR"] = "Suriname"; 	// :SUR:740:
  cm["ST"] = "Sao Tome and Principe"; 	// :STP:678:
  cm["SV"] = "El Salvador"; 	// :SLV:222:
  cm["SY"] = "Syria"; 		// :SYR:760:
  cm["SZ"] = "Swaziland"; 	// :SWZ:748:
  cm["TC"] = "Turks and Caicos Islands"; 	// :TCA:796:
  cm["TD"] = "Chad"; 		// :TCD:148:
  cm["TF"] = "French Southern Territories"; 	// :ATF:260:
  cm["TG"] = "Togo"; 		// :TGO:768:
  cm["TH"] = "Thailand"; 	// :THA:764:
  cm["TJ"] = "Tajikistan"; 	// :TJK:762:
  cm["TK"] = "Tokelau"; 	// :TKL:772:
  cm["TM"] = "Turkmenistan"; 	// :TKM:795:
  cm["TN"] = "Tunisia"; 	// :TUN:788:
  cm["TO"] = "Tonga"; 		// :TON:776:
  cm["TL"] = "East Timor"; 	// :TLS:626:
  cm["TR"] = "Turkey"; 		// :TUR:792:
  cm["TT"] = "Trinidad and Tobago"; 	// :TTO:780:
  cm["TV"] = "Tuvalu"; 		// :TUV:798:
  cm["TW"] = "Taiwan"; 		// :TWN:158:
  cm["TZ"] = "Tanzania"; 	// :TZA:834:
  cm["UA"] = "Ukraine"; 	// :UKR:804:
  cm["UG"] = "Uganda"; 		// :UGA:800:
  cm["UM"] = "United States Minor Outlying Islands"; 	// :UMI:581:
  cm["US"] = "United States"; 	// :USA:840:
  cm["UY"] = "Uruguay"; 	// :URY:858:
  cm["UZ"] = "Uzbekistan"; 	// :UZB:860:
  cm["VA"] = "Holy See (Vatican City State)"; 	// :VAT:336:
  cm["VC"] = "Saint Vincent and the Grenadines"; 	// :VCT:670:
  cm["VE"] = "Venezuela"; 	// :VEN:862:
  cm["VG"] = "British Virgin Islands"; 	// :VGB:092:
  cm["VI"] = "Virgin Islands, U.S."; 	// :VIR:850:
  cm["VN"] = "Vietnam"; 	// :VNM:704:
  cm["VU"] = "Vanuatu"; 	// :VUT:548:
  cm["WF"] = "Wallis and Futuna"; 	// :WLF:876:
  cm["WS"] = "Samoa"; 		// :WSM:882:
  cm["YE"] = "Yemen"; 		// :YEM:887:
  cm["YT"] = "Mayotte"; 	// :MYT:175:
  cm["ZA"] = "South Africa"; 	// :ZAF:710:
  cm["ZM"] = "Zambia"; 		// :ZMB:894:
  cm["ZW"] = "Zimbabwe"; 	// :ZWE:716:
  return cm;
}
