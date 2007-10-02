#include <fstream>
#include <y2util/CommonPkdParser.h>
#include <stdlib.h>

#define CREATETAG(tagname,num) \
    t = new CommonPkdParser::Tag(tagname,CommonPkdParser::Tag::ACCEPTONCE); \
    this->addTag(t); \
    addTagByIndex(num,t);

class CommonPkdTagSet : public CommonPkdParser::TagSet
{
    public:
	enum Tags {
	    FILENAME,
	    SERIES,
	    RPMNAME,
	    INSTPATH,
	    SIZE,
	    BUILDTIME,
	    BUILTFROM,
	    FLAG,
	    RPMGROUP,
	    COPYRIGHT,
	    AUTHORNAME,
	    VERSION,
	    OBSOLETES,
	    REQUIRES,
	    PROVIDES,
	    CONFLICTS,
	    NUM_TAGS
	};
    public:
	CommonPkdTagSet() : TagSet()
	{
	    CommonPkdParser::Tag* t;
	    CREATETAG("Filename",FILENAME)
	    CREATETAG("Series",SERIES)
	    CREATETAG("RpmName",RPMNAME)
	    CREATETAG("InstPath",INSTPATH)
	    CREATETAG("Size",SIZE)
	    CREATETAG("Buildtime",BUILDTIME)
	    CREATETAG("BuiltFrom",BUILTFROM)
	    CREATETAG("Flag",FLAG)
	    CREATETAG("RpmGroup",RPMGROUP)
	    CREATETAG("Copyright",COPYRIGHT)
	    CREATETAG("AuthorName",AUTHORNAME)
	    CREATETAG("Version",VERSION)
	    CREATETAG("Obsoletes",OBSOLETES)
	    CREATETAG("Requires",REQUIRES)
	    CREATETAG("Provides",PROVIDES)
	    CREATETAG("Conflicts",CONFLICTS)
	}
};

class SelectionTagSet : public CommonPkdParser::TagSet
{
    public:
	enum Tags {
	    VERSION,
	    NAME,
	    SUGGESTS,
	    CONFLICTS,
	    CATEGORY,
	    VISIBLE,
	    LABEL,
	    TOINSTALL,
	    TOINSTALL_,
	    NUM_TAGS
	};
    private:
	std::string _locale;
    public:
	SelectionTagSet(const std::string& locale) : TagSet(), _locale(locale) 
	{
	    CommonPkdParser::Tag* t;
	    CREATETAG("Version",VERSION)
	    CREATETAG("Name",NAME)
	    CREATETAG("Suggests",SUGGESTS)
	    CREATETAG("Conflicts",CONFLICTS)
	    CREATETAG("Category",CATEGORY)
	    CREATETAG("Visible",VISIBLE)
	    CREATETAG("Label",LABEL)
	    t->setType(CommonPkdParser::Tag::ACCEPTPREFERREDLOCALE);
	    t->setPreferredLocale(locale);
	    _localetags.push_back(t);
	    CREATETAG("Toinstall",TOINSTALL)
	    t->setEndTag("Llatsniot");
	    CREATETAG("Toinstall.",TOINSTALL_)
	    t->setType(CommonPkdParser::Tag::ACCEPTLOCALEONLY);
	    t->setPreferredLocale(locale);
	    t->setEndTag("Llatsniot");
	    _localetags.push_back(t);
	}
};

class YOUPatchTagSet : public CommonPkdParser::TagSet
{
    public:
	enum Tags {
	    KIND,
	    SHORTDESCRIPTION,
	    LONGDESCRIPTION,
	    SIZE,
	    BUILDTIME,
	    MINYAST1VERSION,
	    MINYAST2VERSION,
	    UPDATEONLYINSTALLED,
	    PACKAGES,
	    NUM_TAGS
	};
    private:
	std::string _locale;
    public:
	YOUPatchTagSet(const std::string& locale) : TagSet(), _locale(locale) 
	{
	    CommonPkdParser::Tag* t;
	    CREATETAG("Kind",KIND)
	    CREATETAG("Shortdescription.",SHORTDESCRIPTION)
	    t->setType(CommonPkdParser::Tag::ACCEPTLOCALEONLY);
	    t->setDefaultLocale("english");
	    t->setPreferredLocale(locale);
	    _localetags.push_back(t);
	    CREATETAG("Longdescription.",LONGDESCRIPTION)
	    t->setType(CommonPkdParser::Tag::ACCEPTLOCALEONLY);
	    t->setDefaultLocale("english");
	    t->setPreferredLocale(locale);
	    t->setEndTag("noitpircsedgnol",CommonPkdParser::Tag::ENDTAG_COMPLETELYREVERSED);
	    _localetags.push_back(t);
	    CREATETAG("Size",SIZE)
	    CREATETAG("Buildtime",BUILDTIME)
	    CREATETAG("MinYaST1Version",MINYAST1VERSION)
	    CREATETAG("MinYaST2Version",MINYAST1VERSION)
	    CREATETAG("UpdateOnlyInstalled",UPDATEONLYINSTALLED)
	    CREATETAG("Packages",PACKAGES)
	    t->setEndTag("Segakcap");
	}
};
#undef CREATETAG

//#define SELECTION
//#define YOUPATCH

int main(void)
{
//    std::string commonpkd("/var/adm/current_package_descr/suse/setup/descr/default.sel");
//    std::string commonpkd("/var/adm/current_package_descr/suse/setup/descr/common.pkd");
    std::cerr << "starting parser" << std::endl;
#ifdef SELECTION
    std::string commonpkd("test.data/default.sel");
#elif defined YOUPATCH
    std::string commonpkd("test.data/sample.youpatch");
#else
    std::string commonpkd("test.data/common.pkd");
#endif
    TagParser parser;
    std::string tagstr;

    std::ifstream commonpkdstream(commonpkd.c_str());
    if(!commonpkdstream)
    {
	std::cerr << "file not found" << std::endl;
	exit(1);
    }

    CommonPkdParser::TagSet* tagset;
#ifdef SELECTION
    tagset = new SelectionTagSet("de_DE");
    tagset->setEncoding(CommonPkdParser::Tag::UTF8);
#elif defined YOUPATCH
    tagset = new YOUPatchTagSet("german");
    tagset->setEncoding(CommonPkdParser::Tag::UTF8);
#else
    tagset = new CommonPkdTagSet();
#endif

    bool repeatassign = false;
    bool parse = true;
    while( parse && parser.lookupTag(commonpkdstream))
    {
	tagstr = parser.startTag();

	do
	{
	    switch(tagset->assign(tagstr.c_str(),parser,commonpkdstream))
	    {
		case CommonPkdParser::Tag::ACCEPTED:
		    repeatassign = false;
		    break;
		case CommonPkdParser::Tag::REJECTED_NOMATCH:
//		    std::cerr << "unknown tag " << tagstr << std::endl;
		    repeatassign = false;
		    break;
		case CommonPkdParser::Tag::REJECTED_FULL:
#ifdef SELECTION
		    if(0)
#else
		    if( tagstr != "Filename" )
#endif
		    {
			std::cerr << "syntax error" << std::endl;
			parse = false;
		    }
//		    std::cerr << "tagset already has " << tagstr << std::endl;
		    tagset->clear();
		    repeatassign = true;
		    break;
		case CommonPkdParser::Tag::REJECTED_NOENDTAG:
		    repeatassign = false;
		    parse = false;
		    break;
	    }
	} while(repeatassign);
    }

    if(parse)
	std::cerr << "*** parsing completed ***" << std::endl;
    else
	std::cerr << "*** parsing was aborted ***" << std::endl;
    
    tagset->print(std::cout);
    std::cout << "***" << std::endl;
#ifdef SELECTION
    std::cout << tagset->getTagByIndex(SelectionTagSet::VERSION) << std::endl;
    tagset->getTagByIndex(SelectionTagSet::VERSION)->print(std::cout);
#elif defined YOUPATCH
    tagset->getTagByIndex(YOUPatchTagSet::SHORTDESCRIPTION)->print(std::cout);
#else
    tagset->getTagByIndex(CommonPkdTagSet::VERSION)->print(std::cout);
#endif

    return 0;
}
// vim:sw=4
