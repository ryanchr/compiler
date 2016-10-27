
#include "cl0reg.h"
#include "rpucfg.h"

int RPUConfig::genCL0Context(){

	// Set Head
	CL0HeadReg headReg;

	headReg.reset();
	headReg.setGroupAdrress(cl1config.GCGMAddress());
	headReg.setSynMode(SYN_MODE_0);
	
	CL0Context.push_back(headReg.reg());

	// Set CEDL
	REDLReg REDL;
	REDL.word0().setSSRAMAddress(SSRAM_LOAD_BASE);
	REDL.word1().setSSRAMHeight((cl1config.getSSRAMTopAddrIn()-1)/FIFO_WIDTH);
	//FIXME
	REDL.word1().setSSRAMLength(FIFO_WIDTH-1);
	//FIXME
	using std::cout;using std::endl;
	cout<<"SSRAMTopAddrIn is "<<cl1config.getSSRAMTopAddrIn()<<endl;
	REDL.word1().setSSRAMJump(0);
	REDL.word1().setMode(REDL_MODE_1L);

	CL0Context.push_back(REDL.word0().reg());
	CL0Context.push_back(REDL.word1().reg());

	// Set CEDS
	REDSReg REDS;
	REDS.word0().setSSRAMAddress(SSRAM_STORE_BASE);
	REDS.word1().setSSRAMHeight((cl1config.getSSRAMTopAddrOut()-1)/FIFO_WIDTH);
	//FIXME
	REDS.word1().setSSRAMLength(FIFO_WIDTH-1);
	cout<<"SSRAMTopAddrOut is"<<cl1config.getSSRAMTopAddrOut()<<endl;
	
	REDS.word1().setSSRAMJump(0);
	REDS.word1().setMode(REDS_MODE_ML);

	CL0Context.push_back(REDS.word0().reg());
	CL0Context.push_back(REDS.word1().reg());

	return 0;
}

