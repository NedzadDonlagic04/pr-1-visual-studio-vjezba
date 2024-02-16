#include<iostream>
#include<iomanip>
#include<limits>
#include<span>
#include<string>

constexpr std::size_t BUFFER_SIZE{ 256 };

void clearBuffer();
void printErrorMsg(const char* const);

void enterText(char*, const std::size_t, const char* const);
void enterTexts(char**, const std::size_t, const std::size_t, const char* const);
void printTexts(const char* const * const, const std::size_t, const char* const);

void enterSize(std::size_t &, const char* const);

void allocateTexts(char**&, const std::size_t, const std::size_t);
void deallocateTexts(char**&, const std::size_t);

[[nodiscard]] std::size_t countWordsInText(const char* const);

struct WordCounter {
	const char* word{};
	std::size_t counter{};

	WordCounter();
	~WordCounter();

	void setWord(const char*, const std::size_t);
	void incrementCounter(const std::size_t = 1);
	
	void printCounterInfo(const std::size_t) const;
	
	[[nodiscard]] const char* getWord() const;
	[[nodiscard]] std::size_t getCounter() const;
};

[[nodiscard]] std::size_t getLongestWord(const char* const);
void splitWordsIntoWordCounter(char *, WordCounter * const, const std::size_t);
void countWordsInText(char*, WordCounter* const, const std::size_t);
void countWordsInTexts(char* const* const, const std::size_t, WordCounter* const, const std::size_t);
void printWordCounterValues(const std::span<const WordCounter>, const std::size_t);

int main() {
	std::size_t textsCount{};

	enterSize(textsCount, "Unesi broj tekstova: ");
	
	char** texts{};
	auto textToCountWords{ std::make_unique<char[]>(BUFFER_SIZE) };

	allocateTexts(texts, textsCount, BUFFER_SIZE);

	enterTexts(texts, textsCount, BUFFER_SIZE, "\nUnesi tekstove\n");
	printTexts(texts, textsCount, "\nIspis tekstova\n");

	enterText(textToCountWords.get(), BUFFER_SIZE, "Unesi recenicu cije ce se rijeci brojati u drugim recenicama: ");

	const std::size_t wordCountersSize{ countWordsInText(textToCountWords.get()) };
	const auto wordCounters{ std::make_unique<WordCounter[]>(wordCountersSize) };

	const std::size_t longestWordSize{ getLongestWord(textToCountWords.get()) };

	splitWordsIntoWordCounter(textToCountWords.get(), wordCounters.get(), wordCountersSize);
	countWordsInTexts(texts, textsCount, wordCounters.get(), wordCountersSize);

	std::cout << std::endl;
	printWordCounterValues({ wordCounters.get(), wordCountersSize }, longestWordSize);

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

std::size_t countWordsInText(const char* const text) {
	std::size_t wordCount{ 0 };

	if (!std::isspace(text[0])) {
		wordCount++;
	}

	for (std::size_t i = 1; i < std::strlen(text); i++) {
		if (std::isspace(text[i - 1]) && !std::isspace(text[i])) {
			wordCount++;
		}
	}

	return wordCount;
}

WordCounter::WordCounter() : word(nullptr), counter(0) {}

WordCounter::~WordCounter() {
	delete[] this->word;
}

void WordCounter::setWord(const char* word, const std::size_t wordLength) {
	if (this->word) {
		delete[] this->word;
	}

	char* temp{ new char[wordLength] {} };
	strcpy_s(temp, wordLength, word);
	this->word = temp;

	// When a word is set the counter is reset to 0
	this->counter = 0;
}

void WordCounter::incrementCounter(const std::size_t incrementVal) {
	this->counter += incrementVal;
}

void WordCounter::printCounterInfo(const std::size_t widthBetweenWordAndEquals) const {
	std::cout << std::quoted(this->word) <<std::string(widthBetweenWordAndEquals, ' ') << " = " << this->counter << std::endl;
}

const char* WordCounter::getWord() const {
	return this->word;
}

std::size_t WordCounter::getCounter() const {
	return this->counter;
}

std::size_t getLongestWord(const char* const text) {
	const std::size_t textCopySize{ std::strlen(text) + 1 };
	char* const textCopy{ new char[textCopySize] };
	strcpy_s(textCopy, textCopySize, text);

	char* context{ nullptr };
	char* tok = strtok_s(textCopy, " ", &context);
	std::size_t longestWordSize{ 0 };

	while (tok) {
		longestWordSize = std::max(longestWordSize, std::strlen(tok));
		tok = strtok_s(nullptr, " ", &context);
	}

	delete[] textCopy;

	return longestWordSize;
}

void splitWordsIntoWordCounter(char* text, WordCounter* const wordCounters, const std::size_t wordCountersSize) {
	std::size_t wordCounterIt{ 0 };

	char* context{ nullptr };
	char* tok = strtok_s(text, " ", &context);
	
	while (tok) {
		wordCounters[wordCounterIt].setWord(tok, std::strlen(tok) + 1);
		wordCounterIt++;
		tok = strtok_s(nullptr, " ", &context);
	}
}

void countWordsInText(char* text, WordCounter* const wordCounters, const std::size_t wordCountersSize) {
	char* context{ nullptr };
	char* tok = strtok_s(text, " ", &context);

	while (tok) {
		for (std::size_t i = 0; i < wordCountersSize; i++) {
			if (std::strcmp(tok, wordCounters[i].getWord()) == 0) {
				wordCounters[i].incrementCounter();
				break;
			}
		}
		tok = strtok_s(nullptr, " ", &context);
	}
}

void countWordsInTexts(char* const* const texts, const std::size_t textsCount, WordCounter* const wordCounters, const std::size_t wordCountersSize) {
	for (std::size_t i = 0; i < textsCount; i++) {
		countWordsInText(texts[i], wordCounters, wordCountersSize);
	}
}

void printWordCounterValues(const std::span<const WordCounter> wordCounters, const std::size_t longestWordSize) {
	std::cout << "Ispis broja rijeci u drugim recenicama (case sensitive):\n";
	for (const auto& wordCounter : wordCounters) {
		wordCounter.printCounterInfo(longestWordSize - std::strlen(wordCounter.getWord()));
	}
}