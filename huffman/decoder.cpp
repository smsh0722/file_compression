#include <iostream>
#include <cstring>
#include "decoder.h"
using namespace std;

int h_decode( char* input_path, char* output_path )
{
    /* 1. Interpret huffman Tree
     * 2. Check target data
     * 3. Decode input data 
     * */
    FILE* rfp;
    rfp = fopen( input_path, "rb" );
    
    int endPosArr[2] = {0};
    searchEndSign( rfp, endPosArr );
    cout << endPosArr[0] << ", " << endPosArr[1] << endl; // Debug

    Node* HT_root;
    interpretHuffmanTree( rfp, HT_root, endPosArr[0] );
}
void searchEndSign( FILE* rfp, int endPosArr[] )
{
    int c;
    int filePos = 0;
    int endPosArr_idx = 0;
    char trg[4] = "END";
    int trgIdx = 0;
    while ( endPosArr_idx < 2 ){
        c = fgetc( rfp );
        filePos++;

        if ( trg[trgIdx] == (char)c )
            trgIdx++;
        else
            trgIdx = 0;

        if ( trgIdx == 3 ){
            endPosArr[endPosArr_idx++] = filePos - 3;
            trgIdx = 0;
        }
    }
}
void interpretHuffmanTree( FILE* rfp, Node* HT_root, const int endPos )
{
    fseek( rfp, 0, SEEK_SET );

    int previousC, currC;
    currC = fgetc( rfp );
    while( previousC != EOF ){

    }
}