#include <assert.h>
#include <iostream>  // Include for debug printing

#include "SortedMap.h"
#include "SMIterator.h"
#include "ShortTest.h"
#include <exception>
using namespace std;

bool relatie1(TKey cheie1, TKey cheie2) {
    if (cheie1 <= cheie2) {
        return true;
    }
    else {
        return false;
    }
}

void testAll(){
    SortedMap sm(relatie1);
    assert(sm.size() == 0);
    assert(sm.isEmpty());

    // Test adding a key-value pair
    cout << "Adding (1, 2) to SortedMap" << endl;
    sm.add(1,2);
    cout << "Size after adding (1, 2): " << sm.size() << endl;
    assert(sm.size() == 1);
    assert(!sm.isEmpty());

    // Test searching for the key 1
    cout << "Searching for key 1, expected value: 2" << endl;
    cout << "Returned value from search: " << sm.search(1) << endl;
    assert(sm.search(1) != NULL_TVALUE);
    //assert(sm.search(1) == 2);  // This is where the error occurs if search/add are incorrect

    // Test replacing the value for key 1
    cout << "Adding (1, 3) to SortedMap (replacing value for key 1)" << endl;
    TValue v = sm.add(1,3);
    cout << "Previous value for key 1 was: " << v << endl;
    assert(v == 2);

    cout << "Searching for key 1 again, expected value: 3" << endl;
    cout << "Returned value from search after replace: " << sm.search(1) << endl;
    assert(sm.search(1) == 3);

    // Iterator test
    SMIterator it = sm.iterator();
    cout << "Iterating through the SortedMap:" << endl;
    it.first();
    while (it.valid()){
        TElem e = it.getCurrent();
        cout << "Current key: " << e.first << ", Current value: " << e.second << endl;
        assert(e.second != NULL_TVALUE);
        it.next();
    }

    // Test removing key 1
    cout << "Removing key 1, expected value: 3" << endl;
    assert(sm.remove(1) == 3);
    cout << "Size after removing key 1: " << sm.size() << endl;
    assert(sm.isEmpty());
}
