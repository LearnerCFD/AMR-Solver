#include "octree.h"
#include "vtk.h"
#include "boundary.h"
#include <iostream>

namespace myOctree {

extern int max_level;

std::list<Octree*> nodes;
std::list<Octree*> leaf_nodes;
std::list<Octree*> root_nodes;
std::list<Octree*> level_nodes[20];


//creates a new list of leaf nodes 
void create_list_of_leaf_nodes() {

	leaf_nodes.clear();

	for (std::list<Octree*>::iterator i = nodes.begin(), end = nodes.end(); i != end; ++i) {
		if((*i)->isLeafNode()) 
			leaf_nodes.push_back(*i);
	}
}

//creates a new list of root nodes
void create_list_of_root_nodes() {

	root_nodes.clear();

	for (std::list<Octree*>::iterator i = nodes.begin(), end = nodes.end(); i != end; ++i) {
		if((*i)->isRootNode()) 
			root_nodes.push_back(*i);
	}
}

//creates a vector of lists of nodes on each level
void create_lists_of_level_nodes() {

	

	//clearing all lists
	for (unsigned level=0; level <= max_level; level++) {
	
	
		level_nodes[level].clear();
	}
	

	//pushing nodes to respective lists	
	for (std::list<Octree*>::iterator j = nodes.begin(), end = nodes.end(); j != end; ++j) {
                int level = (*j)->get_level();
		level_nodes[level].push_back(*j);
	}

}

//creates an octree node
void create_node(int blocknumber, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax, int level, NodeBc east_bc, NodeBc west_bc, NodeBc north_bc, NodeBc south_bc, NodeBc top_bc, NodeBc bottom_bc) {

	//memory allocation to new node
	Octree r(xmin,xmax,ymin,ymax,zmin,zmax,level);
	nodes.pop_back();
	Octree* root = new Octree(r);

	//boundaries are assigned to this root node
	root->east_bc = east_bc;
	root->west_bc = west_bc;
	root->north_bc = north_bc;
	root->south_bc = south_bc;
	root->top_bc = top_bc;
	root->bottom_bc = bottom_bc;

	root->number = blocknumber;
		
}

//reassign neighbours after coarsening or refining the grid and after creating lists of level_nodes
//this is done coz, while assigning neighbours during refinement, neighbours might not have been created and a NULL would be assigned in these situations
//So this is done to ensure that neighbours are assigned after all refinement or coarsening is done for this time step 
void reassign_neighbours() {

	for (unsigned level=0; level <= max_level; level++) {


		for (std::list<Octree*>::iterator iter = level_nodes[level].begin(), end = level_nodes[level].end(); iter != end; ++iter) { 
			//if it is not leaf node	
			if(!((*iter)->isLeafNode())) {
	
				//setting neighbours to children
			        for(int k=0; k<2; k++) {
			                for(int j=0; j<2; j++) {
			                        for(int i=0; i<2; i++) {
			
			                                if(i==0) {
			                                        if((*iter)->west != NULL)  (*iter)->get_child_at(i, j, k)->west = (*iter)->west->get_child_at(i+1, j, k);
			                                }
			                                if(j==0) {
			                                        if((*iter)->south != NULL) (*iter)->get_child_at(i, j, k)->south = (*iter)->south->get_child_at(i, j+1, k);
			                                }
			                                if(k==0) {
			                                        if((*iter)->bottom != NULL)	(*iter)->get_child_at(i, j, k)->bottom = (*iter)->bottom->get_child_at(i, j, k+1);
			                                }
			                                if(i==1) {
			                                        if((*iter)->east != NULL)  (*iter)->get_child_at(i, j, k)->east = (*iter)->east->get_child_at(i-1, j, k);
			                                }
			                                if(j==1) {
			                                        if((*iter)->north != NULL) (*iter)->get_child_at(i, j, k)->north = (*iter)->north->get_child_at(i, j-1, k);
			                                }
			                                if(k==1) {
			                                        if((*iter)->top != NULL)   (*iter)->get_child_at(i, j, k)->top = (*iter)->top->get_child_at(i, j, k-1);
			                                }
			                        }
			                }
			        }
			}
		}
	}
}

//sets neighours to root nodes
void set_root_neighbours() {

	create_list_of_root_nodes();

	for (std::list<Octree*>::iterator i = root_nodes.begin(), end = root_nodes.end(); i != end; ++i) {

		double xmax = (*i)->x_max;			
		double ymax = (*i)->y_max;			
		double zmax = (*i)->z_max;			
		double xmin = (*i)->x_min;			
		double ymin = (*i)->y_min;			
		double zmin = (*i)->z_min;

		for (std::list<Octree*>::iterator j = root_nodes.begin(), end = root_nodes.end(); j != end; ++j) {
			/*change it to xmax_nbr*/
			double x_max = (*j)->x_max;			
			double y_max = (*j)->y_max;			
			double z_max = (*j)->z_max;			
			double x_min = (*j)->x_min;			
			double y_min = (*j)->y_min;			
			double z_min = (*j)->z_min;
	
			if(xmax==x_min&&ymax==y_max&&zmax==z_max)
				(*i)->east = (*j);	

			if(xmin==x_max&&ymax==y_max&&zmax==z_max)
				(*i)->west = (*j);	
			
			if(xmax==x_max&&ymax==y_min&&zmax==z_max)
				(*i)->north = (*j);	
				
			if(xmax==x_max&&ymin==y_max&&zmax==z_max)
				(*i)->south = (*j);	
			
			if(xmax==x_max&&ymax==y_max&&zmax==z_min)
				(*i)->top = (*j);	
			
			if(xmax==x_max&&ymax==y_max&&zmin==z_max)
				(*i)->bottom = (*j);	
				
		}			
	}
}	

void print_neighbour_information(std::list<Octree*>& nodes) {

	for (std::list<Octree*>::iterator i = nodes.begin(), end = nodes.end(); i != end; ++i) {
	
		printf("I am at %g %g %g\n",(*i)->x_centre,(*i)->y_centre,(*i)->z_centre);
		if((*i)->east!=NULL)	printf("East neighbour at %g %g %g\n",(*i)->east->x_centre,(*i)->east->y_centre,(*i)->east->z_centre);		
		if((*i)->west!=NULL)	printf("West neighbour at %g %g %g\n",(*i)->west->x_centre,(*i)->west->y_centre,(*i)->west->z_centre);		
		if((*i)->north!=NULL)	printf("North neighbour at %g %g %g\n",(*i)->north->x_centre,(*i)->north->y_centre,(*i)->north->z_centre);		
		if((*i)->south!=NULL)	printf("South neighbour at %g %g %g\n",(*i)->south->x_centre,(*i)->south->y_centre,(*i)->south->z_centre);		
		if((*i)->top!=NULL)	printf("Top neighbour at %g %g %g\n",(*i)->top->x_centre,(*i)->top->y_centre,(*i)->top->z_centre);		
		if((*i)->bottom!=NULL)	printf("Bottom neighbour at %g %g %g\n",(*i)->bottom->x_centre,(*i)->bottom->y_centre,(*i)->bottom->z_centre);		

	}

}


	
void OctreeGrid() {

	std::cerr << "\n"  <<"Setting up grid" << std::endl;

	set_root_neighbours();

	//prints neighbours information
	//print_neighbour_information(nodes);

}

}