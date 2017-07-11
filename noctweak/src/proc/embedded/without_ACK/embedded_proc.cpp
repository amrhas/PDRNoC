/*
 * embedded_proc.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: anhttran
 */

#include "embedded_proc.h"
#include "../embedded_parameters.h"

/*
 * initialize
 */
void EmbeddedProc::initialize(int x, int y, EmbeddedAppHashTable* app_info ){
	local_x = x;
	local_y = y;
	mDst_x = 1;
	mDst_y = 0;
	TileLocation local_loc = TileLocation(x, y);
	int local_id = tile_loc_to_id(local_loc, CommonParameter::dim_x,
			CommonParameter::dim_y);
	if (app_info->count(local_id) > 0) {
		n_dsts = app_info->find(local_id)->second.n_dsts;
	} else {
		n_dsts = 0;
    }
	cout << "n_dsts[" << x << "," << y << "] = " << n_dsts << endl;

	for (int i = 0; i < n_dsts; i++) {
		int dst_tile_id =
				app_info->find(local_id)->second.dst_info[i].dst_tile_id;

		dst_tile_loc[i] = tile_id_to_loc(dst_tile_id, CommonParameter::dim_x,
				CommonParameter::dim_y);

		double req_bandwidth =
				app_info->find(local_id)->second.dst_info[i].bandwidth;
		flit_inj_rate[i] = (req_bandwidth
				/ EmbeddedParameters::max_req_bandwidth)
				* ProcessorParameters::flit_inject_rate;
	}

//	cout << endl;
}


/*
 *
 */
ProcEvaluationFactors *EmbeddedProc::evaluation(){
	ProcEvaluationFactors *embedded_factors = new ProcEvaluationFactors();

	embedded_factors->n_injected_packets = injected_packet_count;
	embedded_factors->n_received_packets = received_packet_count;
	embedded_factors->total_latency = total_latency;
	embedded_factors->max_latency = max_latency;
	embedded_factors->min_latency = min_latency;
	embedded_factors->total_latency_reconfig = total_latency_reconfig;
	embedded_factors->n_received_packets_reconfig = received_packets_count_reconfig;

	return embedded_factors;
}


/*
 * flit transmit process
 */
void EmbeddedProc::tx_method() {
	if (reset.read()) {	// if reset
		injected_packet_count = 0;
		for (int i = 0; i < n_dsts; i++) { // scan all destinations
			next_injection_time[i] = inter_injection_time(flit_inj_rate[i],
					ProcessorParameters::inter_arrival_time_type);
		}
		reconfig_block_counter = 0;
		// flush all data in the source_queue
		while (!source_queue.empty()) {
			source_queue.pop();
		}
		next_injection_time_reconfig = inter_injection_time(
				ProcessorParameters::flit_inject_rate,
				ProcessorParameters::inter_arrival_time_type);
		queue_out_valid.write(0);
		queue_out.write(Flit());
		mDst_x = CommonParameter::dim_x-1;
		mDst_y = CommonParameter::dim_y-1;
		reconfig_rc_ack=false;
		reconfig_rc=false;
		//for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
		//	count_minus[vo].write(0);
		//}
	} else {	// positive clock edge

		// send flits to its local router
		//if (out_vc_remain[0].read() >= 1 && (source_queue.size() > 0)) {// if local port of router is not full
		//	count_minus[0].write(true);
		//	source_queue.pop();
		//} else {
		//	count_minus[0].write(false);
		//}
		int temp=0;
int reconfig_rc_active = 0;
		// scan all destinations to generate packets
		int current_time = (int) ((sc_time_stamp().to_double()/1000)/(1000/CommonParameter::operating_clk_freq));

		// create a new packet;
		Flit *head_flit = NULL;

		if(current_time >= next_injection_time_reconfig){



				if(((local_x == 0 && local_y == 0) || (reconfig_rc && !reconfig_rc_ack) )
						&& CommonParameter::platform_type == PLATFORM_RECONFIG_EM &&
						(current_time >= CommonParameter::warmup_time)){


						//head_flit = create_head_flit_fixed_reconfig(local_x, local_y, mDst_x, mDst_y,
							//			current_time);
				//cout << sc_time_stamp() << name() << "Syn [" << head_flit->dst_x <<"][" << head_flit->dst_y << "]" << endl;
				if(reconfig_rc && !reconfig_rc_ack){
					head_flit = create_head_flit_fixed_reconfig_ack(local_x, local_y, 0, 0,
															current_time);
					head_flit->dst_x = 0;
					head_flit->dst_y = 0;
					reconfig_rc_ack = true;
					reconfig_rc_active = 0;
					queue_out_valid.write(0);
					queue_out.write(Flit());
					reconfig_rc=false;
					while (!source_queue.empty()) {
					  source_queue.pop();
					}
					//reconfig_rc=0;
					//int current_packet_length = ProcessorParameters::packet_length_reconfig;

					Packet *packet = new Packet(head_flit, 5);
					injected_packet_count += 1;
					//GlobalVariables::reconfig_block_counter += 1;
					cout << local_x<< local_y<<"@ cycle " << sc_time_stamp().to_double()/CommonParameter::operating_clk_freq
							<< ": PE (" << head_flit->dst_x << ", " << head_flit->dst_y << ") injects a reconfig packet" << endl;

					// push all its flits to the source_queue
					for (int i = 0; i < 5; i++) {
						//if(source_queue.size() < 1024){
							source_queue.push(*(packet->flit[i]));// push value of flit to queue, not pointer
							//cout << "@ cycle " << sc_time_stamp().to_double()/1000
							//  << ": PE (" << head_flit->dst_x << ", " << head_flit->dst_y << ") injects a reconfig packet" << endl;
						//}
					}
					//head_flit->type=0;
					Flit * headtmp = create_head_flit_fixed_dest(local_x, local_y, 0, 0, current_time);

					Packet *packet1 = new Packet(headtmp, 10);

					// push all its flits to the source_queue
					//for (int i = 0; i < 5; i++) {
											//if(source_queue.size() < 1024){
					//	source_queue.push(*(packet->flit[i]));// push value of flit to queue, not pointer
												//cout << "@ cycle " << sc_time_stamp().to_double()/1000
												//  << ": PE (" << head_flit->dst_x << ", " << head_flit->dst_y << ") injects a reconfig packet" << endl;
											//}
					//}
					for (int i = 0; i < 10; i++) {
					 source_queue.push(*(packet1->flit[i]));// push value of flit to queue, not pointer
					}


					//for (int i = 0; i < 5; i++) {
						//source_queue.push(*(packet1->flit[i]));// push value of flit to queue, not pointer
					//}
					//for (int i = 0; i < 2; i++) {
						//source_queue.push(*(packet1->flit[i]));// push value of flit to queue, not pointer
					//}
					//if(out_vc_buffer_rd.read() == 0 )
					//incremented=true;
					//reconfig_rc=0;
					delete(headtmp);
					delete [] packet1;
					delete [] packet;
				}
				else if((local_x == 0 && local_y == 0)
						&& GlobalVariables::reconfig_block_counter < ProcessorParameters::block_reconfig_number) {
					head_flit = create_head_flit_fixed_reconfig(local_x, local_y, mDst_x, mDst_y,
																				current_time);
					//int current_packet_length = ProcessorParameters::packet_length_reconfig;
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
					Packet *packet = new Packet(head_flit, 6);

					injected_packet_count += 1;
					GlobalVariables::reconfig_block_counter += 1;
							cout << "@ cycle " << sc_time_stamp().to_double()/CommonParameter::operating_clk_freq
									<< ": PE (" << head_flit->dst_x << ", " << head_flit->dst_y << ") injects a reconfig packet" << endl;

					for (int i = 0; i < 6; i++) {
						///if(source_queue.size() < 1024){
						source_queue.push(*(packet->flit[i]));// push value of flit to queue, not pointer
						//cout << "@ cycle " << sc_time_stamp().to_double()/1000
					//  << ": PE (" << head_flit->dst_x << ", " << head_flit->dst_y << ") injects a reconfig packet" << endl;
						//}
					}
							//head_flit->type=0;
					//Flit * headtmp = create_head_flit_fixed_dest(local_x, local_y, mDst_x, mDst_y, current_time);
					//Packet *packet1 = new Packet(headtmp, 5);
					//for (int i = 0; i < 5; i++) {
					//	source_queue.push(*(packet1->flit[i]));// push value of flit to queue, not pointer
					//}

					// push all its flits to the source_queue

					//GlobalVariables::black_oos_x[GlobalVariables::reconfig_block_counter_oos]=head_flit->dst_x;//local_x;
					//GlobalVariables::black_oos_y[GlobalVariables::reconfig_block_counter_oos]=head_flit->dst_y;//local_y;//
					GlobalVariables::reconfig_block_counter_oos++;
					cout << "reconfig_block_counter_oos" << GlobalVariables::reconfig_block_counter_oos << endl;
					//delete [] packet1;
					delete [] packet;
					//delete (headtmp);

					if(mDst_x == 0 && mDst_y > 0  ){
						mDst_x = CommonParameter::dim_x-1;
						mDst_y--;
					}
					else if (mDst_x == 0 && mDst_y == 0){
					 cout << sc_time_stamp() << name() << "Done all nodes" << endl;
					 sc_stop();
					}
					else {
						mDst_x--;
					}
				}

				// schedule for next packet injection
					 temp = inter_injection_time(ProcessorParameters::flit_inject_rate,
							ProcessorParameters::inter_arrival_time_type);

				//			cout << "temp = " << temp << endl;

				}
				delete (head_flit);
				if (temp == 0)
					temp = 1;
				next_injection_time_reconfig = current_time + temp;
			}
		}
if(reconfig_rc_active == 0){
		for (int i = 0; i < n_dsts; i++) {
			if (current_time >= next_injection_time[i]) {
				head_flit = create_head_flit_fixed_dest(local_x, local_y,
						dst_tile_loc[i].x, dst_tile_loc[i].y, current_time);

				bool black_found =false;
				for(int m=0 ; m< ProcessorParameters::block_reconfig_number ;m++){
				  if( GlobalVariables::black_oos_y[m] == dst_tile_loc[i].y && GlobalVariables::black_oos_x[m] == dst_tile_loc[i].x ){
					  black_found = true;
					  //cout <<local_x << local_y << "@ cycle " << sc_time_stamp().to_double()/1000
						// << ": PE (" << head_flit->dst_x << ", "
						 //<< head_flit->dst_y << ") reject a reconfig packet from" << local_x << local_y<< endl;
				  }
				}
				if( CommonParameter::platform_type != PLATFORM_RECONFIG_EM)
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

				// push all its flits to the source_queue
				for (int j = 0; j < current_packet_length; j++) {
						//if(source_queue.size() < 1024)
					source_queue.push(*(packet->flit[j]));// push value of flit to queue, not pointer
				}
				delete [] packet;
				}
				// schedule for next packet injection
			 temp = inter_injection_time(flit_inj_rate[i],
						ProcessorParameters::inter_arrival_time_type);


				delete (head_flit);
			}
				if (temp == 0)
					temp = 1;
				next_injection_time[i] = current_time + temp;
			}
}

		// send one flit to its local router
		if (source_queue.size() > 0) {
			queue_out_valid.write(true);
			Flit flit_tmp = source_queue.front();
			//cout << sc_time_stamp() << name() << "Syn current_packet_length = " << flit_tmp.packet_length << endl;
			bool black_found =false;
			while(!black_found )
			{
				for(int i=0 ; i< ProcessorParameters::block_reconfig_number ;i++){
			  if( GlobalVariables::black_oos_y[i] == flit_tmp.dst_y && GlobalVariables::black_oos_x[i] == flit_tmp.dst_x ){
				  black_found = true;
				  //cout <<local_x << local_y << "@ cycle " << sc_time_stamp().to_double()/1000
					// << ": PE (" << head_flit->dst_x << ", "
					 //<< head_flit->dst_y << ") reject a reconfig packet from" << local_x << local_y<< endl;
			  }
			}
				if(flit_tmp.type == 2 )
					black_found = true;
				else if(black_found){
					source_queue.pop();
					flit_tmp = source_queue.front();
					black_found=false;
				}
				else
					black_found = true;
			}

			flit_tmp.vc_id = 0;
			queue_out.write(flit_tmp);
		} else {
			queue_out_valid.write(0);
			queue_out.write(Flit());
		}
	}
}

/*
 * flit_out
 */
void EmbeddedProc::flit_out_method() {
	if (reset.read()) {	// if reset
		valid_out.write(0);
		flit_out.write(Flit());
		incremented = false;
	}
	else if (out_vc_buffer_rd.read() != 1 ) {
		//cout << sc_time_stamp() << name() << "Out Syn current_packet_length = " << endl;
		incremented = true;
		valid_out.write(queue_out_valid.read());
		flit_out.write(queue_out.read());
		//queue_out.pop();
		if ( (source_queue.size() > 0)) {// if local port of router is not full
			//count_minus[0].write(1);
			source_queue.pop();
			//incremented = true;
		}
	} else {
		incremented = false;
		valid_out.write(0);
		flit_out.write(Flit());
	}
}

/*
 * flit receive process
 */
void EmbeddedProc::rx_method() {
	if (reset.read()) {
//		in_full.write(0);	//never full
		in_vc_buffer_rd.write(0);

		for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
//			in_port_EFC_en.write(1);	// is always enable to accept flits
			was_head[vi] = 0;
			was_reconfig[vi] =0;
			was_reconfig_ack[vi] =0;
		}
		received_packet_count = 0;
		total_latency = 0;
		min_latency = INT_MAX;
		max_latency = 0;
		total_latency_reconfig = 0;
		received_packets_count_reconfig = 0;
		reconfig_en = false;
		reconfig_done = false;

	} else {	// if positive clk
		if (valid_in.read()) {
			int vc_id = flit_in.read().vc_id;
			// always read if having flit coming

			int current_time = (int) ((sc_time_stamp().to_double()/1000)/(1000/CommonParameter::operating_clk_freq) );
			if (current_time >= CommonParameter::warmup_time) {
				in_vc_buffer_rd.write(1);

				Flit rx_flit = flit_in.read();
				if (rx_flit.head) {
//					received_packet_count += 1;
					head_injected_time = rx_flit.injected_time;
					was_head[vc_id] = 1;
					if(rx_flit.type == 2){
						was_reconfig[vc_id] = 1;
					}
					if(rx_flit.type == 3){
						was_reconfig_ack[vc_id] = 1;
					}
				}
				if (rx_flit.tail) {
					if (was_head[vc_id]) {

						if(rx_flit.type != 2){
							was_reconfig[vc_id] = 0;
						}
						if(rx_flit.type != 3){
							was_reconfig_ack[vc_id] = 1;
						}

						int tail_receiving_time = current_time;

						int packet_latency = tail_receiving_time
								- head_injected_time;

						if (packet_latency < 0) {
							cout
									<< "WARNING as a severe ERROR: packet latency is NEGATIVE at Proc of "
									<< this->get_parent()->basename() << "; = "
									<< packet_latency << "; at cycle = "
									<< current_time << endl;
							exit(0);
						}
						
						if(!was_reconfig[vc_id]){
						received_packet_count += 1;

						total_latency += packet_latency;

						if (packet_latency > max_latency)
							max_latency = packet_latency;

						if (packet_latency < min_latency)
							min_latency = packet_latency;
						}
						was_head[vc_id] = 0;

						if(CommonParameter::platform_type == PLATFORM_RECONFIG_EM){
							if (CommonParameter::sim_mode == SIM_MODE_PACKET) {
						if(was_reconfig[vc_id] && !(local_y ==0 && local_x ==0))

							//GlobalVariables::n_total_rx_packets += 1;
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
						if( was_reconfig[vc_id] && !(local_y ==0 && local_x ==0) && !reconfig_en )
						{
							cout << local_x << local_y << "rec_recon pck" << endl;
							//reconfig_rc = 0;

							was_reconfig[vc_id] = 0;
							total_latency_reconfig += packet_latency;
							received_packets_count_reconfig += 1;
							reconfig_en = true;
							//in_vc_buffer_rd.write(0);
						}
						else if(was_reconfig_ack[vc_id] && (local_y ==0 && local_x ==0)){
							//reconfig_rc = 0;
							was_reconfig[vc_id] = 0;
							total_latency_reconfig += packet_latency;
							//received_packets_count_reconfig += 1;

							cout << local_x << local_y << "reconfig_block" << GlobalVariables::reconfig_block_counter << endl;
							scMut.lock();
							GlobalVariables::reconfig_block_counter -= 1;

							scMut.unlock();

						}
						//else
							was_reconfig[vc_id] = 0;
							was_reconfig_ack[vc_id] = 0;
					} else {
						// no thing
					}
				}
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

void EmbeddedProc::reset_reconfig_process(){
	if (reset.read()) {
		do_activate_syn = false;
	} else {
		if(reconfig_en == 1 ){
			do_activate_syn = true;
		}
		else
			do_activate_syn = false;
	}
}
void EmbeddedProc::reconfig_count_process(){
	if (reconf_done && !reconfig_done ){
		//GlobalVariables::reconfig_block_counter -= 1;
		reconfig_done=true;
		reconfig_rc = true;
		reconfig_en = false;
		reconfig_rc_ack = false;
		scMut.lock();

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
				GlobalVariables::reconfig_block_counter_oos--;
		}
		scMut.unlock();
	}else if (!reconf_done){
		reconfig_rc = false;
	}
}
