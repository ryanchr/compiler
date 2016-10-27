

#include "dfgraph.h"
#include "rpucfg.h"
#include "optmz.h"
#include "split.h"

#include <iostream>
#include <fstream>

#define dfgFileName "..\\..\\test\\dct16_small\\DFG\\dct32.dfg"

int main(int argc, char * argv[]){

	using std::cout;
	using std::cerr;
	using std::endl;

	int err = 0;

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

	if(err)cout<<"Generated context fail!"<<endl;

	// Generate information for debug
	const String debugdfgFileName = 
		config.graph().name()+"_cfginfo.txt";

	std::ofstream debugFile(debugdfgFileName.c_str());
	config.graph().display(debugFile);
	config.display(debugFile);

	const String infoFile =  
		config.graph().name() + "_nodeinfo.txt";

	err |= config.printNodeInfo(infoFile);

	if(!err)cout<<"C code is generated successful!"<<endl;

	system("Pause");

	return err;
}
