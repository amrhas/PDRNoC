/*
 * pe.h
 *
 *  Created on: Apr 16, 2010
 *      Author: Anh Tran, VCL, UC Davis
 *
 *  Model of a processing element (PE)
 *  for injecting and consuming packet into/from the network
 *
 *  Revised:
 *  	+ Oct 25, 2010: now supports virtual-channel router
 *  	+ 2010.11.18: support credit-based flow control
 *  	+ 2010.12.06: add tornado traffic
 */

#ifndef SYNTHETIC_PROC_H_
#define SYNTHETIC_PROC_H_

//#include <math.h>
#include <queue>
#include <systemc.h>
#include "../../../definition.h"
#include "../../virtual_proc.h"
#include "../../../common_functions.h"
#include "../../../router/router_parameters.h"
#include "../../proc_parameters.h"
#include "../synthetic_parameters.h"
//#include "../../out_vc_arbiter.h"

#include "../../../global_variables.h"

#include "../../proc_functions.h"
#include "../../proc_evaluation.h"

using namespace std;

class SyntheticProc: public VirtualProc{
  public:

	sc_signal <int> reconf_done;

	// An "infinite" queue of flits to be injected into the NoC
	queue <Flit> source_queue;

	// functions
	void initialize(int x, int y);

	// for evaluation
	ProcEvaluationFactors *evaluation();

	// processes
	void tx_process();

	void flit_out_process();

	void rx_process();
	void reset_reconfig_process();

	// constructor with process enable
	SC_HAS_PROCESS(SyntheticProc);
	SyntheticProc (sc_module_name name): VirtualProc(name){
		// send flit
		SC_METHOD (tx_process);
		sensitive << reset.pos() << clk.pos();

		// receive flit
		SC_METHOD (rx_process);
		sensitive << reset.pos() << clk.pos();

		// flit_out
		SC_METHOD (flit_out_process);
		sensitive << queue_out_valid << queue_out ;
	}

	//SC_HAS_PROCESS(SyntheticProc);
	SyntheticProc (sc_module_name name, VirtualProc* procIF ): VirtualProc(name),
			mProcIF(procIF){
		// send flit
		SC_METHOD (tx_process);
		sensitive << reset.pos() << clk.pos();

		// receive flit
		SC_METHOD (rx_process);
		sensitive << reset.pos() << clk.pos();

		// flit_out
		SC_METHOD (flit_out_process);
		sensitive << queue_out_valid << queue_out ;

		// reset reconfig  proc signal
		SC_METHOD (reset_reconfig_process);
		sensitive  <<  clk.pos()  << reset.pos() ;

	}
	// functions
  private:
	bool was_head[MAX_N_VCS];	// whether a packet head flit was received on a VC
    bool was_reconfig[MAX_N_VCS];

	int injected_packet_count;
	int received_packet_count;
	double total_latency;
	double max_latency;
	double min_latency;
	double total_latency_reconfig;
	int received_packets_count_reconfig;
	sc_signal <Flit> queue_out;

	sc_signal <bool> reconfig_en;
	sc_signal <bool> queue_out_valid;	// queue output before sent to LOCAL port


	// packet inter-injection time (cycles) with exponential distribution
//	int inter_injection_time(double flit_rate, int type);

	int next_injection_time;	// time for injecting next packet



//	// where the source queue is full;
//	bool is_source_queue_full();

	// time that the head flit was injected
	int head_injected_time;

	// time that the tail flit of the same packet is receiving
	int tail_receiving_time;

	// generate a head flit with random destination
//	Flit *create_head_flit_random(int _src_x, int _src_y, int _time);
	// generate a head flit with transpose destination
//	Flit *create_head_flit_transpose(int _src_x, int _src_y, int _time);
	// generate a head flit with bit-reversal destination
//	Flit *create_head_flit_bit_complement(int _src_x, int _src_y, int _time);

//	Flit *create_head_flit_tornado(int _src_x, int _src_y, int _time);

//	Flit *create_head_flit_ACK(int _src_x, int _src_y, int _dst_x, int _dst_y, int _time);
};

#endif /* SYNTHETIC_PROC_H_ */
