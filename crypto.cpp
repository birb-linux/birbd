#define OPENSSL_NO_DEPRECATED_3_0
#include "Crypto.hpp"
#include <fstream>
#include <openssl/md5.h>

std::string md5sum(const std::string& data)
{
	unsigned char result_chararr[MD5_DIGEST_LENGTH];
	MD5((unsigned char*)data.c_str(), data.length(), result_chararr);

	std::string result;
	char buffer[sizeof(int)];
	for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
	{
		snprintf(buffer, sizeof(int), "%02x", result_chararr[i]);
		result += buffer;
	}
	return result;
}

std::string get_file_hash(const std::string& file_path)
{
	std::ifstream file(file_path, std::ios::binary);
	std::string file_content = std::string(std::istreambuf_iterator<char>{file}, {});
	file.close();
	return md5sum(file_content);
}
