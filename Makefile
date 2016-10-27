####################################################
# Make file for REmus v2.0 Compiler project
#===================================================
# Version: 0.01		Time: Aug 5th, 2010
#
# Author: Xie Li @SJTU.MircoE
# Email: xieli@ic.sjtu.edu.cn
#####################################################

PRO_PATH = .

VPATH = $(PRO_PATH)/include $(PRO_PATH)/src

INC_PATH = $(PRO_PATH)/include
SRC_PATH = $(PRO_PATH)/src
LIB_PATH = $(PRO_PATH)/lib
BIN_PATH = $(PRO_PATH)/bin

CC = gcc
CXX = g++

CFLAGS = -Wall

OBJECTS = $(LIB_PATH)/dfgraph.o \
			$(LIB_PATH)/rca.o \
			$(LIB_PATH)/rpucfg.o \
			$(LIB_PATH)/cl1.o	\
			$(LIB_PATH)/cl1cfg.o	\
			$(LIB_PATH)/genCL0.o	\
			$(LIB_PATH)/genCL1.o	\
			$(LIB_PATH)/genCL2.o	\
			$(LIB_PATH)/creatCL0.o	\
			$(LIB_PATH)/creatCL1.o	\
			$(LIB_PATH)/creatCL2.o	\
			$(LIB_PATH)/creatInf.o	\
			$(LIB_PATH)/split.o		\
			$(LIB_PATH)/cntrca.o		\
			$(LIB_PATH)/dfg2src.o

TARGET = $(BIN_PATH)/dfg2src

$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) 
	@echo Compile successful!

$(LIB_PATH)/%.o: $(SRC_PATH)/%.cpp
	$(CXX) -c -I$(INC_PATH) $(CFLAGS) $< -o $@

clean:
	-rm -fr $(OBJECTS)

distclean:
	-rm -fr $(OBJECTS) $(TARGET)

