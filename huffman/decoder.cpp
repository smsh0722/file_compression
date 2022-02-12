#include <iostream>
#include <cstring>
#include <string>

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

    // Interpret huffman tree
    Node* HT_root = new Node;
    fseek( rfp, 0, SEEK_SET ); // reset 'read' file pointer
    interpretHuffmanTree( rfp, HT_root );
    
    // Check error
    if ( ftell( rfp ) - 1 != endPosArr[0] )
        return -1;
    
    // Get data size
    const int64_t DATA_SIZE = getDataSize( rfp, endPosArr );

    // Decode data

    return 1; // tmp return val
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
int interpretHuffmanTree( FILE* rfp, Node* currNode )
{   
    /* non-leaf node must pass three delimiter
     * leaf node just end with symbol
     * */

    int previousC, currC;
    int count = 0; // for non-leaf node

    // start interpret 
    currC = fgetc( rfp );
    while ( count <= 2 ){
        previousC = currC;
        currC = fgetc( rfp );
        if ( previousC == '(' && currC == ' '){         // delimiter case 1
            currNode->endFlag = false;
            currNode->lChild = new Node;
            currC = interpretHuffmanTree( rfp, currNode->lChild );
            count++;
        }
        else if ( previousC == ',' && currC == ' ' ){   // delimiter case 2
            currNode->endFlag = false;
            currNode->rChild = new Node;
            currC = interpretHuffmanTree( rfp, currNode->rChild );
            count++;
        }
        else if ( previousC == ')' && currC == ' ' ){   // delimiter case 3
            currC = fgetc( rfp );
            count++;
        }
        else { // just symbol
            currNode->endFlag = true;
            currNode->symbol = previousC;
            cout << "curr sym: " << currNode->symbol << endl; // Debug

            return currC;
        }
    }
    return currC;
}
int64_t getDataSize( FILE* rfp, int endPosArr[] )
{
    fseek( rfp, endPosArr[0] + 3, SEEK_SET );

    string dataSize_str = "";
    int remainingCount = endPosArr[1] - endPosArr[0] - 3;
    while ( remainingCount > 0 ){
        dataSize_str += fgetc( rfp );
        remainingCount--;
    }

    return stol( dataSize_str, nullptr, 10 );
}