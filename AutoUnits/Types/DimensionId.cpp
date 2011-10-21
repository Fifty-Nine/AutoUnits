//==============================================================================
/// \file AutoUnits/Types/DimensionId.cpp
/// 
/// Source file for the DimensionId type.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <QSet>

#include "Types/DimensionId.h"

namespace AutoUnits
{

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
    QSet<QString> all_keys = uniqueKeys().toSet();
    all_keys.unite( rhs.uniqueKeys().toSet() );

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

} // namespace AutoUnits
