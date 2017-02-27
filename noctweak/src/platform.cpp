#include "platform.h"

Platform::Platform(sc_module_name name_):
		sc_module(name_){
		for (int x=0; x<CommonParameter::dim_x; x++){
			for (int y=0; y<CommonParameter::dim_y; y++){
				string label = "Tile[";
				label = label + int_to_str(x) + "][" + int_to_str(y) + "]";

//				cout << "Binding " << label << "..." << endl;

				tile[x][y] = new Tile(label.data(),x ,y);

				tile[x][y]->proc->initialize(x, y);
				tile[x][y]->router->initialize(x, y);

				// initialize all constants
				tile[x][y]->initialize();
//				cout << "Done Tile Initialization" << endl;

				tile[x][y]->clk(clk);
				tile[x][y]->reset(reset);


				// binding all interconnect signals
				for (int i=0; i<ROUTER_PORTS-1; i++){
					switch (i){
						case (WEST):
							tile[x][y]->valid_in[i](W_valid_in[x][y]);
							tile[x][y]->flit_in[i](W_flit_in[x][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->in_vc_buffer_rd[i][k](W_in_vc_buffer_rd[x][y][k]);
							}

							tile[x][y]->valid_out[i](W_valid_out[x][y]);
							tile[x][y]->flit_out[i](W_flit_out[x][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->out_vc_buffer_rd[i][k](W_out_vc_buffer_rd[x][y][k]);
							}
							break;
						case (EAST):
							tile[x][y]->valid_in[i](W_valid_out[x+1][y]);
							tile[x][y]->flit_in[i](W_flit_out[x+1][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->in_vc_buffer_rd[i][k](W_out_vc_buffer_rd[x+1][y][k]);
							}

							tile[x][y]->valid_out[i](W_valid_in[x+1][y]);
							tile[x][y]->flit_out[i](W_flit_in[x+1][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->out_vc_buffer_rd[i][k](W_in_vc_buffer_rd[x+1][y][k]);
							}
							break;
						case (NORTH):
							tile[x][y]->valid_in[i](N_valid_in[x][y]);
							tile[x][y]->flit_in[i](N_flit_in[x][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->in_vc_buffer_rd[i][k](N_in_vc_buffer_rd[x][y][k]);
							}

							tile[x][y]->valid_out[i](N_valid_out[x][y]);
							tile[x][y]->flit_out[i](N_flit_out[x][y]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->out_vc_buffer_rd[i][k](N_out_vc_buffer_rd[x][y][k]);
							}
							break;
						case (SOUTH):
							tile[x][y]->valid_in[i](N_valid_out[x][y+1]);
							tile[x][y]->flit_in[i](N_flit_out[x][y+1]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->in_vc_buffer_rd[i][k](N_out_vc_buffer_rd[x][y+1][k]);
							}

							tile[x][y]->valid_out[i](N_valid_in[x][y+1]);
							tile[x][y]->flit_out[i](N_flit_in[x][y+1]);
							for (int k=0; k<MAX_N_VCS; k++){
								tile[x][y]->out_vc_buffer_rd[i][k](N_in_vc_buffer_rd[x][y+1][k]);
							}
							break;
						default:;
					}
				}


//				cout << "Binding " << label << "DONE!" << endl;
			}
		}
		//if (CommonParameter::platform_type == PLATFORM_RECONFIG){
			//SC_THREAD(reconfig_ctrl);
			//sensitive << clk.pos();
		//}
}

void Platform::reconfig_ctrl(){
	int xLowerBound = 0;
	int xUpperBound = 10;
	std::uniform_int_distribution<> distrtime(xLowerBound, xUpperBound);
	int timevar=distrtime(gen);
	int x,y;
	while(true){

		x=mapRandomX();
		y=mapRandomY();
		wait(timevar,SC_US);
		//cout << "send reconfig signal" <<x << ":" <<y << endl ;
		tile[x][y]->proc->do_activate_em = true;
		timevar=distrtime(gen);

		wait();
		tile[x][y]->proc->do_activate_em = false;
	}
}

int Platform::mapRandomX() {

	int xLowerBound = 0;
	int xUpperBound = CommonParameter::dim_x - 1;
	std::uniform_int_distribution<> distrx(xLowerBound, xUpperBound);
	int xRunnable = distrx(gen);

	return(xRunnable);
}

int Platform::mapRandomY() {

	int yLowerBound = 0;
	int yUpperBound = CommonParameter::dim_y - 1;
	std::uniform_int_distribution<> distry(yLowerBound, yUpperBound);
	int yRunnable = distry(gen);

	return(yRunnable);;
}
