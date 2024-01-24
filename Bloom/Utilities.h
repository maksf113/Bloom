#pragma once
#include <iostream>
#include<string>
#include <fstream>
#include <errno.h>
#include <cassert>

bool ReadFile(const char* pFileName, std::string& outFile);
char* ReadBinaryFile(const char* pFileName, int& size);