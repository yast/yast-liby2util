#include <iostream>
#include <string>

#include <y2util/DiskSpace.h>

using namespace std;

int main(int argc, char* argv[])
{
    DiskSpace::DfVec df;
    df = DiskSpace::df();

    for(DiskSpace::DfVec::iterator it = df.begin();
	it != df.end(); ++it)
    {
	cout << it->device << " " << it->percent << endl;
    }
    return 0;
}
