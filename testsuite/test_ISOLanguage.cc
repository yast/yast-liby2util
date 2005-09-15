#include <locale.h>

#include <iostream>
#include <y2util/Y2SLog.h>
#include <y2util/ISOLanguage.h>

using namespace std;

void test()
{
#define OUT(LN) INT << "Code '" << #LN << "' ->" << ISOLanguage(#LN).name() << "<-" << endl

  OUT();
  OUT(de);
  OUT(DE);
  OUT(EN);
  OUT(ast);
  OUT(xxx);

#undef OUT
}


int main()
{
  set_log_filename("-");

  DBG << "==================================" << endl;
  setlocale( LC_ALL, "en_US" );
  test();
  DBG << "==================================" << endl;
  setlocale( LC_ALL, "de_DE" );
  test();
  DBG << "==================================" << endl;
  setlocale( LC_ALL, "fr_FR" );
  test();
  DBG << "==================================" << endl;

  return 0;
}
