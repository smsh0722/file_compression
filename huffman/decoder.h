#include "utilities.h"

int h_decode( char* input_path, char* output_path );
void searchEndSign( FILE* rfp, int endPosArr[] );
int interpretHuffmanTree( FILE* rfp, Node* currNode );
int64_t getDataSize( FILE* rfp, int endPosArr[] );
void decodeData( FILE* rfp, FILE* wfp, Node* HT_root, const int64_t DATA_SIZE );