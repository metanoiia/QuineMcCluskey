#include "impl.h"

Impl::Impl( unsigned int num )
    : m_num( num ),
      m_ind( count( m_num ) ),
      m_P( 0 ),
      m_Pw( false ),
      m_isUndefined( false ),
      m_isBasic( false ),
      m_covers( false )
{

}

bool Impl::isBasic()
{
    return m_isBasic;
}

void Impl::setBasic( bool val )
{
    m_isBasic = val;
}

int Impl::count( unsigned int num )
{
    unsigned int count = 0;

    for ( ; num; count++ )
        num &= ( num - 1 );

    return count;
}

unsigned int Impl::getNum()
{
    return m_num;
}

unsigned int Impl::getInd()
{
    return m_ind;
}

unsigned int Impl::getP()
{
    return m_P;
}

bool Impl::getPw()
{
    return m_Pw;
}

bool Impl::isUndefined()
{
    return m_isUndefined;
}

bool Impl::covers( )
{
    return m_covers;
}

void Impl::setCovers(bool val)
{
    m_covers = val;
}

void Impl::setPw( bool val )
{
    m_Pw = val;
}

void Impl::setP( unsigned int val )
{
    m_P = val;
}
void Impl::setUndefined( bool val )
{
    m_isUndefined = val;
}

std::shared_ptr <Impl> Impl::patch( std::shared_ptr <Impl> & first, std::shared_ptr <Impl> & second )
{
    if ( ( second->getP() == first->getP() ) && ( second->getNum() > first->getNum() )
      && ( second->getInd() - first->getInd() == 1 ) && ( count( second->getNum() - first->getNum() ) == 1 ) )
    {
            std::shared_ptr <Impl> newImpl = std::make_shared <Impl>( first->getNum() );
            newImpl->setP( first->getP() + second->getNum() - first->getNum() );

            first->setPw( true );
            second->setPw( true );

            return newImpl;
    }
    else
        return nullptr;
}

bool operator == ( std::shared_ptr <Impl> & left, std::shared_ptr <Impl> & right )
{
    return ( ( left->m_P == right->m_P )
          && ( left->m_Pw == right->m_Pw )
          && ( left->m_ind == right->m_ind )
          && ( left->m_num == right->m_num ) );


}
bool operator != ( std::shared_ptr <Impl> & left, std::shared_ptr <Impl> & right )
{
    return !(left == right);
}

bool operator > ( std::shared_ptr <Impl> & left, std::shared_ptr <Impl> & right )
{
    return ( ( left->m_P > right->m_P )
          && ( left->m_Pw > right->m_Pw )
          && ( left->m_ind > right->m_ind )
          && ( left->m_num > right->m_num ) );
}

bool operator < ( std::shared_ptr <Impl> & left, std::shared_ptr <Impl> & right )
{
    return ( ( left->m_P < right->m_P )
          && ( left->m_Pw <= right->m_Pw )
          && ( left->m_ind <= right->m_ind )
          && ( left->m_num <= right->m_num) );
}

Impl::~Impl()
{

}
