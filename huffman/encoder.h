#include "utilities.h"

int h_encode( char* input_path, char* output_path );
void getFrequency( FILE* rfp, int freqArr[] );
MinHeap* getMinHeap( int freqArr[] );
Node* getHuffmanTree( MinHeap* minHeap );
void writeHuffmanTree( FILE* wfp, Node* currNode );