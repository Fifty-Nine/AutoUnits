//==============================================================================
/// \file AutoUnits/Unit.h
/// 
/// Header file for the Unit class.
///
//==============================================================================

#include <QString>

#include "Types/Conversion.h"

namespace AutoUnits
{

class Dimension;

//==============================================================================
/// An object that specifies a base unit in a unit system.
/// 
class Unit
{
public:
    //==========================================================================
    /// Immutable interface.
    //==========================================================================
    QString Name() const;
    const Dimension *GetDimension() const;
    const Conversion *ToBase() const;
    const Conversion *FromBase() const;
    bool IsBase() const;

    //==========================================================================
    /// Mutable interface.
    //==========================================================================
    Dimension *GetDimension();
    Conversion *ToBase();
    Conversion *FromBase();
    void SetToBase( std::auto_ptr<Conversion> conv_p );
    void SetFromBase( std::auto_ptr<Conversion> conv_p );

private:
    Unit( const QString& name, Dimension *dimension_p );
    Unit( const QString& name, Dimension *dimension_p, 
        std::auto_ptr<Conversion> to_base, 
        std::auto_ptr<Conversion> from_base );

    friend class UnitSystem;

    /// The unit's name.
    QString m_name;
    /// The unit's dimension.
    Dimension *m_dim_p;
    /// The to-base conversion.
    std::auto_ptr<Conversion> m_to_base_p;
    /// The from-base conversion.
    std::auto_ptr<Conversion> m_from_base_p;
};

} // namespace AutoUnits

