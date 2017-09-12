#include <iostream>
#include <bitset>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <tuple>


// https://stackoverflow.com/questions/27378163/convert-bitseta-to-signed-int-with-a32
// https://graphics.stanford.edu/~seander/bithacks.html#FixedSignExtend
template<std::size_t B>
long bitset_to_long(const std::bitset<B>& b) {
	struct { long x : B; } s;
	return s.x = b.to_ulong();
}

int main()
{
	typedef std::bitset<4> bit_4;
	const bit_4 max = -1;

	// one time init then dextroy
	auto numLen = [&max]() {
		std::stringstream ss;
		ss << max.to_ulong();
		return ss.str().length();
	}();


	auto craftPair = [](const char* string, size_t len) {
		std::stringstream ss(string);
		len = std::max(len, ss.str().length());
		return std::make_tuple(string, len);
	};

	auto index = craftPair("index", numLen);
	auto bit_val = craftPair("bit value", max.count() + 1);
	auto unsigned_ = craftPair("unsigned", numLen + 1);
	auto signed_ = craftPair("signed", numLen + 1);
	auto hexa_ = craftPair("Hexadecimal", numLen + 1);

	// title
	std::cout
		<< std::setw(std::get<1>(index))
		<< "index" // start from 0
		<< "  "
		<< std::setw(std::get<1>(bit_val))
		<< "bit value"
		<< " "
		<< std::setw(std::get<1>(unsigned_))
		<< "unsigned"
		<< " "
		<< std::setw(std::get<1>(signed_))
		<< "signed"
		<< "  "
		<< std::setw(std::get<1>(hexa_))
		<< "hexadecimal"
		<< std::endl;

	bit_4 bit3(0);
	for (unsigned int i = 0; i <= max.to_ulong(); ++i, bit3 = i)
	{
		std::cout
			<< std::setw(std::get<1>(index))
			<< i << ". "
			<< std::setw(std::get<1>(bit_val))
			<< bit3
			<< " "
			<< std::setw(std::get<1>(unsigned_))
			<< bit3.to_ulong()
			<< " "
			<< std::setw(std::get<1>(signed_))
			<< bitset_to_long(bit3)
			<< " "
			<< std::setw(std::get<1>(hexa_))
			<< "0x"
			<< std::uppercase << std::hex << bit3.to_ulong()
			<< std::dec
			<< std::endl;
	}
}