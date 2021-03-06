
#include "rpucfg.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>


int RPUConfig::createCL0File(const String &fileName){

	std::ofstream CL0File(fileName.c_str());

	assert(CL0File);

	const String & graphName = dfgGraph.name();

	// uppercase of name
	String GRAPHNAME = graphName;
	std::transform(GRAPHNAME.begin(), GRAPHNAME.end(), 
		GRAPHNAME.begin(), ::toupper);

	std::size_t cl0size = CL0Context.size();

	// Print file
	//========================================================
	CL0File<<
	"/******************************************************\n"
	" * This file is generated by RPUCompiler automatically.\n"
	" * ----------------------------------------------------\n"
	" */\n"
	"\n\n"
	"#ifndef "<<GRAPHNAME<<"_CL0_H\n"
	"#define "<<GRAPHNAME<<"_CL0_H\n"
	"\n"
	"\n"
	"#define "<<GRAPHNAME<<"_LOAD_BASE\t\t"<<SSRAM_LOAD_BASE<<"\n"
	"#define "<<GRAPHNAME<<"_STORE_BASE\t\t"<<SSRAM_STORE_BASE<<"\n"
	"\n\n"
	"#define "<<GRAPHNAME<<"_CL0_SIZE\t"<<std::dec<<cl0size<<"\n\n"
	"unsigned int "<<graphName<<"_cl0"<<"_ctx [] = {";

	CL0File.fill('0');

	for(std::size_t i =0; i <cl0size ; ++i){

		CL0File<<"\n\t\t0x"<<std::setw(8)<<std::hex<<CL0Context[i];

			if( i + 1 != cl0size)CL0File<<",";
	}

	CL0File<<"\n};\n\n#endif"<<std::endl;

	return 0;
}

