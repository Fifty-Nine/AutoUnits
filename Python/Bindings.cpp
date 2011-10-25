//==============================================================================
/// \file AutoUnits/Python/Bindings.cpp
/// 
/// Source file for the python bindings for AutoUnits.
///
//==============================================================================

#include <boost/python.hpp>
#include "Dimension.h"
#include "Unit.h"
#include "UnitSystem.h"
#include "Types/Conversion.h"
#include "DefinitionParser.h"

using namespace boost::python;
using namespace AutoUnits;

class QStringToPyString
{
public:
    static PyObject* convert( const QString& s )
    {
        return object(s.toStdString()).ptr();
    }
};

class PyStringToQString
{
public:
    PyStringToQString()
    {
        converter::registry::push_back( 
            &convertible, &construct, type_id<QString>() );
    }

    static void* convertible( PyObject* obj_p )
    {
        return PyString_Check( obj_p ) ? obj_p : 0;
    }

    static void construct( PyObject *obj_p, converter::rvalue_from_python_stage1_data* data_p )
    {
        const char *value_p = PyString_AsString( obj_p );
        if ( !value_p ) 
        { 
            throw_error_already_set(); 
        }

        void *storage = ((converter::rvalue_from_python_storage<QString>*)
              data_p)->storage.bytes;

        new (storage) QString( value_p );
        data_p->convertible = storage;
    }
};

template<class V>
struct ListWrapper
{
    typedef QList<const V*> T;

    static void IndexError() { PyErr_SetString( PyExc_IndexError, "Index out of range"); }
    static int Index( const T& self, int i )
    {
        if ( i < 0 )
        {
            i += self.size();
        }

        if ( i < 0 || i >= self.size() )
        {
            IndexError();
        }

        return i;
    }

    static const V& get( const T& self, int i )
    {
        return *self[Index( self, i )];
    }

    static void set( T& self, int i, const V& v )
    {
        self[Index( self, i )] = v;
    }

    static void del( T& self, int i )
    {
        self.erase( self.begin() + Index( self, i ) );
    }

    static void add( T& self, const V& v )
    {
        self.push_back( &v );
    }

};

const UnitSystem *DefParserTakeResultWrapper( DefinitionParser *self_p )
{
    return self_p->TakeResult().release();
}

BOOST_PYTHON_MODULE(AutoUnits)
{
    to_python_converter<QString,QStringToPyString>();
    PyStringToQString();

    const Conversion* (Unit::*UnitToBase)() const = &Unit::ToBase;
    const Conversion* (Unit::*UnitFromBase)() const = &Unit::FromBase;
    const Dimension* (Unit::*UnitGetDimension)() const = &Unit::GetDimension;
    class_<Unit, boost::noncopyable>( "Unit", no_init )
        .def( "Name", &Unit::Name )
        .def( "GetDimension", UnitGetDimension, return_internal_reference<1>() )
        .def( "ToBase", UnitToBase, return_internal_reference<1>() )
        .def( "FromBase", UnitFromBase, return_internal_reference<1>() )
        .def( "IsBase", &Unit::IsBase )
    ;

    const Unit *(Dimension::*DimGetBaseUnit)() const = &Dimension::GetBaseUnit;
    QList<const Unit*> (Dimension::*DimUnits)() const = &Dimension::Units;
    class_<Dimension, boost::noncopyable>( "Dimension", no_init )
        .def( "Name", &Dimension::Name )
        .def( "Id", &Dimension::Id )
        .def( "IsDerived", &Dimension::IsDerived )
        .def( "GetBaseUnit", DimGetBaseUnit, return_internal_reference<1>() )
        .def( "Units", DimUnits, with_custodian_and_ward_postcall<1,0>() )
    ;

    QList<const Dimension*> (UnitSystem::*UsDimensions)() const = 
        &UnitSystem::Dimensions;
    QList<const Unit*> (UnitSystem::*UsUnits)() const = &UnitSystem::Units;
    const Dimension* 
        (UnitSystem::*UsGetDimensionById)(const DimensionId&) const = 
        &UnitSystem::GetDimension;
    const Dimension* (UnitSystem::*UsGetDimensionByName)(const QString&) const =
        &UnitSystem::GetDimension;
    const Unit * (UnitSystem::*UsGetUnit)(const QString&) const = 
        &UnitSystem::GetUnit;
    class_<UnitSystem, boost::noncopyable>( "UnitSystem", no_init )
        .def( "Dimensions", UsDimensions, 
            with_custodian_and_ward_postcall<1,0>() )
        .def( "Units", UsUnits, with_custodian_and_ward_postcall<1,0>() )
        .def( "GetDimension", UsGetDimensionById, 
            return_internal_reference<1>() )
        .def( "GetDimension", UsGetDimensionByName, 
            return_internal_reference<1>() )
        .def( "GetUnit", UsGetUnit, return_internal_reference<1>() )
    ;

    class_<DefinitionParser, boost::noncopyable>( "DefinitionParser",
        init<const QString&>() )
        .def( "TakeResult", &DefParserTakeResultWrapper, 
            return_value_policy<manage_new_object>() )
    ;

    typedef QList<const Dimension*> DimensionList;
    class_<DimensionList>("DimensionList")
        .def( "__len__", &DimensionList::size )
        .def("__getitem__", &ListWrapper<Dimension>::get,
            return_value_policy<reference_existing_object>() )
    ;

}



