
#ifndef DEFINE_H
#define DEFINE_H

#include "systemc.h"
#include "stdio.h"



#define N 160
#define M 8





#define INFILENAME         "in_data.txt"   
#define OUTFILENAME_GOLDEN       "out_data_golden.txt"
#define OUTFILENAME_LARC_GOLDEN       "out_data_larc_golden.txt"
#define OUTFILENAME              "out_data.txt"
#define OUTFILENAME_LARC              "out_data_larc.txt"
#define DIFFFILENAME             "gsmdiff.txt"



typedef short word;		/* 16 bit signed int    */
typedef long longword;		/* 32 bit signed int    */

#define	MIN_WORD	((-32767)-1)
#define	MAX_WORD	( 32767)

#define	SASR(x, by)	((x) >> (by))

#define GSM_MULT_R(a, b)	gsm_mult_r(a, b)
#define GSM_MULT(a, b)		gsm_mult(a, b)
#define GSM_ADD(a, b)		gsm_add(a, b)
#define GSM_ABS(a)		gsm_abs(a)

	
#endif

