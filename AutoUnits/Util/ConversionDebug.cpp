//==============================================================================
/// \file AutoUnits/Util/ConversionDebug.cpp
/// 
/// Source file for the conversion debug utilities.
///
//==============================================================================

#include <QDebug>

#include "Types/Conversion.h"
#include "Util/ConversionDebug.h"

namespace AutoUnits {
namespace Conversions {
namespace {

//==============================================================================
/// The visitor we use to print conversions.
/// 
class DebugVisitor : public ConstVisitor
{
public:
    DebugVisitor( QDebug& debug, const Conversion& node ) : 
        m_debug( debug )
    {
        node.Accept( *this );
    }

    //==========================================================================
    /// Visit a constant node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const Constant& node ) 
    {
        m_debug << QString::number( node.Value() );
    }
    
    //==========================================================================
    /// Visit a value node.
    /// 
    virtual void Visit( const Value& )
    {
        m_debug << "value";
    }

    //==========================================================================
    /// Visit an add node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const AddOp& node )
    {
        m_debug << '(';
        node.GetLeft()->Accept( *this );
        m_debug << '+';
        node.GetRight()->Accept( *this );
        m_debug << ')';
    }

    //==========================================================================
    /// Visit a sub node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const SubOp& node )
    {
        m_debug << '(';
        node.GetLeft()->Accept( *this );
        m_debug << '-';
        node.GetRight()->Accept( *this );
        m_debug << ')';
    }

    //==========================================================================
    /// Visit a mutliply node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const MultOp& node )
    {
        m_debug << '(';
        node.GetLeft()->Accept( *this );
        m_debug << '*';
        node.GetRight()->Accept( *this );
        m_debug << ')';
    }

    //==========================================================================
    /// Visit a div node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const DivOp& node )
    {
        m_debug << '(';
        node.GetLeft()->Accept( *this );
        m_debug << '/';
        node.GetRight()->Accept( *this );
        m_debug << ')';
    }

private:
    /// Not implemented.
    DebugVisitor();
    /// Not implemented.
    DebugVisitor( const DebugVisitor& );
    /// Not implemented.
    DebugVisitor& operator=( const DebugVisitor& );

    /// The output stream.
    QDebug& m_debug;
};
} // namespace

QDebug operator<<( QDebug out, const Conversion& conv )
{
    DebugVisitor( out, conv );
    return out;
}

} // namespace Conversions
} // namespace AutoUnits
