#include <iostream>
#include "dnf.h"
#include <vector>

int main()
{
    DNF  dnf(std::string("1111101-11-00001"));
    dnf.minimize();
    std::list < std::shared_ptr <Impl> > mdnf = dnf.getMDNF();
    //
    return 0;
}
