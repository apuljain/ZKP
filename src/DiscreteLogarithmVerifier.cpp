#include "DiscreteLogarithmVerifier.hpp"
#include "CommonFunctions.hpp"


DiscreteLogarithmVerifier::DiscreteLogarithmVerifier(int n, 
						     vector<Integer> public_keys_in,
		  				     vector<Integer> generators,
		  				     Integer p_in, Integer q_in)
{
	_num_members = n;

	//loop to copy contents		
	for(unsigned int i = 0; i < _num_members; i++)
	{
		_public_keys.push_back(public_keys_in[i]);
		_generators.push_back(generators[i]);
	}	
	
	_p = p_in;
	_q = q_in;
}


bool DiscreteLogarithmVerifier::VerifyProof(const Integer &Hash, vector<Integer> &c,
					    vector<Integer> &r)
{
   	//calculate ti_dash = yi^ci*gi^ri for i = 1 to n
	vector<Integer> ti_dash;
	
	for(unsigned int i = 0; i < _num_members; i++)
	{
		ti_dash.push_back(a_times_b_mod_c(a_exp_b_mod_c(_public_keys[i],
				  c[i], _p), a_exp_b_mod_c(_generators[i], r[i],
				  _p), _p));
	}	

	#ifdef DEBUG
	cout<<"\n=====TESTING ti_dash=====\n";
	PrintVector(ti_dash);
	cout<<endl;
	#endif

	//calculate hash2
	string input_string = ComputeHashString(_generators, _public_keys, ti_dash);
	
	string output = HashCalculate(input_string);
	Integer Hash2(output.c_str());

	cout<<"\n====TESTING HASH1 and HASH2====\n";
	cout<<"Hash1: "<<Hash<<endl;
	cout<<"Hash2: "<<Hash2<<endl;

	//verify proof by sigma{ci} mod p == Hash(g1, y1, g2, y2, ... gn, yn,
	//					   t1', t2', .... tn') mod p 
	//calculate sigma{ci}
	Integer sum;
	for(vector<Integer>::iterator itr1 = c.begin(); itr1 != c.end(); itr1++)
	{
		sum += *itr1;
		sum = sum % _q;
	}	
	
	cout<<"\n====TESTING sum{ci} mod p and Hash2 mod p=====\n";
	cout<<"Sum{ci}: "<<sum%_p<<endl;
	cout<<"Hash2: "<<Hash2%_p<<endl; 	
	
	return(sum%_p == Hash2%_p);
}
