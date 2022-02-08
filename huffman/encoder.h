#include <string>
#include "utilities.h"

int h_encode( char* input_path, char* output_path );
void getFrequency( FILE* rfp, int freqArr[] );
MinHeap* getMinHeap( int freqArr[] );
Node* getHuffmanTree( MinHeap* minHeap );
void getHuffmanCode( Node* currNode, std::string HT_table[], std::string currCode );
int writeData2Code( FILE* rfp, FILE* wfp, std::string HT_table[] );
void writeHuffmanTree( FILE* wfp, Node* currNode );