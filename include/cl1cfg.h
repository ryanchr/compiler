// File name: cl1cfg.h
//
// Author: Xie Li@ SJTU.MicroE, Time: 2010/07/31
// ==============================================
// This file defines class generating the CL1 context.
//

#ifndef CL1CFG_H
#define CL1CFG_H

#include "cl1.h"

class RCA;
class RCAPort;

class RPUConfig;

class CL1Config{

public:

	CL1Config();

	const Vector<CL1Block> genCL1Block(RPUConfig & config, Vector<RCA*> & rcas);

	const Vector<reg32> & genRegs(const Vector<CL1Block> & blocks);

	int RIDLLength() const { return CL1RIDLLength; }

	int CoreLength() const { return CL1CoreLength; }

	int totalLength() const { return CL1TotalLength; }

	//////////////////////////////////////////////////////////////

	int getSSRAMTopAddrIn() const { return SSRAMInTopAddr; }

	int getSSRAMTopAddrOut() const { return SSRAMOutTopAddr; }

	int GCGMAddress() const { return GCGMAddr; }

	void setGCGMAddress(int addr) { GCGMAddr = addr; }

private:

	int remainRCA(const Vector<RCA*> & rcas);

	Vector<RCA*> readyRCA(const Vector<RCA*> & rcas);

	void updateRCAState(const Vector<RCA*> & rcas);

	const Vector<CL1Block> mapRCA(Vector<RCA*> rcas);

	void freeRIMSpace(const Vector<RCA*> rcas);

	int sumPseudoRCA();

	const Vector<CL1Block> insertPseudoRCA(RPUConfig & config);

private:

	int CL1RIDLLength;

	int CL1CoreLength;

	int CL1TotalLength;

	Vector<reg32> Regs;

	CL1RIM RIM;

	Vector<RCAPort*> outPortInRIM;

	Vector<int> outAreaCounter;   /* record data num in Area 0、1、2、3、4、5*/

	int SSRAMInBaseAddr;
	int SSRAMOutBaseAddr;

	int SSRAMInTopAddr;       //the terminate address of input data
	int SSRAMOutTopAddr;

	int GCGMAddr;
};


//-----------------------------------------------

#define MAX_AREA_SUM    6

inline CL1Config::CL1Config(): 
		CL1RIDLLength(0), 
		CL1CoreLength(0), 
		CL1TotalLength(0),
		outAreaCounter(MAX_AREA_SUM, 0),
		SSRAMInBaseAddr(0), 
		SSRAMOutBaseAddr(0), 
		GCGMAddr(0){}

#endif

