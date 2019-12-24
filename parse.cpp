#include "parse.h"
#include "gettoken.h"
#include "tree.h"

static int error_count = 0;

void
ParseError(int line, string msg)
{
	++error_count;
	cout << line+1 << ": " << msg << endl;
}

// Prog is an Slist
ParseTree *Prog(istream& in, int& line)
{
	ParseTree *sl = Slist(in, line);
    
	if( GetToken::Get(in, line) != DONE )
		ParseError(line, "Unrecognized statement");

	if( sl == 0 )
		ParseError(line, "No statements in program");

	if( error_count )
		return 0;

	return sl;
}

// Ssep { Slist } | Stmt Ssep { Slist }
ParseTree *Slist(istream& in, int& line) {
	ParseTree *s = Stmt(in, line);
	if( s == 0 )
		return 0;
    
	Token t;
	if( (t = GetToken::Get(in, line)) != SC && t != NL ) {
		ParseError(line, "Missing statement separator");
		return 0;
	}

    return new StmtList(s, Slist(in,line));
}

ParseTree *Stmt(istream& in, int& line) {
	ParseTree *s = 0;
    
	Token t = GetToken::Get(in, line);
	switch( t.GetTokenType() ) {
	case IF:
		s = IfStmt(in, line);
		break;

	case PRINT:
		s = PrintStmt(in, line);
		break;

	case SET:
		s = SetStmt(in, line);
		break;

	case LOOP:
		s = LoopStmt(in, line);
		break;

	case ERR:
		ParseError(line, "Invalid token");
		break;

	case DONE:
		break;
  
	case NL:
	case SC:
		// an empty statement; get the next one
		s = Stmt(in, line);
		break;

	default:
		GetToken::PushBack(t);
		break;
	}

	return s;
}

ParseTree *IfStmt(istream& in, int& line) {
	ParseTree *st = Expr(in, line);
	if(st==0)
    {   ParseError(line, "Missing expression after if");
		return 0;
    }
	Token t = GetToken::Get(in, line);
	
	if( (t.GetTokenType()) != BEGIN ) {
		ParseError(line, "Missing BEGIN after expression");
		return 0;
	}
	ParseTree *sll = Slist(in, line);
	if(sll==0){
         ParseError(line, "Missing statement list after BEGIN");
		return 0;
    }
    Token t2 = GetToken::Get(in, line);
	if( (t2.GetTokenType()) != END ) {
		ParseError(line, "Missing END Statement");
		return 0;
	}
	return new IfStatement(line, st, sll);
}

ParseTree *LoopStmt(istream& in, int& line) {
	ParseTree *ls = Expr(in, line);
	if(ls==0)
		return 0;
    
	Token t = GetToken::Get(in, line);
	if( (t.GetTokenType()) != BEGIN ) {
		ParseError(line, "Missing BEGIN after expression");
		return 0;
	}
	ParseTree *lss = Slist(in, line);
	if(lss==0){
        ParseError(line, "Missing statement list after BEGIN");
		return 0;
    }
    Token t2 = GetToken::Get(in, line);
	if( (t2.GetTokenType()) != END ) {
		ParseError(line, "Missing END Statement");
		return 0;
	}
	return new LoopStatement(line, lss, ls);
	
}

ParseTree *SetStmt(istream& in, int& line) {
	Token t = GetToken::Get(in, line);
    if(t.GetTokenType()!=ID)
    {
        ParseError(line, "Missing Identifier after set");
        return 0;
    }
    ParseTree *sts = Expr(in, line);
	if(sts == 0)
		return 0;
	
	SetStatement *lala = new SetStatement(line, t.GetLexeme(), sts);
    return lala;
}

ParseTree *PrintStmt(istream& in, int& line) {
	ParseTree *e = Expr(in, line);
	if(e == 0){
		ParseError(line, "Missing expression after print");
		return 0;
	}
	return new PrintStatement(line, e);
}

ParseTree *Expr(istream& in, int& line) {
	ParseTree *t1 = Prod(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Token t = GetToken::Get(in, line);

		if( t != PLUS && t != MINUS ) {
	 		GetToken::PushBack(t);
			return t1;
		}

		ParseTree *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == PLUS )
			t1 = new Addition(t.GetLinenum(), t1, t2);
		else
			t1 = new Subtraction(t.GetLinenum(), t1, t2);
	}
}

ParseTree *Prod(istream& in, int& line) {
	ParseTree *t1 = Primary(in, line);
	if ( t1 == 0) {
		return 0;
	}
	
	while (true){
		Token t = GetToken::Get(in, line);
		
		if (t != STAR && t != SLASH) {
			GetToken::PushBack(t);
			return t1;			
		}
		
		ParseTree *t2 = Primary(in, line);
		if ( t2 ==0) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}
		
		if ( t == STAR )
			t1 = new Multiplication(t.GetLinenum(), t1, t2);
		else
			t1 = new Division(t.GetLinenum(), t1, t2);
		
	}
}

ParseTree *Primary(istream& in, int& line) {
	
	Token t = GetToken::Get(in, line);
		
	if(t.GetTokenType()==ID){
		return new Ident(t);
		
	}
	else if(t.GetTokenType()==ICONST){
		return new IConst(t);
	}
	else if(t.GetTokenType()==SCONST){
		return new SConst(t);
	}
	else if(t.GetTokenType()!=LPAREN){
		
        ParseError(line, "Primary expected");
		return 0;
    }
		
    
    ParseTree *gy = Expr(in, line);
        
    if(gy==0){
		ParseError(line, "Primary expected");
		return 0;
		}
    
    t = GetToken::Get(in, line);
	if(t.GetTokenType()!= RPAREN){
      
	ParseError(line, "Missing ) after expression");
	return 0;
    }
		
    return gy;
}
	
