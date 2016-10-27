
#include "cl1reg.h"
#include "rpucfg.h"

#define MAX_CL1_RCA_NUM		10 // 6 left for pesudo-RCA

int RPUConfig::genCL1Context(){

	Vector<RCA*> CL1RCA;
	CL1RCA.reserve(MAX_CL1_RCA_NUM);

	List<Ptr<RCA> >::iterator rcaIter = rcaList.begin();

	while(rcaIter != rcaList.end()){

		CL1RCA.clear();
		for(int i =0; i <MAX_CL1_RCA_NUM; ++ i, ++ rcaIter){

			if(rcaIter == rcaList.end())break;

			CL1RCA.push_back(rcaIter->get());
		}

		// Generate CL1 context
		CL1Context.push_back(
			cl1config.genRegs( cl1config.genCL1Block(*this, CL1RCA) )
		);
	}

	return 0;
}

