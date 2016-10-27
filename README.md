# A compiler for a 2D systolic array

A high level overview of the systolic array:

External RAM <--> Systolic Array

                       |
                       
                   Local RAM

**Compiler workflow**

Input: C source code

Output: Configuration bits for 2D systolic array

Steps:

1. code partition

2. generation of data flow graph (DFG) using SUIF (standford)

3. DFG partition

4. mapping DFGs onto the 2D systolic array

5. generation of the configuration bits

**Code review**

Class names and function:

1. DFGVex, a node in the DFG

2. DFGNode: public DFGVex, a node enabling lower level operations in the DFG

3. DFGPort: public DFGVex, an input/output port in the DFG

4. DFGImmPort: public DFGPort, a port corresponds to an immediate input

5. DFGVarPort: public DFGPort, a port corresponds to a variable

6. DFGraph, a complete DFG

7. RCAVex, a node in the 2D systolic array

8. RCAPort: public RCAVex, an input/output port in the 2D systolic array

9. RCANode: public RCAVex, a processing element (PE) in the 2D systolic array

10. RCA, describes the structure of the 2D systolic array

11. RPUConfig, module for generating the configuration bits

12. OptimizeMethod, class for DFG optimizations

13. SplitMethod, class for DFG partition

14. CL0Reg, CL0 registers

15. CL1Reg, CL1 registers

16. CL2Reg, CL2 registers

17. CL1Config, CL1 configurations

18. CL1Data, data flow in the CL1

19. CL1Block, hardware block of CL1

20. L1RIM, class for memory space allocation

**Contents**

1. include: header files folder

2. src: source files folder

3. doc: project documents

4. lib: *.o files folder

5. bin: executable file

6. test: testbench files



 
