/*
 * Friction_element.cpp
 *
 *  Created on: 3 Aug 2018
 *      Author: SIV0021
 */

#include "Friction_element.h"

void Friction_element::  call_calculate_forcenl(int number_nodaldia, VectorXf &fglobal, VectorXf &u_corrected, VectorXf &Un,  VectorXf &Unminus)
  {
	Vector4d keys(0,1,2,3);
	Vector4d Iden(1,1,1,1);
	for (int i=0;i<number_nodaldia;i++)
		  {

			  Vector4d new_key = keys + 4*Iden*i;

			  Vector3f dof_master_cosine = get_gdofmap(new_key(0));
			  Vector3f dof_slave_cosine  = get_gdofmap(new_key(1));
			  Vector3f dof_master_sin    = get_gdofmap(new_key(2));
			  Vector3f dof_slave_sin     = get_gdofmap(new_key(3));

              cout << dof_master_cosine.unaryExpr(Un);
              cout << "access un " << endl;

              MatrixXf result_nl= calculate_forcenl (dof_master_cosine.unaryExpr(Unminus),dof_slave_cosine.unaryExpr(Unminus),dof_master_cosine.unaryExpr(Un),dof_slave_cosine.unaryExpr(Un),
					  dof_master_sin.unaryExpr(Unminus),dof_slave_sin.unaryExpr(Unminus),dof_master_sin.unaryExpr(Un),dof_slave_sin.unaryExpr(Un) 			  );



			  Vector3f fnl_global_master = transformation_matrix*result_nl.col(0);
			  Vector3f fnl_global_slave  = calculate_slave_fnl(fnl_global_master);


			  for (int j=0; j<fnl_global_master.size();j++)
			  {
				  fglobal(dof_master_cosine(j)) = fnl_global_master(j);
				  fglobal(dof_slave_cosine(j)) = fnl_global_slave(j);
				  if (j !=2){
				  u_corrected(  dof_master_cosine(j) ) = result_nl(1,0);
		          u_corrected(  dof_slave_cosine(j) )  = result_nl(1,1);
				  }
			  }


			  cout << dof_master_cosine << endl;
			  cout << "dof master cosine for friction element " << id << endl;

		  }
  }


MatrixXf Friction_element::  calculate_forcenl (VectorXf _Unminus_master_cosine, VectorXf _Unminus_slave_cosine,      VectorXf _Un_master_cosine, VectorXf _Un_slave_cosine,
		VectorXf _Unminus_master_sin, VectorXf _Unminus_slave_sin,      VectorXf _Un_master_sin, VectorXf _Un_slave_sin)


{

	MatrixXf relative_displacement = evaluate_relative_displacement(_Unminus_master_cosine, _Un_master_cosine, _Unminus_master_sin, _Un_master_sin );

	Vector3f lrelative_displacement_cosine_nminus = relative_displacement.col(0);
	Vector3f lrelative_displacement_sin_nminus    = relative_displacement.col(1);

	Vector3f lrelative_displacement_cosine_n      = relative_displacement.col(2);
	Vector3f lrelative_displacement_sin_n         = relative_displacement.col(3);

	MatrixXf force_nl(3,2);
	force_nl.fill(0);

	float normal_force_o = N_o + k_n*lrelative_displacement_cosine_nminus(2);

    float normal_force = std:: max (normal_force_o,float (0.0));

    if (normal_force <=0)
    {
	   tx =0;
       ty = 0;
       index_stick_seperation(0) = 0;
       index_stick_seperation(1) = 0;
    }
    else
    {
    	float coloumb_limit = mu*normal_force;
    	tx = ktx* (lrelative_displacement_cosine_n(0) - lrelative_displacement_cosine_nminus(0));
    	ty = kty* (lrelative_displacement_sin_n(1)    - lrelative_displacement_sin_nminus(1));
    	index_stick_seperation(0) = 1;
    	index_stick_seperation(1) = 1;

    	float T       = sqrt(tx*tx + ty*ty);
    	float dir_cos = sqrt(  pow((lrelative_displacement_cosine_n(0) - lrelative_displacement_cosine_nminus(0)),2)+
    			               pow((lrelative_displacement_sin_n(1)    - lrelative_displacement_sin_nminus(1)),2) );

    	if (T >=coloumb_limit)
    	{
    		force_nl(0,0) = coloumb_limit * ( lrelative_displacement_cosine_n(0) - lrelative_displacement_cosine_nminus(0) ) / dir_cos;

    		force_nl(0,1) = coloumb_limit * ( lrelative_displacement_cosine_n(1) - lrelative_displacement_cosine_nminus(1) ) / dir_cos;

    		force_nl(1,0)    = lrelative_displacement_cosine_n(0) - tx/ktx;
    		force_nl(1,1)    = lrelative_displacement_cosine_n(1) - ty/kty;

    		index_stick_seperation(0) = 2;
    		index_stick_seperation(1) = 2;

    	}

    	force_nl(0,2) = k_n*lrelative_displacement_cosine_nminus(2);
    	force_nl(1,2) = lrelative_displacement_cosine_n(2);

    }

  return  force_nl;
}

void Friction_element:: calculate_jacobian ()
{



}

MatrixXf Friction_element::  evaluate_relative_displacement(VectorXf _Unminus_master_cosine,VectorXf _Un_master_cosine,VectorXf _Unminus_master_sin,VectorXf _Un_master_sin )
{

	Vector3f relative_displacement_n      = _Un_master_cosine*(1-cos(sector_angle_radian));
	Vector3f relative_displacement_nminus = _Unminus_master_cosine*(1-cos(sector_angle_radian));
	MatrixXf relative_displacement(3,4);

	relative_displacement.col(0)  = relative_displacement_n;
	relative_displacement.col(2)  = relative_displacement_nminus;
    return relative_displacement;
}

VectorXf Friction_element:: calculate_slave_fnl(Vector3f _fnl_global_master)
{
	Vector3f fnl_global_slave;
	fnl_global_slave(0)= _fnl_global_master(0)*(-cos(sector_angle_radian));
	fnl_global_slave(1)=_fnl_global_master(1)*(-cos(sector_angle_radian));
	fnl_global_slave(2)=_fnl_global_master(2)*(-cos(sector_angle_radian));

	return fnl_global_slave;
}

