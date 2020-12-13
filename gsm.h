
#ifndef GSM_H_
#define GSM_H_

#include "define.h"


SC_MODULE (gsm) {


public:

   // Inputs
   sc_in_clk clk;
   sc_in<bool> rst;
   sc_in<sc_int<16> > in_data [N] ;

   // Output
   sc_out< sc_int<16> > gsm_output [N] ;
   sc_out< sc_int<8> > gsm_larc_output [M] ;

   /* F */
   void gsm_main ( void );


   // Constructor
 SC_CTOR (gsm) {

       SC_CTHREAD (gsm_main, clk.pos() );
       reset_signal_is(rst, false) ;
       sensitive << clk.pos();

   }

   // Destructor
   ~gsm() {};


};


#endif

