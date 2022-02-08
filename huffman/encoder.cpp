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
    {
        int encodedSize;
        // Write data code
        encodedSize = writeData2Code( rfp, wfp, HT_table );
        cout << "Encoded data size: " << encodedSize << endl; // Debug
        // Write data size

        // Write huffman tree
        writeHuffmanTree( wfp, HT_root );
    }
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
void getHuffmanCode( Node* currNode, string HT_table[], string currCode )
{
    // Base case
    if ( currNode->endFlag == true ){
        int symbolIdx = (unsigned int)(currNode->symbol);
        HT_table[symbolIdx] = currCode;
        return;
    }
    // DFS
    getHuffmanCode( currNode->lChild, HT_table, currCode + "0" ); // left child
    getHuffmanCode( currNode->rChild, HT_table, currCode + "1" ); // right child
}
int writeData2Code( FILE* rfp, FILE* wfp, string HT_table[] )
{
    char pack = 0;  // bits pack for huffman code
    char trgSym;    // a symbol of input file
    int sum = 0;    // bits sum
    int count = 0;  // packed count(0~8)

    // Reset read file pointer
    fseek( rfp, 0, SEEK_SET );

    // Pack and write
    trgSym = fgetc( rfp );
    while ( trgSym != EOF ){
        string code = HT_table[(unsigned int)trgSym]; // huffman code for a symbol
        for ( int i = 0; i < code.length(); i++ ){
            pack = pack << 1;
            pack = pack | ( code[i] - '0' );
            count++;
            if ( count == 8 ){
                fwrite( &pack, sizeof(char), 1, wfp );
                sum += 8;
                pack = 0;
                count = 0;
            }
        }
        trgSym = fgetc( rfp );
    }
    if ( count != 0 ){
        fwrite( &pack, sizeof(char), 1, wfp );
        sum += count;
        pack = 0;
        count = 0;
    }

    return sum;
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