#include "ExtendedTest.h"
#include "ShortTest.h"

#include "SortedMap.h"


#include <iostream>
using namespace std;


int main() {
	testAll();
    cout << "Shorts over!" << endl;
	testAllExtended();

	cout << "That's all!" << endl;
	system("pause");
	return 0;
}


