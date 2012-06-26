#include "CommonFunctions.hpp"

/*Generate Group Parameters*/
void GetGroupParameters(Integer &g, Integer &p, Integer &q)
{
	AutoSeededRandomPool rnd;
	unsigned int bits = 1024;

	DH dh;
	dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, bits);

	if(!dh.GetGroupParameters().ValidateGroup(rnd, 3))
		cout << "Failed to validate prime and generator" << endl;

	size_t count = 0;

	p = dh.GetGroupParameters().GetModulus();
	count = p.BitCount();

	q = dh.GetGroupParameters().GetSubgroupOrder();
	count = q.BitCount();

	g = dh.GetGroupParameters().GetGenerator();
	count = g.BitCount();

	#ifdef DEBUG
	cout << "P (" << std::dec << count << "): " << std::hex << p << endl;
	cout << "Q (" << std::dec << count << "): " << std::hex << q << endl;
	cout << "G (" << std::dec << count << "): " << std::dec << g << endl;
	#endif


	Integer v = ModularExponentiation(g, q, p);
	if(v != Integer::One())
	{
		cout << "Failed to verify order of the subgroup" << endl;
	}
}

/*Convert CryptoPP::Integer to std::string*/
string IntegerToString(Integer a)
{
	stringstream ss;
	ss<<a;
	return ss.str();
}

/*Calculate SHA1 hash*/
string HashCalculate(string input_string)
{
	CryptoPP::SHA1 hash;
	byte digest[CryptoPP::SHA1::DIGESTSIZE];
	string output;

	//calculate hash
	hash.CalculateDigest(digest, (const byte *)input_string.c_str(), input_string.size());
	
	//encode in Hex
	CryptoPP::HexEncoder encoder;
	CryptoPP::StringSink *SS = new CryptoPP::StringSink(output);
	encoder.Attach(SS);
	encoder.Put(digest, sizeof(digest));
	encoder.MessageEnd();
	
	//prepend 0x
	output = "0x" + output;
	return output;
}

/*Function to print vector*/
void PrintVector(vector<Integer> v)
{
	for(vector<Integer>::iterator itr = v.begin(); itr != v.end(); itr++)
		cout<<*itr<<" ";	

	cout<<endl;
}

/*Generates string from vector*/
string GenerateString(vector<Integer> v)
{
	string output;
	Integer temp;

	for(vector<Integer>::iterator itr = v.begin(); itr != v.end(); itr++)
	{
		temp = *itr;
		output = output + IntegerToString(temp);
	}
	return output;
}

/*Function to compute input string for hash function*/
string ComputeHashString(vector<Integer> generators, vector<Integer> public_keys,
			 vector<Integer> T)
{
	string output;
	unsigned int i = 0;
	for( vector<Integer>::iterator itr = generators.begin();
	    itr != generators.end(); itr++, i++)
	{
		//g1, y1, g2, y2, ........ gn, yn
		output += IntegerToString(*itr) + IntegerToString(public_keys[i]);
	}

	//append t1, t2, ........ tn
	
	for(vector<Integer>::iterator itr = T.begin(); itr != T.end(); itr++)
	{
		output = output + IntegerToString(*itr);
	}	
	
	return output;
}

