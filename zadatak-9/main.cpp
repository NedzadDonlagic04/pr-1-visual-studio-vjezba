#include<iostream>
#include<limits>
#include<memory>
#include<iomanip>
#include<cmath>

void enterNumberText(char * const, const std::size_t, const char * const);
void clearBuffer();
void printErrorMsg(const char* const);
[[nodiscard]] bool isStringValidInt(const char* const);
[[nodiscard]] int recursiveStrToInt(const char* const, std::size_t = 0, int = 0);

int main() {
	constexpr std::size_t TEXT_BUFFER_SIZE{ 256 };

	const auto text{ std::make_unique<char[]>(TEXT_BUFFER_SIZE) };

	enterNumberText(text.get(), TEXT_BUFFER_SIZE, "Unesi neki broj (bit ce unesen kao tekst): ");

	std::cout << "Broj (ovaj put kao tekst) " << std::quoted(text.get()) << " je " << recursiveStrToInt(text.get()) << " kao broj\n";

	return 0;
}

void enterNumberText(char* const text, const std::size_t bufferSize, const char* const outputMsg) {
	bool repeatLoop{};

	do {
		repeatLoop = false;

		std::cout << outputMsg;
		std::cin.getline(text, bufferSize);

		if (std::cin.fail()) {
			repeatLoop = true;
			printErrorMsg("Nevalidan unos, moguce zbog duzine teksta\n");
			clearBuffer();
		}
		else if (!isStringValidInt(text)) {
			repeatLoop = true;
			printErrorMsg("Tekst nije validan cijeli broj\n");
		}
	} while (repeatLoop);
}

void clearBuffer() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printErrorMsg(const char* const errorMsg) {
	std::cout << "\033[31m";
	std::cout << errorMsg;
	std::cout << "\033[39m";
}

bool isStringValidInt(const char* const text) {
	std::size_t i{ 0 };

	if (text[i] == '-') {
		i++;
	}

	for (; i < std::strlen(text); i++) {
		if (!std::isdigit(text[i])) {
			return false;
		}
	}
	return true;
}

int recursiveStrToInt(const char* const text, std::size_t i, int stringAsInt) {
	if (i == std::strlen(text)) {
		if (text[0] == '-') {
			stringAsInt *= -1;
		}
		return stringAsInt;
	}
	else if (i == 0 && text[0] == '-') {
		i++;
	}

	stringAsInt = stringAsInt * 10 + text[i] - '0';

	return recursiveStrToInt(text, i + 1, stringAsInt);
}