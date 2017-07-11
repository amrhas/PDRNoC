/*
 * embedded_proc.h
 *
 *  Created on: Aug 18, 2011
 *      Author: anhttran
 *
 *  Embedded processor without sending ACK when receiving a packet
 */

#ifndef EMBEDDED_PROC_H_
#define EMBEDDED_PROC_H_

#include <queue>
#include <systemc.h>

//#include <ReChannel.h>

#include "../../../global_variables.h"
#include "../../../definition.h"
#include "../../virtual_proc.h"
#include "../../../common_functions.h"
#include "../../../router/router_parameters.h"
#include "../../proc_parameters.h"
#include "../../proc_functions.h"

#include "../embedded_parameters.h"
//#include "../embedded_app_info.h"
#include "../../proc_evaluation.h"

using namespace std;
//typedef rc_reconfigurable_module<EmbeddedProc> EmbeddedProc_rc;


class EmbeddedProc: public VirtualProc{
public:

	// initialize
	void initialize(int x, int y, EmbeddedAppHashTable* app_info=NULL);

	// evaluation
	ProcEvaluationFactors *evaluation();

	// keep trace of number of idle entries of each output VC
//	sc_signal <int> out_vc_remain[MAX_N_VCS];
//	sc_signal <int> out_vc_remain_reg[MAX_N_VCS];

//	sc_signal <bool> queue_out_valid;	// queue output before sent to LOCAL port
//	sc_signal <Flit> queue_out;

	// processes
	void tx_method();
	void flit_out_method();
	//void out_vc_remain_method();	// update out_vc_remain
	//void count_plus_method();	// pipelined out_vc_remain
//	void out_vc_remain_reg_method();	// pipelined out_vc_remain
	void rx_method();
	void reset_reconfig_process();
	void reconfig_count_process();
	// constructor with process enable
	SC_HAS_PROCESS(EmbeddedProc);
	EmbeddedProc (sc_module_name name): VirtualProc(name){

		// send flit
		SC_METHOD (tx_method);
		sensitive << reset.pos() << clk.pos() << out_vc_buffer_rd;

		// receive flit
		SC_METHOD (rx_method);
		sensitive << reset.pos() << clk.pos();

		// update out_vc_remain
	//	SC_METHOD (out_vc_remain_method);
	//	for (int vo=0; vo<RouterParameter::n_VCs; vo++){
	//		sensitive << out_vc_remain_reg[vo];
	//		sensitive << count_plus[vo];
	//		sensitive << count_minus[vo];
	//	}

		// pipelined out_vc_remain
	//	SC_METHOD (out_vc_remain_reg_method);
	//	sensitive << clk.pos() << reset.pos();

		// count_plus = out_vc_buffer
	//	SC_METHOD (count_plus_method);
	//	for (int vo=0; vo<RouterParameter::n_VCs; vo++){
	//		sensitive << out_vc_buffer_rd;
	//	}

		// flit_out
		SC_METHOD (flit_out_method);
		sensitive << reset.pos() << clk.pos();
	}
	
	EmbeddedProc (sc_module_name name, VirtualProc* procIF): VirtualProc(name),
		mProcIF(procIF){

		// send flit
		SC_METHOD (tx_method);
		sensitive << reset.pos() << clk.pos() << out_vc_buffer_rd;

		// receive flit
		SC_METHOD (rx_method);
		sensitive << reset.pos() << clk.pos();
		
			// flit_out
		SC_METHOD (flit_out_method);
		sensitive << reset.pos() << clk.pos();
		
			// reset reconfig  proc signal
		SC_METHOD (reset_reconfig_process);
		sensitive  <<  clk.pos()  << reset.pos();

		// flit_out
		SC_METHOD (reconfig_count_process);
		sensitive << clk.pos();
		}


private:

//	sc_signal <bool> count_plus[MAX_N_VCS];
//	sc_signal <bool> count_minus[MAX_N_VCS];

	bool was_head[MAX_N_VCS];	// whether a packet head flit was received on a VC
    bool was_reconfig[MAX_N_VCS];
    bool was_reconfig_ack[MAX_N_VCS];
    int reconfig_block_counter;
	int injected_packet_count;
	int received_packet_count;
	int received_packets_count_reconfig;

	sc_mutex scMut;
	bool incremented;

	bool reconfig_rc;
	bool reconfig_rc_ack;
	sc_signal <Flit> queue_out;

	VirtualProc* mProcIF;
	bool reconfig_en;
	bool reconfig_done;
		sc_signal <bool> queue_out_valid;	// queue output before sent to LOCAL port
	// destination info
	int n_dsts;	// number of destinations <= MAX_N_DESTS
	TileLocation dst_tile_loc[MAX_N_DESTS];

	double flit_inj_rate[MAX_N_DESTS];	// injection rate for each dests

	//int injected_packet_count;
	int next_injection_time[MAX_N_DESTS];	// time for injecting next packet
	int next_injection_time_reconfig;
	// an "infinite" queue of injected flits at the source PE
	queue <Flit> source_queue;

	double total_latency;
	double max_latency;
	double min_latency;
	double total_latency_reconfig;

	// time that the head flit was injected
	int head_injected_time;
	int mDst_x;
	int mDst_y;
};

#endif /* EMBEDDED_PROC_H_ */
