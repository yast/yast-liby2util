#include <iostream>
#include <string>

#include <y2util/Url.h>

using namespace std;

int main(int argc, char* argv[])
{
    if(argc<2)
    {
	cerr << "must specify url" << endl;
	return 1;
    }
    string s1 = argv[1];
    Url u1(s1);
    cout << s1 << " is " << (u1.isValid()?"valid ":"invalid ")
	<< " -> " << u1.asString() << endl;

    // reverse shell logic
    return !u1.isValid();
}
