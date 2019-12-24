/*
 * parsetree.h
 */

#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <map>
#include <string>
#include "value.h"
//#include "parse.h"
#include "gettoken.h" 
using std::vector;
using std::map;
using std::string;
using std::cout;
using std::endl;

// NodeType represents all possible types
enum NodeType { ERRTYPE, INTTYPE, STRTYPE };

// a "forward declaration" for a class to hold values
class Value;

static map<string, Value>symbols;

class ParseTree {
	int		linenum;
	ParseTree	*left;
	ParseTree	*right;

public:
	ParseTree(int linenum, ParseTree *l = 0, ParseTree *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~ParseTree() {
		delete left;
		delete right;
	}

	int GetLinenum() const { return linenum; }
	ParseTree *GetLeft() const { return left; }
	ParseTree *GetRight() const { return right; }

	int NodeCount() const {
		int nc = 0;
		if( left ) nc += left->NodeCount();
		if( right ) nc += right->NodeCount();
		return nc + 1;
	}
    
    int StrCount() const {
        int sc = 0;
        if(left) sc += left->StrCount();
        if(right) sc += right->StrCount();
        if(IsString())
            sc++;
        return sc;
    }
    int OperatorCount() const {
        int count = 0;
        if(left) count += left->OperatorCount();
        if(right) count += right->OperatorCount();
        if(OP())
            count++;
        return count;
    }
    int MaxDepth() const{
        int lh=0;
        int rh=0;
        if(left) lh += left->MaxDepth();
        if(right) rh += right->MaxDepth();
        if(rh>lh)
        {
            return rh+1;
        }
        else
        {
            return lh+1;
        }   
    }
    int lvCount() const{ 
        int count = 0;
		if( left ) count += left->lvCount();
		if( right ) count += right->lvCount();
        if(left == 0 && right == 0)
            count++;
        return count ;
	}
   
    virtual Value Eval()const=0;
	virtual bool IsString() const { return false; }
    virtual bool OP() const { return false; }
    
};

class StmtList : public ParseTree {

public:
	StmtList(ParseTree *l, ParseTree *r) : ParseTree(0, l, r) {}
    
    Value Eval() const{ 
        Value val = GetLeft()->Eval();
        if(GetRight() ){
            GetRight()->Eval();
        }
       return Value(val);
    }

};

class IfStatement : public ParseTree {
public:
	IfStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
     Value Eval() const {
         Value val = GetLeft()->Eval();
         if(val.GetInt() != 0)
         {
             return GetRight()->Eval();
         }
         return Value(0);
     }
};

class SetStatement : public ParseTree {
	string id;

public:
	SetStatement(int line, string id, ParseTree *expr) : ParseTree(line, expr), id(id) {}
     Value Eval() const {
         Value val = GetLeft()->Eval();
           symbols[id]=val;  
         return Value(val);
     }
};

class PrintStatement : public ParseTree {
public:
	PrintStatement(int line, ParseTree *e) : ParseTree(line, e) {}
     Value Eval() const {
         Value val = GetLeft()->Eval();
         if(val.isInt() || val.isStr()){
             
           cout<<val;
         }
         
         return Value(val);
     }
};

class LoopStatement : public ParseTree {
public:
	LoopStatement(int line, ParseTree *ex, ParseTree *stmt) : ParseTree(line, ex, stmt) {}
     Value Eval() const {
         while(GetLeft()->Eval().GetInt()!=0){
             GetRight()->Eval();
         }        
         return Value(0);
     }
};

class Addition : public ParseTree {
public:
	Addition(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() const {
        //return GetLeft()->Eval()+GetRight()->Eval();
        //return GetLeft()->Eval()+GetRight()->Eval();
        //return Value(0);
        auto a1 = GetLeft()->Eval();
        auto a2 = GetRight()->Eval();
        return a1+a2;
    }
    bool OP() const { return true; }   
};

class Subtraction : public ParseTree {
public:
	Subtraction(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
    Value Eval() const {
        //return GetLeft()->Eval()-GetRight()->Eval();
        auto a1 = GetLeft()->Eval();
        auto a2 = GetRight()->Eval();
        return a1-a2;
    }
    bool OP() const { return true; }
};

class Multiplication : public ParseTree {
public:
	Multiplication(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
     Value Eval() const {
         //return (GetLeft()->Eval())*(GetRight()->Eval());
         //return Value(0);
        auto a1 = GetLeft()->Eval();
        auto a2 = GetRight()->Eval();
        return a1*a2;
     }
     bool OP() const { return true; }
    
};

class Division : public ParseTree {
public:
	Division(int line, ParseTree *l, ParseTree *r) : ParseTree(line,l,r) {}
     Value Eval() const {
         if(GetRight()->Eval().GetInt()==0)
         {
             throw std::logic_error(" Divide by zero error");
         }
         //return GetLeft()->Eval()/GetRight()->Eval();
         //return Value(0);
        auto a1 = GetLeft()->Eval();
        auto a2 = GetRight()->Eval();
        return a1/a2;
     }
     bool OP() const { return true; }    
};

class IConst : public ParseTree {
	int val;

public:
    IConst(int l, int i):ParseTree(l), val(i) {}
    IConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
    Value Eval() const {
        return Value(val);
    }
};

class SConst : public ParseTree {
	string val;

public:
	SConst(Token& t) : ParseTree(t.GetLinenum()) {
		val = t.GetLexeme();
	}
    Value Eval() const {
        return Value(val);
    }
	bool IsString() const { return true; }
    
};

class Ident : public ParseTree {
	string id;

public:
	Ident(Token& t) : ParseTree(t.GetLinenum()), id(t.GetLexeme()) {}
    
    Value Eval() const {
        if(symbols.find(id) != symbols.end()){
        return symbols[id];
        }
    else{
        throw std::logic_error(" Symbol " + id + " not defined");
      }
    }     
	bool IsIdent() const {return true; }
     
};

#endif /* TREE_H_ */
