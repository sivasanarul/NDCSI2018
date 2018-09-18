/*
 * Assemble.h
 *
 *  Created on: 31 Jul 2018
 *      Author: SIV0021
 */

#ifndef DISCRETIZATION_H_
#define DISCRETIZATION_H_

#include "Main.h"
#include "Node.h"
#include <map>
#include <iterator>
#include <Eigen/Dense>

#include "friction_element/Friction_element.h"
#include "List.h"
class Friction_element;

class Discretization
{
protected:

	List<Node*>  node;
	List<Friction_element*>  friction_elements;
	int no_ND;
	int no_retained_nodes;
    int no_friction_elements;
    int size_of_onesystem =5;
    int size_global_system ;
    VectorXd ND;
public:
    Discretization();

    void read_information();

    void set_fric_elem_gdofdict_(MatrixXd _node_dof_information);


     void assemble_stiffness_mass_mastrix( int _number_ND
		);

     Friction_element* get_friction_element(int i)
    {
  	 return friction_elements[i];
  }

     int get_number_nodaldia()
     {
    	 return no_ND;
     }

     int get_number_frictionelement()
         {
        	 return no_friction_elements;
         }
     int get_size_globalsystem()
              {
             	 return size_global_system;
              }
     void set_size_globalsystem()
         {
    	  size_global_system = size_of_onesystem*no_ND*2;
         }

     int get_size_onesystem()
         {
         return size_of_onesystem;
         }

};
#endif /* DISCRETIZATION_H_ */
