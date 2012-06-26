#ifndef DISCRETE_LOGARITHM_PROVER_H_GUARD
#define DISCRETE_LOGARITHM_PROVER_H_GUARD

#include <iostream>
#include <string>
#include <vector>
#include <cryptopp/integer.h>
#include <cryptopp/modarith.h>
#include <cryptopp/randpool.h>
#include <cryptopp/osrng.h>

using namespace std;
using namespace CryptoPP;

class DiscreteLogarithmProver
{
	private:

	//declare set generators here
	vector<Integer> _generators;

	//declare p, q
	Integer _q, _p;

	//set private keys
	Integer _private_key;

	//set public_keys
	vector<Integer> _public_keys;

	//set no. of members
	unsigned int _num_members;

	//set self identity
	unsigned int _identity;
	public:
	//constructor parameter is no. of members
	DiscreteLogarithmProver(int n, Integer private_key_in,
                            vector<Integer> public_keys_in,
                            vector<Integer> generators,
                            Integer p_in, Integer q_in, unsigned int identity);

	/*Generate proof -- updates arguments*/
	void GenerateProof(Integer &Hash, vector<Integer> &c, vector<Integer> &r);
};

#endif
