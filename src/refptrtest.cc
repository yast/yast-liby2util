#include <RefObject.h>
#include <iostream>
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
	return 0;
}
