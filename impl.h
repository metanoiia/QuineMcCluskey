#ifndef IMPL_H
#define IMPL_H

#include <memory>

class Impl
{
private:
    unsigned int m_num;
    unsigned int m_ind;
    unsigned int m_P;

    bool m_Pw;
    bool m_isUndefined;

public:
    Impl(unsigned int num);
    ~Impl();

    unsigned int getNum();
    unsigned int getInd();
    unsigned int getP();
    bool getPw();

    bool isUndefined();

    void setPw( bool val );
    void setP( unsigned int val );
    void setUndefined( bool val );

    static std::shared_ptr <Impl> patch( std::shared_ptr <Impl> & first, std::shared_ptr <Impl> & second );
    static int count( unsigned int num );

    friend bool operator == ( std::shared_ptr <Impl> & left, std::shared_ptr <Impl> & right );
    friend bool operator != ( std::shared_ptr <Impl> & left, std::shared_ptr <Impl> & right );

    friend bool operator < ( std::shared_ptr <Impl> & left, std::shared_ptr <Impl> & right );
    friend bool operator > ( std::shared_ptr <Impl> & left, std::shared_ptr <Impl> & right );
};

#endif // IMPL_H
