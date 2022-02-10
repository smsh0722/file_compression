#include <iostream>
#include "utilities.h"
using namespace std;

void MinHeap::insert( Node* newNode )
{
    if ( capacity == size ){
        cout << "heap overflow\n";
        exit(-1);
    }
    
    size++;
    minHeap[size-1] = newNode;
    
    // fix minHeap
    int cIdx = size - 1;
    int pIdx = ( cIdx + 1 ) / 2 - 1;
    while ( pIdx >= 0 && minHeap[pIdx]->freq > minHeap[cIdx]->freq ){
        swap( pIdx, cIdx );
        cIdx = pIdx;
        pIdx = ( cIdx + 1 ) / 2 - 1;
    }
}
Node* MinHeap::getMin()
{
    if ( size == 0 ){
        cout << "heap underflow\n";
        exit(-1);
    }
    if ( size == 1 ){
        size--;
        return minHeap[0];
    }

    Node* rst = minHeap[0];
    minHeap[0] = minHeap[size-1];
    size--;

    minHeapify( 0 );

    return rst;
}
void MinHeap::minHeapify( int currIdx )
{
    int lIdx = currIdx * 2 + 1; // left child Idx
    int rIdx = currIdx * 2 + 2; // right child Idx
    int sIdx = currIdx;         // smallest Idx

    if ( lIdx < size && minHeap[currIdx]->freq > minHeap[lIdx]->freq )
        sIdx = lIdx;
    if ( rIdx < size && minHeap[sIdx]->freq > minHeap[rIdx]->freq )
        sIdx = rIdx;
    

    if ( sIdx != currIdx ){
        swap ( currIdx, sIdx );
        minHeapify( sIdx );
    }
}
void MinHeap::swap( int a, int b )
{
    Node* tmp = minHeap[a];
    minHeap[a] = minHeap[b];
    minHeap[b] = tmp;
}

