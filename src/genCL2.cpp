
#include "cl2reg.h"
#include "rpucfg.h"
using std::cout;
using std::endl;

int RPUConfig::genCL2Context(){
	
	List<Ptr<RCA> >::iterator rcaIter = rcaList.begin();
	
	for(; rcaIter != rcaList.end(); ++ rcaIter){

		RCA * thisRCA = rcaIter->get();

		CL2Reg CL2Contex;
		
	/* RCA Array control information */
	    int inputTimes = 0, outputTimes = 0, outputBeginCycle = 0, outputBegin = 0
	    , LoopInterval = 0, ConstOut0Flag = 0, LoopTimes = 0
	    , ConstOut1Flag = 0, OutputGrain = 0, RIFSign = 0, InputAGrain = 0
	    , InputBGrain = 0, InputTempGrain = 0, InputGrainMode = 0;	

		// Set RC and Temp inter-connection context
		/////////////////////////////////////////////////////////////

		for(int i =0; i <RC_REG_NUM; ++ i){

			RCANode & curRCANode = thisRCA->node(i);
			RCANode & curRCATemp = thisRCA->temp(i);

			DFGNode * thisNode = curRCANode.dfgNode();
			DFGNode * thisTemp = curRCATemp.dfgNode();

			// Set RC context
			//==================================================
			

			if(thisNode == 0){ // This RC not used

				CL2Contex.setInputA(i, 0, SRC_TYPE_FIFO);
				CL2Contex.setInputB(i, 0, SRC_TYPE_FIFO);
				CL2Contex.setRCOutEnable(i, false);
			} else 
		{

				// (1) Set A, B input
				//--------------------------------------------

				DFGVex * srcA = 0;
				DFGVex * srcB = 0;
				
				switch(thisNode->sourceSize()){
					case 2: srcB = thisNode->sources(1);
					case 1: srcA = thisNode->sources(0); break;
					default: assert(0);
				}

				assert(srcA != 0);

				// (i) Set inputA

				bool isNode = (typeid(*srcA) == typeid(DFGNode));

				if(isNode) {
					/* check input source :node or fifo */
					RCANode * srcRCANode = static_cast<DFGNode*>(srcA)->rcaNode();
			
					// Input -- RC: 0-7; Temp 8-15;
					CL2Contex.setInputA(i, srcRCANode->col(), SRC_TYPE_LAST);
				} else { // It's a port
					int inputA;

					Vector<RCAPort> & rcaInport = thisRCA->inports();

					int inSize = static_cast<int>(rcaInport.size());
					for(inputA =0; inputA <inSize; ++ inputA){

						if(rcaInport[inputA].dfgPort() == srcA)break;
					}

					assert(inputA != static_cast<int>(rcaInport.size())); // Must can be found

					RCAPort & srcAPort = rcaInport[inputA];

					CL2Contex.setInputA(
						i, 
						srcAPort.RIFRow() * FIFO_WIDTH 
							+ srcAPort.RIFCol(), 
						SRC_TYPE_FIFO
					);

					/* Get the max RIFRow for ControlReg*/
					inputTimes = (inputTimes>(( (CL2Contex.inputA(i)) & 0xe0 )>>5) 
								  ? inputTimes
								  : (( (CL2Contex.inputA(i)) & 0xe0 )>>5) );
					LoopInterval = i/8;
				}

				//(ii) Set inputB

				if(srcB != 0){ // It must be a bypass node

					bool isNode = (typeid(*srcB) == typeid(DFGNode));

					if(isNode) {

						RCANode * srcRCANode = static_cast<DFGNode*>(srcB)->rcaNode();

						// Input -- RC: 0-7; Temp 8-15;
						CL2Contex.setInputB(i, srcRCANode->col(), SRC_TYPE_LAST);
					} else { // It's a port
						int inputB;

						Vector<RCAPort> & rcaInport = thisRCA->inports();
						int inSize = static_cast<int>(rcaInport.size());
						for(inputB =0; inputB <inSize; ++ inputB){

							if(rcaInport[inputB].dfgPort() == srcB)break;
						}

						assert(inputB != static_cast<int>(rcaInport.size())); // Must can be found

						RCAPort & srcBPort = rcaInport[inputB];

						CL2Contex.setInputB(
							i, 
							srcBPort.RIFRow() * FIFO_WIDTH 
								+ srcBPort.RIFCol(), 
							SRC_TYPE_FIFO
						);

					/* Get the max RIFRow*/
					inputTimes = (inputTimes > (( (CL2Contex.inputB(i)) & 0xe0 )>>5)
								  ? inputTimes
								  :  (((CL2Contex.inputB(i)) & 0xe0 )>>5) );
					LoopInterval = i/8;
		
					}
				}

				// (2) Set RC output
				//-----------------------------------------------------------------

				DFGPort * nodeOutport = 0;

				/* traverse targets and check whether it is a dfgport */

				for(int t =0; t <thisNode->targetSize(); ++t){

					DFGVex * thisTgt = thisNode->targets(t);

					bool isNode = (typeid(*thisTgt) == typeid(DFGNode) );

					if(!isNode){

						nodeOutport = static_cast<DFGPort*>(thisTgt);
						break;
					}
				}

				if(nodeOutport != 0){

					int outIndex = 0;
					const Vector<RCAPort> & rcaOutput = thisRCA->outports();
					Vector<RCAPort>::const_iterator outIter = rcaOutput.begin();

					for(; outIter != rcaOutput.end(); ++ outIter, ++outIndex){

						if(outIter->dfgPort() == nodeOutport)break;
					}

					assert(outIter != rcaOutput.end()); // Must can be found

					RCAPort tgtRCAPort = rcaOutput[outIndex];

					CL2Contex.setRCOut(i, 
						tgtRCAPort.ROFRow() * FIFO_WIDTH + tgtRCAPort.ROFCol()
					);

					CL2Contex.setRCOutEnable(i, true);
					/* get the total row of output data */
					outputTimes = (outputTimes > (tgtRCAPort.ROFRow())
											? outputTimes
											: (tgtRCAPort.ROFRow()) ) ;
					if (outputBegin == 0)
						{
						outputBeginCycle = i/8;
						outputBegin = 1;
						}
				}

				// (3) Set opecode
				//-----------------------------------------------------------------

				CL2Contex.setRCOpecode(i, thisNode->opecode());
		} // End set RC context

		// Set Temp context
		//=====================================================================

		if(thisTemp == 0){

				CL2Contex.setInputTemp(i, 0, SRC_TYPE_FIFO);
				CL2Contex.setTempOutEnable(i, false);

		} else 
		{
				// (1) Set temp input

				assert(thisTemp->sources().size() == 1); // It must be a bypass
				DFGVex * srcTemp = thisTemp->sources(0);


				bool isNode = (typeid(*srcTemp) == typeid(DFGNode));

				if(isNode) {

					RCANode * srcRCANode = static_cast<DFGNode*>(srcTemp)->rcaNode();

					// Input -- RC: 0-7; Temp 8-15;
					CL2Contex.setInputTemp(i, srcRCANode->col(), SRC_TYPE_LAST);
				} else { // It's a port
					int inputTemp;

					Vector<RCAPort> & rcaInport = thisRCA->inports();

					int inSize = static_cast<int>(rcaInport.size());

					for(inputTemp =0; inputTemp <inSize; ++ inputTemp){

						if(rcaInport[inputTemp].dfgPort() == srcTemp)break;
					}

					assert(inputTemp != static_cast<int>(rcaInport.size())); // Must can be found

					RCAPort & srcTempPort = rcaInport[inputTemp];

					CL2Contex.setInputTemp(
						i, 
						srcTempPort.RIFRow() * FIFO_WIDTH 
							+ srcTempPort.RIFCol(), 
						SRC_TYPE_FIFO
					);

				/* Get the max RIFRow*/
				inputTimes = (inputTimes>(( (CL2Contex.inputTemp(i)) & 0xe0 )>>5)
							 ? inputTimes
							 : (( (CL2Contex.inputTemp(i)) & 0xe0 )>>5) );
				
				LoopInterval = i/8;
				if (outputBegin == 0)
						{
						outputBeginCycle = i/8;
						outputBegin = 1;
						}
				}

				// (2) Set Temp output
				//-----------------------------------------------------------------

				DFGPort * tempOutport = 0;

				for(int t =0; t <thisTemp->targetSize(); ++t){

					DFGVex * thisTgt = thisTemp->targets(t);

					bool isNode = (typeid(*thisTgt) == typeid(DFGNode) );

					if(!isNode){

						tempOutport = static_cast<DFGPort*>(thisTgt);
						break;
					}
				}

				if(tempOutport != 0){

					int outIndex = 0;
					const Vector<RCAPort> & rcaOutput = thisRCA->outports();
					Vector<RCAPort>::const_iterator outIter = rcaOutput.begin();

					for(; outIter != rcaOutput.end(); ++ outIter, ++outIndex){

						if(outIter->dfgPort() == tempOutport)break;
					}

					assert(outIter != rcaOutput.end()); // Must can be found

					RCAPort tgtRCAPort = rcaOutput[outIndex];

					CL2Contex.setTempOut(i, 
						tgtRCAPort.ROFRow() * FIFO_WIDTH + tgtRCAPort.ROFCol()
					);

					CL2Contex.setTempOutEnable(i, true);

					outputTimes = (outputTimes > (tgtRCAPort.ROFRow())
											? outputTimes
											: (tgtRCAPort.ROFRow()) );
					if (outputBegin == 0)
						{
						outputBeginCycle = i/8;
						outputBegin = 1;
						}
				}
		
		} // End set Temp context

		}

		// Set control context
		///////////////////////////////////////////////////////////////////////

		/* Generate the controlReg */
		{
			/* set input times ;
			   input_times = total rows of input data in RIF during a cycle ; */
			CL2Contex.CL2ControlReg.setInputTimes(inputTimes);
			
			/* set loop interval ;
			   loopinterval = max level of input data in DFG ; */
  		    CL2Contex.CL2ControlReg.setLoopInterval(LoopInterval);
			
			/* set output times ;
			   output_times =  total rows of output data during a cycle ; */
            CL2Contex.CL2ControlReg.setOutputTimes(outputTimes);
			
			/* set output begin time ;
			   output_begin = level of first output data in DFG */
			CL2Contex.CL2ControlReg.setOutputCycle(outputBeginCycle);
			
			/* set the loop times */
		    //LoopTimes = thisRCA.loopNum();
			CL2Contex.CL2ControlReg.setLoop(LoopTimes);
			
			/* set data grain */
			CL2Contex.CL2ControlReg.setInputAGran(InputAGrain);
			CL2Contex.CL2ControlReg.setInputBGran(InputBGrain);
			CL2Contex.CL2ControlReg.setInputTempGran(InputTempGrain);
			CL2Contex.CL2ControlReg.setInputGranMode(InputGrainMode);
			
			OutputGrain = 0x0;     	// default : 8bit 
			CL2Contex.CL2ControlReg.setOutputGran(OutputGrain);
			
			/* set RIMSignMode */
			CL2Contex.CL2ControlReg.setRIFSignMode(RIFSign);

			/* set constgroup controlling information */
			/* FIXME: Constant Group is still unused */
			//CL2Contex.CL2ControlReg.setOutCG0Flag();  
			//CL2Contex.CL2ControlReg.setOutCG1Flag();
		}
		///////////////////////////////////////////////////////////////////////

		Vector<reg32> context;
		context.reserve(CL2_CONTEXT_LEN );

		for(int i =0; i <IN_OP_REG_SUM; ++i)
			context.push_back(CL2Contex.inputReg(i).reg());

		for(int i =0; i <OUT_REG_SUM; ++i)
			context.push_back(CL2Contex.outputReg(i).reg());

		for(int i =0; i <OUT_EN_SUM; ++i)
			context.push_back(CL2Contex.outEnableReg(i).reg());

		context.push_back(CL2Contex.controlReg().reg());

		for(int i =0; i <IN_HBIT_SUM; ++i)
			context.push_back(CL2Contex.inputHBitReg(i).reg());

		CL2Context.push_back(context);
	}

	return 0;
}

