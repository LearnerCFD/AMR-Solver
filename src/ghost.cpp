#include "octreegrid.h"
#include "direction.h"
#include <iostream>
#include <stdlib.h>
#include <string>

namespace myOctree {

void exchange_ghost_val(int level, std::string name) {

	create_lists_of_level_nodes();

	if(level_nodes[level].empty()) {
		std::cerr << "No blocks in level " << level << std::endl; 
		exit(1);
	}

	for (std::list<Octree*>::iterator it = level_nodes[level].begin(), end = level_nodes[level].end(); it != end; ++it) {	

		for(int l = 0; l<scalar_fields.size() ; l++) {
			
			Field *f = (*it)->get_block_data()->scalarfields[l];

                        if( f->name == name ) {
			
				//exchanging ghost cells
				for (int m=0; m<3; m++) {
                        		for (int n=0; n<2; n++) {

						if((*it)->bc[m][n] == NONE && (*it)->neighbour[m][n] != NULL) {
				
										
							if(n==RIGHT && m==XDIR) {
						
								for(int i=nx_block+pad; i<f->Nx; i++) {
                                                			for(int j=pad; j<ny_block+pad; j++) {
                                                        			for(int k=pad; k<nz_block+pad; k++) {
						//std::cout << (*it)->neighbour[m][n]->get_block_data()->scalarfields[l]->val[i-nx_block][j][k] << " " << i-nx_block     << " " << j << " " << k << std::endl;
								
											f->val[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->scalarfields[l]->val[i-nx_block][j][k]; 
										}
									}
								}
							}
							if(n==LEFT && m==XDIR) {
						
								for(int i=0; i<pad; i++) {
                                                			for(int j=pad; j<ny_block+pad; j++) {
                                                        			for(int k=pad; k<nz_block+pad; k++) {
								
											f->val[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->scalarfields[l]->val[i+nx_block][j][k]; 
										}
									}
								}	
							}
							if(n==RIGHT && m==YDIR) {
						//std::cout << "Hi" <<  std::endl;
						//std::cout << f->Ny <<  std::endl;
						
								for(int i=pad; i<nx_block+pad; i++) {
                                                			for(int j=ny_block+pad; j<f->Ny; j++) {
                                                        			for(int k=pad; k<nz_block+pad; k++) {
						//std::cout << (*it)->neighbour[m][n]->get_block_data()->scalarfields[l]->val[i][j-ny_block][k] << " " << i << " " << j-ny_block << " " << k <<  std::endl;
								
											f->val[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->scalarfields[l]->val[i][j-ny_block][k]; 
										}
									}
								}
							}
							if(n==LEFT && m==YDIR) {
						
								for(int i=pad; i<nx_block+pad; i++) {
                                                			for(int j=0; j<pad; j++) {
                                                        			for(int k=pad; k<nz_block+pad; k++) {
								
											f->val[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->scalarfields[l]->val[i][j+ny_block][k]; 
										}
									}
								}	
							}
							if(n==RIGHT && m==ZDIR) {
						
								for(int i=0; i<f->Nx; i++) {
                                                			for(int j=0; j<f->Ny; j++) {
                                                        			for(int k=nz_block+pad; k<f->Nz; k++) {
								
											f->val[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->scalarfields[l]->val[i][j][k-nz_block]; 
										}
									}
								}
							}
							if(n==LEFT && m==ZDIR) {
						
								for(int i=0; i<f->Nx; i++) {
                                                			for(int j=0; j<f->Ny; j++) {
                                                        			for(int k=0; k<pad; k++) {
								
											f->val[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->scalarfields[l]->val[i][j][k+nz_block]; 
										}
									}
								}	
							}
						}
                                   	}
                          	}
			}
		}
		
		for(int l = 0; l<vector_fields.size() ; l++) {
			
			VecField *f = (*it)->get_block_data()->vectorfields[l];

                        if( f->name == name ) {
			
				//exchanging ghost cells
				for (int m=0; m<3; m++) {
                        		for (int n=0; n<2; n++) {

						if((*it)->bc[m][n] == NONE && (*it)->neighbour[m][n] != NULL) {
										
							if(n==RIGHT && m==XDIR) {
						
								for(int i=nx_block+pad; i<f->Nx; i++) {
                                                			for(int j=0; j<f->Ny; j++) {
                                                        			for(int k=0; k<f->Nz; k++) {
								
											f->x[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->x[i-nx_block-pad][j][k]; 
											f->y[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->y[i-nx_block-pad][j][k]; 
											f->z[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->z[i-nx_block-pad][j][k]; 
										}
									}
								}
							}
							if(n==LEFT && m==XDIR) {
						
								for(int i=0; i<pad; i++) {
                                                			for(int j=0; j<f->Ny; j++) {
                                                        			for(int k=0; k<f->Nz; k++) {
								
											f->x[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->x[i+nx_block+pad][j][k]; 
											f->y[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->y[i+nx_block+pad][j][k]; 
											f->z[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->z[i+nx_block+pad][j][k]; 
										}
									}
								}	
							}
							if(n==RIGHT && m==YDIR) {
						
								for(int i=0; i<f->Nx; i++) {
                                                			for(int j=ny_block+pad; j<f->Ny; j++) {
                                                        			for(int k=0; k<f->Nz; k++) {
								
											f->x[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->x[i][j-ny_block-pad][k]; 
											f->y[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->y[i][j-ny_block-pad][k]; 
											f->z[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->z[i][j-ny_block-pad][k]; 
										}
									}
								}
							}
							if(n==LEFT && m==YDIR) {
						
								for(int i=0; i<f->Nx; i++) {
                                                			for(int j=0; j<pad; j++) {
                                                        			for(int k=0; k<f->Nz; k++) {
								
											f->x[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->x[i][j+ny_block+pad][k]; 
											f->y[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->y[i][j+ny_block+pad][k]; 
											f->z[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->z[i][j+ny_block+pad][k]; 
										}
									}
								}	
							}
							if(n==RIGHT && m==ZDIR) {
						
								for(int i=0; i<f->Nx; i++) {
                                                			for(int j=0; j<f->Ny; j++) {
                                                        			for(int k=nz_block+pad; k<f->Nz; k++) {
								
											f->x[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->x[i][j][k-nz_block-pad]; 
											f->y[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->y[i][j][k-nz_block-pad]; 
											f->z[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->z[i][j][k-nz_block-pad]; 
										}
									}
								}
							}
							if(n==LEFT && m==ZDIR) {
						
								for(int i=0; i<f->Nx; i++) {
                                                			for(int j=0; j<f->Ny; j++) {
                                                        			for(int k=0; k<pad; k++) {
								
											f->x[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->x[i][j][k+nz_block+pad]; 
											f->y[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->y[i][j][k+nz_block+pad]; 
											f->z[i][j][k] = \
							 (*it)->neighbour[m][n]->get_block_data()->vectorfields[l]->z[i][j][k+nz_block+pad]; 
										}
									}
								}	
							}
						}
                                   	}
                          	}
			}
		}
	}
}

}