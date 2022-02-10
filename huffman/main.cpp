#include <iostream>
#include <cstring>
#include "encoder.h"
#include "decoder.h"
using namespace std;

int main( int argc, char* argv[] )
{
    if ( argc != 4 ){
        char buffer[128] = {0};
        sprintf( buffer, "Usage: %s -<option>(e or d) <input path> <output path>\n", argv[0] );
        cout << buffer;
    }
    
    char* option = argv[1];
    char* input_path = argv[2];
    char* output_path = argv[3];
    
    if ( strcmp( option, "-e" ) == 0 ){ // encode
        int result;
        if ( ( result = h_encode( input_path, output_path ) ) >= 0 )
            cout << "Result size: " << result << endl;
        else 
            cout << "encode error\n";
    }
    else if ( strcmp( option, "-d" ) == 0 ){ // decode
        cout << "decode\n"; // Debug
        int result;
        if ( ( result = h_decode( input_path, output_path ) ) >= 0 )
            cout << "Result size: " << result << endl;
        else
            cout << "decode error\n";
    }
}