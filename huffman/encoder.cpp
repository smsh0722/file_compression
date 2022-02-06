#include <iostream>
#include "encoder.h"
using namespace std;

int h_encode( char* input_path, char* output_path )
{
    cout << "start encode\n"; // Debug
    
    FILE* rfp;
    rfp = fopen( input_path, "rb" );

    Node* HT_root;
    {
        int freqArr[256] = {0};
        getFrequency( rfp, freqArr );

        MinHeap* minHeap;
        minHeap = getMinHeap( freqArr );

        HT_root = getHuffmanTree( minHeap );
    }

    fclose( rfp );
    return 1;
}

void getFrequency( FILE* rfp, int freqArr[] )
{
    char c = fgetc( rfp );
    while ( c != EOF ){
        freqArr[(int)c]++;
        fgetc( rfp );
    }
}

MinHeap* getMinHeap( int freqArr[] )
{
    MinHeap* minHeap;
    for ( int i = 0; i < 256; i++ ){
        if ( freqArr[i] != 0 ){
            Node* newNode = new Node;
            newNode->endFlag = true;
            newNode->symbol = i;
            newNode->freq = freqArr[i];
            newNode->lChild = nullptr;
            newNode->rChild = nullptr;
            minHeap->insert( newNode );
        }
    }
    return minHeap;
}

Node* getHuffmanTree( MinHeap* minHeap )
{
    Node* root;

    return root;
}

