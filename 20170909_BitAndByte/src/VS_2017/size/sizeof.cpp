#include <iostream>
#include <typeinfo>
#include <iomanip>
#include <stdint.h>

void printTitle()
{
	std::cout
		<< "tipe data"
		<< " => "
		<< "ukuran"
		<< std::endl;
}

void print(const char* name, size_t size)
{
	std::cout
		<< std::setw(9) << name
		<< " => "
		<< size << " byte"
		<< (size > 1 ? "s" : "")
		<< std::endl;
}

#ifndef _MSC_VER
#include <cxxabi.h>
template <typename T>
void print()
{
	int status = 0;
	auto realname = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
	print(realname, sizeof(T));
	free(realname);
}
#else
template <typename T>
void print()
{
	print(typeid(T).name(), sizeof(T));
}

#endif


int main()
{
	printTitle();
	print<bool>();
	print<char>();
	print<wchar_t>();
	print<short>();
	print<int>();
	print<long>();
	print<long long>();
}