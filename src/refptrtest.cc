#include <RefObject.h>
#include <iostream>
#include <list>
#include <string>

typedef RefObject<std::string> strptr;

void useit(strptr t)
{
	std::cout << "t: " << t->c_str() << std::endl;
}

strptr createone()
{
	std::string* s2 = new std::string("teststring 2");
	strptr s(s2);
	return s;
}

void testlist(strptr a, strptr b)
{
	std::list<strptr> strlist;
	strlist.push_front(a);
	strlist.push_front(b);

	for ( std::list<strptr>::iterator it=strlist.begin(); it != strlist.end(); it++ )
	{
		std::cout << it.operator->()->operator->()->c_str() << std::endl;
	}
}

int main(void)
{
	std::string* s1 = new std::string("teststring 1");
	strptr s(s1);
	strptr u;
	std::cout << "s: " << s->c_str() << std::endl;
	std::cout << "call useit" << std::endl;
	useit(s);
	std::cout << "return from useit" << std::endl;
	std::cout << "call createone" << std::endl;
	u = createone();
	std::cout << "return from useit" << std::endl;
	std::cout << "u: " << u->c_str() << std::endl;
	testlist(s,u);
	return 0;
}
