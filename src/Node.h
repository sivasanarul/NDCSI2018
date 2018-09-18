/*
 * Node.h
 *
 *  Created on: 19 Jul 2018
 *      Author: SIV0021
 */

#ifndef NODE_H_
#define NODE_H_

#include "Main.h"

class Node

{
protected:
  int                        id;                 // node number
  int                        n_dof=3;


  /*double                     x[2];*/
  int                        dof[2];
  double                     sol[2];
  double                     bc_force[2];        // Liste mit aeusseren Kraeften
  int                        bc_displ[2];        // Liste mit flags, ob der dof fdestgehalten ist
  VectorXd                   harmonic_index;
public:

  Node( int  _id )                // Knotennummer (i)

  {
    this->id   = _id;

    for (int i=0; i<2; i++)
    {
      dof[i] = 0;
      sol[i] = 0;
      bc_displ[i] = 0;
      bc_force[i] = 0;
    }
  }


  void intialize_node_harmonic_coefficients(int n_indices)
  {
	  harmonic_index.resize(n_indices*n_dof);
  }


  int get_id()
  {
	  return id;
  }

  /*
  void intialize_node_harmonic_coefficients_check(int n_indices)
  {
	  harmonic_index.resize(n_indices*n_dof);
	  for  (int i=0; i<n_indices*n_dof; i++)
		  harmonic_index(i)= 100+i;
	  return;
  }
  */

  VectorXd get_node_harmonic_coefficients()
    {
  	 return harmonic_index;
    }

};




#endif /* NODE_H_ */
