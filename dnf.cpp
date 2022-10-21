#include "dnf.h"
#include <iostream>

DNF::DNF( std::string strData )
{
    m_base = static_cast <int> ( std::ceil( log2( strData.size() ) ) );

    for ( size_t i = 0; i < strData.length(); i++ )
    {
        char sym = strData.at( i );

        if ( sym == '1' || sym == '-' )
        {
            m_data.push_back( std::make_shared <Impl> ( i ) );

            if( sym == '-' )
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

        for ( size_t i = 0; i < dataOld.size() - 1; i++ )
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

    std::vector < std::shared_ptr <Impl> > definedConstituents;
    std::vector < std::shared_ptr <Impl> > ddnfVec;

    std::copy(  m_ddnf.begin(), m_ddnf.end(), std::back_inserter( ddnfVec ) );

    std::copy_if( m_data.begin(), m_data.end(),
                 std::back_inserter( definedConstituents ),
                 [&] ( std::shared_ptr <Impl> impl ){ return !( impl->isUndefined() ); } );

    std::vector < std::vector < bool > > absorbedMatrix( definedConstituents.size(),
                                                         std::vector < bool > ( m_ddnf.size(), false ) ); //implication matrix

    for( size_t i = 0; i < definedConstituents.size(); i++ ) //matrix initializing loop
    {
        std::shared_ptr <Impl> cons = definedConstituents.at( i );
        uint32_t basicCnt = 0;

        for ( size_t j = 0; j < ddnfVec.size(); j++ )
        {
            std::shared_ptr <Impl> impl = ddnfVec.at( j );

            if( ( cons->getNum() & ~( impl->getP() ) ) == impl->getNum() )
            {
                absorbedMatrix.at( i ).at( j ) = true;
                basicCnt++;
            }
        }

        if( basicCnt == 1 )
        {
            std::vector < bool >::iterator absorbedIt = std::find( absorbedMatrix.at( i ).begin(),
                                                           absorbedMatrix.at( i ).end(),
                                                           true);

            int index = absorbedIt - absorbedMatrix.at( i ).begin();
            ddnfVec.at( index )->setBasic( true );
        }
        else if ( !basicCnt )
            return;
    }

    for( size_t i = 0; i < absorbedMatrix.size(); i++ ) // mdnf filling loop
    {
        std::vector < bool > column = absorbedMatrix.at( i );
        int basicNum = std::accumulate( column.begin(), column.end(), 0 );

        if( basicNum == 1 )
        {
            std::vector < bool >::iterator absorbedIt = std::find( absorbedMatrix.at( i ).begin(),
                                                           absorbedMatrix.at( i ).end(),
                                                           true);

            int index = absorbedIt - absorbedMatrix.at( i ).begin();

            if( std::find( m_mdnf.begin(), m_mdnf.end(), ddnfVec.at( index ) ) == m_mdnf.end() ) //if there is no more such implicant yet
                                m_mdnf.push_back( ddnfVec.at( index ) );
        }
        else
        {
            std::list < std::shared_ptr < Impl > > coveredImpl;
            std::list < std::shared_ptr < Impl > >::iterator basicIt;

            for( size_t j = 0; j < column.size(); j++ )
            {
                if( column.at( j ) ) //if implicant at j covered by i constituent
                    coveredImpl.push_back( ddnfVec.at( j ) );
            }

            basicIt = std::find_if( coveredImpl.begin(), coveredImpl.end(), []( std::shared_ptr < Impl > impl ){ return impl->isBasic(); } );

            if( basicIt != coveredImpl.end() ) //if there is no basic impl in this column
            {
                if( std::find( m_mdnf.begin(), m_mdnf.end(), *basicIt ) == m_mdnf.end() ) //if there is no more such implicant yet
                    m_mdnf.push_back( *basicIt );
            }
            else
            {
                std::vector < bool >::iterator firstCoveredIt = std::find( column.begin(),
                                                               column.end(),
                                                               true);

                int index = firstCoveredIt - column.begin();

                if( std::find( m_mdnf.begin(), m_mdnf.end(), ddnfVec.at( index ) ) == m_mdnf.end() ) //if there is no more such implicant yet
                    m_mdnf.push_back( ddnfVec.at( index ) );
            }

        }

    }

}

void DNF::minimize()
{
    makeDDNF();
    absorbDDNF();
}

std::string decToBinStr( int val, int len )
{
    std::string binary = "";
    int mask = 1;

    for( int i = 0; i < len; i++ )
    {
        if( ( mask & val ) >= 1 )
            binary = "1" + binary;
        else
            binary = "0" + binary;
        mask <<= 1;
    }

    return binary;
}

void DNF::print( std::ostream & stream )
{
    if( m_mdnf.empty() )
        return;

    for( std::shared_ptr <Impl> impl : m_mdnf )
    {
        std::string P_str = decToBinStr( impl->getP(), m_base );
        std::string Num_str = decToBinStr( impl->getNum(), m_base );

        std::string impl_str;
        impl_str.resize( m_base );

        std::transform( P_str.begin(), P_str.end(), impl_str.begin(),
                        []( char sym ){ return ( sym == '1' ) ? '-' : '\0'; } );

        std::string::reverse_iterator num_iter = Num_str.rbegin();

        for( std::string::reverse_iterator impl_iter = impl_str.rbegin(); impl_iter != impl_str.rend(); impl_iter++, num_iter++ )
        {
            if( *impl_iter != '-' )
                *impl_iter = *num_iter;
        }

        stream << impl_str << std::endl;
    }
}

std::list < std::shared_ptr <Impl> > DNF::getMDNF()
{
    return m_mdnf;
}

DNF::~DNF()
{

}
