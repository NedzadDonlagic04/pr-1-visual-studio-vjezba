#include<iostream>
#include<cstring>
#include<limits>
#include<iomanip>

constexpr std::size_t BUFFER_SIZE{ 4096 };

void clearBuffer();
void printErrorMsg(const char* const);

void enterSize(std::size_t&, const char* const);
void enterText(char*, const std::size_t, const char* const);

void enterTexts(char* const* const, const std::size_t, const std::size_t);
void printTexts(char* const* const, const std::size_t, const char* const); 

void allocateTexts(char**&, const std::size_t, const std::size_t);
void deallocateTexts(char**&, const std::size_t);

void capitalizeWords(char*);
void capitalizeInAllTextsWords(char* const* const, const std::size_t);

[[nodiscard]] int countOccurences(char*, char*);
[[nodiscard]] std::size_t getAllTextsSizes(const char* const* const, const std::size_t);
[[nodiscard]] char* combineAllTextsIntoOne(const char* const* const, const std::size_t);

int main() {
	std::size_t textCount{};

	enterSize(textCount, "Unesite broj tekstova za unjeti: ");

	char** texts{};
	
	allocateTexts(texts, textCount, BUFFER_SIZE);
	
	enterTexts(texts, textCount, BUFFER_SIZE);
	std::cout << std::endl;
	printTexts(texts, textCount, "Ispis tekstova nakon unosa\n");

	capitalizeInAllTextsWords(texts, textCount);
	std::cout << std::endl;
	printTexts(texts, textCount, "Ispis tekstova nakon povecanja tekstova\n");

	char* const combinedTexts{ combineAllTextsIntoOne(texts, textCount) };

	std::cout << "Tekstovi iznad spojeni u jedan sa ; izmedu su: " << std::quoted(combinedTexts) << std::endl << std::endl;;
	
	char* wordToFind{ new char[BUFFER_SIZE] };

	enterText(wordToFind, BUFFER_SIZE, "Unesite rijec za pronaci: ");

	std::cout << "Broj ponavljanja rijeci " << std::quoted(wordToFind) << " u tekstu iznad je " << countOccurences(combinedTexts, wordToFind) << std::endl;

	delete[] combinedTexts;
	delete[] wordToFind;

	deallocateTexts(texts, textCount);
	
	return 0;
}

void clearBuffer() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void printErrorMsg(const char* const outputMsg) {
	std::cout << "\033[31m";
	std::cout << outputMsg;
	std::cout << "\033[39m";
}

void enterSize(std::size_t& size, const char* const outputMsg) {
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

	// To eat the '\n'
	clearBuffer();
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

void enterTexts(char* const* const texts, const std::size_t textCount, const std::size_t bufferSize) {
	char outputMsg[BUFFER_SIZE]{};

	for (std::size_t i = 0; i < textCount; i++) {
		sprintf_s(outputMsg, "Unos %llu. texta: ", i + 1);
		enterText(texts[i], bufferSize, outputMsg);
	}
}

void printTexts(char* const* const texts, const std::size_t textCount, const char * const outputMsg) {
	std::cout << outputMsg;
	for (std::size_t i = 0; i < textCount; i++) {
		std::cout << i + 1 << ". tekst: " << std::quoted(texts[i]) << std::endl;
	}
}

void allocateTexts(char**& texts, const std::size_t textCount, const std::size_t bufferSize) {
	texts = new char* [textCount] {};
	
	for (std::size_t i = 0; i < textCount; i++) {
		texts[i] = new char[bufferSize] {};
	}
}

void deallocateTexts(char**& texts, const std::size_t textCount) {
	for (std::size_t i = 0; i < textCount; i++) {
		delete[] texts[i];
	}
	delete[] texts;
	texts = nullptr;
}

void capitalizeWords(char* text) {
	if (std::isalpha(text[0])) {
		text[0] = std::toupper(text[0]);
	}

	for (std::size_t i = 1; i < std::strlen(text); i++) {
		if (std::isalpha(text[i]) && std::isspace(text[i - 1])) {
			text[i] = std::toupper(text[i]);
		}
	}
}

void capitalizeInAllTextsWords(char* const* const texts, const std::size_t textCount) {
	for (std::size_t i = 0; i < textCount; i++) {
		capitalizeWords(texts[i]);
	}
}

int countOccurences(char* text, char* word) {
	int counter{ 0 };

	char* whatsLeftOfText{ nullptr };
	char* token{ strtok_s(text, " ", &whatsLeftOfText)};
	
	while (token) {
		if (std::strstr(token, word)) {
			counter++;
		}

		token = strtok_s(nullptr, word, &whatsLeftOfText);
	}
		
	return counter;
}

std::size_t getAllTextsSizes(const char* const* const texts, const std::size_t textCount) {
	std::size_t combinedTextsSize{ 0 };

	for (std::size_t i = 0; i < textCount; i++) {
		combinedTextsSize += std::strlen(texts[i]);
	}

	return combinedTextsSize;
}

char* combineAllTextsIntoOne(const char* const* const texts, const std::size_t textCount) {
	const std::size_t combinedTextsSize{ getAllTextsSizes(texts, textCount) + textCount };
	// Adding textCount like above ensures we have an extra space behind every sentence for the ; and last character to be \0
	// IE if we have 3 sentences "foo" "bar" "foobar", for them this function would return 12 and for the extra characters we add 3 more which totals up to 15
	// so when we add them we get foo;bar;foobar\0 which counts up to 15

	char* combinedTexts{ new char[combinedTextsSize] {} };
	// This should be a std::unique_ptr but due to not knowing would the prof allow it we'll keep it raw for now

	for (std::size_t i = 0; i < textCount; i++) {
		strcat_s(combinedTexts, combinedTextsSize, texts[i]);
		if (i + 1 != textCount) {
			strcat_s(combinedTexts, combinedTextsSize, ";");
		}
	}

	return combinedTexts;
}