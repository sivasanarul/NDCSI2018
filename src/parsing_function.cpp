/*
 * parsing_function.cpp
 *
 *  Created on: 31 Jul 2018
 *      Author: SIV0021
 */

#include "parsing_function.h"
#include "Main.h"
#include <string>
#include <fstream>
using std::cout;
using std::endl;

MatrixXd  read_column_file(std ::string filename)
{

	        std ::ifstream lInputFile(filename);



		    int lDim;

		    lInputFile >> lDim;



		    Eigen::MatrixXd lReturnMatrix(lDim, lDim);

		    int lRow = 0;
		    int lCol = 0;
		    for (int i = 0; i < lDim * lDim; i++)
		    {
		        lInputFile >> lReturnMatrix(lRow, lCol);

		        lRow++;
		        if (lRow == lDim)
		        {
		            lCol++;
		            lRow = 0;
		        }
		    }

		    lInputFile.close();

		    return lReturnMatrix;

}


MatrixXf  read_column_file_to_matrix(std ::string filename)
{

	        std ::ifstream lInputFile(filename);



		    int lDim;

		    lInputFile >> lDim;



		    Eigen::MatrixXf lReturnMatrix(lDim, lDim);

		    int lRow = 0;
		    int lCol = 0;
		    for (int i = 0; i < lDim * lDim; i++)
		    {
		        lInputFile >> lReturnMatrix(lRow, lCol);

		        lRow++;
		        if (lRow == lDim)
		        {
		            lCol++;
		            lRow = 0;
		        }
		    }

		    lInputFile.close();

		    return lReturnMatrix;

}


MatrixXd  read_column_file_to_matrix_sparse(std ::string filename)
{

}
