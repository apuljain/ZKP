#ifndef COMMON_FUNCTIONS_H_GUARD
#define COMMON_FUNCTIONS_H_GUARD

#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cryptopp/integer.h>
#include <cryptopp/modarith.h>
#include <cryptopp/nbtheory.h>
#include <cryptopp/randpool.h>
#include <cryptopp/osrng.h>
#include <cryptopp/dh.h>

using namespace std;
using namespace CryptoPP;

/*Generate group parameters*/
void GetGroupParameters(Integer&, Integer&, Integer&);

/*Convert Integer to String*/
string IntegerToString(Integer);

/*Calculate SHA1 hash*/
string HashCalculate(string);

/*Function to print vector*/
void PrintVector(vector<Integer>);

/*Generates string from vector*/
string GenerateString(vector<Integer>);

/*Function to compute input string for hash function*/
string ComputeHashString(vector<Integer> generators, vector<Integer> public_keys,
                         vector<Integer> T);
#endif
