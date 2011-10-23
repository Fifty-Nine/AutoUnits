#ifndef AUTO_UNITS_UTIL_ERROR_H
#define AUTO_UNITS_UTIL_ERROR_H
//==============================================================================
/// \file AutoUnits/Util/Error.h
/// 
/// Header file for the AutoUnits::Util::Error class.
///
//==============================================================================

#include <QString>

namespace AutoUnits
{

namespace Util
{

//==============================================================================
/// A generic exception interface for the AutoUnits library.
/// 
class ErrorInterface
{
public:
    /// Destructor.
    virtual ~ErrorInterface() { } 

    /// Convert the error into a user-[un]friendly description string.
    /// \return The description.
    virtual operator QString() const = 0;

    /// Override this in subclasses to ensure slicing does not occur when
    /// re-throwing an exception.
    virtual void Throw() = 0;
};

//==============================================================================
/// A simple concrete implementation of the error interface.
/// 
class Error : public ErrorInterface
{
public:
    Error( const QString& desc );
    virtual operator QString() const;
    virtual void Throw();

private:
    /// The error's description.
    QString m_desc;
};

} // namespace Util

} // namespace AutoUnits

#endif // AUTO_UNITS_UTIL_ERROR_H
