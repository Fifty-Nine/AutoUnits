//==============================================================================
/// \file AutoUnits/Types/DimensionId.cpp
/// 
/// Source file for the DimensionId type.
///
//==============================================================================

#include <QSet>

#include "Types/DimensionId.h"


namespace AutoUnits
{

namespace
{

QSet<QString> AllKeys( const DimensionId& lhs, const DimensionId& rhs )
{
    return lhs.uniqueKeys().toSet().unite( rhs.uniqueKeys().toSet() );
}

int add( int a, int b ) { return a + b; }
int sub( int a, int b ) { return a - b; }

DimensionId ApplyToAll( const DimensionId& lhs, const DimensionId& rhs, 
    int (*operation)(int,int) )
{
    QSet<QString> all_keys = AllKeys( lhs, rhs );

    DimensionId result;

    for ( QSet<QString>::const_iterator it = all_keys.begin(); 
        it != all_keys.end(); ++it )
    {
        result[*it] = operation( lhs[*it], rhs[*it] );
    }
    return result;
}

} // namespace 

//==============================================================================
/// Construct a fundamental dimension id.
/// 
/// \param [in] dim The dimension name.
/// 
DimensionId::DimensionId( const QString& dim )
{
    operator[](dim) = 1;
}

//==============================================================================
/// Equality test operator.
///
/// \param [in] rhs The right hand side.
/// 
/// \return True if the operands are equal.
/// 
bool DimensionId::operator==( const DimensionId& rhs ) const
{
    QSet<QString> all_keys = AllKeys( *this, rhs );

    for ( QSet<QString>::const_iterator it = all_keys.begin(); 
        it != all_keys.end(); ++it )
    {
        if ( operator[]( *it ) != rhs[*it] )
        {
            return false;
        }
    }

    return true;
}

//==============================================================================
/// Calculate the resulting dimension of multiplying two dimensions.
/// 
/// \param [in] rhs The right hand side.
/// 
/// \return The resulting dimension.
/// 
DimensionId DimensionId::operator*( const DimensionId& rhs ) const
{
    return ApplyToAll( *this, rhs, &add );
}

//==============================================================================
/// Calculate the resulting dimension of dividing two dimensions.
/// 
/// \param [in] rhs The right hand side.
/// 
/// \return The resulting dimension.
/// 
DimensionId DimensionId::operator/( const DimensionId& rhs ) const
{
    return ApplyToAll( *this, rhs, &sub );
}

//==============================================================================
/// Calculate the resulting dimension of raising a dimension to a power.
/// 
/// \param [in] exp The power.
/// 
/// \return The resulting dimension.
/// 
DimensionId DimensionId::operator^( int exp ) const
{
    DimensionId result = *this;
    for ( iterator it = result.begin(); it != result.end(); ++it )
    {
        *it *= exp;
    }
    return result;
}

} // namespace AutoUnits
