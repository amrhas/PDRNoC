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

#ifndef PROC_RC_H_
#define PROC_RC_H_

#include <systemc.h>
#include "ReChannel.h"
//#include "asap/router_to_asapcore.h"
//#include "asap/asap_core.h"
//#include "asap/asapcore_to_router.h"
#include "../definition.h"
#include "proc_evaluation.h"

//#include "embedded/with_ACK/embedded_with_ACK_proc_rc.h"
//#include "embedded/without_ACK/embedded_proc_rc.h"

//#include "synthetic/with_ACK/synthetic_with_ACK_proc_rc.h"
#include "synthetic/without_ACK/synthetic_proc_rc.h"
#include "embedded/without_ACK/embedded_proc_rc.h"

#define NUM_RC_PROC 4

class procRCIf: public VirtualProc,
public rc_reconfigurable{
  public:
	// clk and reset
	rc_in_portal<bool> p_clk;
	//rc_in_portal<bool> p_reset;

	// Input interface
	rc_in_portal<bool> p_valid_in;
	rc_in_portal<Flit> p_flit_in;
	rc_out_portal<bool> p_in_vc_buffer_rd;

	// output interface
	rc_out_portal<bool> p_valid_out;
	rc_out_portal<Flit> p_flit_out;
	rc_in_portal<bool> p_out_vc_buffer_rd;	// "full" signals from virtual channels of the local router port
	rc_in_portal <bool> p_vaild_in;
	//rc_in_portal<bool> p_reconf_done;

	//----------- functions
	// initialize all constants inside the processor (x,y)
    void initialize(int x, int y, EmbeddedAppHashTable* app_info=NULL);

	ProcEvaluationFactors *evaluation();
	void change_module();
	void reconfig_signal_process();
	//void reconfig_done_signal_process();
	EmbeddedProc_rc*  		procEm;
	EmbeddedProc_rc*     	procEm1;
	//SyntheticWithACKProc_rc procSynAck;
	SyntheticProc_rc*    	 procSyn;
	SyntheticProc_rc*    	 procSyn1;
	VirtualProc *currentProc;
	VirtualProc *rcProc[NUM_RC_PROC];

	rc_control ctrl;
	sc_signal <bool> reset_int;
	sc_signal <bool> recof_done_s;
	// constructor
	SC_HAS_PROCESS(procRCIf);
	procRCIf (sc_module_name name): VirtualProc(name),
			ctrl("control")
	{
		//procEm = new EmbeddedProc_rc("EmbeddedProc_rc");
		procSyn = new SyntheticProc_rc("SyntheticProc_rc", this);
		procSyn1 = new SyntheticProc_rc("SyntheticProc_rc1", this);
		procEm = new EmbeddedProc_rc("EmbeddedProc_rc", this);
		procEm1 = new EmbeddedProc_rc("EmbeddedProc_rc1", this);

		rcProc[0] = procSyn;
		rcProc[1] = procSyn1;
		rcProc[2] = procEm;
		rcProc[3] = procEm1;

		p_clk.static_port(clk);
		//p_reset.static_port(reset);
//		procEm->clk(clk);
		//procEm->reset(reset);
	//	procSyn->clk(clk);
		procSyn->reset(reset);
//		procSyn1->clk(clk);
		procSyn1->reset(reset);
		procEm->reset(reset);
		procEm1->reset(reset);


		p_valid_in.static_port(valid_in);
		p_flit_in.static_port(flit_in);

		p_valid_out.static_port(valid_out);
		p_flit_out.static_port(flit_out);
		p_in_vc_buffer_rd.static_port(in_vc_buffer_rd);
		p_out_vc_buffer_rd.static_port(out_vc_buffer_rd);
		p_vaild_in.static_port(out_buf_vaild_in);
		//p_reconf_done.static_port(recof_done_s);
		for(int i=0; i<NUM_RC_PROC; i++){
			p_clk.dynamic_port(rcProc[i]->clk);

			p_valid_in.dynamic_port(rcProc[i]->valid_in);
			p_flit_in.dynamic_port(rcProc[i]->flit_in);

			p_valid_out.dynamic_port(rcProc[i]->valid_out);
			p_flit_out.dynamic_port(rcProc[i]->flit_out);

			p_in_vc_buffer_rd.dynamic_port(rcProc[i]->in_vc_buffer_rd);
			p_out_vc_buffer_rd.dynamic_port(rcProc[i]->out_vc_buffer_rd);
			p_vaild_in.dynamic_port(rcProc[i]->out_buf_vaild_in);
			//p_reconf_done.dynamic_port(rcProc[i]->reconf_done);
		}

		procSyn->rc_set_delay(RC_LOAD, sc_time(ProcessorParameters::proc_reconfig_time_1, SC_NS));
		procSyn1->rc_set_delay(RC_LOAD, sc_time(ProcessorParameters::proc_reconfig_time_1, SC_NS));
		procEm->rc_set_delay(RC_LOAD, sc_time(ProcessorParameters::proc_reconfig_time_1, SC_NS));
		procEm1->rc_set_delay(RC_LOAD, sc_time(ProcessorParameters::proc_reconfig_time_1, SC_NS));
//procEm->rc_set_delay(RC_LOAD, sc_time(5, SC_NS));
		ctrl.add (*procSyn + *procSyn1 + *procEm + *procEm1);
		if(CommonParameter::platform_type == PLATFORM_RECONFIG_EM){
			ctrl.activate(*procEm);
		    currentProc=procEm;
		    active_module = EM;
		}
		else{
		ctrl.activate(*procSyn);
        currentProc=procSyn;
        active_module = SYN;
		}
		//active_module = &procEm;
        do_activate_syn = false ;
        do_activate_syn1 = false ;

        do_activate_em = false ;
        do_activate_em1 = false ;

		//SC_THREAD(change_module);
		//sensitive << clk.pos() << do_activate_em << do_activate_syn;

		SC_THREAD(reconfig_signal_process);
	    sensitive << reset.pos() << clk.pos() << procSyn->do_activate_em << procSyn1->do_activate_em
	    		<< procEm->do_activate_em << procEm1->do_activate_em;

	}
	~procRCIf(){

		delete procSyn;
		delete procSyn1;
		delete procEm;
		delete procEm1;

	}
  private:
    enum modules{SYN,SYN1, EM, EM1} active_module;
    sc_signal<bool>  do_activate_syn1;
    sc_signal<bool>  do_activate_em1;

};

#endif /* PROC_RC_H_ */
