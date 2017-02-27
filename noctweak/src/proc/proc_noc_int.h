/*
 * virtual_core.h
 *
 *  Created on: May 2, 2010
 *      Author: Anh Tran
 *
 *  Define of a virtual core with only I/O ports and virtual functions without implemented
 *  All cores have to inherit this virtual core and to implement these virtual functions
 *
 *  Revised:
 *  	+ Oct 25, 2010: now support virtual-channel router
 *  	+ Nov 18, 2010: support credit-based flow control
 */

#ifndef PROC_NOC_INTER_H_
#define PROC_NOC_INTER_H_

#include <systemc.h>
#include "../definition.h"
#include "../common_functions.h"
#include "../router/router_parameters.h"
#include "../router/wormhole_pipeline/buffer.h"

#include "proc_parameters.h"
//#include "../../out_vc_arbiter.h"

#include "../global_variables.h"


class proc_noc_interface: public sc_module{
  public:

	// clk and reset
	sc_in <bool> clk;
	sc_in <bool> reset;

	// Signal from the router saying that he received a flit for this interface
	sc_in <bool> r_valid_in;

	// Signal from the router with the flit to be received by this interface
	sc_in <Flit> r_flit_in;

	// Signal from virtual channels of the router saying that one buffer entry is available
	sc_in <bool> r_out_vc_buffer_rd[MAX_N_VCS];

	// Signal to router saying that this interface want to send a flit
	sc_out <bool> r_valid_out;

	// Signal to router with the flit to be sent by this interface
	sc_out <Flit> r_flit_out;

	// Signal to router saying saying that this interface has read an input flit on the given VC (the router can free it)
	sc_out <bool> r_in_vc_buffer_rd[MAX_N_VCS];

	sc_out <bool> r_interface_buffer_rd[MAX_N_VCS];

	// Signal from the processor saying that he received a flit for this interface
	sc_in <bool> p_valid_in;

	// Signal from the processor with the flit to be received by this interface
	sc_in <Flit> p_flit_in;

	// Signal from virtual channels of the processor saying that one buffer entry is available
	sc_in <bool> p_out_vc_buffer_rd;

	// Signal to processor saying that this interface want to send a flit
	sc_out <bool> p_valid_out;

	// Signal to processor with the flit to be sent by this interface
	sc_out <Flit> p_flit_out;

	// Signal to processor saying saying that this interface has read an input flit on the given VC (the router can free it)
	sc_out <bool> p_buff_out_full;

	sc_signal <int> out_vc_remain[MAX_N_VCS];		// keep trace of number of idle entries of each output VC
	sc_signal <int> out_vc_remain_reg[MAX_N_VCS];

	sc_signal <int> in_vc_remain[MAX_N_VCS];		// keep trace of number of idle entries of each output VC
    sc_signal <int> in_vc_remain_reg[MAX_N_VCS];

    sc_signal <bool> out_buff_empty;
    sc_signal <int> in_buff_empty;
    sc_signal <int> in_buff_empty_reg;
    sc_signal <bool> in_buf_out_buffer_rd;
    sc_signal <bool> out_buf_out_buffer_rd;

    sc_signal <bool> out_buffer_full;
    sc_signal <bool> in_buffer_full;

	int local_x;
	int local_y;

	queue <Flit> out_buffer;
	queue <Flit> in_buffer;

	// initialize all constants inside the processor (x,y)
    void initialize(int x, int y);

	void out_vc_remain_process();

	void count_plus_process();	// pipelined out_vc_remain
	void out_vc_remain_reg_process();	// pipelined out_vc_remain

	void in_vc_remain_process();
	void in_vc_remain_reg_process();	// pipelined out_vc_remain

	void tx_process_out_buff();
	void rx_process_in_buff();

	void out_buffer_process();
	void in_buffer_process();

	// constructor
	SC_HAS_PROCESS(proc_noc_interface);
    proc_noc_interface (sc_module_name name): sc_module(name){
		string in_buffer_name;
		in_buffer_name = "in_buffer_interface";

    	buffer_size = RouterParameter::buffer_size;

		//in buff full
		SC_METHOD (tx_process_out_buff);
		sensitive << clk.pos() << reset.pos() ;

		SC_METHOD (rx_process_in_buff);
		sensitive << clk.pos() << reset.pos();

		SC_METHOD (in_buffer_process);
		sensitive << clk.pos() << reset.pos();

		SC_METHOD (out_buffer_process);
		sensitive << clk.pos() << reset.pos();

		// update out_vc_remain
		SC_METHOD (out_vc_remain_process);
		for (int vo=0; vo<RouterParameter::n_VCs; vo++){
			sensitive << out_vc_remain_reg[vo];
			sensitive << count_plus[vo];
			sensitive << count_minus[vo];
		}

		// pipelined out_vc_remain
		SC_METHOD (out_vc_remain_reg_process);
		sensitive << clk.pos() << reset.pos();

		// count_plus = out_vc_buffer
		SC_METHOD (count_plus_process);
		for (int vo=0; vo<RouterParameter::n_VCs; vo++){
			sensitive << r_out_vc_buffer_rd[vo];
		}
		SC_METHOD (in_vc_remain_process);
		for (int vo=0; vo<RouterParameter::n_VCs; vo++){
			sensitive << in_vc_remain_reg[vo];
			sensitive << in_count_plus[vo];
			sensitive << in_count_minus[vo];
		}

		// pipelined out_vc_remain
		SC_METHOD (in_vc_remain_reg_process);
		sensitive << clk.pos() << reset.pos();

    }
private:

	unsigned int buffer_size;	// number of flits

	sc_signal <bool> in_count_plus[MAX_N_VCS];	// = out_vc_buffer_rd
	sc_signal <bool> in_count_minus[MAX_N_VCS];
	sc_signal <bool> count_plus[MAX_N_VCS];	// = out_vc_buffer_rd
	sc_signal <bool> count_minus[MAX_N_VCS];
};

#endif /* PROC_NOC_INTER_H_ */
