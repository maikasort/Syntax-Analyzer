/*
 * main.cpp
 *
 * CS280 - Spring 2019
 * Lexical Analyzer test engine
 */

#include <fstream>
using std::ifstream;

#include <map>
using std::map;

#include "tokens.h"
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

int
main(int argc, char *argv[])
{
	int lineNumber = 0;

	Token	tok;

	istream *in = &cin;
	ifstream file;

	bool vflag = false;
	bool idsflag = false;
	bool strflag = false;

	int tokCount = 0;
	int stringCount = 0;
	int identCount = 0;
	map<string,bool> identifiers;
	map<string,bool> strings;

	for( int i=1; i<argc; i++ ) {
		string arg( argv[i] );
		if( arg == "-v" )
			vflag = true;
		else if( arg == "-ids" )
			idsflag = true;
		else if( arg == "-strings" )
			strflag = true;
		else if( arg[0] == '-' ) {
			cerr << "UNRECOGNIZED FLAG " << arg << endl;
			return 0;
		}
		else if( in != &cin ) {
			cerr << "ONLY ONE FILE NAME ALLOWED" << endl;
			return 0;
		}
		else {
			file.open(arg);
			if( file.is_open() == false ) {
				cerr << "CANNOT OPEN " << arg << endl;
				return 0;
			}

			in = &file;
		}
	}

	while( (tok = getNextToken(*in, lineNumber)) != DONE && tok != ERR ) {
		if( vflag )
			cout << tok << endl;

		++tokCount;
		if( tok == SCONST ) {
			stringCount++;
			strings[ tok.GetLexeme() ] = true;
		}
		else if( tok == ID ) {
			identCount++;
			identifiers[ tok.GetLexeme() ] = true;
		}
	}

	if( tok == ERR ) {
		cout << "Error on line " << tok.GetLinenum()+1 << " (" << tok.GetLexeme() << ")" << endl;
		return 0;
	}

	if( strflag && strings.size() > 0 ) {
		cout << "STRINGS:" << endl;
        for( const auto& sp : strings )
            cout << sp.first << endl;
	}

	if( idsflag && identifiers.size() > 0 ) {
     	cout << "IDENTIFIERS: ";
     	auto it = identifiers.begin();
     	cout << it->first;

		for( it++; it != identifiers.end(); it++ )
			cout << ", " << it->first;
		cout << endl;
	}
    
    // summary
	cout << "Total lines: " << lineNumber << endl;
	if( lineNumber )
        cout << "Total tokens: " << tokCount << endl;

	return 0;
}