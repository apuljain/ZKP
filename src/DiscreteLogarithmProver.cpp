#include "DiscreteLogarithmProver.hpp"
#include "CommonFunctions.hpp"

DiscreteLogarithmProver::DiscreteLogarithmProver(int n, Integer private_key_in,
                                                 vector<Integer> public_keys_in,
                                                 vector<Integer> generators,
                                                 Integer p_in, Integer q_in,
                                                 unsigned int identity)
{
		_num_members = n;
		_private_key = private_key_in;

		//loop to copy contents
		for(unsigned int i = 0; i < _num_members; i++)
		{
			_public_keys.push_back(public_keys_in[i]);
			_generators.push_back(generators[i]);
		}

		_p = p_in;
		_q = q_in;
		_identity = identity;
}


void DiscreteLogarithmProver::GenerateProof(Integer &Hash, vector<Integer> &C,
                                            vector<Integer> &R)
{
	//select v1,v2,.....vn randomly from the Zq
	//container for random vi's and wi's
	vector<Integer> vi, wi;

	unsigned int count = _num_members;

	RandomPool rng;

	while(count--)
	{
		//store vi
		vi.push_back(Integer(rng, 0, _q - 1));

		//store wi
		wi.push_back(Integer(rng, 0, _q - 1));
	}

	//calculate t1,t2,......tn-1
	vector<Integer> ti_values;

	for(unsigned int i = 0; i < _num_members; i++)
	{
		if(i == _identity)
			ti_values.push_back(a_exp_b_mod_c(_generators[i], vi[i]
					    , _p));
		else
		{
			//calculate ti = yi^wi*gi^vi (i = 1 to n-1)
			ti_values.push_back(a_times_b_mod_c(a_exp_b_mod_c(
                                    _public_keys[i], wi[i], _p),
                                a_exp_b_mod_c(_generators[i], vi[i],
                                    _p), _p));
		}
	}

	//convert into string to be used in hash function
	string input_string = ComputeHashString(_generators, _public_keys, ti_values);

	#ifdef DEBUG
	cout << endl << "Input string for hash: " << input_string << endl;
	#endif

	//calculate hash 'c' = H(g1, y1, g2, y2,.... gn, yn, t1, .....tn)
	string output = HashCalculate(input_string);	//now output contains
                                                    //hashed value in hexadecimal format

	Integer Hash1(output.c_str());	//output Hash string in decimal format

	#ifdef DEBUG
	cout << "=====TESTING HASH1 VALUE=====" << endl << Hash1 << endl;
	#endif

	//calculate proof components ci's
	//ci = wi for(i = 1 to n-1) and cn = c - sigma(ci (i = 1 to n - 1))

	vector<Integer> ci;
	count = _num_members - 1;

	//declare variable for integer sum
	Integer sum = 0;

	for(unsigned int i = 0; i < _num_members; i++)
	{
		ci.push_back(wi[i]);
		if(i != _identity)
			sum = (sum + wi[i]) % _q;
	}

	//FIXME Should Hash function automatically calculate modulo q ?
	Hash1 = Hash1 % _q;

	//calculate cn  = c - sum

	ci[_identity] = ((Hash1 - sum) % _q);

	//calculate ri = vi(mod q) and rn = vn - cn*xn(mod q)
	vector<Integer> ri;

	for(unsigned int i = 0; i < _num_members; i++)
	{
		if(i == _identity)
		{
			ri.push_back((vi[i] - a_times_b_mod_c(ci[_identity],
                                 _private_key, _q))%_q);
		}
		else
			ri.push_back(vi[i]);
	}

	//update function parameters
	Hash = Hash1;
	for(vector<Integer>::iterator itr = ri.begin(); itr != ri.end(); itr++)
	{
		R.push_back(*itr);
	}

	for(vector<Integer>::iterator itr = ci.begin(); itr != ci.end(); itr++)
	{
		C.push_back(*itr);
	}
}

