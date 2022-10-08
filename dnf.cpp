#include "dnf.h"
#include <iostream>

DNF::DNF( std::string strData )
{
    for (size_t i = 0; i < strData.length(); i++)
    {
        char sym = strData.at( i );

        if ( sym == '1' || sym == '-' )
        {
            m_data.push_back( std::make_shared <Impl> ( i ) );

            if(sym == '-')
                m_data.back()->setUndefined( true );
        }
    }

    m_numPatch = 0;
}

void DNF::makeDDNF()
{
    if( m_data.empty() )
        return;

    std::vector < std::shared_ptr <Impl> > dataNew;
    std::vector < std::shared_ptr <Impl> > dataOld;

    std::copy( m_data.begin(), m_data.end(), std::back_inserter( dataOld  ) );

    while(1)
    {
        m_numPatch++;

        for (size_t i = 0; i < dataOld.size() - 1; i++ )
        {
            for ( size_t j = i + 1; j < dataOld.size(); j++ )
            {
                std::shared_ptr <Impl> newImpl = Impl::patch( dataOld.at( i ), dataOld.at( j ) );

                if ( newImpl != nullptr )
                    dataNew.push_back( newImpl );
            }
        }

        std::copy_if( dataOld.begin(), dataOld.end(),
                     std::back_inserter( m_ddnf ),
                     [&] ( std::shared_ptr <Impl> impl ){ return !( impl->getPw() ); } );

        if ( dataNew.empty() )
        {
            m_ddnf.sort();
            m_ddnf.unique();

            break;
        }
        else
            dataOld = std::move( dataNew );
    }
}

void DNF::absorbDDNF()
{
    if( m_data.empty() )
        return;

    std::list < std::shared_ptr <Impl> > definedConstituents;

    std::copy_if( m_data.begin(), m_data.end(),
                 std::back_inserter( definedConstituents ),
                 [&] ( std::shared_ptr <Impl> impl ){ return !( impl->isUndefined() ); } );

    for( std::shared_ptr <Impl> & cons : definedConstituents )
    {
        for( std::shared_ptr <Impl> & impl : m_ddnf )
        {
            if( ( cons->getNum() & ~( impl->getP() ) ) == impl->getNum() )
            {
                if( std::find( m_mdnf.begin(), m_mdnf.end(), impl ) == m_mdnf.end() )
                    m_mdnf.push_back( impl );

                break;
            }
        }
    }
}

void DNF::minimize()
{
    makeDDNF();
    absorbDDNF();
}

std::list < std::shared_ptr <Impl> > DNF::getMDNF()
{
    return m_mdnf;
}

DNF::~DNF()
{

}
