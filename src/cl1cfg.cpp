// Filename: genCL1info.cpp
//
// Author: Xie Li@ MicroE of SJTU
// Time: July 29th, 2010
// ======================================
// This file generate the infomation for 
// Cl1 configration.
//

#include "platf.h"
#include "cl1.h"
#include "rpucfg.h"

#include <iostream>

extern bool IsInnerPort(DFGPort * port);

// shedule state

#define STA_WAIT	0
#define STA_REDY	1
#define STA_OVER	2

#define BYTE_PER_LINE	8
#define LINE_PER_CYCLE	4


// Basic routine
/////////////////////////////////////////////////////////////////////

static bool scheduleOver(const Vector<RCA*> & rcas){

	bool over = true;
	Vector<RCA*>::const_iterator rcaIter;
	for(rcaIter = rcas.begin(); rcaIter != rcas.end(); ++ rcaIter){
		
		if((*rcaIter)->state() != STA_OVER){

			over = false;
			break;
		}
	}

	return over;
}

static int areaBelongTo(int row, int col){

	if(row < RIM_HEIGHT/2){

		if( (RIM_WIDTH - 1 - col) > 2*BYTE_PER_LINE) return 4;
		else if((RIM_WIDTH - 1 - col) > BYTE_PER_LINE) return 2;
		else return 0;
	} else {

		if( (RIM_WIDTH - 1 - col) > 2*BYTE_PER_LINE) return 5;
		else if((RIM_WIDTH - 1 - col) > BYTE_PER_LINE) return 3;
		else return 1;
	}
}

///////////////////////////////////////////////////////////////////

int CL1Config::remainRCA(const Vector<RCA*> & rcas){

	int remain = 0;
	Vector<RCA*>::const_iterator rcaIter;
	for(rcaIter = rcas.begin(); rcaIter != rcas.end(); ++ rcaIter){

		if((*rcaIter)->state() != STA_OVER) ++ remain;
	}

	return remain;
}

Vector<RCA*> CL1Config::readyRCA(const Vector<RCA*> & rcas) {

	Vector<RCA*> readyRCAs;
	Vector<RCA*>::const_iterator rcaIter;
	for(rcaIter = rcas.begin(); rcaIter != rcas.end(); ++ rcaIter){

		RCA * thisRCA = *rcaIter;

		if(thisRCA->state() == STA_REDY)readyRCAs.push_back(thisRCA);
	}

	return readyRCAs;
}

void CL1Config::updateRCAState(const Vector<RCA*> & rcas){

	Vector<RCA*>::const_iterator rcaIter = rcas.begin();

	for(; rcaIter != rcas.end(); ++ rcaIter){

		RCA * thisRCA = *rcaIter;

		assert(thisRCA != 0);
		/* check thisRCA is schuduled or not */
		if(thisRCA->state() == STA_OVER) continue;	
		
		const Vector<RCA*> & srcRCAs = thisRCA->sources();

		bool thisRCAReady = true;
		Vector<RCA*>::const_iterator srcRCAIter;
		
		/* thisRCAReady is false if its sourceRCAs haven't got scheduled yet */
		for(srcRCAIter = srcRCAs.begin(); 
			srcRCAIter != srcRCAs.end(); ++ srcRCAIter){

			RCA * thisSrcRCA = *srcRCAIter;

			if(thisSrcRCA->state() != STA_OVER){
				thisRCAReady = false;
				break;
			}
		}

		if(thisRCAReady)thisRCA->setState(STA_REDY);

	}
}

const Vector<CL1Block> 
CL1Config::mapRCA(Vector<RCA*> rcas){

	Vector<CL1Block> mapBlocks;

	Vector<RCA*>::iterator rcaIter;
	for(rcaIter = rcas.begin(); rcaIter != rcas.end(); ++ rcaIter){

		RCA * thisRCA = * rcaIter;

		assert(thisRCA != 0);

		Vector<RCAPort> & rcaOutport = thisRCA->outports();

		// Count the output inner ports and external ports
		int totalInternPort = 0, totalExternPort = 0;
		Vector<RCAPort>::iterator portIter;

		for(portIter = rcaOutport.begin(); 
			portIter != rcaOutport.end(); ++ portIter) {

				IsInnerPort(portIter->dfgPort())? 
					++ totalInternPort : ++ totalExternPort;
		}

		// Allocate the RIM for CDS 
		CL1Data CDSData = RIM.allocate(thisRCA->seqNo(), totalInternPort, totalExternPort);

		if(CDSData.baseAddress() == -1){ // Allocation fail

			std::cout<<"Allocate RIM for RCA"<<thisRCA->seqNo()<<" fail!"<<std::endl;
			continue;
		}

		/////////////////////////////////////////////////////////////////////////////////
		//
		// Mark the ROF and RIM location of port, meanwhile record the output data.
		//------------------------------------------------------------------------------

		int outRegionIndex = 0, tempRegionIndex = 0;
		for(portIter = rcaOutport.begin(); 
			portIter != rcaOutport.end(); ++ portIter) {

			if(IsInnerPort(portIter->dfgPort())){ 
			/* Internal ports: output to TempArea */

			portIter->setROFRow(tempRegionIndex / TEMP_REGION_WIDTH);
			portIter->setRIMRow(CDSData.baseAddress() + portIter->ROFRow());

			portIter->setROFCol(tempRegionIndex % TEMP_REGION_WIDTH);
			portIter->setRIMCol(portIter->ROFCol());

			++ tempRegionIndex;

			} else { 
			/* External ports : output to OutArea */
			/* external data are stored col by col */
			portIter->setROFRow(outRegionIndex % CDSData.height());
			//FIXME
			//cout<<"RCA["<<thisRCA->seqNo()<<"]'s "<<outRegionIndex<<"th external port: height = "<<(CDSData.height())<<endl;
			//cout<<"RCA["<<thisRCA->seqNo()<<"]'s "<<outRegionIndex<<"th external port: ROFRow = "<<(outRegionIndex % CDSData.height())<<endl;
			portIter->setRIMRow(CDSData.baseAddress() + portIter->ROFRow());
			portIter->setROFCol(CDSData.length() - outRegionIndex / CDSData.height() - 1);
			//cout<<"RCA["<<thisRCA->seqNo()<<"]'s "<<outRegionIndex<<"th external port: length = "<<(CDSData.length())<<endl;
			portIter->setRIMCol(CDSData.offset() + portIter->ROFCol());

			++ outRegionIndex;
			
			/* record data num in Out Area(0,1,2,3,4,5) */
			outAreaCounter[areaBelongTo(portIter->RIMRow(), portIter->RIMCol())] ++;

			/* push this port into InRIM */
			outPortInRIM.push_back(&(*portIter));

			}
		}

		// Mark the input ports ,which will be transfered into RIF.
		//----------------------------------------------------------

		Vector<RCAPort> & rcaInport = thisRCA->inports();

		// Count the input inner ports and external ports
		totalInternPort = 0;
		totalExternPort = 0;

		for(portIter = rcaInport.begin(); 
			portIter != rcaInport.end(); ++ portIter) {

				IsInnerPort(portIter->dfgPort())? 
					++ totalInternPort : ++ totalExternPort;
		}

		int externFIFOIndex = 0;

		const int remainder = totalExternPort % FIFO_WIDTH;
		
		const int externDataHeight = 
			totalExternPort / FIFO_WIDTH + ((remainder)? 1:0);

		int internFIFOIndex = externDataHeight * FIFO_WIDTH;

		for(portIter = rcaInport.begin(); 
			portIter != rcaInport.end(); ++ portIter) {

			if(IsInnerPort(portIter->dfgPort())){ // It's an internal port

				portIter->setRIFRow(internFIFOIndex / FIFO_WIDTH);
				portIter->setRIFCol(internFIFOIndex % FIFO_WIDTH);

				++ internFIFOIndex;

			} else { // It's an external port

				portIter->setRIFRow(externFIFOIndex / FIFO_WIDTH);
				portIter->setRIFCol(externFIFOIndex % FIFO_WIDTH);
				
				portIter->dfgPort()->setSSRAMAddress(
					SSRAMInBaseAddr + externFIFOIndex
				);
				/* FIXME for debug */
				//using std::cout;using std::endl;
				//cout<<"extern FIFO index is "<<externFIFOIndex<<endl;
				//cout<<"SSRAMInBaseAddr is "<<SSRAMInBaseAddr<<endl;
				
				++ externFIFOIndex;
			}
		}

		SSRAMInBaseAddr += externDataHeight * FIFO_WIDTH;
		SSRAMInTopAddr = SSRAMInBaseAddr;

		//////////////////////////////////////////////////////////////////
		// Map the block data

		CL1Block block;

		// Set CEDL
		block.CEDLData().setTarget(CEDL_TGT_RIF);
		block.CEDLData().setHeight(externDataHeight);	

		// Set CIDL
		//--------------------------------------------
		//  CIDL need to Find all output data of source 
		//  RCA in temperary data region of RIM, and 
		//  connect them to one 2D data. That need to 
		//  find the lowest base address and height of
		//  the end address first.

		int lowestBaseAddr = RIM_HEIGHT;
		int highestEndAddr = 0;

		Vector<RCA*>::iterator srcIter;
		Vector<RCA*> & thisRCASource = thisRCA->sources();
		/* find BaseAddr and height of inputdata in TempAera */
		for(srcIter = thisRCASource.begin();
			srcIter != thisRCASource.end(); ++ srcIter){

				int srcRCABaseAddr = RIM.getBaseAddress((*srcIter)->seqNo());
				int srcRCAEndAddr = RIM.getEndAddress((*srcIter)->seqNo());

				if(srcRCABaseAddr < lowestBaseAddr) 
					lowestBaseAddr = srcRCABaseAddr;

				if(highestEndAddr < srcRCAEndAddr) 
					highestEndAddr = srcRCAEndAddr;
		}

		block.CIDLData().setInputMode(MODE_IN_2D);
		block.CIDLData().setBaseAddress(lowestBaseAddr);
		block.CIDLData().setLength(TEMP_REGION_WIDTH);
		block.CIDLData().setHeight(highestEndAddr - lowestBaseAddr);
		block.CIDLData().setOutputMode(MODE_OUT_4L);
		//FIXME:for debug
        //cout<<"RCA["<<thisRCA->seqNo()<<"] "<<": CIDL DataInBaseAddr"<<lowestBaseAddr<<endl;
		// Set CDS
		//--------------------------------------------

		CDSData.setTarget(CDS_TGT_RIM);
		block.setCDSData(CDSData);

		// Set Core

		block.setRCAIndex(thisRCA->seqNo());
		block.setRCACoreLoop(1);
		block.setConst1Address(0);
		block.setConst2Address(0);
		block.setRIDLEnable(false);

		bool CEDLEnable = externDataHeight? true : false;
		bool CIDLEnable = !thisRCASource.empty();

		block.setCEDLEnable(CEDLEnable);
		block.setCIDLEnable(CIDLEnable);

		mapBlocks.push_back(block);
		thisRCA->setState(STA_OVER);
	}

	return mapBlocks;
}


void CL1Config::freeRIMSpace(const Vector<RCA*> rcas){

	Vector<RCA*>::const_iterator rcaIter;
	for(rcaIter = rcas.begin(); rcaIter != rcas.end(); ++ rcaIter){

		RCA * thisRCA = *rcaIter;
		assert(thisRCA != 0);
		/* thisRCA not mapped,next circulation */
		if(thisRCA->state() != STA_OVER)continue;

		const Vector<RCA*> & rcaTarget = thisRCA->targets();
		Vector<RCA*>::const_iterator tgtIter;
		/* CanBeFree is false,if thisRCA's targets haven't got mapped  */
		bool canBeFree = true;
		for(tgtIter = rcaTarget.begin(); 
			tgtIter != rcaTarget.end(); ++ tgtIter){

			if((*tgtIter)->state() != STA_OVER){

				canBeFree = false;
				break;
			}
		}

		if(canBeFree)RIM.free(thisRCA->seqNo());
	}
}

//////////////////////////////////////////////////////////////////////////

#define RCA_BYPASS_SUM      128

int CL1Config::sumPseudoRCA(){

	int sum = 0;
	for(int i =0; i <MAX_AREA_SUM; i = i + 2){

		int dataSum = outAreaCounter[i] + outAreaCounter[i+1];
		sum += (dataSum > RCA_BYPASS_SUM)? 2:1;
	}

	return sum;
}

/* use one pseudoRCA to tansfer data in one area (area 0 or 1 or 2 or 3... ) */
#define TRANS_MODE_0	0 
/* use two pseudoRCAs to tansfer data in two area (area 0、1 or 2、3 or 4、5) */
#define TRANS_MODE_1	1 

const Vector<CL1Block> 
CL1Config::insertPseudoRCA(RPUConfig & config){
	//FIXME
	using std::cout;using std::endl;
	cout<<"insertPseudoRCA"<<endl;
	Vector<CL1Block> pseudoBlock;

	int transferMode = TRANS_MODE_1;
	for(int i =0; i <MAX_AREA_SUM; ++ i){
		/* the 0th、2th、4th pseudo RCA...*/
		bool isUpArea = (i & 0x01)? false : true;
		/* the 1th、3th、5th pseudo RCA... */
		bool isDownArea = !isUpArea;

		// Reset transfer mode
		if(isUpArea){

			const int numData0 = outAreaCounter[i];
			const int numData1 = outAreaCounter[i+1];

			const int numData = numData0 + numData1;

			if(numData == 0){
				++ i; // skip the down area
				continue;
			}

			transferMode = (numData0 + numData1 > RCA_BYPASS_SUM)?
							TRANS_MODE_1 : TRANS_MODE_0;
		}

		if( (transferMode == TRANS_MODE_0) && isDownArea) continue;

		RCA * thisRCA = config.newRCA();

		assert(thisRCA != 0);

		Vector<RCAPort> & rcaInports = thisRCA->inports();
		Vector<RCAPort> & rcaOutports = thisRCA->outports();

		// Judge the data in out region of RIM whether is in 
		// data source area of this RCA.
		Vector<RCAPort*>::iterator outIter;

		const int edgeRow = RIM_HEIGHT/2;

		int rcIndex = 0;
		for(outIter = outPortInRIM.begin(); 
			outIter != outPortInRIM.end(); ++ outIter){

				RCAPort * thisPort = * outIter;
				
				const int colIndex = RIM_WIDTH - 1 - thisPort->RIMCol();
				bool colBelongTo = (i/2 * BYTE_PER_LINE <= colIndex) 
									&& (colIndex < (i/2+1) * BYTE_PER_LINE);

				
				bool rowBelongTo = isUpArea ? thisPort->RIMRow() < edgeRow 
											:thisPort->RIMRow() >= edgeRow;

				if( colBelongTo && rowBelongTo ){


					// Add new inport and outport to pseudo-RCA
					rcaInports.push_back(*thisPort);
					rcaInports.back().setRCA(thisRCA);

					rcaOutports.push_back(*thisPort);
					rcaOutports.back().setRCA(thisRCA);

					// Add bypass node to pseudo-RCA
					DFGNode * bypNode = config.newBypsNode();
					bypNode->addSource(thisPort->dfgPort());
					bypNode->addTarget(thisPort->dfgPort());
					
					if(rcIndex < RC_TEMP_NUM)
						thisRCA->temp(rcIndex).setDFGNode(bypNode);
					else
						thisRCA->node(rcIndex - RC_TEMP_NUM).setDFGNode(bypNode);

					++ rcIndex;
				}
		}

		// Set port loction in RIF and ROF
		Vector<RCAPort>::iterator portIter;

		for(portIter = rcaInports.begin();
			portIter != rcaInports.end(); ++ portIter){

				const int RIFIndex = portIter->RIMRow() * BYTE_PER_LINE
								   + (i + 1) * BYTE_PER_LINE 
								   - (RIM_WIDTH - portIter->RIMCol());

				portIter->setRIFRow(RIFIndex / FIFO_WIDTH);
				portIter->setRIFCol(RIFIndex % FIFO_WIDTH);
		}

		int ROFIndex = 0;

		for(portIter = rcaOutports.begin();
			portIter != rcaOutports.end(); ++ portIter){

				portIter->setRIFRow(ROFIndex / FIFO_WIDTH);
				portIter->setRIFCol(ROFIndex % FIFO_WIDTH);

				DFGPort * port = portIter->dfgPort(); 
				assert(port != 0);

				port->setSSRAMAddress(SSRAMOutBaseAddr + ROFIndex);

				++ ROFIndex;
		}

		const int ROFHeight = ROFIndex/FIFO_WIDTH 
							+ ((ROFIndex % FIFO_WIDTH)? 1 : 0);
		
		SSRAMOutBaseAddr +=  ROFHeight * FIFO_WIDTH ;
		SSRAMOutTopAddr = SSRAMOutBaseAddr;

		//     Generate the pseudo-block
		//-------------------------------------------------------

		CL1Block block;

		const int baseAddr = 
			(transferMode == TRANS_MODE_1 && isDownArea) ? 
			edgeRow : 0;

		const int height = (transferMode == TRANS_MODE_0)? 
							RIM_HEIGHT : RIM_HEIGHT/2;

		CL1Data & CIDLData = block.CIDLData();
		CIDLData.setBaseAddress(baseAddr);
		CIDLData.setLength(BYTE_PER_LINE);
		CIDLData.setHeight(height);
		CIDLData.setOffset(RIM_WIDTH - (i/2+1)*BYTE_PER_LINE);
		cout<<"the "<<i<<" th pseudo CIDLoffset is "<<(RIM_WIDTH - (i/2+1)*BYTE_PER_LINE)<<endl;
		CIDLData.setInputMode(MODE_IN_T2D);
		CIDLData.setOutputMode(MODE_OUT_4L);

		CL1Data & CDSData = block.CDSData();
		CDSData.setTarget(CDS_TGT_ESDF);
		cout<<"set target CDS_TGT_ESDF"<<endl;
		CDSData.setHeight(ROFHeight);

		block.setRCAIndex(thisRCA->seqNo());
		block.setRCACoreLoop(1);
		block.setConst1Address(0);
		block.setConst2Address(0);
		block.setRIDLEnable(false);
		block.setCEDLEnable(false);
		block.setCIDLEnable(true);

		pseudoBlock.push_back(block);
	}

	return pseudoBlock;
}

///////////////////////////////////////////////////////////////

const Vector<CL1Block> 
CL1Config::genCL1Block( RPUConfig & config, Vector<RCA*> & rcas){

	// Initialize the schedule states
	Vector<RCA*>::iterator rcaIter;

	for(rcaIter = rcas.begin(); 
		rcaIter != rcas.end(); ++ rcaIter){

			(*rcaIter)->setState(STA_WAIT);
	}

	// Initialize RIM
	RIM.clear();

	// Schedule begin
	Vector<CL1Block> blockMapped;

	while(!scheduleOver(rcas)){

		updateRCAState(rcas);
		/* map RCA which has got ready */
		const Vector<CL1Block> 
			blocks = mapRCA(readyRCA(rcas));

		assert(!blocks.empty());

		freeRIMSpace(rcas);

		// Append the block mapped
		Vector<CL1Block>::const_iterator blockIter;

		for(blockIter = blocks.begin(); 
			blockIter != blocks.end(); ++ blockIter){

			blockMapped.push_back(*blockIter);
		}
	}

	// Append the pseudo-RCA
	const Vector<CL1Block> 
		pseudoBlock = insertPseudoRCA(config);

	Vector<CL1Block>::const_iterator blockIter;

	for(blockIter = pseudoBlock.begin(); 
		blockIter != pseudoBlock.end(); ++ blockIter){

		blockMapped.push_back(*blockIter);
	}

	return blockMapped;	
}

//////////////////////////////////////////////////////////////
/* generate config words : RCContex、CEDL、CIDL、CDS */
const Vector<reg32> & CL1Config::genRegs(const Vector<CL1Block> & blocks){

	CL1Regs CL1ConfigRegs;

	CL1CoreLength = static_cast<int>(blocks.size());
	assert(CL1CoreLength != 0);

	Vector<CL1Block>::const_iterator iter, end;

	CL1RIDLLength = 0;

	int CoreIndex = 0;	
	for(iter = blocks.begin(); iter != blocks.end(); ++ iter, ++CoreIndex){
		
		const CL1Block & curBlock = *iter;

		// Set RIDL Context
		if(curBlock.RIDLEnable()){

			RIDLReg & curRIDLReg = CL1ConfigRegs.RIDLContext(CL1RIDLLength++);

			const CL1Data & data = curBlock.RIDLData();

			curRIDLReg.setSourceType(data.source());
			curRIDLReg.setInputBaseAddress(data.baseAddress());
			curRIDLReg.setInputLength(data.length());
			curRIDLReg.setInputHeight(data.height());
			curRIDLReg.setInputMode(data.inputMode());

			if(curRIDLReg.targetType() == RIDL_TGT_RIM){

				//!TODO: Allocate CL1RIM Space ...
			}
		}

		// Set CoreContext
		//--------------------------------------------------------------

		// (1) Core
		RCACoreReg & curCoreReg = CL1ConfigRegs.RCContext(CoreIndex);

		curCoreReg.setCoreIndex(curBlock.RCAIndex());
		curCoreReg.setCoreLoop(curBlock.RCACoreLoop());
		curCoreReg.setConst1Address(curBlock.const1Address());
		curCoreReg.setConst2Address(curBlock.const2Address());
		curCoreReg.setCEDLEnable(curBlock.CEDLEnable());
		curCoreReg.setCIDLEnable(curBlock.CIDLEnable());

		// (2) CEDL
		if(curBlock.CEDLEnable()){

			CEDLReg & curCEDLReg = CL1ConfigRegs.CEDLContext(CoreIndex);

			const CL1Data & data = curBlock.CEDLData();

			curCEDLReg.setTargetType(data.target());
			curCEDLReg.setDataSum(data.height());
		}

		// (3) CIDL
		if(curBlock.CIDLEnable()){
			
			CIDLReg & curCIDLReg = CL1ConfigRegs.CIDLContext(CoreIndex);

			const CL1Data & dataCIDL = curBlock.CIDLData();

			curCIDLReg.reset();
			curCIDLReg.setInputMode(dataCIDL.inputMode());
			curCIDLReg.setInput1BaseAddress(dataCIDL.baseAddress());
			curCIDLReg.setInput1Length(dataCIDL.length());
			curCIDLReg.setInputHeight(dataCIDL.height());
			
			/* merge four line to one */
			curCIDLReg.setOutputMode(dataCIDL.outputMode()); 
			curCIDLReg.setInputOffset(dataCIDL.offset());
			cout<<"dataCIDL.offset["<<CoreIndex<<"] is "<<dataCIDL.offset()<<endl;
		}

		// (4) CDS
		CDSReg & curCDSReg = CL1ConfigRegs.CDSContext(CoreIndex);

		const CL1Data & dataCDS = curBlock.CDSData();

		int CDSTarget = dataCDS.target();
		curCDSReg.setTargetType(CDSTarget);

		curCDSReg.setDataSum(dataCDS.height());
		switch(CDSTarget){
			case CDS_TGT_RIF: break;
			case CDS_TGT_RIM: 
				curCDSReg.setRIMAddress(dataCDS.baseAddress());
				curCDSReg.setRIMLength(dataCDS.length());
				curCDSReg.setRIMOffset(dataCDS.offset());
				break;
			case CDS_TGT_ESDF: /* Add code here */ break;
			case CDS_TGT_MB: /* Add code here */ break;
			case CDS_TGT_RB: /* Add code here */ break;
			default: assert(0); // Unknown target;
		}
	}

	CL1TotalLength = 1 + CL1RIDLLength + 4 * CL1CoreLength;

	// Set GroupContext
	GroupHeadReg & groupReg = CL1ConfigRegs.groupContext();

	groupReg.reset();
	
	if(CL1RIDLLength != 0){
		groupReg.setRIDLSum(CL1RIDLLength);
		groupReg.setRIDLEnable(true);
	}

	groupReg.setRCASum(CL1CoreLength);

	// Generate Hardware registors

	Regs.push_back(groupReg.reg());

	for(int i =0; i <CL1RIDLLength; ++i)
		Regs.push_back( CL1ConfigRegs.RIDLContext(i).reg() );

	for(int i =0; i <CL1CoreLength; ++i){

		Regs.push_back( CL1ConfigRegs.RCContext(i).reg() );
		Regs.push_back( CL1ConfigRegs.CEDLContext(i).reg() );
		Regs.push_back( CL1ConfigRegs.CIDLContext(i).reg() );
		Regs.push_back( CL1ConfigRegs.CDSContext(i).reg() );
	}

	return Regs;
}

