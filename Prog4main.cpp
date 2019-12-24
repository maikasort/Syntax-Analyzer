#include "tokens.h"
#include "parse.h"
//#include "tree.h"
//#include "value.h"
//#include "gettoken.h"
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
	ifstream file;
	istream *in = &cin;
	int linenum = 0;

    
	if(argc>2)
    {
		cout<< "TOO MANY FILENAMES"<< endl;
        return 0;
	}
    else if(argc==2)
    {
		file.open(argv[1]);
		if( file.is_open() == false ) 
           {
				cout<< "COULD NOT OPEN " << argv[1] << endl;
				return 0;
			}
        in = &file;
    }
   
	ParseTree *prog = Prog(*in, linenum);
	try{
		prog->Eval();
	}
	catch (logic_error& e){
		cout<<"RUNTIME ERROR"<<e.what()<<endl;
	}
    
}

		