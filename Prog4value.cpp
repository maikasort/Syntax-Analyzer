#include <stdexcept>
#include <string>
using std::string;
#include <iostream>
using std::ostream;

#ifndef _VALUE_H_
#define _VALUE_H_

class Value {
public:
    enum ValType { ERRVAL, INTVAL, STRVAL };
    
private:
    ValType        theType;
    int                 theInt;
    string            theStr;
    
public:
    Value() : theType(ERRVAL), theInt(0) {}
    Value(int i) : theType(INTVAL), theInt(i) {}
    Value(string s) : theType(STRVAL), theInt(0) , theStr(s) {}
    
    bool isInt() const { return theType == INTVAL; }
    bool isStr() const { return theType == STRVAL; }
    bool isErr() const { return theType == ERRVAL; }
    
    int GetInt() const {
        if( isInt() ) return theInt;
        throw std::logic_error(" Conditional is not an integer");
    }
    
    string GetStr() const {
        if( isStr() ) return theStr;
        throw std::logic_error("This Value is not a string");
    }
   
    Value operator+(const Value& o) const{
        if(o.isInt() && isInt()){
            return Value(GetInt()+o.GetInt());
        }
         if(o.isStr() && isStr()){
            return Value(GetStr()+o.GetStr());
        } 
           else 
               throw std::logic_error(" Type mismatch for arguments of +");
    
  }
    
    Value operator-(const Value& o) const{
          if(o.isInt() && isInt()){
            return Value(GetInt()-o.GetInt());
        }
           else 
               throw std::logic_error(" Type mismatch for arguments of -");
    
  }
    Value operator*(const Value& o) const{
         if(isInt() && o.isInt()){
            return Value(GetInt() * o.GetInt());
        }
        else if(isStr() && o.isInt())
        {
            if(o.GetInt()<0)
            {
                throw std::logic_error(" Repetition count less than 0");
            }
            else
            {    
                
                int val = o.GetInt(); 
                string s="";
                for (int i =0;i<val;i++)
                {
                    s+=GetStr();
                }
                return (Value(s));
            }
        }
        else if(isInt() && o.isStr())
        {
            if(GetInt()<0)
            {
                throw std::logic_error(" Repetition count less than 0");
            }
            else
            {    
                
                int val = GetInt();
                string s="";
                for (int i =0;i<val;i++)
                {
                    s+=o.GetStr();
                }
                return(Value(s));
            }
        }
        else
        {
            throw std::logic_error(" Type mismatch for arguments of *");
        }
    }
    Value operator/(const Value& o) const{
          if(o.isInt() && isInt()){
            return Value(GetInt()/o.GetInt());
        }
           else 
               throw std::logic_error(" Type mismatch for arguments of /");
    
  }
    
    friend ostream& operator<<(ostream& out, const Value& v) {
        switch( v.theType ) {
            case ERRVAL:
                out << "RUNTIME ERROR: " << v.theStr;
                break;
            case INTVAL:
                out << v.theInt;
                break;
            case STRVAL:
                out << v.theStr;
                break;
        }
        
        return out;
    }
};

#endif
