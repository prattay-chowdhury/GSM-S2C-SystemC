
#ifdef sim_cycle
#include "gsm_C.h"
#else
#include "gsm.h"
#endif


#include "tb_gsm.h"


int sc_main(int argc, char** argv)
{
  sc_clock                clk("clk", 25, SC_NS, 0.5, 12.5, SC_NS, true);
  sc_signal<bool>         rst;
  sc_signal<sc_int<16> >  in_data[N];

 
  sc_signal<sc_int<16> > gsm_output[N]; 
  sc_signal<sc_int<8> > gsm_larc_output[M];
  int i;
  char name[20];

  gsm u_GSM("GSM");
  test_GSM test("test_GSM");

  //connect to gsm module
  u_GSM.clk( clk );
  u_GSM.rst( rst );

	for(i=0;i<N;i++){
	u_GSM.in_data[i]( in_data[i] );
	}

	for(i=0;i<N;i++)
	u_GSM.gsm_output[i]( gsm_output[i] );

	for(i=0;i<M;i++){
	u_GSM.gsm_larc_output[i]( gsm_larc_output[i] );
	}
  
  // connect to test bench
  test.clk( clk );
  test.rst( rst );
  
	for(i=0;i<N;i++){
	test.in_data[i]( in_data[i] );
	}

	for(i=0;i<N;i++)
	test.gsm_output[i]( gsm_output[i] );
	
	for(i=0;i<M;i++){
	test.gsm_larc_output[i]( gsm_larc_output[i]);
	}

#ifdef WAVE_DUMP
  // Trace files
  sc_trace_file* trace_file = sc_create_vcd_trace_file("trace_behav");

  // Top level signals
  sc_trace(trace_file, clk , "clk");
  sc_trace(trace_file, rst , "rst"); 
  
  for(i=0;i<N;i++){
  sprintf(name, "in_data%d",i);
  sc_trace(trace_file, in_data[i], name);
  
  sprintf(name, "gsm_output%d",i);
  sc_trace(trace_file, gsm_output[i] , name);
  }
  
   for(i=0;i<M;i++){
	   sprintf(name, "gsm_larc_output%d",i);
	   sc_trace(trace_file, gsm_larc_output[i], name);
   }

#endif  // End WAVE_DUMP

  sc_start( 25, SC_NS );
  rst.write(0);

  sc_start( 25, SC_NS );
  rst.write(1);

  sc_start();

#ifdef WAVE_DUMP
   sc_close_vcd_trace_file(trace_file);
#endif

    return 0;

};
