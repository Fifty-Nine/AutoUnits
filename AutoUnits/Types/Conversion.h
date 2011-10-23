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

    typedef std::auto_ptr<Conversion> AutoPtr;
    
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
    /// Call operator for conversions. Lets conversion objects be used as a 
    /// function object.
    /// 
    /// \param [in] value The value to convert.
    ///
    /// \return The converted value.
    ///
    inline double operator()( double value ) const { return Eval( value ); }

    //==========================================================================
    /// Compose the conversion with the given conversion. 
    /// 
    /// For some compositions f, g: 
    /// f->Compose( g )->Eval( x ) == f->Eval( g->Eval( x ) )
    /// 
    /// \param [in] other The conversion to compose with.
    /// 
    /// \return The composed conversion.
    /// 
    virtual AutoPtr Compose( const Conversion& other ) const = 0;

    //==========================================================================
    /// Clone the conversion.
    /// 
    virtual AutoPtr Clone() const = 0;

    //==========================================================================
    /// Test whether the node is a constant.
    /// 
    /// \return True if the node is a constant.
    /// 
    virtual bool IsConstant() const { return false; }

    static AutoPtr ScaleFactor( double factor );
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

    //==========================================================================
    /// Clone the conversion tree.
    /// 
    /// \return The cloned conversion.
    /// 
    AutoPtr Clone() const
    {
        return AutoPtr( new C( *(const C*)this ) );
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
    typedef typename ImplementConversion<C>::AutoPtr AutoPtr;

    //==========================================================================
    /// Constructor.
    /// 
    /// \param [in] lhs_p The left-hand side.
    /// \param [in] rhs_p The right-hand side. 
    ///
    BinOp( AutoPtr lhs_p, AutoPtr rhs_p ) : 
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

    //==========================================================================
    /// Check whether the conversion has a constant value.
    /// 
    /// \return True if the conversion is constant.
    /// 
    virtual bool IsConstant() const 
    {
        return m_lhs_p->IsConstant() && m_rhs_p->IsConstant();
    }

protected:
    //==========================================================================
    /// Copy constructor.
    /// 
    BinOp( const BinOp& other ) : 
        m_lhs_p( other.m_lhs_p->Clone() ), 
        m_rhs_p( other.m_rhs_p->Clone() )
    {
    }

    /// The left-hand side.
    AutoPtr m_lhs_p;

    /// The right-hand side.
    AutoPtr m_rhs_p;
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
    virtual AutoPtr Compose( const Conversion& other ) const;
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
    virtual AutoPtr Compose( const Conversion& other ) const;
}; 

//==============================================================================
/// A '+' operation node.
/// 
class AddOp : public Private::BinOp<AddOp>
{
public:
    AddOp( AutoPtr lhs_p, AutoPtr rhs_p );
    virtual double Eval( double value ) const;
    virtual AutoPtr Compose( const Conversion& other ) const;
};

//==============================================================================
/// The '-' binary operation node.
/// 
class SubOp : public Private::BinOp<SubOp>
{
public:
    SubOp( AutoPtr lhs_p, AutoPtr rhs_p );
    virtual double Eval( double value ) const;
    virtual AutoPtr Compose( const Conversion& other ) const;
};

//==============================================================================
/// The '*' binary operation node.
/// 
class MultOp : public Private::BinOp<MultOp>
{
public:
    MultOp( AutoPtr lhs_p, AutoPtr rhs_p );
    virtual double Eval( double value ) const;
    virtual AutoPtr Compose( const Conversion& other ) const;
};

//==============================================================================
/// The '*' binary operation node.
/// 
class DivOp : public Private::BinOp<DivOp>
{
public:
    DivOp( AutoPtr lhs_p, AutoPtr rhs_p );
    virtual double Eval( double value ) const;
    virtual AutoPtr Compose( const Conversion& other ) const;
};

Conversion::AutoPtr Compose( const Conversion& f, const Conversion& g );
Conversion::AutoPtr Compose( 
    const Conversion::AutoPtr& f, const Conversion::AutoPtr& g );

} // namespace Conversions

using Conversions::Conversion;

} // namespace AutoUnits

#endif // AUTO_UNITS_TYPES_CONVERSION_H
