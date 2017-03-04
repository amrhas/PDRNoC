/*
 * embedded_proc.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: anhttran
 */

#include "proc_rc_if.h"

/*
 * initialize
 */
void procRCIf::initialize(int x, int y){
	local_x = x;
	local_y = y;
	//procEm->initialize(x,y);
    procSyn->initialize(x,y);
    procSyn1->initialize(x,y);
	return;
}


/*
 *
 */
ProcEvaluationFactors *procRCIf::evaluation(){
//return currentProc->evaluation();

ProcEvaluationFactors *proc_factors = new ProcEvaluationFactors();

proc_factors->n_injected_packets = procSyn1->evaluation()->n_injected_packets + procSyn->evaluation()->n_injected_packets ; //+ procEm->evaluation()->n_injected_packets;
proc_factors->n_received_packets = procSyn1->evaluation()->n_received_packets + procSyn->evaluation()->n_received_packets ;//+ procEm->evaluation()->n_received_packets;
proc_factors->total_latency = procSyn1->evaluation()->total_latency + procSyn->evaluation()->total_latency;// + procEm->evaluation()->total_latency;
proc_factors->max_latency = procSyn1->evaluation()->max_latency + procSyn->evaluation()->max_latency ;//+ procEm->evaluation()->max_latency;
proc_factors->min_latency = procSyn1->evaluation()->min_latency + procSyn->evaluation()->min_latency;// + procEm->evaluation()->min_latency;
proc_factors->total_latency_reconfig = procSyn1->evaluation()->total_latency_reconfig + procSyn->evaluation()->total_latency_reconfig;// + procEm->evaluation()->total_latency_reconfig;
proc_factors->n_received_packets_reconfig = procSyn1->evaluation()->n_received_packets_reconfig + procSyn->evaluation()->n_received_packets_reconfig; // + procEm->evaluation()->n_received_packets_reconfig;

	return proc_factors;
}

void procRCIf::reconfig_signal_process(){


	while(true){
		if (reset.read()) {
			do_activate_em = false;
			do_activate_syn =false;

			procSyn1->reconf_done.write(0);
			procSyn->reconf_done.write(0);
			wait();

		}else{
			procSyn1->reconf_done.write(0);
			procSyn->reconf_done.write(0);

			if(procSyn->do_activate_em == 1 && active_module == SYN ){
				//do_activate_em = 1;

				procSyn1->reconf_done.write(1);
				ctrl.unload(*procSyn);
				cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): deactivated Synthetic" << endl;
				ctrl.activate(*procSyn1);
				currentProc=procSyn1;
				procSyn1->reconf_done.write(0);
				cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): activated Synthetic1" << endl;
				active_module = SYN1;
				wait();
			}

			if( procSyn1->do_activate_em == 1 && active_module == SYN1 ){
				//do_activate_syn = 1;

				procSyn->reconf_done.write(1);
				ctrl.unload(*procSyn1);
				cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): deactivated Synthetic1" << endl;
				ctrl.activate(*procSyn);
				currentProc=procSyn;
				procSyn->reconf_done.write(0);
				cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): activated Synthetic" << endl;
				active_module = SYN;
				wait();
			}
			//do_activate_syn =0;
			//do_activate_em = 0;

			wait();
		}
	}
}


