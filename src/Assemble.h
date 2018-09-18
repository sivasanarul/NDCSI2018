/*
 * Assemble.h
 *
 *  Created on: 2 Aug 2018
 *      Author: SIV0021
 */

#ifndef ASSEMBLE_H_
#define ASSEMBLE_H_

#include "Main.h"
#include "Node.h"
#include "Discretization.h"
#include "List.h"

#include <map>
#include <iterator>
#include <Eigen/Dense>
#include "friction_element/Friction_element.h"
class Discretization;
class Assemble

{
protected:
      VectorXf fglobal;
      VectorXf u_corrected;
      Discretization *dis;

public:
	Assemble(Discretization *_dis);

	VectorXf assemble_fglobal(VectorXf _Un, VectorXf _Unminus);

	void resize_fglobal();

	void resize_u_corrected();

	~Assemble() { }
};



#endif /* ASSEMBLE_H_ */
