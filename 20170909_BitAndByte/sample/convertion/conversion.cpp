#include <iostream>
#include <bitset>
#include <iomanip>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include "wy_menu.h"

enum class print
{
	bin,
	hex,
	dec,
};
// private helper
namespace {

	// https://stackoverflow.com/questions/27952032/how-to-insert-a-character-every-n-characters-in-a-string-in-c
	auto separate_(const std::string & input, size_t num, const char sep)
	{
		std::string str(input);
		for (auto it = str.begin(); (num + 1) <= std::distance(it, str.end()); ++it)
		{
			std::advance(it, num);
			it = str.insert(it, sep);
		}
		return std::move(str);
	}

	// highest bit set position
	// http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogDeBruijn
	unsigned int hibitPos(unsigned int v)
	{
		static const int MultiplyDeBruijnBitPosition[32] =
		{
			0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
			8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
		};

		v |= v >> 1; // first round down to one less than a power of 2
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;

		return MultiplyDeBruijnBitPosition[(uint32_t)(v * 0x07C4ACDDU) >> 27];
	}

	// highest bit set position
	// https://stackoverflow.com/questions/21888140/de-bruijn-algorithm-binary-digit-count-64bits-c-sharp
	unsigned int hibitPos64(uint64_t v)
	{
		static const int MultiplyDeBruijnBitPosition[64] =
		{
			0, // change to 1 if you want bitSize(0) = 1
			1,  2, 53,  3,  7, 54, 27, 4, 38, 41,  8, 34, 55, 48, 28,
			62,  5, 39, 46, 44, 42, 22,  9, 24, 35, 59, 56, 49, 18, 29, 11,
			63, 52,  6, 26, 37, 40, 33, 47, 61, 45, 43, 21, 23, 58, 17, 10,
			51, 25, 36, 32, 60, 20, 57, 16, 50, 31, 19, 15, 30, 14, 13, 12
		}; // table taken from http://chessprogramming.wikispaces.com/De+Bruijn+Sequence+Generator

		v |= v >> 1; // first round down to one less than a power of 2
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v |= v >> 32;
		if (v == (uint64_t)-1) return 64;
		v++;
		return  MultiplyDeBruijnBitPosition[(uint64_t)(v * 0x022fdd63cc95386dull) >> 58];
	}


	template <size_t S>
	void printBit(unsigned long long val)
	{
		std::bitset<S> value(val);
		std::cout << "binary: " << separate_(value.to_string(), 4, ' ') << std::endl;
	}

	void printResult(print type, long long value, bool as32Bit = false)
	{
		switch (type)
		{
		case print::bin:
		{
			// NOTE: no semicolon
#define print_if(bit_pos, max_bit, val)\
		if (bit_pos <= max_bit)\
			printBit<max_bit>(val)
#define else_print_if(bit_pos, max_bit, val)\
		else print_if(bit_pos, max_bit, val)

			auto abs_val = abs(value);
			unsigned int bit_pos = 0;
			if ((abs_val < std::numeric_limits<unsigned int>::max())
				&& (as32Bit == true))
				bit_pos = hibitPos(value);
			else
				bit_pos = hibitPos64(value);

			print_if(bit_pos, 4, value);
			else_print_if(bit_pos, 8, value);
			else_print_if(bit_pos, 16, value);
			else_print_if(bit_pos, 24, value);
			else_print_if(bit_pos, 32, value);
			else_print_if(bit_pos, 36, value);
			else_print_if(bit_pos, 40, value);
			else_print_if(bit_pos, 44, value);
			else_print_if(bit_pos, 48, value);
			else_print_if(bit_pos, 52, value);
			else_print_if(bit_pos, 56, value);
			else_print_if(bit_pos, 60, value);
			else_print_if(bit_pos, 64, value);

#undef print_if
#undef else_print_if

		}
		break;
		case print::hex:
			std::cout
				<< "hexadecimal: "
				<< "0x"
				<< std::uppercase
				<< std::hex
				<< std::setw(8)
				<< std::setfill('0')
				<< value
				<< std::endl;
			break;
		case print::dec:
			std::cout
				<< "decimal: "
				<< std::resetiosflags(std::ios_base::basefield)
				<< value
				<< std::endl;
			break;
		default:
			break;
		}
	}
}
class displayMenu
	: public wy::menu
{
public:
	displayMenu() : m_width(2) {}
	void onPause() override
	{
		std::cout << "press any key to continue...";
		std::cin.putback('\n');
		std::cin.get();
		_clearCin();
	}
	void displayItem(size_t index, const std::string& item) override
	{
		std::cout << std::setw(m_width) << index << ". " << item << std::endl;
	}
	size_t userInput() override
	{
		size_t input = -1;
		std::cout << "pilihan anda: ";
		if (!(std::cin >> input))
			input = -1;
		_clearCin();
		return input;
	}

	bool onBadInput(size_t badIndex) override
	{
		std::cout << "Invalid input!" << std::endl;
		return true;
	}

	void onStartMenu() override
	{
		_clearScreen();
		std::cout << "konversi Binary - Decimal - Hexadecimal" << std::endl
			<< "Ery E. Wahyutama" << std::endl
			<< "Indonesia C/C++ Warriors" << std::endl;
	}
	void onExitMenu() override
	{
		this->clear();
		std::cout << "Terima kasih..." << std::endl;
	}
	void onInitMenu() override
	{
		//TODO: bongkar wxwidgets bagian onEvent to remplement this
		this->push("convert from binary to decimal",
			static_cast<Entry>(&displayMenu::BinToDec));
		this->push("convert from binary to hexadecimal",
			static_cast<Entry>(&displayMenu::BinToHex));
		this->push("convert from decimal to binary",
			static_cast<Entry>(&displayMenu::DecToBin));
		this->push("convert from decimal to hexadecimal",
			static_cast<Entry>(&displayMenu::DecToHex));
		this->push("convert from hexadecimal to binary",
			static_cast<Entry>(&displayMenu::HexToBin));
		this->push("convert from hexadecimal to decimal",
			static_cast<Entry>(&displayMenu::HexToDec));
		_init();
	}

	bool getInput(std::string& out, bool(*validate)(std::string&))
	{
		std::string line;
		std::getline(std::cin, line);
		if (validate(line))
		{
			out = std::move(line);
			return true;
		}
		out = "";
		return false;
	}
	long long getBinaryInput()
	{
		bool is_true = false;
		std::string line;
		do {
			std::cout << "input: ";
			if (!(is_true = getInput(line, [](std::string& str)->bool {
				if (str.length() == 0)
					return false;

				// remove any space
				wy::str::removeCharsFromString(str, " ");

				return wy::str::is_any(str, [](int C)->int {
					switch (C)
					{
					case '0':
					case '1':
						return 1;
					}
					return 0;
				});
			})))
			{
				std::cout << "Invalid input!" << std::endl;
			}
		} while (is_true == false);
		char* endptr = nullptr;
		return std::strtoll(line.c_str(), &endptr, 2);
	}
	retval BinToDec()
	{
		std::cout << "--Binary to Decimal--" << std::endl;

		printResult(print::dec, getBinaryInput());

		return retval::pause_;
	}
	retval BinToHex()
	{
		_clearScreen();
		std::cout << "--Binary to Hexadecimal--" << std::endl;

		printResult(print::hex, getBinaryInput());


		return retval::pause_;
	}

	long long getDecimalInput()
	{
		bool is_true = false;
		std::string line;
		do {
			std::cout << "input: ";
			if (!(is_true = getInput(line, [](std::string& str)->bool {
				if (str.length() == 0)
					return false;

				return wy::str::is_any(str, [](int C)->int {
					switch (C)
					{
					case '0': case '5':
					case '1': case '6':
					case '2': case '7':
					case '3': case '8':
					case '4': case '9':
					case '-':
						return 1;
					}
					return 0;
				});
			})))
			{
				std::cout << "Invalid input!" << std::endl;
			}
		} while (is_true == false);
		char* endptr = nullptr;
		return std::strtoll(line.c_str(), &endptr, 10);
	}



	retval DecToBin() {
		_clearScreen();
		std::cout << "--Decimal to Binary--" << std::endl;
		printResult(print::bin, getDecimalInput(), true);

		return retval::pause_;
	}

	retval DecToHex() {
		_clearScreen();
		std::cout << "--Decimal to Hexadecimal--" << std::endl;

		printResult(print::hex, getDecimalInput(), true);

		return retval::pause_;
	}

	long long getHexadecimalInput()
	{
		bool is_true = false;
		std::string line;
		do {
			std::cout << "input: ";
			if (!(is_true = getInput(line, [](std::string& str)->bool {
				if (str.length() == 0)
					return false;

				std::string internal_str(str);
				std::transform(internal_str.begin(), internal_str.end(), internal_str.begin(),
					[](unsigned char c) -> unsigned char { return std::toupper(c); });

				// advance if there is 0X
				if (internal_str.length() > 2)
				{
					if ((internal_str[0] == '0')
						&& (internal_str[1] == 'X'))
					{
						internal_str = internal_str.substr(2);
					}
				}

				if (wy::str::is_any(internal_str, [](int C)->int {
					switch (C)
					{
					case '0': case '5':
					case '1': case '6':
					case '2': case '7':
					case '3': case '8':
					case '4': case '9':
					case 'A': case 'B':
					case 'C': case 'D':
					case 'E': case 'F':
						return 1;
					}
					return 0;
				}))
				{
					str = std::move(internal_str);
					return true;
				}
				return false;
			})))
			{
				std::cout << "Invalid input!" << std::endl;
			}
		} while (is_true == false);
		char* endptr = nullptr;
		return std::strtoll(line.c_str(), &endptr, 16);
	}
	retval HexToBin() {
		_clearScreen();
		std::cout << "--Hexadecimal to Binary--" << std::endl;
		printResult(print::bin, getHexadecimalInput());

		return retval::pause_;
	}

	retval HexToDec() {
		_clearScreen();
		std::cout << "--Hexadecimal to Decimal--" << std::endl;
		printResult(print::dec, getHexadecimalInput());
		return retval::pause_;
	}

private:
	void _init()
	{
		std::stringstream ss;
		ss << (this->menuSize() + 1);
		m_width = std::max(size_t(2), ss.str().length());
	}
	void _clearCin()
	{
		std::cin.clear(); //clear bad input flag
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	void _clearScreen()
	{
#ifdef _WIN32
		::system("cls");
#else
		::system("clear");
#endif // _WIN32
	}
	size_t m_width;

};

int main()
{
	displayMenu menu;
	menu.init();
	menu.display();
}