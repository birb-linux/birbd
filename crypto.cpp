#include "Crypto.hpp"
#include <fstream>
#include <openssl/evp.h>

std::string md5sum(const std::string& data)
{
	/* Some boilerplate politely stolen partially
	 * from https://stackoverflow.com/a/73843621 */
	EVP_MD_CTX* context = EVP_MD_CTX_new();
	const EVP_MD* md = EVP_md5();
	unsigned char md_value[EVP_MAX_MD_SIZE];
	unsigned int md_len;

	EVP_DigestInit_ex(context, md, NULL);
	EVP_DigestUpdate(context, data.c_str(), data.size());
	EVP_DigestFinal_ex(context, md_value, &md_len);
	EVP_MD_CTX_free(context);

	std::string result;
	result.reserve(md_len * 2);

	char buffer[sizeof(int)];
	for (int i = 0; i < md_len; i++)
	{
		snprintf(buffer, sizeof(int), "%02x", md_value[i]);
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
