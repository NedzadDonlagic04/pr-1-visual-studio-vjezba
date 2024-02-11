#include<iostream>
#include<iomanip>
#include<limits>

constexpr std::size_t BUFFER_SIZE{ 256 };

void clearBuffer();
void printErrorMsg(const char* const);

void enterText(char*, const std::size_t, const char* const);
void enterTexts(char**, const std::size_t, const std::size_t, const char* const);
void printTexts(const char* const * const, const std::size_t, const char* const);

void enterSize(std::size_t &, const char* const);

void allocateTexts(char**&, const std::size_t, const std::size_t);
void deallocateTexts(char**&, const std::size_t);

int main() {
	std::size_t textsCount{};

	enterSize(textsCount, "Unesi broj tekstova: ");
	
	char** texts{};

	allocateTexts(texts, textsCount, BUFFER_SIZE);

	enterTexts(texts, textsCount, BUFFER_SIZE, "\nUnesi tekstove\n");
	printTexts(texts, textsCount, "\nIspis tekstova\n");

	deallocateTexts(texts, textsCount);

	return 0;
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

void enterText(char* text, const std::size_t bufferSize, const char* const outputMsg) {
	do {
		if (std::cin.fail()) {
			printErrorMsg("Nevalidan unos\n");
			clearBuffer();
		}

		std::cout << outputMsg;
		std::cin.getline(text, bufferSize);
	} while (std::cin.fail());
}

void enterTexts(char** texts, const std::size_t textsCount, const std::size_t bufferSize, const char* const outputMsg) {
	std::cout << outputMsg;

	char tempOutputMsg[BUFFER_SIZE]{};
	for (std::size_t i = 0; i < textsCount; i++) {
		sprintf_s(tempOutputMsg, BUFFER_SIZE, "Unesi %llu. tekst: ", i + 1);
		enterText(texts[i], bufferSize, tempOutputMsg);
	}
}

void printTexts(const char* const* const texts, const std::size_t textsCount, const char* const outputMsg) {
	std::cout << outputMsg;

	for (std::size_t i = 0; i < textsCount; i++) {
		std::cout << i + 1 << ". tekst: " << std::quoted(texts[i]) << std::endl;
	}
}

void enterSize(std::size_t &size, const char* const outputMsg) {
	bool repeatLoop{};

	do {
		repeatLoop = false;

		std::cout << outputMsg;
		std::cin >> size;

		if (std::cin.fail()) {
			printErrorMsg("Nevalidan unos\n");
			clearBuffer();
			repeatLoop = true;
		}
		else if (size < 2 || size > 1000) {
			printErrorMsg("Unos treba biti u intervalu [2, 1000]\n");
			clearBuffer();
			repeatLoop = true;
		}
	} while (repeatLoop);

	clearBuffer();
}

void allocateTexts(char**& texts, const std::size_t textsCount, const std::size_t bufferSize) {
	texts = new char* [textsCount] {};
	for (std::size_t i = 0; i < textsCount; i++) {
		texts[i] = new char[bufferSize] {};
	}
}

void deallocateTexts(char**& texts, const std::size_t textsCount) {
	for (std::size_t i = 0; i < textsCount; i++) {
		delete[] texts[i];
	}
	delete[] texts;
	texts = nullptr;
}
