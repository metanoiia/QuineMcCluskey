#ifndef DNF_H
#define DNF_H

#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include "impl.h"


class DNF
{
private:
    std::vector < std::shared_ptr <Impl> > m_data;
    std::list < std::shared_ptr <Impl> > m_ddnf;
    std::list < std::shared_ptr <Impl> > m_mdnf;

    uint16_t m_numPatch;

    void makeDDNF();
    void absorbDDNF();

public:
    DNF(std::string strData);
    ~DNF();

    void minimize();


    std::list < std::shared_ptr <Impl> > getMDNF();
//    void print(std::ostream & stream);


};

//const std::string VARIABLES = "xyzuw";

#endif // DNF_H
