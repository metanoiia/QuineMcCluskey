#include "dnf.h"
#include <fstream>

int main()
{
    std::ifstream fin( "scale.txt" );

    if( !fin.is_open() )
        return -1;

    std::string inData;

    fin >> inData;
    fin.close();

    DNF dnf( inData );

    dnf.minimize();

    std::ofstream fout( "mdnf.txt", std::ios_base::trunc );

    if( !fout.is_open() )
        return -1;

    dnf.print( fout );
    fout.close();

    return 0;
}
