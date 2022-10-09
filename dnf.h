#ifndef DNF_H
#define DNF_H

#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <cmath>
#include "impl.h"


class DNF
{
private:
    std::vector < std::shared_ptr <Impl> > m_data;
    std::list < std::shared_ptr <Impl> > m_ddnf;
    std::list < std::shared_ptr <Impl> > m_mdnf;

    uint16_t m_numPatch;
    int m_base;

    void makeDDNF();
    void absorbDDNF();

public:
    DNF( std::string strData );
    ~DNF();

    void minimize();
    void print( std::ostream & stream );

    std::list < std::shared_ptr <Impl> > getMDNF();

    friend std::string decToBinStr( int a, int len );
};

//const std::string VARIABLES = "xyzuw";

#endif // DNF_H
