#include "utilities.h"

int h_decode( char* input_path, char* output_path );
void searchEndSign( FILE* rfp, int endPosArr[] );
void interpretHuffmanTree( FILE* rfp, Node* HT_root );