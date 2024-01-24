#include "Utilities.h"

bool ReadFile(const char* pFileName, std::string& outFile)
{
	std::ifstream f(pFileName);
	bool ret = false;

	if (f.is_open())
	{
		std::string line;
		while (getline(f, line))
		{
			outFile.append(line);
			outFile.append("\n");
		}
		f.close();
		ret = true;
	}
	else
		std::cout << "ReadFile error" << std::endl;
	return ret;
}

char* ReadBinaryFile(const char* pFileName, int& size)
{
	FILE* file = NULL;
	errno_t err = fopen_s(&file, pFileName, "rb");

	if (!file)
	{
		char buf[256] = { 0 };
		strerror_s(buf, sizeof(buf), err);
		std::cout << "Error opening file " << std::string(pFileName) << " error: " << buf << std::endl;
		exit(0);
	}

	struct stat stat_buf;
	int error = stat(pFileName, &stat_buf);

	if (error)
	{
		char buf[256] = { 0 };
		strerror_s(buf, sizeof(buf), err);
		std::cout << "Error getting file stats: " << buf << std::endl;
		return NULL;
	}

	size = stat_buf.st_size;

	char* p = (char*)malloc(size);
	assert(p);

	size_t bytes_read = fread(p, 1, size, file);

	if (bytes_read != size)
	{
		char buf[256] = { 0 };
		strerror_s(buf, sizeof(buf), err);
		std::cout << "Read file error: " << buf << std::endl;
		exit(0);
	}

	fclose(file);
	
	return p;
}