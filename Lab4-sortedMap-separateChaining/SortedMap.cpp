#include "SMIterator.h"
#include "SortedMap.h"
#include <exception>
#include <iostream>
using namespace std;

SortedMap::SortedMap(Relation r) {
    rel = r;
    maxCapacity = 4;
    sizeBag = 0;
    Table = new Node * [maxCapacity];
    initTable();
}

void SortedMap::resizeRehash() {
    int oldCapacity = maxCapacity;
    maxCapacity *= 2;
    Node ** newTable = new Node * [maxCapacity];
    
    // Initialize new table with nullptr
    for (int i = 0; i < maxCapacity; i++) {
        newTable[i] = nullptr;
    }

    std::cout << "Starting resize rehash. Old capacity: " << oldCapacity
              << ", New capacity: " << maxCapacity << std::endl;

    for (int i = 0; i < oldCapacity; i++) {
        Node *current = Table[i];
        while (current != nullptr) {
            Node *nextNode = current->next;
            int newIndex = h(current->element.first);

            std::cout << "Rehashing element (" << current->element.first << ", "
                      << current->element.second << ") from old index " << i
                      << " to new index " << newIndex << std::endl;

            // Insert in sorted order within the new bucket
            if (newTable[newIndex] == nullptr) {
                // If the bucket is empty
                current->next = nullptr;
                newTable[newIndex] = current;
                std::cout << "Inserted at start of empty bucket " << newIndex << std::endl;
            } else {
                // If bucket is not empty, insert in sorted order
                Node *bucketCurrent = newTable[newIndex];
                Node *bucketPrev = nullptr;
                
                while (bucketCurrent != nullptr && rel(bucketCurrent->element.first, current->element.first)) {
                    bucketPrev = bucketCurrent;
                    bucketCurrent = bucketCurrent->next;
                }

                if (bucketPrev == nullptr) {
                    current->next = newTable[newIndex];
                    newTable[newIndex] = current;
                    std::cout << "Inserted at head of bucket " << newIndex
                              << ", before (" << bucketCurrent->element.first
                              << ", " << bucketCurrent->element.second << ")" << std::endl;
                } else {
                    bucketPrev->next = current;
                    current->next = bucketCurrent;
                    std::cout << "Inserted after (" << bucketPrev->element.first
                              << ", " << bucketPrev->element.second
                              << ") in bucket " << newIndex << std::endl;
                }
            }

            current = nextNode;
        }
    }

    // After rehashing, print the new table structure
    std::cout << "New table structure after rehashing:" << std::endl;
    for (int i = 0; i < maxCapacity; i++) {
        std::cout << "Bucket " << i << ": ";
        Node * temp = newTable[i];
        while (temp != nullptr) {
            std::cout << "(" << temp->element.first << ", " << temp->element.second << ") -> ";
            temp = temp->next;
        }
        std::cout << "nullptr" << std::endl;
    }

    delete [] Table;
    Table = newTable;
    updateAlpha();
    std::cout << "Resize rehash complete." << std::endl;
}

//hash
int SortedMap::h(TKey key) const {
    int index = std::abs(key) % maxCapacity;
    std::cout << "Key " << key << " hashes to bucket " << index << std::endl;
    return index;
}

//update alpha
void SortedMap::updateAlpha(){
    alphaLoadFactor = static_cast<float>(sizeBag)/maxCapacity;
};
//init table
void SortedMap::initTable(){
    for (int i = 0; i < maxCapacity; i++) {
        Table[i] = nullptr;
    }
};

TValue SortedMap::add(TKey k, TValue v) {
    if (alphaLoadFactor > 0.8) {
        resizeRehash();
    }
    std::cout << "Adding (" << k << ", " << v << ") to SortedMap" << std::endl;
    int pos = h(k);
    std::cout << "Adding (" << k << ", " << v << ") to position " << pos << std::endl;

    // Navigators for the bucket
    Node* current = Table[pos];
    Node* prev = nullptr;

    // Search for an existing key to replace its value
    while (current != nullptr) {
        if (current->element.first == k) {
            TValue oldVal = current->element.second;
            current->element.second = v;
            std::cout << "Key " << k << " already exists. Replacing value " << oldVal << " with " << v << std::endl;
            return oldVal;
        }
        prev = current;
        current = current->next;
    }

    // Create new node for the pair (k, v)
    TElem newPairElem = std::make_pair(k, v);
    Node* newNode = new Node(newPairElem);

    // Insert cases with debug information
    if (Table[pos] == nullptr) {
        // Case 1: Empty bucket
        Table[pos] = newNode;
        std::cout << "Inserted at start of empty bucket " << pos << std::endl;
    }
    else if (!rel(Table[pos]->element.first, k)) {
        // Case 2: Insert at the head if it should come first
        newNode->next = Table[pos];
        Table[pos] = newNode;
        std::cout << "Inserted at head of bucket " << pos << ", before (" << newNode->next->element.first << ", " << newNode->next->element.second << ")" << std::endl;
    }
    else {
        // Case 3: Insert in sorted position within the bucket
        current = Table[pos];
        prev = nullptr;

        // Traverse to find the correct position based on the relation
        while (current != nullptr && rel(current->element.first, k)) {
            prev = current;
            current = current->next;
        }

        prev->next = newNode;
        newNode->next = current;

        if (current != nullptr) {
            std::cout << "Inserted (" << k << ", " << v << ") between (" << prev->element.first << ", " << prev->element.second << ") and (" << current->element.first << ", " << current->element.second << ") in bucket " << pos << std::endl;
        } else {
            std::cout << "Inserted (" << k << ", " << v << ") at the end of bucket " << pos << " after (" << prev->element.first << ", " << prev->element.second << ")" << std::endl;
        }
    }

    // Update size and load factor
    sizeBag++;
    updateAlpha();

    return NULL_TVALUE;
}

TValue SortedMap::remove(TKey k) {
    int pos = h(k);
    Node* current = Table[pos];
    Node* prev = nullptr;
    
    while (current != nullptr && current->element.first != k) {
        prev = current;
        current = current->next;
    }
    if (current == nullptr) {
        return NULL_TVALUE; // Key not found
    }
    
    TValue oldValue = current->element.second;
    
    // Update pointers correctly to remove the node
    if (prev == nullptr) {
        // Remove first node in the chain
        Table[pos] = current->next;
    } else {
        // Remove a node from middle or end
        prev->next = current->next;
    }
    
    delete current;
    sizeBag--;
    updateAlpha();
    return oldValue;
}

TValue SortedMap::search(TKey k) const {
    int pos = h(k);
    Node* current = Table[pos];
    
    while (current != nullptr && current->element.first != k) {
        current = current->next;
    }
    
    if (current != nullptr) {
        //it should have moved to element that key == k
        return current->element.second;
    } else {
        return NULL_TVALUE;
    }
}

int SortedMap::size() const {
    return sizeBag;
}

bool SortedMap::isEmpty() const {
    return sizeBag == 0;
}

SMIterator SortedMap::iterator() const {
	return SMIterator(*this);
}

SortedMap::~SortedMap() {
    for (int i = 0; i < maxCapacity; i++) {
        Node* current = Table[i];
        while (current != nullptr) {
            Node* toDelete = current;
            current = current->next;
            delete toDelete;
        }
    }
    delete[] Table;
}
