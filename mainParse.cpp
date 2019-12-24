#include "tokens.h"
#include "parse.h"
#include "tree.h"
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

    if( prog == 0 )
	return 0; //STOPS IF ERROR IS FOUND
    int x = prog->NodeCount();
    int y = prog->lvCount();
    int nonleaf = x-y;
    cout<<"NODE COUNT: "<<prog->NodeCount()<<endl;
    cout<<"INTERIOR COUNT: "<<nonleaf<<endl;
    if(prog->OperatorCount()>0)
    cout<<"OPS COUNT: "<<prog->OperatorCount()<<endl;
    if(prog->StrCount()>0)
    cout<<"STRING COUNT: "<<prog->StrCount()<<endl;
    if(prog->MaxDepth()>0)
    cout<<"MAX DEPTH: "<<prog->MaxDepth()<<endl;
    

	return 0;
}
