#include <iostream>
#include <cstring>
#include "encoder.h"
using namespace std;

const int NUMOFSYMBOL = 256;

int h_encode( char* input_path, char* output_path )
{
    cout << "start encode\n"; // Debug
    
    FILE* rfp;
    rfp = fopen( input_path, "rb" );

    Node* HT_root; // Huffman tree's root
    int freqArr[NUMOFSYMBOL] = {0}; // Symbol's freqency
    { // Build huffman tree
        getFrequency( rfp, freqArr );
        cout << "Done freq!\n"; // Debug

        MinHeap* minHeap;
        minHeap = getMinHeap( freqArr );
        cout << "Done getMinHeap!\n"; // Debug

        HT_root = getHuffmanTree( minHeap );
        cout << "done getHuffmanTree!\n"; // Debug
    }

    string HT_table[NUMOFSYMBOL]; // Huffman code table
    { // Build huffman code table
        for ( int i = 0; i < NUMOFSYMBOL; i++ )
            HT_table[i] = "";
        getHuffmanCode( HT_root, HT_table, "" );

        for ( int i = 0; i < NUMOFSYMBOL; i++ ){ // Debug
            if ( HT_table[i].length() > 0 ){
                cout << char(i) << ": ";
                cout << HT_table[i] << endl;
            }
        }
    }

    /* Start Encoding
     * write huffman tree (HEADER)
     * write data size    (HEADER)
     * write data code
     * */
    FILE* wfp; 
    int64_t expectedSize, encodedSize;
    wfp = fopen( output_path, "wb" );
    {
        // Write huffman tree
        writeHuffmanTree( wfp, HT_root );
        writeEndSign( wfp );

        // Write data size
        expectedSize = writeDataSize( wfp, freqArr, HT_table );
        writeEndSign( wfp );

        // Write data code
        encodedSize = writeData2Code( rfp, wfp, HT_table );
        cout << "Encoded data size: " << encodedSize << endl; // Debug
    }
    fclose( rfp );
    fclose( wfp );
    
    if ( expectedSize == encodedSize )
        return encodedSize;
    else 
        return -1;
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
    for ( int i = 0; i < NUMOFSYMBOL; i++ ){
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
int64_t writeData2Code( FILE* rfp, FILE* wfp, string HT_table[] )
{
    char pack = 0;  // bits pack for huffman code
    char trgSym;    // a symbol of input file
    int64_t sum = 0;    // bits sum
    int count = 0;  // packed count(0~8)

    // Reset 'read' file pointer
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
int64_t writeDataSize( FILE* wfp, int freqArr[], string HT_table[] )
{
    // Calculate encoded size
    int64_t encodedSize = 0;
    for ( int symIdx = 0; symIdx < NUMOFSYMBOL; symIdx++ )
        encodedSize += (int64_t)freqArr[symIdx] * HT_table[symIdx].length();
    
    // write size
    char encodedSize_string[32] = { 0 };
    sprintf( encodedSize_string, "%ld", encodedSize );
    fwrite( encodedSize_string, sizeof(char), strlen(encodedSize_string), wfp );

    return encodedSize;
}
void writeHuffmanTree( FILE* wfp, Node* currNode )
{
    // fseek( wfp, 0, SEEK_SET );

    // Base
    if ( currNode->endFlag == true ) {
        fwrite( &(currNode->symbol), sizeof(char), 1, wfp );
        return;
    }
    // Recursive write
    fwrite( "( ", sizeof(char)*strlen("( "), 1, wfp );
    writeHuffmanTree( wfp, currNode->lChild);
    fwrite( ", ", sizeof(char)*strlen(", "), 1, wfp );
    writeHuffmanTree( wfp, currNode->rChild);
    fwrite( ") ", sizeof(char)*strlen(") "), 1, wfp );
}
void writeEndSign( FILE* wfp )
{
    // Write "END"
    char HEADER[4] = "END";
    fwrite( HEADER, sizeof(char), strlen(HEADER), wfp );
}