

#include "gsm.h"
#include "define.h"
#define	saturate(x) 	\
	((x) < MIN_WORD ? MIN_WORD : (x) > MAX_WORD ? MAX_WORD: (x))

word
gsm_add (word a, word b)
{
  longword sum;
  sum=a+b;
return sum;
}

word
gsm_mult (word a, word b)
{
  if (a == MIN_WORD && b == MIN_WORD)
    return MAX_WORD;
  else
    return SASR ((longword) a * (longword) b, 15);
}

word
gsm_mult_r (word a, word b)
{
  longword prod;
  if (b == MIN_WORD && a == MIN_WORD)
    return MAX_WORD;
  else
    {
      prod = (longword) a *(longword) b + 16384;
      prod >>= 15;
      return prod & 0xFFFF;
    }
}

word
gsm_abs (word a)
{
  return a < 0 ? (a == MIN_WORD ? MAX_WORD : -a) : a;
}

const unsigned char bitoff[256] = {
  8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

word
gsm_norm (longword a)

{

  if (a < 0)
    {
      if (a <= -1073741824)
	return 0;
      a = ~a;
    }

  return a & 0xffff0000 ?
    (a & 0xff000000 ? -1 + bitoff[0xFF & (a >> 24)] :
     7 + bitoff[0xFF & (a >> 16)])
    : (a & 0xff00 ? 15 + bitoff[0xFF & (a >> 8)] : 23 + bitoff[0xFF & a]);
}

word
gsm_div (word num, word denum)
{
  longword L_num;
  longword L_denum;
  word div;
  int k;

  L_num = num;
  L_denum = denum;
  div = 0;
  k = 15;
  

  if (num == 0)
    return 0;

  while (k--)
    {
      div <<= 1;
      L_num <<= 1;

      if (L_num >= L_denum)
	{
	  L_num -= L_denum;
	  div++;
	}
    }

  return div;
}




/*
 *  4.2.4 .. 4.2.7 LPC ANALYSIS SECTION
 */

/* 4.2.4 */


void
Autocorrelation (word s[N] /* [0..159]     IN/OUT  */ , word s_out[N],
		 longword L_ACF[M+1] /* [0..8]       OUT     */ )
/*
 *  The goal is to compute the array L_ACF[k].  The signal s[i] must
 *  be scaled in order to avoid an overflow situation.
 */
{
  register int k, i;

  word temp;
  word smax;
  word scalauto, n;


  /*  Search for the maximum.
   */
  smax = 0;
/*Cyber unroll_times=0 */
  for (k = 0; k <= 159; k++)
    {
      temp = GSM_ABS (s[k]);
      if (temp > smax)
	smax = temp;
    }

  /*  Computation of the scaling factor.
   */
   
  if (smax == 0)
    scalauto = 0;
  else
    scalauto = 4 - gsm_norm ((longword) smax << 16);	/* sub(4,..) */

  if (scalauto > 0 && scalauto <= 4)
    {
      n = scalauto;
      /*Cyber unroll_times=0 */
      for (k = 0; k <= 159; k++)
	s[k] = GSM_MULT_R (s[k], 16384 >> (n - 1));
    }
   
  /*  Compute the L_ACF[..].
   */
  {

int current,sl;
#define STEP(k)	 L_ACF[k] += ((longword)s[sl] * s[sl-k]);



#define NEXTI	 sl = ++current;

 /*Cyber unroll_times=0 */   
for (k = 8; k >= 0; k--)
      L_ACF[k] = 0;
    
    current=0;
    sl=0;
    STEP (0);
    NEXTI;
    STEP (0);
    STEP (1);
    NEXTI;
    STEP (0);
    STEP (1);
    STEP (2);
    NEXTI;
    STEP (0);
    STEP (1);
    STEP (2);
    STEP (3);
    NEXTI;
    STEP (0);
    STEP (1);
    STEP (2);
    STEP (3);
    STEP (4);
    NEXTI;
    STEP (0);
    STEP (1);
    STEP (2);
    STEP (3);
    STEP (4);
    STEP (5);
    NEXTI;
    STEP (0);
    STEP (1);
    STEP (2);
    STEP (3);
    STEP (4);
    STEP (5);
    STEP (6);
    NEXTI;
    STEP (0);
    STEP (1);
    STEP (2);
    STEP (3);
    STEP (4);
    STEP (5);
    STEP (6);
    STEP (7);

/*Cyber unroll_times=0 */
    for (i = 8; i <= 159; i++)
      {

	NEXTI;

	STEP (0);
	STEP (1);
	STEP (2);
	STEP (3);
	STEP (4);
	STEP (5);
	STEP (6);
	STEP (7);
	STEP (8);
      }
/*Cyber unroll_times=0 */
    for (k = 8; k >= 0; k--)
      L_ACF[k] <<= 1;

  }
  /*   Rescaling of the array s[0..159]
   */

  if (scalauto > 0)
    /*Cyber unroll_times=0 */
    for (k = 159; k >= 0; k--)
      s[k] <<= scalauto;
   
   /*Cyber unroll_times=0 */
   for (i=0; i<N;i++)
	 s_out[i]=s[i];
  
}

/* 4.2.5 */

void
Reflection_coefficients (longword L_ACF[M+1] ,
			 register word r[M+1] )
{
  register int i, m, n;
  register word temp;
  word ACF[9];			/* 0..8 */
  word P[9];			/* 0..8 */
  word K[9];			/* 2..8 */

  /*  Schur recursion with 16 bits arithmetic.
   */

  if (L_ACF[0] == 0)
    {
/*Cyber unroll_times=0 */
      for (i = 8; i > 0; i--)
		r[i] = 0;
      return;
    }

  temp = gsm_norm (L_ACF[0]);
  sc_int<32> tmp;
/*Cyber unroll_times=0 */
  for (i = 0; i <= 8; i++){
    tmp = L_ACF[i] << temp;
    ACF[i] = (word) SASR (tmp, 16);
  }

  /*   Initialize array P[..] and K[..] for the recursion.
   */
/*Cyber unroll_times=0 */
  for (i = 1; i <= 7; i++)
    K[i] = ACF[i];
/*Cyber unroll_times=0 */
  for (i = 0; i <= 8; i++)
    P[i] = ACF[i];

  /*   Compute reflection coefficients
   */
 /*Cyber unroll_times=0 */
  for (n = 1; n <= 8; n++)
    {

      temp = P[1];
      temp = GSM_ABS (temp);
      if (P[0] < temp)
		{
		/*Cyber unroll_times=0 */
		for (i = n; i <= 8; i++)
			r[i]= 0;
		return;
		}

      r[n] = gsm_div (temp, P[0]);

      if (P[1] > 0)
		r[n] = -r[n];		/* r[n] = sub(0, r[n]) */
      if (n == 8)
		return;
	
      /*  Schur recursion
       */

      temp = GSM_MULT_R (P[1], r[n]);
      P[0] = GSM_ADD (P[0], temp);
      /*Cyber unroll_times=0 */
      for (m = 1; m <= 8 - n; m++)
	{
	  temp = GSM_MULT_R (K[m], r[n]);
	  P[m] = GSM_ADD (P[m + 1], temp);

	  temp = GSM_MULT_R (P[m + 1], r[n]);
	  K[m] = GSM_ADD (K[m], temp);
	}
	 
    }

	
}

/* 4.2.6 */

void
Transformation_to_Log_Area_Ratios (register word r[M] /* 0..7    IN/OUT */ )
/*
 *  The following scaling for r[..] and LAR[..] has been used:
 *
 *  r[..]   = integer( real_r[..]*32768. ); -1 <= real_r < 1.
 *  LAR[..] = integer( real_LAR[..] * 16384 );
 *  with -1.625 <= real_LAR <= 1.625
 */
{
  register word temp;
  register int i;


  /* Computation of the LAR[0..7] from the r[0..7]
   */
/*Cyber unroll_times=0 */
  for (i = 1; i <= 8; i++)
    {

      temp = r[i];
      temp = GSM_ABS (temp);

      if (temp < 22118)
	{
	  temp >>= 1;
	}
      else if (temp < 31130)
	{
	  temp -= 11059;
	}
      else
	{
	  temp -= 26112;
	  temp <<= 2;
	}

      r [i]= r[i] < 0 ? -temp : temp;
    }
}

/* 4.2.7 */

void
Quantization_and_coding (register word LAR[M] /* [0..7]       IN/OUT  */ )
{
  register word temp;


  /*  This procedure needs four tables; the following equations
   *  give the optimum scaling for the constants:
   *  
   *  A[0..7] = integer( real_A[0..7] * 1024 )
   *  B[0..7] = integer( real_B[0..7] *  512 )
   *  MAC[0..7] = maximum of the LARc[0..7]
   *  MIC[0..7] = minimum of the LARc[0..7]
   */
int index=0;
#	undef STEP
#	define	STEP( A, B, MAC, MIC )		\
		temp = GSM_MULT( A,  LAR[index] );	\
		temp = GSM_ADD(  temp,   B );	\
		temp = GSM_ADD(  temp, 256 );	\
		temp = SASR(     temp,   9 );	\
		LAR[index]  =  temp>MAC ? MAC - MIC : (temp<MIC ? 0 : temp - MIC); \
		index++;

  STEP (20480, 0, 31, -32);
  STEP (20480, 0, 31, -32);
  STEP (20480, 2048, 15, -16);
  STEP (20480, -2560, 15, -16);

  STEP (13964, 94, 7, -8);
  STEP (15360, -1792, 7, -8);
  STEP (8534, -341, 3, -4);
  STEP (9036, -1144, 3, -4);

#	undef	STEP
}

void
Gsm_LPC_Analysis (word s[N],  word s_out[N] ,
		  word LARc[M] )
{
	int i;
  
  longword L_ACF[9];
  Autocorrelation (s, s_out, L_ACF);
  
  word LARc_temp[M+1];
  
  Reflection_coefficients (L_ACF, LARc_temp);
 
 /*Cyber unroll_times=0 */
  for (i=0;i<M;i++)
	  LARc[i]=LARc_temp[i+1];
  
  
  
  Transformation_to_Log_Area_Ratios (LARc);
  
  Quantization_and_coding (LARc);


}








//  Main thread  
void gsm::gsm_main ( void ) {

	// Variables declaration

	word out_data_larc_write[M];
	word in_data_read[N];
	word out_data_read[N];
	int i;

	// Reset state - should be executable in 1 clock cycle

	wait();  


	// Main thread	
	while (1) {
		
		/*Cyber unroll_times=0 */
		for(i=0;i<N;i++){
		in_data_read[i] = (word) in_data[i].read();
		}
	
			
		Gsm_LPC_Analysis (in_data_read, out_data_read, out_data_larc_write );
		
		
		
		/*Cyber unroll_times=0 */
		for(i=0;i<N;i++){
		gsm_output[i].write( out_data_read[i]);
		}

		/*Cyber unroll_times=0 */
		for(i=0;i<M;i++){
		gsm_larc_output[i].write( out_data_larc_write[i]);
		}
	
		wait();

	}
}
