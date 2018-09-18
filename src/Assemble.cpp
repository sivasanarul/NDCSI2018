/*
 * Assemble.cpp
 *
 *  Created on: 2 Aug 2018
 *      Author: SIV0021
 */

#include "Assemble.h"

Assemble :: Assemble (
		Discretization *_dis)
{
    dis = _dis;
    return;
}


VectorXf Assemble ::assemble_fglobal(VectorXf _Un,VectorXf  _Unminus)
{

	cout << "getting there"<< endl;
    int number_nodal_diameter = dis->get_number_nodaldia();
    VectorXd keys(number_nodal_diameter*4);
	for (int i=0;i<dis->get_number_frictionelement() ;i++)
	{


		fglobal.fill(0);
		u_corrected.fill(0);
        dis->get_friction_element(i)->call_calculate_forcenl(number_nodal_diameter,
        	                                        fglobal,u_corrected, _Un,_Unminus);


	}





	return _Un;
}

void Assemble ::resize_fglobal()
   {
	    dis->set_size_globalsystem();
		int size_of_global= dis->get_size_globalsystem();
        cout << size_of_global<< "size of global system" << endl;
		fglobal.resize(size_of_global);
		cout<< "resize fglobal" << endl;
	}


void Assemble ::resize_u_corrected()
   {

		int size_of_global= dis->get_size_globalsystem();
        cout << size_of_global<< "size of global system" << endl;
		u_corrected.resize(size_of_global);
		cout<< "resize u_ corrected" << endl;
	}


