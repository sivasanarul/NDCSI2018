/*
 * friction_elements.h
 *
 *  Created on: 20 Jul 2018
 *      Author: SIV0021
 */

#ifndef FRICTION_ELEMENT_H_
#define FRICTION_ELEMENT_H_

#include <math.h>       /* sqrt */
#include "../Main.h"
#include "../Node.h"
#include "../List.h"
#include <tuple>

#include <map>
#include <string>
#include <iterator>

struct dof_info {
	int nodaldia_number;
	int sincosine;
	int masterslave;
	Vector3f gdof;

	dof_info(int _nodaldia_number, char _sincosine, int _masterslave, Vector3f _gdof)
	{
		nodaldia_number = _nodaldia_number;
		sincosine       = _sincosine;
		masterslave     = _masterslave;
		gdof            = _gdof;
	}

  };

class Friction_element

{
protected:
  int                        id;
  Vector3f                   normal_vector;
  Vector2d                   nodes;
  Vector3d                   initial_gap;

  Matrix3f                   local_base;
  Matrix3f                   transformation_matrix;
  MatrixXd                   gdof_map;
  double                     sector_angle_radian;

  Vector2i index_stick_seperation;

  List<Node*>                node_object;
  std :: map <int, dof_info > dof_dict;
  std :: map <int, dof_info >:: iterator it;
  int  k_n ;  double ktx ;  double kty ; int mu ;int N_o ; float tx; float ty;

  public:

  Friction_element(
      int               _id,
      int            _master,
      int            _slave,
	  List<Node*>    &_node
      )
  {
    this->id       = _id;
    this->nodes(0) = _master;
    this->nodes(1) = _slave;


    this->node_object.resize(2);
    this->node_object[0] = _node[_master];
    this->node_object[1] = _node[_slave];
  }

  int get_id()
    {
      return id;
    }

  int get_master()
  {
       return nodes(1);
     }

  Node* get_master_object()
   {
        return node_object[1];
      }

  int get_slave()
  {
       return nodes(0);
     }

  Node* get_slave_object()
     {
          return node_object[0];
        }


  void set_normal_vector (double _x, double _y, double _z)
  {
	  normal_vector(0) = _x;
	  normal_vector(1) = _y;
	  normal_vector(2) = _z;
	  return;
     }

  Vector3f get_normal_vector()
  {

     return normal_vector;
  }

  void calculate_bases()
  {

		Vector3f m, o;
		Matrix3f b;
		float l1, l2, l3;						//Initialising norms of vectors
		m(0) = -normal_vector(1); m(1) = normal_vector(0); m(2) = 0;	//finding perpendicular to N vector
		o = normal_vector.cross(m);							//finding third perpendicular vector
		l1 = normal_vector.norm();							//normalising the vectors
		l2 = m.norm();
		l3 = o.norm();
		for (int i = 0; i < 3; i++)				//finding unit vectors
		{
			normal_vector(i) = normal_vector(i) / l1;
			m(i) = m(i) / l2;
			o(i) = o(i) / l3;
		}
		local_base<<normal_vector,m,o;								//building Basis Matrix

	  cout<< "went through calculate bases" << endl;
	  return;
  }

  double calculate_transformation()
   {

	  	Matrix3d i;
	  	i =  Matrix3d::Identity();				//Identity Matrix Formation

	  	//Matrix3f f = i.cast<float>();			//Converting Identity Matrix to float format
	  	transformation_matrix = local_base ;								//Transformation Matrix

	  cout<< "went through calculate transformation matrix" << endl;
 	  return 0;
   }

  void initialize_gdof(int _number_NDs)
  {
     int number_row = 2*_number_NDs*3;
	  gdof_map.resize(number_row,3);
	  cout<< "went through gdof_matrix initialization" << endl;
  }

  void set_sector_angle_radian(double sector_angle)
  {
	  sector_angle_radian = sector_angle;
  }

  void set_element__normalforce_stiffness(double _kn, double _ktx , double _kty , double _mu, double _n_o)
  {
	    k_n = _kn;
	    ktx = _ktx;
	    kty = _kty;
	    mu  = _mu;
	    N_o = _n_o;

	    cout << "set the normal force and stiffness as" << endl;
	    cout << k_n << endl;
	    cout << ktx << endl;
	    cout << kty << endl;
	    cout << mu << endl;
	    cout << N_o  << endl;



  }

  void initialize_gdofmap(VectorXd _ND)
    {
	  Vector3f gdof(1000,2000,3000);
	  for (int i=0; i<_ND.size();i++)
	  {
		  /*if (_ND(i) == 0 )
		 {

		 }*/
          dof_dict.insert(std::make_pair(i,dof_info(_ND(i),'c', 'm', gdof) ) );

	  }

  	  cout<< "went through gdof_map initialization" << endl;
    }

  void add_gdofmap(int key,int _slave_master, int _cosine_sin, int _nodal_dia, VectorXf _dof)
      {


            dof_dict.insert(std::make_pair(key,dof_info(_nodal_dia,_cosine_sin, _slave_master, _dof) ) );

    	  cout<< " gdof_map add nod_dia cos_sin sla_mas" << endl;
    	  cout<< "                "<<_nodal_dia  << "       "<< _cosine_sin << "      "<< _slave_master<< endl;
    	  cout << _dof << endl;
      }

  VectorXf get_gdofmap(int _key)
  {
	  VectorXf gdof;
   it = dof_dict.find(_key);
      if (it != dof_dict.end())
      {
       gdof = it ->second.gdof;
       }
      return gdof;
  }


  MatrixXd getelement_gdofmap()
    {
      return gdof_map;

    }

  void call_calculate_forcenl(int number_nodaldia, VectorXf &fglobal, VectorXf &u_corrected ,VectorXf &Un,  VectorXf &Unminus);


  MatrixXf  calculate_forcenl(VectorXf _Unminus_master_cosine, VectorXf _Unminus_slave_cosine,      VectorXf _Un_master_cosine, VectorXf _Un_slave_cosine,
			VectorXf _Unminus_master_sin, VectorXf _Unminus_slave_sin,      VectorXf _Un_master_sin, VectorXf _Un_slave_sin);

  void calculate_jacobian ();

  MatrixXf evaluate_relative_displacement(VectorXf _Unminus_master_cosine,VectorXf _Un_master_cosine,
		  VectorXf _Unminus_master_sin,VectorXf _Un_master_sin );

  VectorXf calculate_slave_fnl(Vector3f _fnl_global_master);


};







#endif /* FRICTION_ELEMENT_H_ */

