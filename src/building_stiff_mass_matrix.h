/*
 * building_stiff_mass_matrix.h
 *
 *  Created on: 2 Aug 2018
 *      Author: SIV0021
 */

#ifndef BUILDING_STIFF_MASS_MATRIX_H_
#define BUILDING_STIFF_MASS_MATRIX_H_

#include "Main.h"
#include "parsing_function.h"
#include <string>
#include <fstream>
using std::cout;
using std::endl;

   void read_stiffness_matrix(int _no_ND, int _size_onesystem, MatrixXf &_global_stiffness);

   void read_mass_matrix(int _no_ND, int _size_onesystem, MatrixXf &_global_mass);

   void write_stiffness_matrix( MatrixXf &_matrix);

   void write_mass_matrix( MatrixXf &_matrix);
#endif /* BUILDING_STIFF_MASS_MATRIX_H_ */
