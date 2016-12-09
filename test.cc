#include <iostream>
#include <string>

#include "Cache.h"

int
main( )
{
    int calls = 0;

    // define your expensive function to retrieve the value of a key
    auto fetch_row = [&calls]( int i ) {
        ++calls;
        return std::string( "row_" ) + std::to_string( i );
    };

    // create your cache with lambda. You can use functors also if you want
    Cache< int, std::string, decltype( fetch_row ) > cache{10, fetch_row};

    for ( int x = 1; x < 10; ++x )
    {
        for ( int i = 0; i < 10; ++i )
        {
            std::cout << i << " " << cache[ x + i ] << std::endl;
        }
    }

    std::cout << "exectuted calls=" << calls << std::endl;

    return 0;
}
