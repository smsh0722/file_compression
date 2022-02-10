#pragma once
struct Node {
    bool endFlag;
    char symbol;
    int freq;
    Node* lChild;
    Node* rChild;
};

class MinHeap {
    int capacity;
    int size;
    Node** minHeap;
    void minHeapify( int currIdx );
    void swap( int a, int b );
public:
    MinHeap() { minHeap = new Node*[512]; capacity = 512; size = 0; }
    void insert( Node* newNode );
    Node* getMin();
    int getSize(){ return size; }
};