//==============================================================================
/// \file AutoUnits/Util/Error.cpp
/// 
/// Source file for the AutoUnits::Util::Error class.
///
//==============================================================================

#include "Util/Error.h"

namespace AutoUnits
{

namespace Util
{

//==============================================================================
/// Constructor.
///
/// \param [in] desc The description.
/// 
Error::Error( const QString& desc ) : 
    m_desc( desc )
{
}

//==============================================================================
/// Convert the error into a description.
/// 
/// \return The description.
/// 
Error::operator QString() const
{
    return m_desc;
}

//==============================================================================
/// Throw the error.
/// 
/// \return Never returns.
///
void Error::Throw() 
{
    throw *this;
}

} // namespace Util

} // namespace AutoUnits

