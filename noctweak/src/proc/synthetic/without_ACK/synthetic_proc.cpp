/*
 * pe.cpp
 *
 * (C) copyright by the VCL laboratory, ECE Department, UC Davis
 *
 *  Please send en email to anhtrandavis@gmail.com to ask for your permission
 *  before using this code;
 *  and cite the paper "..." in your related publications.
 *
 *  Created on: Apr 19, 2010
 *      Author: Anh Tran, VCL, UC Davis
 *
 *  An implementation of synthetic_proc.h
 *
 *  Revised:
 *  	+ 2011.01.25:
 *  		- support variable packet length
 *
 */

#include "synthetic_proc.h"

void SyntheticProc::initialize(int x, int y) {
	local_x = x;
	local_y = y;
}

//SyntheticFactors *PE::synthetic_evaluation(){
ProcEvaluationFactors *SyntheticProc::evaluation(){
	ProcEvaluationFactors *synth_factors = new ProcEvaluationFactors();

	synth_factors->n_injected_packets = injected_packet_count;
	synth_factors->n_received_packets = received_packet_count;
	synth_factors->total_latency = total_latency;
	synth_factors->max_latency = max_latency;
	synth_factors->min_latency = min_latency;
	synth_factors->total_latency_reconfig = total_latency_reconfig;
	synth_factors->n_received_packets_reconfig = received_packets_count_reconfig;
	return synth_factors;
}

// flit transmit process
void SyntheticProc::tx_process() {
	if (reset.read()) {	// if reset
		injected_packet_count = 0;
//		next_injection_time = 0;
		next_injection_time = inter_injection_time(
				ProcessorParameters::flit_inject_rate,
				ProcessorParameters::inter_arrival_time_type);
		reconfig_block_counter = 0;
		// flush all data in the source_queue
		while (!source_queue.empty()) {
			source_queue.pop();
		}

//		valid_out.write(0);
//		flit_out.write(Flit());
		queue_out_valid.write(0);
		queue_out.write(Flit());
		reconfig_rc_ack=0;
		reconfig_rc=0;
//		tail_reg.write(0);

		//for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
		//	count_minus[vo].write(0);
		//}
	} else {	// positive clock edge
int temp=0;
		// If we sent a flit to the local router on the previous cycle:
		// we decrement the number of remaining entry in the buffer of VC 0
		// and we remove the flit from the local queue
		//if (out_vc_buffer_rd.read() != 1 && (source_queue.size() > 0)) {// if local port of router is not full
			//count_minus[0].write(1);
		//	source_queue.pop();
		//}

		// Generate packets
		int current_time = (int) (sc_time_stamp().to_double() / 1000);
		if (current_time >= next_injection_time ) {

			// create a new packet;
			Flit *head_flit = NULL;


			if(((local_x == 0 && local_y == 0) || (reconfig_rc && !reconfig_rc_ack) )
					&& CommonParameter::platform_type == PLATFORM_RECONFIG &&
					(current_time >= CommonParameter::warmup_time)){
				head_flit = create_head_flit_random_reconfig(local_x, local_y,
										current_time);
				//cout << sc_time_stamp() << name() << "Syn [" << head_flit->dst_x <<"][" << head_flit->dst_y << "]" << endl;
				if(reconfig_rc){
					head_flit->dst_x = 0;
					head_flit->dst_y = 0;
					reconfig_rc_ack = 1;
					//reconfig_rc=0;
					int current_packet_length = ProcessorParameters::packet_length_reconfig;

					Packet *packet = new Packet(head_flit, current_packet_length);
					injected_packet_count += 1;
					//GlobalVariables::reconfig_block_counter += 1;
					cout << local_x<< local_y<<"@ cycle " << sc_time_stamp().to_double()/1000
							<< ": PE (" << head_flit->dst_x << ", " << head_flit->dst_y << ") injects a reconfig packet" << endl;

					// push all its flits to the source_queue
					for (int i = 0; i < current_packet_length; i++) {
						if(source_queue.size() < 1024){
							source_queue.push(*(packet->flit[i]));// push value of flit to queue, not pointer
							//cout << "@ cycle " << sc_time_stamp().to_double()/1000
							//  << ": PE (" << head_flit->dst_x << ", " << head_flit->dst_y << ") injects a reconfig packet" << endl;
						}
					}

					delete packet;
				}
				else if(((head_flit->dst_x != local_x)
						|| (head_flit->dst_y != local_y))
						&& GlobalVariables::reconfig_block_counter < ProcessorParameters::block_reconfig_number) {
					int current_packet_length = ProcessorParameters::packet_length_reconfig;
					bool black_found =false;
					for(int i=0 ; i< ProcessorParameters::block_reconfig_number ;i++){
					  if( GlobalVariables::black_oos_y[i] == head_flit->dst_y && GlobalVariables::black_oos_x[i] == head_flit->dst_x ){
						  black_found = true;
						  //cout <<local_x << local_y << "@ cycle " << sc_time_stamp().to_double()/1000
							// << ": PE (" << head_flit->dst_x << ", "
							 //<< head_flit->dst_y << ") reject a reconfig packet from" << local_x << local_y<< endl;
					  }
					}
					if(!black_found){
					Packet *packet = new Packet(head_flit, current_packet_length);
					injected_packet_count += 1;
					GlobalVariables::reconfig_block_counter += 1;
							cout << "@ cycle " << sc_time_stamp().to_double()/1000
									<< ": PE (" << head_flit->dst_x << ", " << head_flit->dst_y << ") injects a reconfig packet" << endl;

					// push all its flits to the source_queue
					for (int i = 0; i < current_packet_length; i++) {
						if(source_queue.size() < 1024){
							source_queue.push(*(packet->flit[i]));// push value of flit to queue, not pointer
							//cout << "@ cycle " << sc_time_stamp().to_double()/1000
							//  << ": PE (" << head_flit->dst_x << ", " << head_flit->dst_y << ") injects a reconfig packet" << endl;
						}
					}

					delete packet;
				}

				// schedule for next packet injection
				 temp = inter_injection_time(
						ProcessorParameters::flit_inject_rate_reconfig,
						ProcessorParameters::inter_arrival_time_type);

				//			cout << "temp = " << temp << endl;

				}
				//delete (head_flit);
			}
			else {
				switch (SyntheticParameters::traffic_type) {
				case (TRAFFIC_RANDOM):
					head_flit = create_head_flit_random(local_x, local_y,
							current_time);
					break;
				case (TRAFFIC_TRANSPOSE):
					head_flit = create_head_flit_transpose(local_x, local_y,
							current_time);
					break;
				case (TRAFFIC_BIT_COMPLEMENT):
					head_flit = create_head_flit_bit_complement(local_x, local_y,
							current_time);
					break;
				case (TRAFFIC_BIT_REVERSAL):
					head_flit = create_head_flit_bit_reversal(local_x, local_y,
							current_time);
					break;
				case (TRAFFIC_TORNADO):
					head_flit = create_head_flit_tornado(local_x, local_y,
							current_time);
					break;
				case (TRAFFIC_NEIGHBOR):
					head_flit = create_head_flit_neighbor(local_x, local_y,
							current_time);
					break;
				case (TRAFFIC_REGIONAL):
					head_flit = create_head_flit_regional(local_x, local_y,
							current_time);
					break;
				case (TRAFFIC_HOTSPOT):
					head_flit = create_head_flit_hotspot(local_x, local_y,
							current_time);
					break;
				case (TRAFFIC_SHUFFLE):
					head_flit = create_head_flit_shuffle(local_x, local_y,
							current_time);
					break;
				case (TRAFFIC_ROTATE):
					head_flit = create_head_flit_rotate(local_x, local_y,
							current_time);
					break;

				default:
					;
				}


				// create and push a packet to the source queue if the destination is different from the source
				if ((head_flit->dst_x != local_x)
						|| (head_flit->dst_y != local_y)) {
					bool black_found =false;
					for(int i=0 ; i< ProcessorParameters::block_reconfig_number ;i++){
					  if( GlobalVariables::black_oos_y[i] == head_flit->dst_y && GlobalVariables::black_oos_x[i] == head_flit->dst_x ){
						  black_found = true;
						  //cout <<local_x << local_y << "@ cycle " << sc_time_stamp().to_double()/1000
							// << ": PE (" << head_flit->dst_x << ", "
							 //<< head_flit->dst_y << ") reject a reconfig packet from" << local_x << local_y<< endl;
					  }
					}
					if( CommonParameter::platform_type != PLATFORM_RECONFIG)
						black_found = false;
					if(!black_found){
					int current_packet_length;
					if (ProcessorParameters::packet_length_type
							== PACKET_LENGTH_TYPE_FIXED)
						current_packet_length = ProcessorParameters::packet_length;
					else
						// generate a random packet length in range [min.max]
						current_packet_length =
								rand()
										% (ProcessorParameters::max_packet_length
												- ProcessorParameters::min_packet_length
												+ 1)
										+ ProcessorParameters::min_packet_length;

					Packet *packet = new Packet(head_flit, current_packet_length);
					injected_packet_count += 1;

					//			cout << "@ cycle " << sc_time_stamp().to_double()/1000
					//					<< ": PE (" << local_x << ", " << local_y << ") injects a packet" << endl;

					// push all its flits to the source_queue
					for (int i = 0; i < current_packet_length; i++) {
						if(source_queue.size() < 1024)
							source_queue.push(*(packet->flit[i]));// push value of flit to queue, not pointer
					}

					delete [] packet;	// clean the memory
						// schedule for next packet injection
						 temp = inter_injection_time(
								ProcessorParameters::flit_inject_rate,
								ProcessorParameters::inter_arrival_time_type);

				}
				}
			}

				//			cout << "temp = " << temp << endl;

				if (temp == 0)
					temp = 1;
				next_injection_time = current_time + temp;

				delete [] (head_flit);
			//}
		} else {
			// do nothing
		}
//		}

	    if (source_queue.size() > 0 && out_vc_buffer_rd.read() != 1) {
			queue_out_valid.write(1);
			//cout << sc_time_stamp() << name() << "Syn current_packet_length = " << endl;

			Flit flit_tmp = source_queue.front();
			flit_tmp.vc_id = 0;
			queue_out.write(flit_tmp);
			source_queue.pop();
		} else {
			queue_out_valid.write(0);
			queue_out.write(Flit());
		}
	}
}

/*
 * flit_out
 */
void SyntheticProc::flit_out_process() {
	if (out_vc_buffer_rd.read() != 1) {
		//cout << sc_time_stamp() << name() << "Out Syn current_packet_length = " << endl;

		valid_out.write(queue_out_valid.read());
		flit_out.write(queue_out.read());
		//queue_out.pop();
	} else {
		valid_out.write(0);
		flit_out.write(Flit());
	}
}

/*
 * flit receive process
 */
void SyntheticProc::rx_process() {
	if (reset.read()) {
//		in_full.write(0);	//never full
		in_vc_buffer_rd.write(0);

		for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
//			in_port_EFC_en.write(1);	// is always enable to accept flits
			was_head[vi] = 0;
			was_reconfig[vi] =0;
		}
		received_packet_count = 0;
		total_latency = 0;
		min_latency = INT_MAX;
		max_latency = 0;
		total_latency_reconfig = 0;
		received_packets_count_reconfig = 0;
		reconfig_en = 0;

	} else {	// if positive clk
		if (valid_in.read()) {
			int vc_id = flit_in.read().vc_id;
			// always read if having flit coming

			int current_time = (int) (sc_time_stamp().to_double() / 1000);
			if (current_time >= CommonParameter::warmup_time) {

				Flit rx_flit = flit_in.read();
				if (rx_flit.head) {
//					received_packet_count += 1;
					head_injected_time = rx_flit.injected_time;
					was_head[vc_id] = 1;
					if(rx_flit.type == 2){
						was_reconfig[vc_id] = 1;
					}
				}
				if (rx_flit.tail) {
					if (was_head[vc_id]) {

						tail_receiving_time = current_time;
						//cout << sc_time_stamp() << name() << "recived normal Syn [" << rx_flit.dst_x <<"][" << rx_flit.dst_y << "]" << endl;

						int packet_latency = tail_receiving_time
								- head_injected_time;

						if (packet_latency < 0)
							cout
									<< "WARNING: packet latency is NEGATIVE at Proc of "
									<< this->get_parent()->basename() << "; = "
									<< packet_latency << "; at cycle = "
									<< current_time << endl;

						if(!was_reconfig[vc_id]){
						received_packet_count += 1;


						total_latency += packet_latency;

						if (packet_latency > max_latency)
							max_latency = packet_latency;

						if (packet_latency < min_latency)
							min_latency = packet_latency;
						}
						was_head[vc_id] = 0;

						if(CommonParameter::platform_type == PLATFORM_RECONFIG){
							if (CommonParameter::sim_mode == SIM_MODE_PACKET) {
						if(was_reconfig[vc_id] && !(local_y ==0 && local_x ==0))


							if (GlobalVariables::n_total_rx_packets
									>= CommonParameter::max_n_rx_packets) {
								GlobalVariables::last_simulation_time =
										current_time;
								sc_stop();	// stop simulation
							}
						}
						}else if (CommonParameter::sim_mode == SIM_MODE_PACKET) {

							GlobalVariables::n_total_rx_packets += 1;

							if (GlobalVariables::n_total_rx_packets
									>= CommonParameter::max_n_rx_packets) {
								GlobalVariables::last_simulation_time =
										current_time;
								sc_stop();	// stop simulation
							}
						}
						if( was_reconfig[vc_id] && !(local_y ==0 && local_x ==0) && reconfig_en ==0 )
						{
							GlobalVariables::black_oos_x[GlobalVariables::reconfig_block_counter_oos]=local_x;
							GlobalVariables::black_oos_y[GlobalVariables::reconfig_block_counter_oos]=local_y;
							GlobalVariables::reconfig_block_counter_oos++;
							cout << local_x << local_y << "reconfig_block_counter_oos" << GlobalVariables::reconfig_block_counter_oos << endl;

							was_reconfig[vc_id] = 0;
							total_latency_reconfig += packet_latency;
							received_packets_count_reconfig += 1;
							reconfig_en = 1;
							//in_vc_buffer_rd.write(0);
						}
						else if(was_reconfig[vc_id] && (local_y ==0 && local_x ==0)){
							GlobalVariables::reconfig_block_counter -= 1;
							was_reconfig[vc_id] = 0;
							total_latency_reconfig += packet_latency;
							//received_packets_count_reconfig += 1;
							cout << local_x << local_y << "reconfig_block" << GlobalVariables::reconfig_block_counter << endl;
						}
					} else {
						// no thing
					}
					//delete [] rx_flit;
				}
				//else if(!rx_flit.tail && reconfig_en == 1){
				//reconfig_en = 0;
				//}
			} else {
				// nothing
			}
		} else {
			//for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
				in_vc_buffer_rd.write(1);
			//}
			//reconfig_en = 0;

		}
	}
}

void SyntheticProc::reset_reconfig_process(){
	if (reset.read()) {
		do_activate_em = false;
	} else {
		if(reconfig_en == 1){
			do_activate_em = true;
		}
		else
			do_activate_em = false;
	}
}
void SyntheticProc::reconfig_count_process(){
	if (reconf_done && !reconfig_rc){
		//GlobalVariables::reconfig_block_counter -= 1;
		reconfig_rc = 1;
		reconfig_en = 0;
		reconfig_rc_ack = 0;
		GlobalVariables::n_total_rx_packets += 1;
		for(int i =0 ; i< ProcessorParameters::block_reconfig_number ; i++ ){
			if( GlobalVariables::black_oos_x[i] == local_x && GlobalVariables::black_oos_y[i]== local_y){
				//GlobalVariables::black_oos_x[i]=500;
				//GlobalVariables::black_oos_y[i]=500;
				for(int m =i ; m< ProcessorParameters::block_reconfig_number-i ; m++ ){
					GlobalVariables::black_oos_x[m]=GlobalVariables::black_oos_x[m+1];
					GlobalVariables::black_oos_y[m]=GlobalVariables::black_oos_y[m+1];
				}
				GlobalVariables::reconfig_block_counter_oos--;
				cout << local_x << local_y << "reconf_done_reconfig_block_counter_oos" << GlobalVariables::reconfig_block_counter_oos << endl;

				break;
			}
		}

	}else if (!reconf_done){
		reconfig_rc = 0;
	}
}
