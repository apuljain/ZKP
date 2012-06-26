#include "DiscreteLogarithmVerifier.hpp"
#include "DiscreteLogarithmProver.hpp"
#include <iostream>
#include <cryptopp/Integer>
#include <cryptopp/osrng.h>

using namespace Cryptopp
using namespace std;

int main()
{
	//generate group parameters
	Integer g, p, q;
	
	//set group members
	unsigned int num_members = 2;

	GetGroupParameters(g, p, q);

	//set identity
	unsigned int identity = 0;	

	//generators
	vector<Integer> generators, public_keys;

	//variable to store private key.
	Integer private_key;	

	for(unsigned int i = 0; i < num_members; i++)
	{
		//taking generators g, g^2, g^3..
		generators.push_back(a_exp_b_mod_c(g, i, p));
		
		//FIXME Take care of case when private_key is 1.
		
		if(i == identity)
		{
			private_key = Integer(rng, 1, q - 1);
			public_keys.push_back(a_exp_b_mod_c(g, private_key, p));
		}		
			
		else
		{		
			public_keys.push_back(a_exp_b_mod_c(g, Integer(rng, 1, q - 1), p));
		}	
	}

	

	//pass no. of members as initialization parameter
	DiscreteLogarithmProver P(num_members, private_key, public_keys, generators, p,
			    q, identity);

	//declare input parameters
	Integer Hash;
	vector<Integer> C, R;

	P.GenerateProof(Hash, C, R);
	
	DiscreteLogarithmVerifier V(num_members, public_keys, generators, p, q);
	V.VerifyProof(Hash, C, R);
}
