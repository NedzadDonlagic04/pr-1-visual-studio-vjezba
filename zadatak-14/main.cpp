#include<iostream>
#include<memory>
#include<iomanip>

constexpr std::size_t SENTENCE_SIZE{ 200 };
constexpr std::size_t WORD_SIZE{ 5 };

void printLineBreak();

namespace strUtils {
	void strToLower(char* const);
}

namespace caseSensitive {
	[[nodiscard]] std::size_t countWordInSentence(const char* const, const char* const);
}

namespace caseInsensitive {
	[[nodiscard]] std::size_t countWordInSentence(const char* const, const char* const);
}

int main() {
	char sentence[SENTENCE_SIZE]{ "Most u Mostaru je Stari most a pored njega je jos jedan most" };
	char word[WORD_SIZE]{ "most" };
	

	printLineBreak();
	const auto sensitiveWordCountInSentence{ caseSensitive::countWordInSentence(sentence, word) };
	std::cout << "Case sensitive search\n";
	std::cout << "U recenici " << std::quoted(sentence) << " se rijec " << std::quoted(word) << " ponavlja ";
	std::cout << sensitiveWordCountInSentence << " put" << (sensitiveWordCountInSentence == 1 ? ' ' : 'a') << std::endl;
	printLineBreak();
	std::cout << std::endl;
	printLineBreak();
	const auto insensitiveWordCountInSentence{ caseInsensitive::countWordInSentence(sentence, word) };
	std::cout << "Case insensitive search\n";
	std::cout << "U recenici " << std::quoted(sentence) << " se rijec " << std::quoted(word) << " ponavlja ";
	std::cout << insensitiveWordCountInSentence << " put" << (insensitiveWordCountInSentence == 1 ? ' ' : 'a') << std::endl;
	printLineBreak();

	return 0;
}

void printLineBreak() {
	std::cout << "------------------------------------------\n";
}

namespace strUtils {
	void strUtils::strToLower(char* const sentence) {
		for (std::size_t i = 0; i < std::strlen(sentence); i++) {
			sentence[i] = std::tolower(sentence[i]);
		}
	}
}

namespace caseSensitive {
	std::size_t countWordInSentence(const char* const sentence, const char* const word) {
		std::size_t counter{ 0 };
		const char* wordInstance{ std::strstr(sentence, word) };

		while (wordInstance) {
			wordInstance = std::strstr(wordInstance + 1, word);
			counter++;
		}

		return counter;
	}
}

namespace caseInsensitive {
	std::size_t countWordInSentence(const char* const sentence, const char* const word) {
		const std::size_t sentenceCopySize{ std::strlen(sentence) + 1 };
		auto sentenceCopy{ std::make_unique<char[]>(sentenceCopySize) };

		strcpy_s(sentenceCopy.get(), sentenceCopySize, sentence);
		strUtils::strToLower(sentenceCopy.get());

		std::size_t counter{ 0 };
		const char* wordInstance{ std::strstr(sentenceCopy.get(), word) };

		while (wordInstance) {
			wordInstance = std::strstr(wordInstance + 1, word);
			counter++;
		}

		return counter;
	}
}