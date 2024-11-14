#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>

using namespace std;

SMIterator::SMIterator(const SortedMap& m) : map(m) {
    first();
}

void SMIterator::first() {
    currentPos = 0;
    // Move to the first non-empty bucket
    while (currentPos < map.maxCapacity && map.Table[currentPos] == nullptr) {
        currentPos++;
    }
    // Set the current node to the first node in the non-empty bucket (if found)
    if (currentPos < map.maxCapacity) {
        currentNode = map.Table[currentPos];
    } else {
        currentNode = nullptr;
    }
}

void SMIterator::next() {
    if (!valid()) {
        throw exception();
    }

    // Move to the next node in the current chain if possible
    if (currentNode->next != nullptr) {
        currentNode = currentNode->next;
    } else {
        // Otherwise, find the next non-empty bucket
        currentNode = nullptr;
        currentPos++;
        while (currentPos < map.maxCapacity && map.Table[currentPos] == nullptr) {
            currentPos++;
        }
        // Set currentNode to the head of the next non-empty bucket, if any
        if (currentPos < map.maxCapacity) {
            currentNode = map.Table[currentPos];
        }
    }
}

bool SMIterator::valid() const {
    return currentNode != nullptr;
}

TElem SMIterator::getCurrent() const {
    if (!valid()) {
        throw exception();
    }
    return currentNode->element;
}
