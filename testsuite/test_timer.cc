#include <unistd.h>
#include <iostream>

#include <y2util/timeclass.h>

using namespace std;

int main(int argc, char* argv[])
{
	TimeClass t;

	t.startTimer();
	sleep(0);

	cout << t.stopTimer() << endl;

	cout << t << endl;
	
	cout << t.seconds() << "." << t.useconds() << endl;

	t.resetTimer();
	
	cout << t << endl;

	return 0;
}
