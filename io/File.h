/**
*   usage : 1.  file operations.
*
*   note :  1.
*/

#ifndef FILE_H
#define FILE_H


#include <iostream>
#include <fstream>
#include <string>

#include "../algorithm/SetOperation.h"


namespace szx
{
    class File
    {
    public:
        // if (isComplement == false), set the input position indicator to 
        //     the last appearance of any character in charSet.
        // else, set it to the last appearance of any character out of charSet.
        // that is, locate_last_of<true> has the meaning of locate_last_not_of.
        // the search will start at (startDir + startOffset)
        template <const bool isComplement = false, typename CharSet >
        static std::istream& locate_last_of( std::istream& is, const CharSet &charSet,
            int startOffset = 0, std::ios_base::seekdir startDir = std::ios_base::cur )
        {
            is.seekg( startOffset, startDir );

            char c;
            do {
                is.seekg( -1, std::ios_base::cur );
                if (is.tellg() <= 0) {
                    is.seekg( 0 );
                    break;
                }
                c = static_cast<char>(is.peek());
            } while (contain( charSet, c ) == isComplement);

            return is;
        }

        static std::string getLastNoneEmptyLine( const std::string &filename );

    private:

    };
}


#endif