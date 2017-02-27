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

void procRCIf::change_module(){



    sc_time Emtime(500,SC_NS);
    sc_time Syntime(50,SC_NS);
    sc_time time(100,SC_NS);
    wait(100,SC_NS);
	    while(true){
//if(sc_time_stamp() == sc_time(500,SC_NS)){
	//do_activate_adder = false;
	//do_activate_adder = ~do_activate_adder;
	//return;
//}

	        //cout << sc_time_stamp() << "\tprocRCIf::change_module checking... " << endl;

	        /*if( (do_activate_em == true) && (active_module != SYN))
	        {
	            //if(!first_activation){
	        	//currentProc->local_x;
	        	//currentProc->local_y;
	                //cout << sc_time_stamp() << "\tprocRCIf::change_module(): trying to unload embedded" << endl;
	                ctrl.unload(*procEm);
	                cout << sc_time_stamp() << "\t[" << local_x << "]["  << local_y << "]procRCIf::change_module(): deactivated embedded" << endl;
	            //}
	            //first_activation = false;

	            //cout << sc_time_stamp() << "\tprocRCIf::change_module(): trying to activate Synthetic" << endl;
	            ctrl.activate(*procSyn);
	            active_module = SYN;
	            currentProc=procSyn;
	            //do_activate_adder = false;
	            cout << sc_time_stamp() << "\t[" << local_x << "]["  << local_y << "]procRCIf::change_module(): activated Synthetic" << endl;
	            //do_activate_syn = false;
	            wait(Syntime);
	        }*/

	        //wait(50,SC_NS);

	        if( (do_activate_em == true) && (active_module == SYN))
	        {
				ctrl.unload(*procSyn);

				cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): deactivated Synthetic" << endl;
				ctrl.activate(*procSyn1);
				currentProc=procSyn1;

	            cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): activated Synthetic1" << endl;
	            wait();
	            active_module = SYN1;
	            wait();
	        }

	        if( (do_activate_syn == true) && (active_module == SYN1))
	       	{
        		ctrl.unload(*procSyn1);

                cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): deactivated Synthetic1" << endl;
            	ctrl.activate(*procSyn);
            	currentProc=procSyn;

				//do_activate_em = false;
				cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): activated Synthetic" << endl;
				wait();
				active_module = SYN;
				wait();

	       	}
	        wait();
	    }
}
void procRCIf::reconfig_signal_process(){


	while(true){
		if (reset.read()) {
			do_activate_em = false;
			do_activate_syn =false;
			//reset_int.write(1);
			//wait(2,SC_NS);
			//reset_int.write(0);
			procSyn1->reconf_done.write(0);
			procSyn->reconf_done.write(0);
			wait();

		}else{
			//reset_int.write(0);
			procSyn1->reconf_done.write(0);
			procSyn->reconf_done.write(0);
	if(procSyn->do_activate_em == 1 && active_module == SYN && do_activate_em == false){
		do_activate_em = 1;

						//reset_int.write(1);
						//for (int vo=0; vo<RouterParameter::n_VCs; vo++){
						//	procSyn1->out_vc_remain_reconf_reg[vo].write(procSyn->out_vc_remain_reg[vo].read());
							//procSyn1->out_vc_remain_reconf[vo].write(procSyn->out_vc_remain[vo].read());
							//procSyn1->count_plus_reconf[vo].write(procSyn->count_plus[vo].read());
							//procSyn1->count_minus_reconf[vo].write(procSyn->count_minus[vo].read());
						//}
						//procSyn1->queue_out_valid_reconf.write(procSyn->queue_out_valid.read());
						/*while (!procSyn1->source_queue.empty()) {
							procSyn1->source_queue.pop();
						}
						while (!procSyn->source_queue.empty()) {
							Flit flit_tmp = procSyn->source_queue.front();

												procSyn1->source_queue.push(flit_tmp);
												procSyn->source_queue.pop();
											}*/
						//procSyn->reconf_done.write(1);
						procSyn1->reconf_done.write(1);
						ctrl.unload(*procSyn);

						cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): deactivated Synthetic" << endl;
						ctrl.activate(*procSyn1);
						currentProc=procSyn1;
						wait(5,SC_NS);
						procSyn1->reconf_done.write(0);
						//reset.write(0);
			            cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): activated Synthetic1" << endl;
			            //wait();
			            active_module = SYN1;
			            //do_activate_em = 0;
		wait();
	}

	if( procSyn1->do_activate_em == 1 && active_module == SYN1 &&  do_activate_syn == false){
		do_activate_syn = 1;



						//for (int vo=0; vo<RouterParameter::n_VCs; vo++){
						//	procSyn->out_vc_remain_reconf_reg[vo].write(procSyn1->out_vc_remain_reg[vo].read());
						//	procSyn->out_vc_remain_reconf[vo].write(procSyn1->out_vc_remain[vo].read());
							//procSyn->count_plus_reconf[vo].write(procSyn1->count_plus[vo].read());
							//procSyn->count_minus_reconf[vo].write(procSyn1->count_minus[vo].read());
						//}
						//procSyn->queue_out_valid_reconf.write(procSyn1->queue_out_valid.read());
					/*	while (!procSyn->source_queue.empty()) {
							procSyn->source_queue.pop();
						}
						while (!procSyn1->source_queue.empty()) {
							Flit flit_tmp = procSyn1->source_queue.front();

							procSyn->source_queue.push(flit_tmp);
							procSyn1->source_queue.pop();
						}*/
						procSyn->reconf_done.write(1);
						//procSyn1->reconf_done.write(1);

						ctrl.unload(*procSyn1);
						cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): deactivated Synthetic1" << endl;
						ctrl.activate(*procSyn);
						currentProc=procSyn;
						wait(6,SC_NS);
						procSyn->reconf_done.write(0);

						cout << sc_time_stamp() << "\t[" << currentProc->local_x << "]["  << currentProc->local_y << "]procRCIf::change_module(): activated Synthetic" << endl;
						active_module = SYN;
						//do_activate_syn = 0;
		wait();
	}
	do_activate_syn =0;
	do_activate_em = 0;

	wait();
		}
	}
}


