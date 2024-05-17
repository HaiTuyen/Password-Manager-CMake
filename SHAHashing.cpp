#include "SHAHashing.h"

SHAHashing::SHAHashing(string message)
{
	this->message = message;
}

SHAHashing::~SHAHashing()
{
}

string SHAHashing::Hash() const
{
	string digest;
	SHA256 sha256;
	StringSource _(this->message, true, new HashFilter(sha256, new StringSink(digest)));
	return digest;
}

string SHAHashing::Hash_hex() const
{
	string digest;
	SHA256 sha256;
	StringSource _(this->message, true, new HashFilter(sha256, new HexEncoder(new StringSink(digest))));
	return digest;
}
