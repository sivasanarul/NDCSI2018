/*
 * Main.cpp
 *
 *  Created on: 19 Jul 2018
 *      Author: SIV0021
 */


#include <fstream>
#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;

#include "Main.h"




struct Configs
{
    int number_amplitudes=10;

};

int main() {
	cout << "Hello World!!!" << endl; // prints Hello World!!!

	Discretization       *discretization;
	discretization =  new Discretization();

	discretization->read_information();

	Assemble *assemble;
    assemble = new Assemble(discretization);


    assemble->resize_fglobal();
    assemble->resize_u_corrected();
    cout<< "number of ND " <<discretization->get_number_nodaldia()<< endl;

    for (int i=0;i< discretization->get_number_frictionelement();i++ )
    {
    	cout << "-------- friction element " << i << endl;
   	for (int j=0; j< 4*discretization->get_number_nodaldia() ; j++)
   	cout << discretization->get_friction_element(i)->get_gdofmap(j) << endl;
    }


    MatrixXf global_stiffness( discretization->get_size_globalsystem(), discretization->get_size_globalsystem());
    global_stiffness.fill(0);
    read_stiffness_matrix(discretization->get_number_nodaldia(),
    		              discretization->get_size_onesystem(),global_stiffness);

    MatrixXf global_mass( discretization->get_size_globalsystem(), discretization->get_size_globalsystem());
    global_mass.fill(0);
    read_mass_matrix(discretization->get_number_nodaldia(),
        		              discretization->get_size_onesystem(),global_mass);


        write_stiffness_matrix(global_stiffness);
        write_mass_matrix(global_mass);

        VectorXf force_nl ;VectorXf Un; VectorXf Unminus;
        Un.resize(1000);
        Un.fill(10);

        Unminus.resize(1000);
        Unminus.fill(10);
	force_nl= assemble->assemble_fglobal(Un,  Unminus);
	cout << "----"<<endl;
	//cout << force_nl << endl;

	cout << "force non linear" << endl;
	cout << "--The end--"<<endl;
	return 0;
}



