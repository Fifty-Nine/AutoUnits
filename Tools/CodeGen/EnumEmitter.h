#ifndef CODEGEN_ENUM_EMITTER_H
#define CODEGEN_ENUM_EMITTER_H
//==============================================================================
/// \file CodeGen/EnumEmitter.h
/// 
/// Code to generate an enumeration of units from a unit system.
///
//==============================================================================

#include <QTextStream>

namespace AutoUnits { class UnitSystem; }

namespace CodeGen
{

struct EnumEmitter
{
    EnumEmitter( const UnitSystem *system_p ) : 
        m_system_p( system )
    {
    }

};


};

#endif // CODEGEN_ENUM_EMITTER_H
