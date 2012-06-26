#ifndef DISCRETE_LOGARITHM_VERIFIER_H_GUARD
#define DISCRETE_LOGARITHM_VERIFIER_H_GUARD

#include <iostream>
#include <string>
#include <vector>
#include <cryptopp/integer.h>

using namespace std;
using namespace CryptoPP;

class DiscreteLogarithmVerifier
{
	private:

	//declare set generators here
	vector<Integer> _generators;

	//declare p, q
	Integer _q, _p;

	//set public_keys
	vector<Integer> _public_keys;

	//set no. of members
	unsigned int _num_members;

	public:

	DiscreteLogarithmVerifier(int n, vector<Integer> public_keys_in,
                              vector<Integer> generators,
                              Integer p_in, Integer q_in);

	//Verifies Proof
	bool VerifyProof(const Integer &Hash, vector<Integer> &c, vector<Integer> &r);
};

#endif
