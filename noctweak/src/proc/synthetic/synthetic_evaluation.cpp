/*
 * synthetic_evaluation.cpp
 *
 *  Created on: Aug 21, 2011
 *      Author: anhttran
 */

#include "synthetic_evaluation.h"

SyntheticEvaluation::SyntheticEvaluation(Platform *_platform){
	platform = _platform;

	// initializing calculation
	initialize();
}

void SyntheticEvaluation::initialize(){
	ProcEvaluationFactors *synth_factors = NULL;

	total_latency = 0;
	total_rx_packets = 0;
	total_latency_reconfig = 0;
	total_rx_packets_reconfig = 0;
	for (int x=0; x<=CommonParameter::dim_x-1; x++){
		for (int y=0; y<=CommonParameter::dim_y-1; y++){
			synth_factors = (ProcEvaluationFactors*) platform->tile[x][y]->proc->evaluation();
			total_latency += synth_factors->total_latency;
			total_rx_packets += synth_factors->n_received_packets;
			total_latency_reconfig += synth_factors->total_latency_reconfig;
			total_rx_packets_reconfig += synth_factors->n_received_packets_reconfig;

			cout << "total latency at [" << x << "," << y << "]= "
					<< synth_factors->total_latency << "      ;";
			cout << "total packets received at [" << x << "," << y << "]= "
					<< synth_factors->n_received_packets << endl;
			cout << "total reconfig latency at [" << x << "," << y << "]= "
					<< synth_factors->total_latency_reconfig << "      ;";
			cout << "total reconfig packets received at [" << x << "," << y << "]= "
					<< synth_factors->n_received_packets_reconfig << endl;

		}
	}

	cout << "total_latency :" << total_latency << endl;
	cout << "total_rx_packets :" << total_rx_packets << endl;
	cout << "total_latency_reconfig :" << total_latency_reconfig << endl;
	cout << "total_rx_packets_reconfig :" << total_rx_packets_reconfig << endl;
	cout << "avg_packets_reconfig :" << ((double)(total_latency_reconfig) / (double)total_rx_packets_reconfig) << endl;
	if (CommonParameter::sim_mode == SIM_MODE_CYCLE){
		GlobalVariables::n_total_rx_packets = total_rx_packets;
	}

	delete (synth_factors);
}

double SyntheticEvaluation::avg_latency_cal(){
	return (double)(total_latency) / (double)total_rx_packets;
}

double SyntheticEvaluation::avg_throughput_cal(){
	double tmp;
	if (CommonParameter::sim_mode == SIM_MODE_CYCLE){
//		tmp =  (double)(total_rx_packets)
//					/ (CommonParameter::dim_x * CommonParameter::dim_y)
//					/ (CommonParameter::simulation_time - CommonParameter::warmup_time);

		GlobalVariables::last_simulation_time = CommonParameter::simulation_time;
	}
//	else {
		tmp =  (double)(total_rx_packets)
					/ (CommonParameter::dim_x * CommonParameter::dim_y)
					/ (GlobalVariables::last_simulation_time - CommonParameter::warmup_time);
//	}

	return tmp;
}

double SyntheticEvaluation::avg_reconfig_time_cal(){
	double tmp;
	if (CommonParameter::sim_mode == SIM_MODE_CYCLE){
//		tmp =  (double)(total_rx_packets)
//					/ (CommonParameter::dim_x * CommonParameter::dim_y)
//					/ (CommonParameter::simulation_time - CommonParameter::warmup_time);

		GlobalVariables::last_simulation_time = CommonParameter::simulation_time;
	}
//	else {
		tmp =  (double)(GlobalVariables::last_simulation_time - CommonParameter::warmup_time)/
				((GlobalVariables::n_total_rx_packets));
//	}

	return tmp;
}
