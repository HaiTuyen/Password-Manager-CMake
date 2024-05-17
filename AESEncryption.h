// #pragma once
// #ifndef AES_ENCRYPTION_H
// #define AES_ENCRYPTION_H

#include <iostream>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include "SHAHashing.h"
#include "assert.h"

using std::cout;
using std::cerr;

#include <string>
using std::string;

#include <cryptopp/hex.h>
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

#include <cryptopp/osrng.h>
using CryptoPP::AutoSeededRandomPool;

#include <cryptopp/cryptlib.h>
using CryptoPP::BufferedTransformation;
using CryptoPP::AuthenticatedSymmetricCipher;

#include <cryptopp/filters.h>
using CryptoPP::Redirector;
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::AuthenticatedEncryptionFilter;
using CryptoPP::AuthenticatedDecryptionFilter;
using CryptoPP::ArraySink;

#include <cryptopp/aes.h>
using CryptoPP::AES;

#include <cryptopp/gcm.h>
using CryptoPP::GCM;

#define KEY_SIZE 32
#define IV_SIZE 16
#define TAG_SIZE 12

class AESEncryption
{
private:
	AutoSeededRandomPool prng;
	vector<CryptoPP::byte> key = vector<CryptoPP::byte>(KEY_SIZE);
	vector<CryptoPP::byte> iv = vector<CryptoPP::byte>(IV_SIZE);
	string plainText;

public:
	AESEncryption(const string& master_password, string plain_text);
	~AESEncryption();
	string encrypt();
	void setKey(const string& key);
	void setIV(const string& iv);
	vector<CryptoPP::byte> convertStringToByte(const string& str);
	string convertByteToString(const vector<CryptoPP::byte> byteArr);
	string getKey() const;
	string getIV() const;
};


class AESDecryption
{
private:
	AutoSeededRandomPool prng;
	vector<CryptoPP::byte> key = vector<CryptoPP::byte>(KEY_SIZE);
	vector<CryptoPP::byte> iv = vector<CryptoPP::byte>(IV_SIZE);
	string cipherText;

public:
	AESDecryption(string master_password, string cipher_byte_str);
	~AESDecryption();
	string decrypt();
	void setKey(const string&);
	void setIV(const string&);
	vector<CryptoPP::byte> convertStringToByte(const string& str);
	string convertByteToString(const vector<CryptoPP::byte> byteArr);
	string getKey() const;
	string getIV() const;
};

string convert_hex_str_to_byte_str(const string& hex);
string convert_byte_str_to_hex_str(const string& byte_str);

// #endif // AES_ENCRYPTION_H