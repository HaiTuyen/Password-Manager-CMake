#ifndef SHAHashing_H
#define SHAHashing_H
#pragma once
#include <iostream>
using std::string;

#include <cryptopp/sha.h>
using CryptoPP::SHA256;

#include <cryptopp/filters.h>
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::HashFilter;

#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;


class SHAHashing
{
private: 
	string message;
public:
	SHAHashing(string);
	~SHAHashing();
	string Hash() const;
	string Hash_hex() const;
};

#endif // SHAHashing_H

