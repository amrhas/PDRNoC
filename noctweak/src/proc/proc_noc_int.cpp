
#include "proc_noc_int.h"

void proc_noc_interface::initialize(int x, int y) {
	local_x = x;
	local_y = y;
}

/*
 * update out_vc_remain of all VCs
 */
void proc_noc_interface::out_vc_remain_process() {
	for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
		int tmp = out_vc_remain_reg[vo];
		if (count_minus[vo].read())
			tmp -= 1;
		if (count_plus[vo].read())
			tmp += 1;
		out_vc_remain[vo].write(tmp);
	}
}

void proc_noc_interface::out_vc_remain_reg_process() {
	if (reset.read()) {
		for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
//			out_vc_remain_reg[vo].write(1);
			out_vc_remain_reg[vo].write((int) RouterParameter::buffer_size);
		}
	}
	 else{
		for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
			out_vc_remain_reg[vo].write(out_vc_remain[vo].read());
		}
	 }
}

/*
 * count_plus = out_vc_buffer_rd
 */
void proc_noc_interface::count_plus_process() {
	for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
		count_plus[vo].write(r_out_vc_buffer_rd[vo].read());
	}
}


// flit transmit process
void proc_noc_interface::tx_process_out_buff() {
	if (reset.read()) {	// if reset
		out_buf_out_buffer_rd.write(0);
		r_valid_out.write(0);
		for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
			count_minus[vo].write(0);
		}

	} else {	// positive clock edge
		//for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {

		if (out_vc_remain[0].read() >= 1 && (out_buffer.size() > 0) ) {// if local port of router is not full
			Flit flit_tmp = out_buffer.front();
			//flit_tmp.vc_id = vo;
			out_buf_out_buffer_rd.write(1);
			count_minus[0].write(1);
			r_valid_out.write(1);
			r_flit_out.write(flit_tmp);
			//break;
		}
		else {
	//		count_minus[0].write(0);
			out_buf_out_buffer_rd.write(0);
			for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
			  count_minus[vo].write(0);
			}
			r_valid_out.write(0);
			r_flit_out.write(Flit());
		}
		//}
	}
}

/*
 * flit receive process
 */
void proc_noc_interface::in_vc_remain_process() {
	for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
		int tmp = in_vc_remain_reg[vo];
		if (in_count_minus[vo].read())
			tmp -= 1;
		if (in_count_plus[vo].read())
			tmp += 1;
		in_vc_remain[vo].write(tmp);
	}
}

void proc_noc_interface::in_vc_remain_reg_process() {
	if (reset.read()) {
		for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
			in_vc_remain_reg[vo].write((int) RouterParameter::buffer_size);
		}
	}
	 else{
		for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
			in_vc_remain_reg[vo].write(in_vc_remain[vo].read());
		}
	 }
}



void proc_noc_interface::rx_process_in_buff() {
	if (reset.read()) {
		in_buf_out_buffer_rd.write(0);
		for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
				in_count_minus[vo].write(0);
			}
	} else {	// if positive clk
		for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {

		if (p_out_vc_buffer_rd.read() == 1 && in_vc_remain[vo].read() > 0 && in_buffer.size() >= 1) {
			in_count_minus[vo].write(1);
			in_buf_out_buffer_rd.write(1);
			break;
		}
		else {
			for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
				in_count_minus[vo].write(0);
			}
			in_buf_out_buffer_rd.write(0);
		}
		}
	}
}

void proc_noc_interface::in_buffer_process() {

	if (reset.read()) {	// if reset
		in_buffer_full.write(0);
		in_buff_empty.write(0);
		// flush all data in the buffer
		while (!in_buffer.empty()) {
			in_buffer.pop();
		}
		for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
			r_in_vc_buffer_rd[vi].write(0);
			r_interface_buffer_rd[vi].write(0);
		}
		p_valid_out.write(0);
		p_flit_out.write(Flit());

		for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
			in_count_plus[vo].write(0);
		}
	} else {	// if positive clk edge

		if (in_buffer.size() >= buffer_size-1 && r_valid_in.read()) {
			in_buffer_full.write(1);
		}
		else
			in_buffer_full.write(0);

		int current_time = (int) (sc_time_stamp().to_double() / 1000);

		if (r_valid_in.read() && in_buf_out_buffer_rd.read()) {

			// read
			if (in_buffer.empty()) {
				cout << "ERROR: read from an EMPTY in buffer: " << name() << endl;
				exit(-1);
			}
			// remove the first flit from the buffer
			p_valid_out.write(1);
			p_flit_out.write(in_buffer.front());
			Flit tmp = in_buffer.front();
			int vc_id = tmp.vc_id;
			in_count_plus[vc_id].write(1);
			in_buffer.pop();

			// write
			if (in_buffer.size() == buffer_size) {
				for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
					r_in_vc_buffer_rd[vi].write(0);
				}
				Flit tmp = in_buffer.front();
				int vc_id = tmp.vc_id;
				r_interface_buffer_rd[vc_id].write(1);// always read if having flit coming

				for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
					if (vi != vc_id)
						r_interface_buffer_rd[vi].write(0);
				}
			}
			else{
				Flit tmp = r_flit_in.read();
				int vc_id = tmp.vc_id;
				r_in_vc_buffer_rd[vc_id].write(1);// always read if having flit coming
				for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
					if (vi != vc_id)
						r_in_vc_buffer_rd[vi].write(0);
				}
				in_buffer.push(tmp);
			}
			in_buff_empty.write(in_buffer.empty());
			//for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
			//	r_interface_buffer_rd[vi].write(0);
			//}
		} else if (r_valid_in.read()) {

			if (in_buffer.size() == buffer_size) {
				for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
					r_in_vc_buffer_rd[vi].write(0);
				}
			}
			else{
				Flit tmp = r_flit_in.read();
				int vc_id = tmp.vc_id;
				r_in_vc_buffer_rd[vc_id].write(1);// always read if having flit coming

				for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
					if (vi != vc_id)
						r_in_vc_buffer_rd[vi].write(0);
					}
				// write the input flit to the buffer
				in_buffer.push(tmp);
				in_buff_empty.write(0);
			}
			for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
				r_interface_buffer_rd[vi].write(0);
			}
			p_valid_out.write(0);
			p_flit_out.write(Flit());
		} else if (in_buf_out_buffer_rd.read()) {
			if (in_buffer.empty()) {
				cerr << "ERROR: read from an EMPTY in buffer: " << name() << endl;				exit(-1);
			}

			if (in_buffer.size() >= buffer_size-1 ) {
				Flit tmp = in_buffer.front();
				int vc_id = tmp.vc_id;
				r_interface_buffer_rd[vc_id].write(1);// always read if having flit coming

				for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
					if (vi != vc_id)
						r_interface_buffer_rd[vi].write(0);
				}
			}
			p_valid_out.write(1);
			p_flit_out.write(in_buffer.front());
			// remove the first flit from the buffer

			Flit tmp = in_buffer.front();
			int vc_id = tmp.vc_id;
			in_count_plus[vc_id].write(1);
			in_buffer.pop();

			in_buff_empty.write(in_buffer.empty());
			// update flit_out and empty signals

			for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
				r_in_vc_buffer_rd[vi].write(0);
			}
		}
		else{
			for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
				r_interface_buffer_rd[vi].write(0);
			}
			for (int vi = 0; vi < RouterParameter::n_VCs; vi++) {
				r_in_vc_buffer_rd[vi].write(0);
				in_count_plus[vi].write(0);

			}
			in_buff_empty.write(in_buffer.empty());
			p_valid_out.write(0);
			p_flit_out.write(Flit());
		}
	}
}

void proc_noc_interface::out_buffer_process() {

	if (reset.read()) {	// if reset
		p_buff_out_full.write(0);
		p_vaild_out_buff.write(1);
		// flush all data in the buffer
		while (!out_buffer.empty()) {
			out_buffer.pop();
		}
	} else {

		//else
		//p_buff_out_full.write(0);

		int current_time = (int) (sc_time_stamp().to_double() / 1000);

		if (p_valid_in.read() && out_buf_out_buffer_rd.read()) {
			// read
			if (out_buffer.empty()) {
				cout << "ERROR: read from an EMPTY out buffer: " << name() << endl;
						exit(-1);
			}
			// remove the first flit from the buffer
			out_buffer.pop();
			// write
			if (out_buffer.size() == buffer_size) {
				cerr << "ERROR: write to a FULL out buffer: " << name() << endl;
				//p_buff_out_full.write(0);

				//exit(-1);
				//p_buff_out_full.write(0);

			}
			else{
			// Write the input flit to the buffer
			out_buffer.push(p_flit_in.read());
			//p_buff_out_full.write(1);


			}
		} else if (p_valid_in.read()) {
			if (out_buffer.size() == buffer_size) {
				cerr << "ERROR: write to a FULL out buffer: " << name() << endl;
				//p_buff_out_full.write(0);

				//exit(-1);
			}
			else {
				// write the input flit to the buffer
				out_buffer.push(p_flit_in.read());
				//p_buff_out_full.write(1);

			}

		} else if (out_buf_out_buffer_rd.read()) {
			if (out_buffer.empty()) {
				cerr << "ERROR: read from an EMPTY out buffer: " << name() << endl;				exit(-1);
			}
			// remove the first flit from the buffer
			//p_buff_out_full.write(0);
			out_buffer.pop();
			//count_minus[0].write(1);
		}
		//else{
			//p_buff_out_full.write(0);
			//for (int vo = 0; vo < RouterParameter::n_VCs; vo++) {
			//	count_minus[vo].write(0);
			//}
		//}
		if (out_buffer.size() >= buffer_size-1) {
			p_buff_out_full.write(1);
		}
		else{
			p_buff_out_full.write(0);
		}
		out_buff_empty = out_buffer.empty();
	}
}

