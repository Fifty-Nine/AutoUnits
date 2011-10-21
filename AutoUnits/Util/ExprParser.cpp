//==============================================================================
/// \file AutoUnits/Util/ExprParser.cpp
/// 
/// Source file for the expression parser class.
///
//  Copyright 2011 AgLeader Technology, Inc.
//==============================================================================

#include "Util/ExprParser.h"

namespace AutoUnits
{

namespace Util
{

void ParseExpr( ParserState& state )
{
    while ( !state.tokens.isEmpty() )
    {
        Token *tok_p( state.tokens.dequeue() );
        tok_p->Process( state );
    }

    while ( !state.opstack.isEmpty() )
    {
        std::auto_ptr<Operator> op_p( state.opstack.pop() );

        if ( op_p->IsLParen() )
        {
            throw Error( "Unmatched '('." );
        }
        op_p->Apply( state );
    }
}

} // namespace Util

} // namespace AutoUnits

