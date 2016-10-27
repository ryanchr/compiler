
////////////////////////////////////////////////////////////
//				       ESLCompiler
//==========================================================
//
// Author: L. Xie	Time: 2009.03.02
// Department: MicroE of Shanghai Jiao Tong University
//----------------------------------------------------------
// This file generates an executable file for ESL project,
// which needs a DFG file as input and then generate a 
// C code file executed in ESL platform.
//
////////////////////////////////////////////////////////////


#include "dfgraph.h"
#include "rpucfg.h"
#include "optmz.h"
#include "split.h"

#include <iostream>
#include <fstream>


int main(int argc, char * argv[]){

	using std::cout;
	using std::cerr;
	using std::endl;

	int err = 0;

	// Read the command

	bool needHelp = false;
	bool needDebug = false;
	bool hasFile = false;

	char * dfgFileName;

	for(int i = 1; i <argc; ++i){
		if(argv[i][0] == '-'){

			switch(argv[i][1]){
				case 'h': needHelp = true; break;
				case 'd': needDebug = true; break;
				case 'f': if(++i >= argc)break;
					hasFile = true;
					dfgFileName = argv[i];
					break;
				default:
					cerr<<"Error: unkown option: -"
						<<argv[i][1]<<"\n";
			}
		}
	}

	if(needHelp){
		cout<<"Usage: Command [option] [parameter]\n"
			"\toption:"
			"\n"
			"\t\t-h: Print this message\n"
			"\t\t-d: Print debug information to file\n"
			"\t\t-f: Input DFG file, need [parameter] = <File Name>\n"
			<<endl;
	}

	if(hasFile){
		std::ifstream dfgFile(dfgFileName);

		if(!dfgFile){
			cerr<<"Can't open file \""<<dfgFileName<<"\""<<endl;

			return -1;
		}

		//---------------------------------------------------
		
		RPUConfig config;

		err |= config.graph().parse(dfgFile);
		if(err){
			cerr<<"Bad DFG file format!"<<endl;
			
			return -1;
		}

		const String CL0FileName = 
			config.graph().name()+"_cl0.h";

		const String CL1FileName = 
			config.graph().name()+"_cl1.h";

		const String CL2FileName = 
			config.graph().name()+"_cl2.h";

		const String InterfaceFileName = 
			config.graph().name()+"_intf.h";

		// Optimize and Splite DFG
		err |= config.mapDFGraph(OptimizeMethod(), SplitMethod());
	err |= config.genContext();

	err |= config.createCL0File(CL0FileName);
	err |= config.createCL1File(CL1FileName);
	err |= config.createCL2File(CL2FileName);
	err |= config.createInterface(InterfaceFileName);

	if(err)cerr<<"Generated context fail!"<<endl;

		if(needDebug){

			const String debugdfgFileName = 
				config.graph().name()+"_cfginfo.txt";

			std::ofstream debugFile(debugdfgFileName.c_str());
			config.graph().display(debugFile);
			config.display(debugFile);

			const String infoFile =  
				config.graph().name() + "_nodeinfo.txt";

			err |= config.printNodeInfo(infoFile);
		}

		if(!err)cout<<"C code is generated successful!"<<endl;
	}

	return err;
}
