
#include "rpucfg.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <algorithm>

int RPUConfig::createCL2File(const String &fileName){

	// Create file
	std::ofstream CL2File(fileName.c_str());

	assert(CL2File);

	const String & graphName = dfgGraph.name();

	// uppercase of name
	String GRAPHNAME = graphName;
	std::transform(GRAPHNAME.begin(), GRAPHNAME.end(), 
		GRAPHNAME.begin(), ::toupper);

	CL2File<<
	"/******************************************************\n"
	" * This file is generated by RPUCompiler automatically.\n"
	" * ----------------------------------------------------\n"
	" * This file include CL2 context to initialize GCCM. \n"
	" */\n\n"
	"#ifndef "<<GRAPHNAME<<"_CL2_H\n"
	"#define "<<GRAPHNAME<<"_CL2_H\n"<<std::endl;

	
	CL2File<<"\n#define "<<GRAPHNAME<<"_CL2_SIZE\t"
		<<std::dec<<CL2Context.size()<<";\n\n";

	int cl2SeqNo = 0;
	Vector<Vector<reg32> >::iterator cl2Iter;
	for(cl2Iter = CL2Context.begin(); 
		cl2Iter != CL2Context.end(); ++ cl2Iter, ++ cl2SeqNo){

		Vector<reg32> & curCL2Context = * cl2Iter;

		const String & graphName = dfgGraph.name();

		// Print context
		//---------------------------------------------------------

		int cl2Size = static_cast<int>(curCL2Context.size());

		CL2File<<"\n#define "<<GRAPHNAME<<"_CL2_SIZE\t"<<std::dec<<cl2Size<<";\n\n";
		CL2File<<"unsigned int "<<graphName<<"_cl2_ctx_"<<cl2SeqNo<<"[] = {";

		CL2File.fill('0');

		std::size_t cl2size = curCL2Context.size();

		for(std::size_t i =0; i <cl2size; ++ i){

			CL2File<<"\n\t\t0x"<<std::setw(8)<<std::hex<<curCL2Context[i];

			if( i + 1 != cl2size)CL2File<<",";
		}

		CL2File<<"\n};\n\n"<<std::endl;
	}

	CL2File<<"#endif"<<std::endl;

	return 0;
}
