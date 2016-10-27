
#ifndef RCAVEX_H
#define RCAVEX_H

class RCA;

#include "dfgport.h"
#include "dfgnode.h"

class RCAVex{

public:

	RCAVex (RCA * rca =0): vexRCA(rca) {}

	RCA * rca() const { return vexRCA; }

	void setRCA(RCA* rca) { vexRCA = rca; }

private:

	RCA * vexRCA;

};

//-------------------------------------------------

class RCAPort: public RCAVex{      //RCAPort in one RCA,one to a DFGport

public:

	RCAPort(): portRIFRow(0), portRIFCol(0),
		portROFRow(0), portROFCol(0),
		portRIMRow(0), portRIMCol(0),
		rcaDFGPort(0) {}

	RCAPort(DFGPort * port): rcaDFGPort(port){}

	// Hardware location
	int RIFRow() const { return portRIFRow; }

	void setRIFRow(int row) { portRIFRow = row; }

	int RIFCol() const { return portRIFCol; }

	void setRIFCol(int col) { portRIFCol = col; }

	int ROFRow() const { return portROFRow; }

	void setROFRow(int row) { portROFRow = row; }

	int ROFCol() const { return portROFCol; }

	void setROFCol(int col) { portROFCol = col; }

	int RIMRow() const { return portRIMRow; }

	void setRIMRow(int row) { portRIMRow = row; }

	int RIMCol() const { return portRIMCol; }

	void setRIMCol(int col) { portRIMCol = col; }

	//------------------------------------------------

	DFGPort * dfgPort() const { return rcaDFGPort; }

	void setDFGPort(DFGPort * port) { rcaDFGPort = port; }

private:

	int portRIFRow, portRIFCol;    //

	int portROFRow, portROFCol;

	int portRIMRow, portRIMCol;
	
	DFGPort * rcaDFGPort;
};

//--------------------------------------------------

class RCANode: public RCAVex {

public:

	RCANode(DFGNode * node = 0): //nodeColor(0), 
	  rcaDFGNode(node){}

	int row() const { return rcaRow; }

	void setRow(int row) { rcaRow = row; }

	int col() const { return rcaCol; }

	void setCol(int col) { rcaCol = col; }

	DFGNode * dfgNode() const { return rcaDFGNode; }

	void setDFGNode(DFGNode * node) { 
		
		rcaDFGNode = node; 
		if(node != 0){
			node->setRCANode(this);     //link this RCANode to DFGNode "node"
			node->setPlaced(true);
		}
	}

private:

	int rcaRow, rcaCol;      //the location in physical RCA

	DFGNode * rcaDFGNode;    //a point to DFGnode associated
};

#endif
