/************************************************
 *			File name: rpucfg.h
 *-----------------------------------------------
 * Description:
 *
 * Author: L. Xie @ MicroE. SJTU
 * Date : 2009/11/13
 *************************************************
 */

#ifndef REMUSCFG_H
#define REMUSCFG_H


#include "rca.h"
#include "cl1cfg.h"
#include "dfgraph.h"

#include <iostream>
#include <map>

class OpCL2timizeMethod;
class SplitMethod;
class CL1Block;


//////////////////////////////////////////////////////////

class RPUConfig{

public:

	RPUConfig() {}

	RPUConfig(const DFGraph & graph): dfgGraph(graph) {}

	DFGraph & graph() { return dfgGraph; }

	List<Ptr<RCA> > & rcas() { return rcaList; }

	RCA * newRCA();

	//----------------------------------------------------

	// The functions below used by the spliting algorithm 
	DFGNode * newBypsNode();

	DFGPort * getNodePort (DFGNode * node);

	DFGNode * getNodeByps (DFGNode * node);

	DFGPort * cutSource(DFGNode * node, int srcIndex);

	DFGPort * insertPortOf (DFGNode * node);

	DFGNode * insertBypassBetween(
		DFGNode * srcNode, DFGNode * tgtNode
	);

	void display(std::ostream & out = std::cout) const;

	//-------------------------------------------------

	// This funnction use to optimizing and splitiing a DFG, 
	// and map the subgraph into a series of RCA.
	int mapDFGraph(
		const OptimizeMethod & optmize, const SplitMethod & split
	);
	
	// This function used to generate the CL0, CL1, CL2 context
	int genContext();

	// -------------------------------------------------

	// The functions below create header file including context
	// and macro definition of function interface.
	int createInterface(const String & fileName);

	int createCL0File(const String & fileName);

	int createCL1File(const String & fileName);

	int createCL2File(const String & fileName);

	// Create file for debug
	int printNodeInfo(const String & fileName);

	// ----------------------------------------------------

private:

	// This function used to get the source 
	// and target information of RCA after splitting
	int connectRCA();

	int genCL0Context(); // GenerateCL0 context

	int genCL1Context(); // GenerateCL1 context

	int genCL2Context(); // GenerateCL2 context

private:

	DFGraph dfgGraph; // DFG structrue of this configuration

	List<Ptr<RCA> > rcaList; // RCA list after splitting.

	List<Ptr<CL1Block> > blockList; // CL1Block list after CL1 mapping

	List<Ptr<DFGPort> > innerDFGPorts; // Internal port after splitting

	List<Ptr<DFGNode> > bypassNodes; // Bypass node in DFG

	// Record the RCA port attach to a DFG node
	std::map<DFGNode*, DFGPort*> nodePortTable;

	// Record the last bypass node of a DFG node
	std::map<DFGNode*, DFGNode*> nodeBypsTable;

	CL1Config cl1config;

	// Store context
	Vector<reg32> CL0Context;

	Vector<Vector<reg32> > CL1Context;

	Vector<Vector<reg32> > CL2Context;
};


#endif
