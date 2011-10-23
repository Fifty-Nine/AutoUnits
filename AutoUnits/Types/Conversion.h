#ifndef AUTO_UNITS_TYPES_CONVERSION_H
#define AUTO_UNITS_TYPES_CONVERSION_H
//==============================================================================
/// \file AutoUnits/Types/Conversion.h
/// 
/// Header file for the conversion type.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include <memory>

namespace AutoUnits
{

namespace Conversions 
{
class Visitor;
class ConstVisitor;

//==============================================================================
/// A type representing a conversion from one unit to another.
/// 
class Conversion
{
public:
    Conversion();
    virtual ~Conversion();
    
    //==========================================================================
    /// Accept a visitor to the node.
    /// 
    /// \param [in] visitor The visitor.
    /// 
    virtual void Accept( Visitor& visitor ) = 0;

    //==========================================================================
    /// Accept a visitor to the node.
    ///
    /// \param [in] visitor The visitor.
    /// 
    virtual void Accept( ConstVisitor& visitor ) const = 0;

    //==========================================================================
    /// Evaluate the conversion for the given value.
    /// 
    /// \return The converted value.
    /// 
    virtual double Eval( double value ) const = 0;

    //==========================================================================
    /// Test whether the node is a constant.
    /// 
    /// \return True if the node is a constant.
    /// 
    virtual bool IsConstant() const { return false; }

private:
    /// Not implemented.
    Conversion( const Conversion& );
    /// Not implemented.
    Conversion& operator=( const Conversion& );
};

class Constant;
class Value;
class AddOp;
class SubOp;
class MultOp;
class DivOp;

//==============================================================================
/// A visitor for conversions.
/// 
class Visitor
{
public:
    //==========================================================================
    /// Visit a constant node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( Constant& node ) = 0;
    
    //==========================================================================
    /// Visit a value node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( Value& node ) = 0;

    //==========================================================================
    /// Visit an add node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( AddOp& node ) = 0;

    //==========================================================================
    /// Visit a sub node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( SubOp& node ) = 0;

    //==========================================================================
    /// Visit a mutliply node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( MultOp& node ) = 0;

    //==========================================================================
    /// Visit a div node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( DivOp& node ) = 0;
};

//==============================================================================
/// A visitor for const conversions.
/// 
class ConstVisitor
{
public:
    //==========================================================================
    /// Visit a constant node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const Constant& node ) = 0;
    
    //==========================================================================
    /// Visit a value node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const Value& node ) = 0;

    //==========================================================================
    /// Visit an add node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const AddOp& node ) = 0;

    //==========================================================================
    /// Visit a sub node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const SubOp& node ) = 0;

    //==========================================================================
    /// Visit a mutliply node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const MultOp& node ) = 0;

    //==========================================================================
    /// Visit a div node.
    /// 
    /// \param [in] node The node to visit.
    /// 
    virtual void Visit( const DivOp& node ) = 0;
};

namespace Private
{

//==========================================================================
/// The base class for all concrete conversions. 
/// 
/// \tparam C The derived class (CRTP).
///
template<class C>
class ImplementConversion : public Conversion
{
public:
    //======================================================================
    /// Accept the visitor.
    /// 
    /// \param [in] visitor The visitor to accept.
    /// 
    virtual void Accept( Visitor& visitor ) 
    {
        visitor.Visit( *(C*)this ); 
    }

    //======================================================================
    /// Accept the visitor.
    /// 
    /// \param [in] visitor The visitor to accept.
    /// 
    virtual void Accept( ConstVisitor& visitor ) const
    { 
        visitor.Visit( *(const C*)this ); 
    }
};

//==========================================================================
/// The base class for binary operators.
/// 
/// \tparam C The derived class.
/// 
template<class C>
class BinOp : public ImplementConversion<C>
{
public:
    //==========================================================================
    /// Constructor.
    /// 
    /// \param [in] lhs_p The left-hand side.
    /// \param [in] rhs_p The right-hand side. 
    ///
    BinOp( std::auto_ptr<Conversion> lhs_p, std::auto_ptr<Conversion> rhs_p ) : 
        m_lhs_p( lhs_p ), m_rhs_p( rhs_p )
    {
    }
    
    //==========================================================================
    /// Get the left-hand side.
    /// 
    Conversion *GetLeft()
    {
        return m_lhs_p.get();
    }

    //==========================================================================
    /// Get the left-hand side.
    /// 
    const Conversion *GetLeft() const
    {
        return m_lhs_p.get();
    }

    //==========================================================================
    /// Get the right-hand side.
    /// 
    Conversion *GetRight()
    {
        return m_rhs_p.get();
    }

    //==========================================================================
    /// Get the right-hand side.
    /// 
    const Conversion *GetRight() const
    {
        return m_rhs_p.get();
    }

protected:
    /// The left-hand side.
    std::auto_ptr<Conversion> m_lhs_p;

    /// The right-hand side.
    std::auto_ptr<Conversion> m_rhs_p;
};

}

//==============================================================================
/// A constant value.
/// 
class Constant : public Private::ImplementConversion<Constant>
{
public:
    Constant( double value );
    double Value() const; 
    void SetValue( double value ); 
    virtual double Eval( double value ) const;
    virtual bool IsConstant() const;

private:
    /// The constant's value.
    double m_value;
};

//==============================================================================
/// A value placeholder.
/// 
class Value : public Private::ImplementConversion<Value> 
{ 
public:
    virtual double Eval( double value ) const;
}; 

//==============================================================================
/// A '+' operation node.
/// 
class AddOp : public Private::BinOp<AddOp>
{
public:
    AddOp( std::auto_ptr<Conversion> lhs_p, std::auto_ptr<Conversion> rhs_p );
    virtual double Eval( double value ) const;
};

//==============================================================================
/// The '-' binary operation node.
/// 
class SubOp : public Private::BinOp<SubOp>
{
public:
    SubOp( std::auto_ptr<Conversion> lhs_p, std::auto_ptr<Conversion> rhs_p );
    virtual double Eval( double value ) const;
};

//==============================================================================
/// The '*' binary operation node.
/// 
class MultOp : public Private::BinOp<MultOp>
{
public:
    MultOp( std::auto_ptr<Conversion> lhs_p, std::auto_ptr<Conversion> rhs_p );
    virtual double Eval( double value ) const;
};

//==============================================================================
/// The '*' binary operation node.
/// 
class DivOp : public Private::BinOp<DivOp>
{
public:
    DivOp( std::auto_ptr<Conversion> lhs_p, std::auto_ptr<Conversion> rhs_p );
    virtual double Eval( double value ) const;
};

} // namespace Conversions

using Conversions::Conversion;

} // namespace AutoUnits

#endif // AUTO_UNITS_TYPES_CONVERSION_H
