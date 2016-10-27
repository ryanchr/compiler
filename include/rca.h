
#ifndef RCA_H
#define RCA_H

#include "platf.h"
#include "rcavex.h"

#include <iostream>
#include <cassert>

class RCARegs;

class RCA {
	
public:

	RCA(int seqNo = -1);

	RCA(const RCA & rca);

	int seqNo() const { return seqNumber; }

	void setSeqNo(int num) { seqNumber = num; }

	//------------------------------------------------------

	const RCANode & node(int row, int col) const;

	RCANode & node(int row, int col);

	const RCANode & node(int index) const ;

	RCANode & node(int index);

	const RCANode & temp(int row, int col) const;

	RCANode & temp(int row, int col);

	const RCANode & temp(int index) const ;

	RCANode & temp(int index);

	//----------------------------------------

	RCA * sources(int seqNo) 
	
	{ return rcaSources.at(seqNo); }

	const RCA * sources(int seqNo) const 
	
	{ return rcaSources.at(seqNo); }

	Vector<RCA*> & sources() 
	
	{ return rcaSources; }

	const Vector<RCA*> & sources() const 
	
	{ return rcaSources; }

	void removeSource(RCA * rca);

	//-----------------------------------------

	RCA * targets(int seqNo) 
	
	{ return rcaTargets.at(seqNo); }

	const RCA * targets(int seqNo) const 
	
	{ return rcaTargets.at(seqNo); }

	Vector<RCA*> & targets() 
	
	{ return rcaTargets; } 

	const Vector<RCA*> & targets() const 
	
	{ return rcaTargets; }

	void removeTarget(RCA * rca);

	//-----------------------------------------

	RCAPort & inports(int seqNo) 
	
	{ return rcaInports.at(seqNo); }

	const RCAPort & inports(int seqNo) const 
	
	{ return rcaInports.at(seqNo); }

	Vector<RCAPort> & inports() 
	
	{ return rcaInports; }

	const Vector<RCAPort> & inports() const 
	
	{ return rcaInports; }

	//-----------------------------------------

	RCAPort & outports(int seqNo) 
	
	{ return rcaOutports.at(seqNo); }

	const RCAPort & outports(int seqNo) const 
	
	{ return rcaOutports.at(seqNo); }

	Vector<RCAPort> & outports() 
	
	{ return rcaOutports; }

	const Vector<RCAPort> & outports() const 
	
	{ return rcaOutports; }

	//-------------------------------------------

	// Schedule state
	int state() const { return rcaState; }

	void setState(int state) { rcaState = state; }

	//-------------------------------------------

	int loadSize() const { return rcaLoadSize; }

	void setLoadSize(int size) { rcaLoadSize = size; }

	int storeSize() const { return rcaStoreSize; }

	void setStoreSize(int size) { rcaStoreSize = size; }

	int loopNum () const { return rcaLoopNum; }

	void setLoopNum(int loop) { rcaLoopNum = loop; }

	//-------------------------------------------------------

	RCA * copy() { return new RCA(*this); }

private:

	int seqNumber;

	Vector<RCAPort> rcaInports;

	Vector<RCAPort> rcaOutports;

	Vector<RCA*> rcaSources;

	Vector<RCA*> rcaTargets;

	RCANode nodes[RC_REG_NUM];

	RCANode tempNodes[RC_TEMP_NUM];

	// Schedule state

	int rcaState;

	// Execute parameters

	int rcaLoadSize;

	int rcaAHBLoad;

	int rcaStoreSize;

	int rcaAHBStore;

	int rcaExecuteNum;

	int rcaReloadNum;

	int rcaLoopNum;

};

//////////////////////////////////////////////////////////////

inline const RCANode & RCA::node(int row, int col) const{ 

	assert(row >=0 && row < RCA_ROW);
	assert(col >=0 && col < RCA_COL); 
	return nodes[row * RCA_COL + col]; 
}

inline RCANode & RCA::node(int row, int col){ 

	assert(row >=0 && row < RCA_ROW);
	assert(col >=0 && col < RCA_COL); 
	return nodes[row * RCA_COL + col]; 
}

inline const RCANode & RCA::node(int index) const {

	assert(index >=0 && index < RC_REG_NUM); 
	return nodes[index]; 
}

inline RCANode & RCA::node(int index) { 

	assert(index >=0 && index < RC_REG_NUM); 
	return nodes[index]; 
}

inline const RCANode & RCA::temp(int row, int col) const{ 

	assert(row >=0 && row < RCA_ROW);
	assert(col >=0 && col < RCA_COL); 
	return tempNodes[row * RCA_COL + col]; 
}

inline RCANode & RCA::temp(int row, int col){ 

	assert(row >=0 && row < RCA_ROW);
	assert(col >=0 && col < RCA_COL); 
	return tempNodes[row * RCA_COL + col]; 
}

inline const RCANode & RCA::temp(int index) const {

	assert(index >=0 && index < RC_REG_NUM); 
	return tempNodes[index]; 
}

inline RCANode & RCA::temp(int index) { 

	assert(index >=0 && index < RC_REG_NUM); 
	return tempNodes[index]; 
}

inline RCA::RCA(int seqNo)
	: seqNumber(seqNo),
	rcaInports(),
	rcaOutports(),
	rcaSources(),
	rcaTargets(),
	rcaState(0),
	rcaLoadSize(1), 
	rcaAHBLoad(0), 
	rcaStoreSize(1), 
	rcaAHBStore(0), 
	rcaExecuteNum(0),
	rcaReloadNum(0),
	rcaLoopNum(1) {

	for(int r =0; r <RCA_ROW; ++r)
		for(int c =0; c <RCA_COL; ++c){
			int curIndex = r * RCA_COL + c;

			nodes[curIndex].setRow(r);
			nodes[curIndex].setCol(c);

			nodes[curIndex].setRCA(this);

			tempNodes[curIndex].setRow(r);
			tempNodes[curIndex].setCol(RCA_COL + c);

			tempNodes[curIndex].setRCA(this);
		}
}

inline RCA::RCA(const RCA & rca)
	: seqNumber(rca.seqNumber),
	rcaInports(rca.rcaInports),
	rcaOutports(rca.rcaOutports),
	rcaSources(rca.rcaSources),
	rcaTargets(rca.rcaTargets),
	rcaState(rca.rcaState),
	rcaLoadSize(rca.rcaLoadSize), 
	rcaAHBLoad(rca.rcaAHBLoad), 
	rcaStoreSize(rca.rcaStoreSize), 
	rcaAHBStore(rca.rcaAHBStore), 
	rcaExecuteNum(rca.rcaExecuteNum),
	rcaReloadNum(rca.rcaReloadNum),
	rcaLoopNum(rca.rcaLoopNum) {

	for(int i =0; i <RC_REG_NUM; ++i){
		nodes[i] = rca.nodes[i];
		nodes[i].setRCA(this);

		tempNodes[i] = rca.tempNodes[i];
		tempNodes[i].setRCA(this);
	}
}

inline void RCA::removeSource(RCA * rca){
	Vector<RCA*>::iterator iter, end;

	end = rcaSources.end();
	for(iter = rcaSources.begin(); iter != end; ++ iter)
		if(*iter == rca)break;

	rcaSources.erase(iter);
}

inline void RCA::removeTarget(RCA * rca){
	Vector<RCA*>::iterator iter, end;

	end = rcaTargets.end();
	for(iter = rcaTargets.begin(); iter != end; ++ iter)
		if(*iter == rca)break;

	rcaTargets.erase(iter);
}

///---------------------------------------------------

inline std::ostream & 
operator <<(std::ostream & out, const RCA & rca){
	//Print RCA inport
	Vector<RCAPort>::const_iterator portIter;

	out<<"In("<<rca.inports().size()<<"):\n";
	for(portIter = rca.inports().begin(); 
		portIter != rca.inports().end(); ++ portIter)
		out<<'\t'<<*portIter->dfgPort()<<"\n";


	for(int r =0; r <RCA_ROW; ++ r){
		for(int c =0; c <RCA_COL; ++ c){

			const RCANode & curNode = rca.node(r, c);
			DFGNode * thisNode = curNode.dfgNode();

			thisNode ? out<<*thisNode<<"("<<thisNode->weight()<<") " : out<<"X ";
		}

		out<<"\n";
	}

	out<<"Out("<<rca.outports().size()<<"):\n";
	for(portIter = rca.outports().begin(); 
		portIter != rca.outports().end(); ++ portIter)
		out<<'\t'<<*portIter->dfgPort()<<"\n";

	return out;
}

#endif
