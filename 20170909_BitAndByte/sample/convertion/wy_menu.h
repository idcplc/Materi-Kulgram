#pragma once

#include <vector>
#include <tuple>
#include <iostream>
#include <functional>

namespace wy {
	//TODO: move into separate file
	//COMMENT: but for demonstration purposes, this is sufficient;
	namespace str {
		// https://stackoverflow.com/questions/8888748/how-to-check-if-given-c-string-or-char-contains-only-digits
		static inline bool is_digits(const std::string &str)
		{
			return std::all_of(str.begin(), str.end(), ::isdigit); // C++11
		}
		static inline bool is_any(const std::string& str, int(*_pre)(int C))
		{
			return std::all_of(str.begin(), str.end(), _pre); // C++11
		}

		// https://stackoverflow.com/questions/5891610/how-to-remove-certain-characters-from-a-string-in-c
		static inline void removeCharsFromString(std::string &str_, char* charsToRemove)
		{
			for (unsigned int i = 0; i < strlen(charsToRemove); ++i) {
				str_.erase(std::remove(str_.begin(), str_.end(), charsToRemove[i]), str_.end());
			}
		}
	}
	//TODO: tenplate me (?)
	class menu
	{
	public:
		enum class retval
		{
			// quit the menu
			quit_,
			// return to menu
			return_,
			// pause the screen and then return to menu
			pause_,
		};
		typedef retval(menu::*Entry)();

		menu() { putExitMenu(); }
		// override this one to customize the exit menu text
		virtual void putExitMenu()
		{
			this->push("Exit", &menu::menuExit);
		}
		retval menuExit() { return retval::quit_; }
		void clear() { m_entries.clear(); }


		virtual void displayItem(size_t index, const std::string& item) = 0;
		virtual size_t userInput() = 0;
		virtual void onPause() = 0;
		virtual bool onBadInput(size_t badIndex) = 0;
		virtual void onStartMenu() = 0;
		virtual void onExitMenu() = 0;

		virtual void onInitMenu() = 0;
		size_t menuSize() { return m_entries.size(); }
		void init()
		{
			this->onInitMenu();
		}
		void display()
		{
			size_t init = menuSize() + 1;
			size_t input = init;
			do {
				this->onStartMenu();
				// 0 is reserved for exit(prev menu if any)
				size_t index = 0;
				for (auto& item : m_entries)
				{
					if (index != 0)
						this->displayItem(index, std::get<0>(item));
					index++;
				}
				this->displayItem(0, std::get<0>(m_entries[0]));

				// input
				input = init;
				input = this->userInput();

				if (input >= 0 && input < m_entries.size())
				{

					auto tuple = m_entries[input];
					auto invokeMe = std::get<1>(tuple);
					switch (std::invoke(invokeMe, this))
					{
					case retval::quit_:
						this->onExitMenu();
						input = 0;
						break;
					case retval::pause_:
						this->onPause();
						// --fallthrough--
					case retval::return_:
						break;

					}
				}
				else
				{
					// bad input
					if (true == this->onBadInput(input))
						this->onPause();
					else
					{
						this->onExitMenu();
						break;
					}
					input = init;
				}
			} while (input != 0);
		}

		void push(const std::string& menuText, Entry entry)
		{
			m_entries.push_back(std::make_tuple(menuText, entry));
		}
	private:
		std::vector<std::tuple<std::string, Entry>> m_entries;
	};
};