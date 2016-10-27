
#ifndef MAT_MUL_NODE_2_H
#define MAT_MUL_NODE_2_H

#include "mat_mul_NODE_2_cl0.h"

#define MAT_MUL_NODE_2_RCA_FUC	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 0, mat_mul._mat_mul4_4Tmp2, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 1, mat_mul._mat_mul4_4Tmp3, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 2, mat_mul._mat_mul4_4Tmp4, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 3, mat_mul._mat_mul4_4Tmp5, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 4, mat_mul._mat_mul4_4Tmp6, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 5, mat_mul._mat_mul4_4Tmp7, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 32, mat_mul._mat_mul4_4Tmp8, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 33, mat_mul._mat_mul4_4Tmp9, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 6, mat_mul._mat_mul4_4Tmp11, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 7, mat_mul._mat_mul4_4Tmp12, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 8, mat_mul._mat_mul4_4Tmp13, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 34, mat_mul._mat_mul4_4Tmp14, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 9, mat_mul._mat_mul4_4Tmp16, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 10, mat_mul._mat_mul4_4Tmp17, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 11, mat_mul._mat_mul4_4Tmp18, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 35, mat_mul._mat_mul4_4Tmp19, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 12, mat_mul._mat_mul4_4Tmp21, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 13, mat_mul._mat_mul4_4Tmp22, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 14, mat_mul._mat_mul4_4Tmp23, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 36, mat_mul._mat_mul4_4Tmp24, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 15, mat_mul._mat_mul4_4Tmp26, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 16, mat_mul._mat_mul4_4Tmp27, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 17, mat_mul._mat_mul4_4Tmp28, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 37, mat_mul._mat_mul4_4Tmp29, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 18, mat_mul._mat_mul4_4Tmp34, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 19, mat_mul._mat_mul4_4Tmp35, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 20, mat_mul._mat_mul4_4Tmp36, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 38, mat_mul._mat_mul4_4Tmp37, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 21, mat_mul._mat_mul4_4Tmp42, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 22, mat_mul._mat_mul4_4Tmp43, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 23, mat_mul._mat_mul4_4Tmp44, 1);	\
		writeSSRAM(MAT_MUL_NODE_2_LOAD_BASE + 39, mat_mul._mat_mul4_4Tmp45, 1);	\
		\
		writeCL0(mat_mul_NODE_2_cl0, MAT_MUL_NODE_2_CL0_SIZE);	\
		\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 8, mat_mul._mat_mul4_4Tmp10, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 9, mat_mul._mat_mul4_4Tmp15, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 10, mat_mul._mat_mul4_4Tmp20, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 11, mat_mul._mat_mul4_4Tmp25, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 0, mat_mul._mat_mul4_4Tmp30, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 1, mat_mul._mat_mul4_4Tmp31, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 2, mat_mul._mat_mul4_4Tmp32, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 3, mat_mul._mat_mul4_4Tmp33, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 4, mat_mul._mat_mul4_4Tmp38, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 5, mat_mul._mat_mul4_4Tmp39, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 6, mat_mul._mat_mul4_4Tmp40, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 7, mat_mul._mat_mul4_4Tmp41, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 32, mat_mul._mat_mul4_4Tmp46, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 33, mat_mul._mat_mul4_4Tmp47, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 34, mat_mul._mat_mul4_4Tmp48, 1);	\
		readSSRAM(MAT_MUL_NODE_2_STORE_BASE + 35, mat_mul._mat_mul4_4Tmp49, 1);	\

#endif

