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


	//----------- functions
	// initialize all constants inside the processor (x,y)
    void initialize(int x, int y);
	ProcEvaluationFactors *evaluation();
	void change_module();
	void reconfig_signal_process();

	//EmbeddedWithACKProc_rc  procEmACK;
	//EmbeddedProc_rc*         procEm;
	//SyntheticWithACKProc_rc procSynAck;
	SyntheticProc_rc*    	 procSyn;
	SyntheticProc_rc*    	 procSyn1;
	VirtualProc *currentProc;
	rc_control ctrl;
	sc_signal <bool> reset_int;
	// constructor
	SC_HAS_PROCESS(procRCIf);
	procRCIf (sc_module_name name): VirtualProc(name),
			//procEmACK("EmbeddedWithACKProc_rc"),
			//procEm("EmbeddedProc_rc"),
			//procSynAck("SyntheticWithACKProc_rc"),
			//procSyn("SyntheticProc_rc"),
			ctrl("control")
	{
		//procEm = new EmbeddedProc_rc("EmbeddedProc_rc");
		procSyn = new SyntheticProc_rc("SyntheticProc_rc", this);
		procSyn1 = new SyntheticProc_rc("SyntheticProc_rc1", this);
		p_clk.static_port(clk);
		//p_reset.static_port(reset);
		/*sc_trace_file *wf;
		if (CommonParameter::is_vcd_traced){
		wf = sc_create_vcd_trace_file("reconfig_waveform");
		string label;

		sc_trace(wf, clk, "clk");
		sc_trace(wf, reset, "reset");
		label = "(" + int_to_str(local_x) + ")(" + int_to_str(local_y) + ")_" + "valid_in";
		sc_trace(wf, valid_in.read(), label);

		label = "(" + int_to_str(local_x) + ")(" + int_to_str(local_y) + ")_" + "valid_out";
		sc_trace(wf, valid_out.read(), label);

		label = "(" + int_to_str(local_x) + ")(" + int_to_str(local_y) + ")_" + "out_vc_buffer_rd";
		sc_trace(wf, out_vc_buffer_rd[0].read(), label);

		label = "(" + int_to_str(local_x) + ")(" + int_to_str(local_y) + ")_" + "in_vc_buffer_rd";
		sc_trace(wf, in_vc_buffer_rd[0].read(), label);

		}*/
//		procEm->clk(clk);
		//procEm->reset(reset);
	//	procSyn->clk(clk);
		procSyn->reset(reset);
//		procSyn1->clk(clk);
		procSyn1->reset(reset);
		p_valid_in.static_port(valid_in);
		p_flit_in.static_port(flit_in);

		p_valid_out.static_port(valid_out);
		p_flit_out.static_port(flit_out);
		//p_do_activate_syn.bind_static(do_activate_syn);
		//p_do_activate_em.bind_static(do_activate_em);
		//for(int x = 0 ; x<MAX_N_VCS; x++ ){
		  p_in_vc_buffer_rd.static_port(in_vc_buffer_rd);
		  p_out_vc_buffer_rd.static_port(out_vc_buffer_rd);
		//}

	/*	p_clk.dynamic_port(procEm->clk);
		//p_reset.dynamic_port(procEm->reset);

		p_valid_in.dynamic_port(procEm->valid_in);
		p_flit_in.dynamic_port(procEm->flit_in);

		p_valid_out.dynamic_port(procEm->valid_out);
		p_flit_out.dynamic_port(procEm->flit_out);
*/
		//p_do_activate_syn.bind_dynamic(procEm->do_activate_syn);
		//p_do_activate_em.bind_dynamic(procEm->do_activate_em);

		//for(int x = 0 ; x<MAX_N_VCS; x++ ){
		//	p_in_vc_buffer_rd.dynamic_port(procEm->in_vc_buffer_rd);
		//	p_out_vc_buffer_rd.dynamic_port(procEm->out_vc_buffer_rd);
		//}

		p_clk.dynamic_port(procSyn->clk);
		//p_reset.dynamic_port(procSyn->reset);

		p_valid_in.dynamic_port(procSyn->valid_in);
		p_flit_in.dynamic_port(procSyn->flit_in);

		p_valid_out.dynamic_port(procSyn->valid_out);
		p_flit_out.dynamic_port(procSyn->flit_out);

		//p_do_activate_syn.bind_dynamic(procSyn->do_activate_syn);
		//p_do_activate_em.bind_dynamic(procSyn->do_activate_em);

		//for(int x = 0 ; x<MAX_N_VCS; x++ ){
			p_in_vc_buffer_rd.dynamic_port(procSyn->in_vc_buffer_rd);
			p_out_vc_buffer_rd.dynamic_port(procSyn->out_vc_buffer_rd);
		//}

		p_clk.dynamic_port(procSyn1->clk);
				//p_reset.dynamic_port(procSyn1->reset);

				p_valid_in.dynamic_port(procSyn1->valid_in);
				p_flit_in.dynamic_port(procSyn1->flit_in);

				p_valid_out.dynamic_port(procSyn1->valid_out);
				p_flit_out.dynamic_port(procSyn1->flit_out);

				//p_do_activate_syn.bind_dynamic(procSyn->do_activate_syn);
				//p_do_activate_em.bind_dynamic(procSyn->do_activate_em);

				//for(int x = 0 ; x<MAX_N_VCS; x++ ){
					p_in_vc_buffer_rd.dynamic_port(procSyn1->in_vc_buffer_rd);
					p_out_vc_buffer_rd.dynamic_port(procSyn1->out_vc_buffer_rd);
				//}

		procSyn->rc_set_delay(RC_LOAD, sc_time(ProcessorParameters::proc_reconfig_time_1, SC_NS));
		procSyn1->rc_set_delay(RC_LOAD, sc_time(ProcessorParameters::proc_reconfig_time_2, SC_NS));
		//procEm->rc_set_delay(RC_LOAD, sc_time(5, SC_NS));
		ctrl.add (*procSyn + *procSyn1);
		ctrl.activate(*procSyn);
        currentProc=procSyn;
        active_module = SYN;
		//active_module = &procEm;
        do_activate_syn = false ;
        do_activate_syn1 = false ;

        do_activate_em = false ;
		//SC_THREAD(change_module);
		//sensitive << clk.pos() << do_activate_em << do_activate_syn;

		SC_THREAD(reconfig_signal_process);
	    sensitive << reset.pos() << procSyn->do_activate_em << procSyn1->do_activate_em;

	}
	~procRCIf(){

		delete procSyn;
		delete procSyn1;
		//delete procEm;
	}
  private:
    enum modules{SYN,SYN1, EM} active_module;
    sc_signal<bool>  do_activate_syn1;
};

#endif /* PROC_RC_H_ */
