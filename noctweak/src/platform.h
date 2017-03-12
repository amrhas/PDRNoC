/*
 * noc.h
 *
 * (C) copyright by the VCL laboratory, ECE Department, UC Davis
 *
 *  Please send en email to anhtrandavis@gmail.com to ask for your permission
 *  before using this code;
 *  and cite the paper "..." in your related publications.
 *
 *
 *  Created on: Apr 22, 2010
 *      Author: Anh Tran
 *
 *  A 2-D Mesh NoC of Tiles (PE + Router)
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "common_functions.h"
#include <systemc.h>
#include "tile.h"
#include <iostream>
#include <random>

using namespace std;

class Platform: public sc_module {

public:


	sc_in <bool> clk;
	sc_in <bool> reset;

	sc_signal <bool> W_valid_in[MAX_DIM][MAX_DIM];
	sc_signal <Flit> W_flit_in[MAX_DIM][MAX_DIM];
//	sc_signal <bool> W_in_full[MAX_DIM][MAX_DIM][MAX_N_VCS];
	sc_signal <bool> W_in_vc_buffer_rd[MAX_DIM][MAX_DIM][MAX_N_VCS];

	sc_signal <bool> N_valid_in[MAX_DIM][MAX_DIM];
	sc_signal <Flit> N_flit_in[MAX_DIM][MAX_DIM];
//	sc_signal <bool> N_in_full[MAX_DIM][MAX_DIM][MAX_N_VCS];
	sc_signal <bool> N_in_vc_buffer_rd[MAX_DIM][MAX_DIM][MAX_N_VCS];

	sc_signal <bool> W_valid_out[MAX_DIM][MAX_DIM];
	sc_signal <Flit> W_flit_out[MAX_DIM][MAX_DIM];
//	sc_signal <bool> W_out_full[MAX_DIM][MAX_DIM][MAX_N_VCS];
	sc_signal <bool> W_out_vc_buffer_rd[MAX_DIM][MAX_DIM][MAX_N_VCS];

	sc_signal <bool> N_valid_out[MAX_DIM][MAX_DIM];
	sc_signal <Flit> N_flit_out[MAX_DIM][MAX_DIM];
//	sc_signal <bool> N_out_full[MAX_DIM][MAX_DIM][MAX_N_VCS];
	sc_signal <bool> N_out_vc_buffer_rd[MAX_DIM][MAX_DIM][MAX_N_VCS];

	Tile *tile[MAX_DIM][MAX_DIM];

	SC_HAS_PROCESS(Platform);
	//SC_CTOR (Platform){
	Platform(sc_module_name name_);
	~Platform(){
		for (int x=0; x<CommonParameter::dim_x; x++){
					for (int y=0; y<CommonParameter::dim_y; y++){
			delete tile[x][y];
					}
		}
			//delete procEm;
	}
private:
	void reconfig_ctrl();
	int mapRandomX();
	std::mt19937 gen;

	int mapRandomY();
};

#endif /* PLATFORM_H_ */
