/*
 * building_stiff_mass_matrix.cpp
 *
 *  Created on: 2 Aug 2018
 *      Author: SIV0021
 */
#include "building_stiff_mass_matrix.h"


void read_stiffness_matrix(int _no_ND, int _size_onesystem, MatrixXf &_global_stiffness)
{
	int additive= 0;

     for (int i=0; i<_no_ND; i++)
     {


    	 ostringstream file;




    	 file << "K_red_" << i << "_real.dat";
    	 string filename = file.str();
    	 cout << filename << endl;

    	 MatrixXf K_singlesys = read_column_file_to_matrix(filename);
    	 file.str(std::string());
    	 cout << "---------- fisrt file " << endl;
    	 //cout << K_singlesys ;
         additive = _size_onesystem*2*i;
    	 for (int j=0; j<_size_onesystem; j++ )
    	     {
    		 for (int k=0; k<_size_onesystem; k++ )
    		    {
    			 cout << K_singlesys(j,k) << endl;
    			 int ind_row = j + additive;
				 int ind_col = k + additive;
    			 _global_stiffness(ind_row,ind_col) += K_singlesys(j,k);
    		    }
    	     }




    	 file << "K_red_" << i << "_imag.dat";
    	  filename = file.str();
    	 cout << filename << endl;
    	 K_singlesys.setZero();
    	 K_singlesys = read_column_file_to_matrix(filename);
    	 file.str(std::string());
    	 additive += _size_onesystem;
    	 for (int j=0; j<_size_onesystem; j++ )
    	     	     {
    	     		 for (int k=0; k<_size_onesystem; k++ )
    	     		    {
    	     			 int ind_row = j + additive;
    	 				 int ind_col = k + additive;
    	 				 cout << K_singlesys(j,k)  << endl;
    	     			 _global_stiffness(ind_row,ind_col) += K_singlesys(j,k);
    	     		    }
    	     	     }
     }


}


void read_mass_matrix(int _no_ND, int _size_onesystem, MatrixXf &_global_mass)
{
	int additive= 0;

     for (int i=0; i<_no_ND; i++)
     {


    	 ostringstream file;




    	 file << "M_red_" << i << "_real.dat";
    	 string filename = file.str();
    	 cout << filename << endl;

    	 MatrixXf K_singlesys = read_column_file_to_matrix(filename);
    	 file.str(std::string());
    	 cout << "---------- fisrt file " << endl;
    	 //cout << K_singlesys ;
         additive = _size_onesystem*2*i;
    	 for (int j=0; j<_size_onesystem; j++ )
    	     {
    		 for (int k=0; k<_size_onesystem; k++ )
    		    {
    			 cout << K_singlesys(j,k) << endl;
    			 int ind_row = j + additive;
				 int ind_col = k + additive;
				 _global_mass(ind_row,ind_col) += K_singlesys(j,k);
    		    }
    	     }




    	 file << "M_red_" << i << "_imag.dat";
    	  filename = file.str();
    	 cout << filename << endl;
    	 K_singlesys.setZero();
    	 K_singlesys = read_column_file_to_matrix(filename);
    	 file.str(std::string());
    	 additive += _size_onesystem;
    	 for (int j=0; j<_size_onesystem; j++ )
    	     	     {
    	     		 for (int k=0; k<_size_onesystem; k++ )
    	     		    {
    	     			 int ind_row = j + additive;
    	 				 int ind_col = k + additive;
    	 				 cout << K_singlesys(j,k)  << endl;
    	 				_global_mass(ind_row,ind_col) += K_singlesys(j,k);
    	     		    }
    	     	     }
     }


}



void write_stiffness_matrix( MatrixXf &_matrix)
{

	  if( remove( "stiffness_matrix.txt" ) != 0 )
	    perror( "Error deleting file" );
	  else
	    puts( "File successfully deleted" );

    	 std::ofstream output("stiffness_matrix.txt");
    	 output << _matrix.cols() ; output << "\n";
    	 for (int i =0;i<_matrix.cols() ;i++)
         {
    	 for (int j =0;j<_matrix.rows() ;j++)
    	       {
    		  output << _matrix(j,i);
    		  output << "\n";
    	      }
         }

}


void write_mass_matrix( MatrixXf &_matrix)
{
	      if( remove( "mass_matrix.txt" ) != 0 )
		    perror( "Error deleting file" );
		  else
		    puts( "File successfully deleted" );


    	 std::ofstream output("mass_matrix.txt");
    	 output << _matrix.cols() ; output << "\n";
    	 for (int i =0;i<_matrix.cols() ;i++)
         {
    	 for (int j =0;j<_matrix.rows() ;j++)
    	       {
    		  output << _matrix(j,i);
    		  output << "\n";
    	      }
         }

}
