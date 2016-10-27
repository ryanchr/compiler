

#ifndef DFGPORT_H
#define DFGPORT_H

#include "dfgvex.h"
#include <iostream>


class DFGPort: public DFGVex{

public:

	DFGPort(): portSeqNo(-1), portSSRAMAddr(-1){}

	//---------------------------------------------------

	int seqNo() const { return portSeqNo; }

	void setSeqNo(int number) { portSeqNo = number; }

	// Hardware Address
	int SSRAMAddress() const { return portSSRAMAddr; }

	void setSSRAMAddress(int addr) { portSSRAMAddr = addr; }

private:

	int portSeqNo;

	int portSSRAMAddr;
};


class DFGImmPort: public DFGPort {

public:

	DFGImmPort() {}

	DFGImmPort(int immediate): immdt(immediate) {}

	int value() const { return immdt; }

	void setValue(int value) { immdt = value; }

	DFGVex * copy() const { return new DFGImmPort(*this); }

	void print(std::ostream & out) const { out <<immdt; }

private:

	int immdt;
};


class DFGVarPort: public DFGPort{

public:

	DFGVarPort() {}

	DFGVarPort(const String & name): varName(name) {}

	const String & name() const { return varName; }

	void setName(const String & name) { varName = name; }

	DFGVex * copy() const { return new DFGVarPort(*this); }

	void print(std::ostream & out) const { out<<varName; }

private:

	String varName;
};


#endif
