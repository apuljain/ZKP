#include "DiscreteLogarithmVerifier.hpp"
#include "DiscreteLogarithmProver.hpp"
#include "CommonFunctions.hpp"
#include <iostream>
#include <cryptopp/integer.h>
#include <cryptopp/modarith.h>
#include <cryptopp/randpool.h>
#include <cryptopp/osrng.h>

using namespace CryptoPP;
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

	RandomPool rng;

	for(unsigned int i = 0; i < num_members; i++)
	{
		//taking generators g, g^2, g^3..
		generators.push_back(a_exp_b_mod_c(g, i + 1, p));

		if(i == identity)
		{
		    //FIXME Take care of weak private key - such as 1.
			private_key = Integer(rng, 1, q - 1);
			public_keys.push_back(a_exp_b_mod_c(generators[i], private_key, p));
		}

		else
		{
            //FIXME Take care of weak private key - such as 1.
			public_keys.push_back(a_exp_b_mod_c(g, Integer(rng, 1, q - 1), p));
		}
	}



	//pass no. of members as initialization parameter
	DiscreteLogarithmProver P(num_members, private_key, public_keys,
                              generators, p, q, identity);

	//declare input parameters
	Integer Hash;
	vector<Integer> C, R;

	P.GenerateProof(Hash, C, R);

	DiscreteLogarithmVerifier V(num_members, public_keys, generators, p, q);

	if(V.VerifyProof(Hash, C, R))
        cout << "SUCCESS" << endl;
}
