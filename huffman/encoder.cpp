#include <iostream>
#include <cstring>
#include "encoder.h"
using namespace std;

int h_encode( char* input_path, char* output_path )
{
    cout << "start encode\n"; // Debug
    
    FILE* rfp;
    rfp = fopen( input_path, "rb" );

    Node* HT_root; // Huffman tree's root
    { // Build huffman tree
        int freqArr[256] = {0};
        getFrequency( rfp, freqArr );
        cout << "Done freq!\n"; // Debug

        MinHeap* minHeap;
        minHeap = getMinHeap( freqArr );
        cout << "Done getMinHeap!\n"; // Debug

        HT_root = getHuffmanTree( minHeap );
        cout << "done getHuffmanTree!\n"; // Debug
    }

    string HT_table[256]; // Huffman code table
    { // Build huffman code table
        for ( int i = 0; i < 256; i++ )
            HT_table[i] = "";
        getHuffmanCode( HT_root, HT_table, "" );

        for ( int i = 0; i < 256; i++ ){ // Debug
            if ( HT_table[i].length() > 0 ){
                cout << char(i) << ": ";
                cout << HT_table[i] << endl;
            }
        }
    }

    /* Start Encoding
     * write data code
     * write data size    (HEADER)
     * write huffman tree (HEADER)
     * */
    FILE* wfp; 
    wfp = fopen( output_path, "wb" );
    writeHuffmanTree( wfp, HT_root );

    fclose( rfp );
    fclose( wfp );
    return 1;
}

void getFrequency( FILE* rfp, int freqArr[] )
{
    char c = fgetc( rfp );;
    while ( c != EOF ){
        freqArr[(unsigned int)c]++;
        c = fgetc( rfp );
    }
}
MinHeap* getMinHeap( int freqArr[] )
{
    MinHeap* minHeap = new MinHeap;
    for ( int i = 0; i < 256; i++ ){
        if ( freqArr[i] != 0 ){
            Node* newNode = new Node;
            {
                newNode->endFlag = true;
                newNode->symbol = (char)i;
                newNode->freq = freqArr[i];
                newNode->lChild = nullptr;
                newNode->rChild = nullptr;
            }
            minHeap->insert( newNode );
        }
    }
    return minHeap;
}
Node* getHuffmanTree( MinHeap* minHeap )
{
    Node* root = nullptr;
    while ( minHeap->getSize() > 1 ){
        Node* lNode = minHeap->getMin();
        Node* rNode = minHeap->getMin();
        Node* newNode = new Node;
        {
            newNode->endFlag = false;
            newNode->symbol = 0;
            newNode->freq = lNode->freq + rNode->freq;
            newNode->lChild = lNode;
            newNode->rChild = rNode;
        }
        minHeap->insert( newNode );
    }
    root = minHeap->getMin();
    return root;
}
void getHuffmanCode( Node* HT_root, string HT_table[], string currCode )
{
    // Base case
    if ( HT_root->endFlag == true ){
        int symbol = (unsigned int)(HT_root->symbol);
        HT_table[symbol] = currCode;
        return;
    }
    // DFS
    getHuffmanCode( HT_root->lChild, HT_table, currCode + "0" ); // left child
    getHuffmanCode( HT_root->rChild, HT_table, currCode + "1" ); // right child
}
void writeHuffmanTree( FILE* wfp, Node* currNode )
{
    // Base
    if ( currNode->endFlag == true ) {
        char buffer[32] = {0};
        sprintf( buffer, "%c[%d]", currNode->symbol, currNode->freq );
        fwrite( buffer, sizeof(char)*strlen(buffer), 1, wfp );
        return;
    }
    // Recursive write
    fwrite( "( ", sizeof(char)*strlen("( "), 1, wfp );
    writeHuffmanTree( wfp, currNode->lChild);
    fwrite( ", ", sizeof(char)*strlen(", "), 1, wfp );
    writeHuffmanTree( wfp, currNode->rChild);
    fwrite( " )", sizeof(char)*strlen(" )"), 1, wfp );
}