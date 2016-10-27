
#include "cl1.h"
#include <cassert>
//FIXME
#include <iostream>

CL1RIM::CL1RIM(){

	for(int i =0; i < RIM_HEIGHT; ++ i){
		memoryRecord[i] = -1;
		freeOutSpace[i] = OUT_REGION_WIDTH; 
	}

}

CL1Data CL1RIM::allocate(int RCAIndex, int tempNum, int outNum){

	assert(RCAIndex >= 0);

	// Search the best consecutive free space
	int bestHeight = -1;
	int bestBaseAddr = -1;
	int bestMinOutLength = 0;
	int minWastSpace = 1024;    // MAX space of CL1RIM
	
	/* minimal height of temp data in RIM TEMP region */
	int minTempHeight = tempNum/TEMP_REGION_WIDTH 
		+ (tempNum%TEMP_REGION_WIDTH? 1: 0);

	if(minTempHeight == 0)minTempHeight = 1;

	for(int height = minTempHeight; height <RIM_HEIGHT; ++ height){
		//FIXME
		/* outSize: Bytes of each line */
		const int outSize = outNum/height + (outNum%height? 1: 0);
		//const int outSize = outNum/OUT_REGION_WIDTH+ (outNum%OUT_REGION_WIDTH? 1: 0);
		using std::cout;using std::endl;
		//cout<<"rca["<<RCAIndex<<"]'s externalport = "<<outNum<<endl;
		//cout<<"rca["<<RCAIndex<<"]'s height = "<<height<<endl;
		//cout<<"rca["<<RCAIndex<<"]'s outsize = "<<outSize<<endl;
		
		int baseAddr = 0;
		/**/
		for(int i =0; i < RIM_HEIGHT; ++i){
			
			if(memoryRecord[i] != -1              //this line of RIM in used
				|| freeOutSpace[i] < outSize ){   //available Bytes BLT outSize

				baseAddr = i + 1;				  //next line of RIM
			} 
			
			if( i - baseAddr + 1 == height ){
			//cout<<"rca["<<RCAIndex<<"]:  i= "<<i<<endl;
			//cout<<"rca["<<RCAIndex<<"]:  baseAddr= "<<baseAddr<<endl;
			//cout<<"rca["<<RCAIndex<<"]:  height= "<<height<<endl;
			//cout<<"rca["<<RCAIndex<<"]:  i-baseAddr+1= "<<i - baseAddr + 1<<endl;

				// Find the line with minimal free space
				int minOutLength = freeOutSpace[baseAddr];
				for(int j = 0; j <height; ++ j)
					if(freeOutSpace[baseAddr + j] < minOutLength)
						minOutLength = freeOutSpace[baseAddr + j];
				
				// Calculate the wast space
				int wastSpace = 0;
				for(int j =0; j <height; ++ j){

					int curAddr = baseAddr + j;
					wastSpace += freeOutSpace[curAddr] - minOutLength;
				}

				const int wastThreshold = 8; // This variable used to make the data 
											 // more compact in horizontal direction.

				if(wastSpace + wastThreshold < minWastSpace){
					bestBaseAddr = baseAddr;
					bestHeight = height;
					bestMinOutLength = minOutLength;

					minWastSpace = wastSpace;
				}

				++ baseAddr;
			}
			
			
		}
		cout<<"rca["<<RCAIndex<<"]:  bestBaseAddr= "<<bestBaseAddr<<endl;
		cout<<"rca["<<RCAIndex<<"]:  bestHeight = "<<bestHeight<<endl;
		cout<<"rca["<<RCAIndex<<"]:  bestMinOutLength= "<<bestMinOutLength<<endl;
		cout<<"rca["<<RCAIndex<<"]:  minWastSpace= "<<minWastSpace<<endl;
	}

	if(bestBaseAddr != -1){ // Allocate CL1RIM space

		for(int j =0; j <bestHeight; ++ j){

			int curAddr = bestBaseAddr + j;
			memoryRecord[curAddr] = RCAIndex;
			freeOutSpace[curAddr] = bestMinOutLength - outNum/bestHeight - 1;
		}

		for(int j =0; j <outNum % bestHeight; ++ j)
			-- memoryRecord[bestBaseAddr + j];

	}

	// Set Allocated space
	CL1Data data;

	data.setBaseAddress(bestBaseAddr);
	data.setHeight(bestHeight);
	data.setLength(TEMP_REGION_WIDTH + bestMinOutLength);

	return data;
}

CL1Data CL1RIM::getRCACIDLData(int RCAIndex) const{

	assert(RCAIndex > 0);

	// Find the base address
	int baseAddr;
	for(baseAddr =0; baseAddr <RIM_HEIGHT; ++ baseAddr)
		if(memoryRecord[baseAddr] == RCAIndex)break;

	assert(baseAddr <RIM_HEIGHT); // Must can be found!

	// Find the end address
	int endAddr;
	for(endAddr = baseAddr + 1; endAddr <RIM_HEIGHT; ++ endAddr)
		if(memoryRecord[endAddr] != RCAIndex)break;

	int height = endAddr - baseAddr;

	CL1Data CIDLData;

	CIDLData.setBaseAddress(baseAddr);
	CIDLData.setLength(TEMP_REGION_WIDTH);
	CIDLData.setWidth(TEMP_REGION_WIDTH);
	CIDLData.setHeight(height);

	return CIDLData;
}

int CL1RIM::getBaseAddress(int RCAIndex) const {

	for(int i =0; i <RIM_HEIGHT; ++ i)
		if(memoryRecord[i] == RCAIndex)return i;

	return -1;
}

int CL1RIM::getEndAddress(int RCAIndex) const  {

	for(int i =RIM_HEIGHT -1; i >=0; -- i)
		if(memoryRecord[i] == RCAIndex)return i+1;

	return -1;
}

int CL1RIM::getOutBaseAddress() const {

	for(int i =0; i <RIM_HEIGHT; ++ i)
		if(freeOutSpace[i] < OUT_REGION_WIDTH)return i;

	return -1;
}

int CL1RIM::getOutEndAddress() const {

	for(int i =RIM_HEIGHT -1; i >=0; -- i)
		if(freeOutSpace[i] < OUT_REGION_WIDTH)return i+1;

	return -1;
}

int CL1RIM::getOutOffset() const {

	int offset = OUT_REGION_WIDTH;
	for(int i = RIM_HEIGHT -1; i >=0; -- i)
		if( freeOutSpace[i] < offset) offset = freeOutSpace[i];

	return offset;
}

void CL1RIM::free(int RCAIndex){

	int baseAddr;
	for(baseAddr =0; baseAddr <RIM_HEIGHT; ++baseAddr)
		if(memoryRecord[baseAddr] == RCAIndex)break;

	for(; baseAddr <RIM_HEIGHT; ++baseAddr){

		if(memoryRecord[baseAddr] == RCAIndex)
			memoryRecord[baseAddr] = -1;
		else break;
	}
}

void CL1RIM::clear() {

	for(int i =0; i < RIM_HEIGHT; ++ i){
		memoryRecord[i] = -1;
		freeOutSpace[i] = OUT_REGION_WIDTH; 
	}
}

