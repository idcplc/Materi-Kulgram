#include <iostream>
#include <limits>
#include <iomanip>
#include <type_traits>

void printTitle()
{
	std::cout
		<< std::left
		<< std::setw(17)
		<< "tipe data"
		<< std::right
		<< std::setw(20)
		<< "nilai min"
		<< " "
		<< std::setw(20)
		<< "nilai max"
		<< std::endl;
}

void print(const char* name)
{
	std::cout
		<< std::left
		<< std::setw(16) << name << " "
		<< std::right;
}

#ifndef _MSC_VER
#include <cxxabi.h>
template <typename T>
void printName()
{
	int status = 0;
	auto realname = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
	print(realname);
	free(realname);
}
#else
template <typename T>
void printName()
{
	print(typeid(T).name());
}
#endif

// explisit cast dibutuhkan untuk tipe (unsigned) char
template <typename T, typename cast = T>
//typename std::enable_if<std::is_integral<T>::value>::type // c++11
std::enable_if_t<std::is_integral<T>::value> // c++14
print()
{
	printName<T>();
	std::cout
		<< std::setw(20) << (cast)std::numeric_limits<T>::min()
		<< " "
		<< std::setw(20) << (cast)std::numeric_limits<T>::max()
		<< std::endl;
}

template <typename T>
//typename std::enable_if<std::is_integral<T>::value>::type // c++11
std::enable_if_t<std::is_floating_point<T>::value> // c++14
print()
{
	printName<T>();
	std::cout
		<< std::setw(20) << -std::numeric_limits<T>::max()
		<< " "
		<< std::setw(20) << std::numeric_limits<T>::max()
		<< std::endl;
}

int main()
{
	printTitle();
	print<bool>();

	// note the second arg
	print<char, int>();
	print<unsigned char, int>();

	print<short>();
	print<unsigned short>();
	print<int>();
	print<unsigned int>();
	print<long>();
	print<unsigned long>();
	print<long long>();
	print<unsigned long long>();

	// bonus
	print<float>();
	print<double>();
	print<long double>();
}