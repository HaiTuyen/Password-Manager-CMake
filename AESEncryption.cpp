#include "AESEncryption.h"

AESEncryption::AESEncryption(const string& master_password, string plainText)
{
	SHAHashing sha256(master_password);
	this->key = convertStringToByte(sha256.Hash());
	prng.GenerateBlock(this->iv.data(), this->iv.size());
	this->plainText = plainText;
}

AESEncryption::~AESEncryption()
{
}

string AESEncryption::encrypt()
{
	string cipherText, encoded;
	string ivString = convertByteToString(this->iv);
	//// Pretty print
	//encoded.clear();
	//StringSource(this->key.data(), this->key.size(), true,
	//	new HexEncoder(
	//		new StringSink(encoded)
	//	) // HexEncoder
	//); // StringSource
	////cout << "[Encryption] key: " << encoded << endl;

	//// Pretty print
	//encoded.clear();
	//StringSource(this->iv.data(), this->iv.size(), true,
	//	new HexEncoder(
	//		new StringSink(encoded)
	//	) // HexEncoder
	//); // StringSource
	//cout << "[Encryption] iv: " << encoded << endl;

	try
	{
		GCM<AES>::Encryption e;
		e.SetKeyWithIV(this->key.data(), this->key.size(), this->iv.data(), this->iv.size());

		StringSource(this->plainText, true,
			new AuthenticatedEncryptionFilter(e,
				new StringSink(cipherText), false, TAG_SIZE
			) // AuthenticatedEncryptionFilter
		); // StringSource
		cipherText.insert(0, ivString);

	}
	catch (CryptoPP::InvalidArgument& e)
	{
		cerr << "Caught InvalidArgument..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
	}
	catch (CryptoPP::Exception& e)
	{
		cerr << "Caught Exception..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
	}

	//// Pretty print
	//encoded.clear();
	//StringSource(cipherText, true,
	//	new HexEncoder(
	//		new StringSink(encoded)
	//	) // HexEncoder
	//); // StringSource
	//cout << "cipher text: " << encoded << endl;
	////cout << "cipher text size: " << cipherText.size() << endl;
	return cipherText;
}


vector<CryptoPP::byte> AESEncryption::convertStringToByte(const string& str)
{
	vector<CryptoPP::byte> byteArr(str.begin(), str.end());
	return byteArr;
}

string AESEncryption::convertByteToString(const vector<CryptoPP::byte> byteArr)
{
	string str(byteArr.begin(), byteArr.end());
	return str;
}


void AESEncryption::setKey(const string& key)
{
	this->key.assign(key.begin(), key.end());
}

void AESEncryption::setIV(const string& iv)
{
	this->iv.assign(iv.begin(), iv.end());
}


string AESEncryption::getKey() const
{
	return string(this->key.begin(), this->key.end());
}

string AESEncryption::getIV() const
{
	return string(this->iv.begin(), this->iv.end());
}


AESDecryption::AESDecryption(string master_password, string cipherText)
{
	SHAHashing sha256(master_password);
	this->key = convertStringToByte(sha256.Hash());
	this->iv = vector<CryptoPP::byte>(cipherText.begin(), cipherText.begin() + IV_SIZE);
	this->cipherText.assign(cipherText.begin() + IV_SIZE, cipherText.end());
}

AESDecryption::~AESDecryption()
{
}

string AESDecryption::decrypt()
{
	string plainText, encoded;
	//// Pretty print
	//encoded.clear();
	//StringSource(this->key.data(), this->key.size(), true,
	//	new HexEncoder(
	//		new StringSink(encoded)
	//	) // HexEncoder
	//); // StringSource
	//cout << "[Decryption] key: " << encoded << endl;

	//// Pretty print
	//encoded.clear();
	//StringSource(this->iv.data(), this->iv.size(), true,
	//	new HexEncoder(
	//		new StringSink(encoded)
	//	) // HexEncoder
	//); // StringSource
	//cout << "[Decryption] iv: " << encoded << endl;

	//// Pretty print
	//encoded.clear();
	//StringSource(convertStringToByte(this->cipherText).data(), this->cipherText.size(), true,
	//	new HexEncoder(
	//		new StringSink(encoded)
	//	) // HexEncoder
	//); // StringSource
	//cout << "[Decryption] ciphertext: " << encoded << endl;

	try
	{
		GCM<AES>::Decryption d;
		d.SetKeyWithIV(this->key.data(), this->key.size(), this->iv.data(), this->iv.size());

		AuthenticatedDecryptionFilter df(d,
			new StringSink(plainText),
			AuthenticatedDecryptionFilter::DEFAULT_FLAGS,
			TAG_SIZE
		); // AuthenticatedDecryptionFilter

		StringSource(this->cipherText, true,
			new Redirector(df /*, PASS_EVERYTHING */)
		); // StringSource

		bool b = df.GetLastResult();
		assert(true == b);

		//cout << "recovered text: " << plainText << endl;
		return plainText;
	}
	catch (CryptoPP::HashVerificationFilter::HashVerificationFailed& e)
	{
		cerr << "Caught HashVerificationFailed..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
	}
	catch (CryptoPP::InvalidArgument& e)
	{
		cerr << "Caught InvalidArgument..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
	}
	catch (CryptoPP::Exception& e)
	{
		cerr << "Caught Exception..." << endl;
		cerr << e.what() << endl;
		cerr << endl;
	}
}

vector<CryptoPP::byte> AESDecryption::convertStringToByte(const string& str)
{
	vector<CryptoPP::byte> byteArr(str.begin(), str.end());
	return byteArr;
}

string AESDecryption::convertByteToString(const vector<CryptoPP::byte> byteArr)
{
	string str(byteArr.begin(), byteArr.end());
	return str;
}


string convert_byte_str_to_hex_str(const string& byte_str)
{
	string encoded;
	encoded.clear();
	StringSource(byte_str, true,
		new HexEncoder(
			new StringSink(encoded)
		) // HexEncoder
	); // StringSource
	return encoded;
}

string convert_hex_str_to_byte_str(const string& hex)
{
	string decoded;
	decoded.clear();
	StringSource(hex, true,
		new HexDecoder(
			new StringSink(decoded)
		) // HexDecoder
	); // StringSource
	return decoded;
}




