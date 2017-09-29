/*
 * Data Structures and Algorithms II Assignment #2: Heaps
 * George Ho, Fall 2017
 * 
 * This code implements a priority queue using a binary heap implementation
 */

#include "heap.h"

heap::heap(int capacity) {
    this->capacity = capacity;
    this->size = 0;

    node empty;
    empty.id = "";
    empty.key = 0;
    empty.pData = nullptr;

    this->data.resize(capacity+1, empty);
    mapping = HashTable(2*capacity);
}

int heap::insert(const std::string &id, int key, void *pv) {
    if (this->size >= this->capacity) {
        return 1;
    }
    else if ((this->mapping).contains(id)) {
        return 2;
    }

    node newNode;
    newNode.id = id;
    newNode.key = key;
    newNode.pData = pv;

    // Order of the next 2 lines matter: we ignore data.at(0)!
    this->size++;
    this->data.at(size) = newNode;

    node *ptr = &(this->data.at(size));
    mapping.insert(id, ptr);
    percolateUp(getPos(ptr));

    return 0;
}

int heap::setKey(const std::string &id, int key) {
    bool b = true;
    node *pNode = static_cast<node *>(this->mapping.getPointer(id, &b));

    if (!b) {
        return 1;
    }

    int oldKey = pNode->key;
    pNode->key = key;

    int index = this->getPos(pNode);
    if (oldKey < key) {
        this->percolateDown(index);
    }
    else if (oldKey > key) {
        this->percolateUp(index);
    }

    return 0;
}

int heap::deleteMin(std::string *pId, int *pKey, void *ppData) {
    if (this->size == 0) {
        return 1;
    }

    node minNode = this->data.at(1);

    if (pId != NULL) {
        *pId = minNode.id;
    }

    if (pKey != NULL) {
        *pKey = minNode.key;
    }
    
    if (ppData != NULL) {
        *(static_cast<void **> (ppData)) = minNode.pData;
    }

    this->data.at(1) = this->data.at(this->size);
    this->size--;
    percolateDown(1);

    this->mapping.remove(minNode.id);

    return 0;
}

int heap::remove(const std::string &id, int *pKey, void *ppData) {
    bool b = true;
    node *pNode = static_cast<node *>(mapping.getPointer(id, &b));

    if (!b) {
        return 1;
    }

    if (pKey != NULL) {
        *pKey = pNode->key;
    }

    if (ppData != NULL) {
        ppData = pNode->pData;
    }

    node lastNode = this->data.at(this->size);
    pNode->id = lastNode.id;
    pNode->key = lastNode.key;
    pNode->pData = lastNode.pData;
    this->size--;

    int pos = getPos(pNode);
    if (pNode->key > this->data.at(2*pos).key
            || pNode->key > this->data.at(2*pos+1).key) {
        percolateUp(getPos(pNode));
    }
    else {
        percolateDown(getPos(pNode));
    }

    this->mapping.remove(id);

    return 0;
}

void heap::percolateUp(int currentPos) {
    int parent = 0;
    node temp = this->data.at(currentPos);

    for ( ; parent < this->size; currentPos = parent) {
        parent = currentPos/2;    // C++ integer division

        if (temp.key < this->data.at(parent).key) {
            this->data.at(currentPos) = this->data.at(parent);
        }
        else {
            break;
        }
    }

    this->data.at(currentPos) = temp;

    this->mapping.setPointer(this->data.at(currentPos).id,
            &(this->data.at(currentPos)));
}

void heap::percolateDown(int currentPos) {
    int child = 0;
    node temp = this->data.at(currentPos);

    for ( ; child < this->size; currentPos = child) {
        child = 2*currentPos;

        if (child != this->size
                && this->data.at(child).key < this->data.at(child+1).key) {
            child++;
        }

        if (temp.key > this->data.at(child).key) {
            this->data.at(currentPos) = this->data.at(child);
        }
        else {
            break;
        }
    }

    this->data.at(currentPos) = temp;

    this->mapping.setPointer(this->data.at(currentPos).id,
            &(this->data.at(currentPos)));
}

int heap::getPos(node *pNode) {
    int pos = pNode - &(this->data)[0];
    return pos;
}
