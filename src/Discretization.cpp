/*
 * Assemble.cpp
 *
 *  Created on: 31 Jul 2018
 *      Author: SIV0021
 */

#include "Discretization.h"
#include "Main.h"
Discretization::Discretization()
{


	 return;
}



void Discretization :: set_fric_elem_gdofdict_(

		MatrixXd _node_dof_information)


{
	/*for (int i=0; i< _friction_element.get_size();i++)
	{
		MatrixXd gdof_map= _friction_element[i] -> getelement_gdofmap();
		cout << gdof_map << endl;
		cout << "----------" << endl;
	}*/
	int counter=0;
	std :: map <int, Vector3f > local_dof_dict;
	       map<int, Vector3f >::iterator it;
	for (int i=0; i<node.get_size();i++)
	{


			Vector3f local_dof (_node_dof_information(counter,2),
                                  _node_dof_information(counter+1,2),
					              _node_dof_information(counter+2,2) );

		local_dof_dict.insert(std::pair<int, Vector3f>(i,local_dof));
		counter+=3;



	}



VectorXd global_dof;
VectorXd dof_oneblock = _node_dof_information.col(2);
VectorXd dof_oneblock_sin;
VectorXd dof_oneblock_cosin;
int slave_id;
int master_id;

int slave_index =0;
int master_index =1;
int key_index_cosine = 0;
int key_index_sine = 1;

int dof_additive = size_of_onesystem;
cout << "-----"<< endl;
//cout << _node_dof_information.col(2);
	for (int i=0; i< no_ND ; i++)    //_ND.size()
	{
		Vector4d keys(0,1,2,3);
		Vector4d Iden(1,1,1,1);
		Vector4d new_key = keys + 4*Iden*i;
		cout << new_key << " keys"<< endl;

		cout << "---------ND" << i << "----------" << endl;
    //global_dof = dof_oneblock;
          for (int j=0; j< friction_elements.get_size();j++ )
          {
              slave_id = friction_elements[j] -> get_slave_object() -> get_id();
        	  master_id = friction_elements[j] ->get_master_object() -> get_id();
              VectorXf dof_slave = local_dof_dict[slave_id] ;
              VectorXf dof_master = local_dof_dict[master_id] ;

              dof_additive = size_of_onesystem*2*i;
              VectorXf dof_slave_modified = dof_slave + dof_additive*VectorXf::Ones(3);
              VectorXf dof_master_modified = dof_master + dof_additive*VectorXf::Ones(3);

             friction_elements[j]  -> add_gdofmap(new_key(0), slave_index,
            		                               key_index_cosine,
					                               ND(i),
												   dof_slave_modified	 );
             friction_elements[j]  -> add_gdofmap(new_key(1), master_index,
                        		                               key_index_cosine,
            					                               ND(i),
															   dof_master_modified	 );

             dof_additive = dof_additive+ size_of_onesystem;
              dof_slave_modified = dof_slave + dof_additive*VectorXf::Ones(3);
              dof_master_modified = dof_master + dof_additive*VectorXf::Ones(3);


             friction_elements[j]  -> add_gdofmap(new_key(2), slave_index,
                        		                               key_index_sine,
            					                               ND(i),
															   dof_slave_modified	 );
             friction_elements[j]  -> add_gdofmap(new_key(3), master_index,
                        		                               key_index_sine,
            					                               ND(i),
															   dof_master_modified	 );

          }







	}

}


void Discretization :: read_information()
{
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// read file and put it in a vector
		   ifstream myReadFile;
		   myReadFile.open("output_from_python.txt");
		   int no_lines;
		   myReadFile >> no_lines;
		   Eigen:: VectorXd fri_ele_information(no_lines);

		   int counter =0;
		   if (myReadFile.is_open()) {
		      while (!myReadFile.eof()) {
		         myReadFile >> fri_ele_information(counter);
		         counter++;
		      }
		     }

	// read the second file
		  	std::ifstream lInputFile("output_from_python_ver4.txt");

		   		int number_of_lines;

		   		lInputFile >> number_of_lines;
		   		no_retained_nodes = number_of_lines/3;
		   		cout << no_retained_nodes << " is the number of nodes"<< endl;

		   		Eigen::MatrixXd node_dof_information(number_of_lines,3);
		   		int lRow,lCol,lValue,lEntryNumber =0;
		   		while (!lInputFile.eof())
		   		    {
		   		        lInputFile >> lRow;
		   		        lInputFile >> lCol;

	                    lInputFile >> lValue;

		   		     node_dof_information(lEntryNumber, 0) = lRow;
		   		     node_dof_information(lEntryNumber, 1) = lCol;
		   		     node_dof_information(lEntryNumber, 2) = lValue;

		   		     lEntryNumber++;
		   		    }
		   	   lInputFile.close();
		   	   cout << " node - direction - dof for the reduced system" << endl;
		   	   cout << node_dof_information << endl;

	// read the third file
		   	std::ifstream lInputFile1("output_from_python_ND.txt");

		   	lEntryNumber =0;
		   	lInputFile1 >> no_ND;
		   	no_ND--;
		   	ND.resize(no_ND);

		   	while (!lInputFile1.eof())
		   		   		    {
		   		             lInputFile1 >> ND(lEntryNumber);
		   		   		     lEntryNumber++;
		   		   		    }
		   		   	   lInputFile1.close();
		   		   	   cout << "nodal diameters" << endl;
		   		   	   cout << ND << endl;
double sector_angle = 0.0436332;

		   		   	///////////////////////////////////////////////////////////////////////////////////////////////////////////
		   		   	// create node objects
		   		   	node.resize( no_retained_nodes );
		   		   	for (int i=0; i<no_retained_nodes; i++){


		   		   		node[ i ] = new Node( i );

		   		   		cout << node[i]-> get_id() << endl;
		   		   	}
		   		   	///////////////////////////////////////////////////////////////////////////////////////////////////////////
		   		   	// create friction elements
		   		   	no_friction_elements    = fri_ele_information(0);
		   		   	int friction_element_counter      = 1;
		   		   	int fric_ele_normal_ind           = no_friction_elements*2+1;
		   		    int fric_ele_stiffn_ind           = no_friction_elements*2+no_friction_elements*3 +1;

		   		   	int master_node, slave_node;
		   		   	friction_elements.resize(no_friction_elements);
		   		   	//cout << fric_ele_normal_ind << " fric ele normal index" <<endl;
		   		   	//cout << fri_ele_information(fric_ele_normal_ind) << " fric ele normal index" <<endl;

		   		   	    for (int i=0; i<no_friction_elements; i++)
		   		   	    {
		   		   			cout << "------------------------" << endl;
		   		   			cout << " Friction element " << i+1<<endl;
		   		   			slave_node = fri_ele_information(friction_element_counter);

		   		   			cout << slave_node << " slave node"<<endl;
		   		   			master_node  = fri_ele_information(++friction_element_counter);

		   		   			cout << master_node << " master node"<< endl;
		   		   			friction_elements[ i ] = new Friction_element(i,slave_node,master_node,node);
		   		   			friction_element_counter++;

		   		   			friction_elements[i] -> set_normal_vector(fri_ele_information(fric_ele_normal_ind),fri_ele_information(fric_ele_normal_ind+1),fri_ele_information(fric_ele_normal_ind+2));
		   		   			friction_elements[i] -> calculate_bases();
		   		   			friction_elements[i] -> calculate_transformation();
		   		   			friction_elements[i] -> initialize_gdof(no_ND);
		   		   		    friction_elements[i] -> set_sector_angle_radian(sector_angle);
		   		   		    friction_elements[i] -> set_element__normalforce_stiffness(fri_ele_information(fric_ele_normal_ind),
		   		   		    	                                                       fri_ele_information(fric_ele_normal_ind+1),
								                                                       fri_ele_information(fric_ele_normal_ind+2),
						                                                            	fri_ele_information(fric_ele_normal_ind+3),
								                                                       fri_ele_information(fric_ele_normal_ind+4)
		   		   		    		                                                                                          );
		   		   			//friction_element[i] -> initialize_gdofmap(ND);
		   		   			fric_ele_normal_ind+=3;
		   		   		    fric_ele_stiffn_ind+=5;
		   		   			Vector3f disp_vect = friction_elements[i]->get_normal_vector();
		   		   			cout << disp_vect << " is the normalvector" <<  endl;
		   		   	        //cout << friction_element[i]->get_gdofmap(1) << endl;
		   		   	    }
		   		   	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		   		   	// set the global dof for the friction elements

		   		   	    set_fric_elem_gdofdict_(node_dof_information);


}




