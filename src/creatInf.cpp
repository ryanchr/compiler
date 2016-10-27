
#include "rpucfg.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>


int RPUConfig::createInterface(const String &fileName){

	std::ofstream InfFile(fileName.c_str());

	assert(InfFile);

	const String & graphName = dfgGraph.name();

	// uppercase of name
	String GRAPHNAME = graphName;
	std::transform(GRAPHNAME.begin(), GRAPHNAME.end(), 
		GRAPHNAME.begin(), ::toupper);

	const String LOAD_BASE = GRAPHNAME + "_LOAD_BASE";
	const String STORE_BASE = GRAPHNAME + "_STORE_BASE";

	// Print file
	//========================================================
	InfFile<<
		"\n#ifndef "<<GRAPHNAME<<"_H\n"
		"#define "<<GRAPHNAME<<"_H\n\n"
		"#include \""<<graphName<<"_cl0.h\"\n\n"
		"#define "<<GRAPHNAME<<"_RCA_FUC\t\\\n";
	
	// Write SSRAM
	for(int i =0; i <dfgGraph.inSize(); ++i){

		DFGPort * thisPort = & dfgGraph.inport(i);

		int varAddr = thisPort->SSRAMAddress();

		if(varAddr == -1){

			std::cerr<<"Error: A unkown address outport '"
				<<thisPort->id()<<"' found!\n";

			continue;
		}

		InfFile<<"\t\twriteSSRAM("<<LOAD_BASE<<" + "<<varAddr<<", ";

		if(typeid(*thisPort) == typeid(DFGImmPort)){

			InfFile<<static_cast<DFGImmPort*>(thisPort)->value()
				<<", 1);\t\\\n";
		} else {

			InfFile<<static_cast<DFGVarPort*>(thisPort)->name()
				<<", 1);\t\\\n";
		}
	}
	InfFile<<"\t\t\\\n";

	// Write CL0
	InfFile<<"\t\twriteCL0("<<graphName<<"_cl0, "
		<<GRAPHNAME<<"_CL0_SIZE);\t\\\n";

	InfFile<<"\t\t\\\n";

	// Read SSRAM
	for(int i =0; i <dfgGraph.outSize(); ++i){

		DFGPort * thisPort = & dfgGraph.outport(i);

		int varAddr = thisPort->SSRAMAddress();

		if(varAddr == -1){

			std::cerr<<"Error: A unkown address outport '"
				<<thisPort->id()<<"' found!\n";

			continue;
		}

		InfFile<<"\t\treadSSRAM("<<STORE_BASE<<" + "<<varAddr<<", ";

		assert(typeid(*thisPort) == typeid(DFGVarPort));

		InfFile<<static_cast<DFGVarPort*>(thisPort)->name()
				<<", 1);\t\\\n";
	}
	
	InfFile<<"\n#endif\n"<<std::endl;

	return 0;
}

