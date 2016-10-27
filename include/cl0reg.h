// Filename: cl0reg.h
//--------------------------------------------------------------
// Author: Xie Li@ MicroE. SJTU  Time: 2010/08/01
//
// Define the registor operation of CL0, Based on RPU SPEC v1.3
//===============================================================
// Reference: RPU SPEC v1.3.doc, Configuration1.8.doc
//

#ifndef CL0REG_H
#define CL0REG_H

#include "reg.h"
#include <cassert>

#define SYN_MODE_0	0
#define SYN_MODE_1	1
#define SYN_MODE_2	2
#define SYN_MODE_3	3

class CL0HeadReg: public Reg{

public:

	int groupAddress() const;

	void setGroupAdrress(int addr);

	int synMode() const;

	void setSynMode(int mode);

	int constSelect() const;

	void setConstSelect(int sel);

	int constOffset0() const;

	void setConstOffset0(int offset);

	int constOffset1() const;

	void setConstOffset1(int offset);
};

inline int CL0HeadReg::groupAddress() const {

	return static_cast<int>(regst & 0x000007ff);
}

inline void CL0HeadReg::setGroupAdrress(int addr){

	assert(0 <= addr && addr <= 0x7ff);

	regst &= 0xfffff800;
	regst |= static_cast<reg32>(addr);
}

inline int CL0HeadReg::synMode() const{

	return static_cast<int>((regst & 0x0000e000) >> 13);
}

inline void CL0HeadReg::setSynMode(int mode) {

	assert(0 <= mode && mode <= 7);

	regst &= 0xffff1fff;
	regst |= static_cast<reg32>(mode) << 13;
}

inline int CL0HeadReg::constSelect() const{

	return static_cast<int>((regst & 0x00010000) >> 16);
}

inline void CL0HeadReg::setConstSelect(int sel) {

	assert(0 <= sel && sel <= 1);

	regst &= 0x0000e000;
	regst |= static_cast<reg32>(sel) << 16;
}

inline int CL0HeadReg::constOffset0() const {

	return static_cast<int>((regst & 0x003e0000) >> 17);
}

inline void CL0HeadReg::setConstOffset0(int offset){

	assert(0 <= offset && offset <= 0x1f);

	regst &= 0x00c10000;
	regst |= static_cast<reg32>(offset) << 17;
}

inline int CL0HeadReg::constOffset1() const {

	return static_cast<int>((regst & 0xffc00000) >> 22);
}

inline void CL0HeadReg::setConstOffset1(int offset){

	assert(0 <= offset && offset <= 0x3ff);

	regst &= 0x003fffff;
	regst |= static_cast<reg32>(offset) << 22;
}

////////////////////////////////////////////////////////////

#define REDL_MODE_1L	0
#define REDL_MODE_2L	1
#define REDL_MODE_4L	2
#define REDL_MODE_8L	3

//---------------------------------------------------------

class REDLReg0: public Reg{

public:

	int SSRAMAddress() const;

	void setSSRAMAddress(int addr);
};

inline int REDLReg0::SSRAMAddress() const {

	return static_cast<int>(regst & 0x00ffffff);
}

inline void REDLReg0::setSSRAMAddress(int addr) {

	assert(0 <= addr && addr <= 0xffffff);

	regst &= 0xff000000;
	regst |= static_cast<reg32>(addr);
}

//------------------------------------------------

class REDLReg1: public Reg{

public:

	int SSRAMLength() const;

	void setSSRAMLength(int length);

	int SSRAMHeight() const;

	void setSSRAMHeight(int height);

	int SSRAMJump() const;

	void setSSRAMJump(int jump);

	int mode() const;

	void setMode(int mode);
};

inline int REDLReg1::SSRAMLength() const {

	return static_cast<int>(regst & 0x0000001f);
}

inline void REDLReg1::setSSRAMLength(int length){

	assert(0 <= length && length <= 0x1f);

	regst &= 0xffffffe0;
	regst |= static_cast<reg32>(length);
}

inline int REDLReg1::SSRAMHeight() const {

	return static_cast<int>((regst & 0x000003e0) >> 5);
}

inline void REDLReg1::setSSRAMHeight(int height){

	assert(0 <= height && height <= 0x1f);

	regst &= 0xfffffc1f;
	regst |= static_cast<reg32>(height) << 5;
}

inline int REDLReg1::SSRAMJump() const {

	return static_cast<int>((regst & 0x001ffe00) >> 10);
}

inline void REDLReg1::setSSRAMJump(int jump){

	assert(0 <= jump && jump <= 0x7ff);

	regst &= 0xffe003ff;
	regst |= static_cast<reg32>(jump) << 10;
}

inline int REDLReg1::mode() const {

	return static_cast<int>((regst & 0x00600000) >> 21);
}

inline void REDLReg1::setMode(int mode){

	assert(0 <= mode && mode <= 3);

	regst &= 0xff9fffff;
	regst |= static_cast<reg32>(mode) << 21;
}

class REDLReg {

public:

	REDLReg0 & word0() { return REDLWord0; }

	REDLReg1 & word1() { return REDLWord1; }

private:

	REDLReg0 REDLWord0;
	REDLReg1 REDLWord1;
};

/////////////////////////////////////////////////////////

class REDSReg0: public Reg{

public:

	int SSRAMAddress() const;

	void setSSRAMAddress(int addr);
};

inline int REDSReg0::SSRAMAddress() const {

	return static_cast<int>(regst & 0x00ffffff);
}

inline void REDSReg0::setSSRAMAddress(int addr) {

	assert(0 <= addr && addr <= 0xffffff);

	regst &= 0xff000000;
	regst |= static_cast<reg32>(addr);
}

//------------------------------------------------

#define REDS_MODE_ML	0
#define REDS_MODE_DL	1

class REDSReg1: public Reg{

public:

	int SSRAMLength() const;

	void setSSRAMLength(int length);

	int SSRAMHeight() const;

	void setSSRAMHeight(int height);

	int SSRAMJump() const;

	void setSSRAMJump(int jump);

	int mode() const;

	void setMode(int mode);

	int FIFOLineNum() const;

	void setFIFOLineNum(int num);
};

inline int REDSReg1::SSRAMLength() const {

	return static_cast<int>(regst & 0x0000001f);
}

inline void REDSReg1::setSSRAMLength(int length){

	assert(0 <= length && length <= 0x1f);

	regst &= 0xffffffe0;
	regst |= static_cast<reg32>(length);
}

inline int REDSReg1::SSRAMHeight() const {

	return static_cast<int>((regst & 0x000003e0) >> 5);
}

inline void REDSReg1::setSSRAMHeight(int height){

	assert(0 <= height && height <= 0x1f);

	regst &= 0xfffffc1f;
	regst |= static_cast<reg32>(height) << 5;
}

inline int REDSReg1::SSRAMJump() const {

	return static_cast<int>((regst & 0x001ffe00) >> 10);
}

inline void REDSReg1::setSSRAMJump(int jump){

	assert(0 <= jump && jump <= 0x7ff);

	regst &= 0xffe003ff;
	regst |= static_cast<reg32>(jump) << 10;
}

inline int REDSReg1::mode() const {

	return static_cast<int>((regst & 0x00200000) >> 21);
}

inline void REDSReg1::setMode(int mode){

	assert(0 <= mode && mode <= 1);

	regst &= 0xff9fffff;
	regst |= static_cast<reg32>(mode) << 21;
}

inline int REDSReg1::FIFOLineNum() const{

	return static_cast<int>((regst & 0x03e00000) >> 22);
}

inline void REDSReg1::setFIFOLineNum(int num){

	assert(0 <= num && num <= 0xf);

	regst &= 0xfc1fffff;
	regst |= static_cast<reg32>(num) << 22;
}

//--------------------------------------------

class REDSReg {

public:

	REDSReg0 & word0() { return REDSWord0; }

	REDSReg1 & word1() { return REDSWord1; }

private:

	REDSReg0 REDSWord0;
	REDSReg1 REDSWord1;
};

#endif

