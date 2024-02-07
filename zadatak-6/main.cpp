#include<iostream>
#include<random>
#include<iomanip>
#include<optional>
#include<cstring>
#include<limits>
#include<memory>

constexpr std::size_t BUFFER_SIZE{ 256 };
constexpr char MANGA_NAMES[][BUFFER_SIZE]{
	"One Piece",
	"Bleach",
	"Jujutsu Kaisen",
	"Naruto",
	"Black Clover",
	"Fire Force",
	"Mashle",
	"One Punch Man",
	"Attack on Titan",
	"Vinland Saga",
	"Demon Slayer",
	"HxH",
	"SpyXFamily",
};
constexpr char MANGA_AUTHOR_NAMES[][BUFFER_SIZE]{
	"Eiichiro Oda",
	"Tite Kubo",
	"Gege Akutami",
	"Masashi Kishimoto",
	"Yuki Tabata",
	"Atsushi Ohkubo",
	"Hajime Komoto",
	"ONE",
	"Hajime Isayama",
	"Makoto Yukimura",
	"Koyoharu Gotouge",
	"Yoshihiro Togashi",
	"Tatsuya Endo",
};


struct Book {
	char* imeKnjige{};
	char* imeAutora{};
	int brojOcjena{};
	int* ocjena{};

	Book();
	~Book();

	void alokacija();
	void dealokacija();

	void unos();
	void generateBookData();
	
	void ispis() const;
};

void clearBuffer();
void printErrorMsg(const char * const);
void printBreakLine();

void enterSize(std::size_t&, const char* const);
void enterText(char*, const std::size_t, const char* const);
void enterNum(int&, const char* const, const std::optional<int> = std::nullopt, const std::optional<int> = std::nullopt);

[[nodiscard]] unsigned int countWordsInText(const char* const);
[[nodiscard]] std::size_t getIndexForBiggestNum(const int* const, const std::size_t);

int main() {
	std::size_t bookSize{};

	enterSize(bookSize, "Unesi broj knjiga: ");

	const std::unique_ptr<Book[]> books{new Book[bookSize]};

	// Remove comments from code below for manual input
	/*
	for (std::size_t i = 0; i < bookSize; i++) {
		printBreakLine();
		std::cout << "Unos podataka za knjigu " << i + 1 << '\n';
		(books.get() + i)->unos();
		printBreakLine();
	}
	*/

	// For easier testing data is auto generated
	for (std::size_t i = 0; i < bookSize; i++) {
		(books.get() + i)->generateBookData();
	}

	for (std::size_t i = 0; i < bookSize; i++) {
		printBreakLine();
		std::cout << "Ispis podataka za knjigu " << i + 1 << '\n';
		(books.get() + i)->ispis();
		printBreakLine();
	}

	return 0;
}

Book::Book() {
	this->alokacija();
}

Book::~Book() {
	this->dealokacija();
}

void Book::alokacija() {
	this->imeKnjige = new char[BUFFER_SIZE] {};
	this->imeAutora = new char[BUFFER_SIZE] {};
	this->ocjena = nullptr;
}

void Book::dealokacija() {
	delete[] this->imeKnjige;
	this->imeKnjige = nullptr;

	delete[] this->imeAutora;
	this->imeAutora = nullptr;

	delete[] this->ocjena;
	this->ocjena = nullptr;
}

void Book::unos() {
	enterText(this->imeKnjige, BUFFER_SIZE, "Unesite ime knjige: ");
	enterText(this->imeAutora, BUFFER_SIZE, "Unesite ime autora: ");
	enterNum(this->brojOcjena, "Unesite broj ocjena knjige: ", 2, 1000);

	if (this->ocjena) {
		delete[] ocjena;
	}

	this->ocjena = new int[this->brojOcjena] {};

	char outputMsg[BUFFER_SIZE]{};
	for (std::size_t i = 0; i < this->brojOcjena; i++) {
		sprintf_s(outputMsg, BUFFER_SIZE, "Unesi %llu. ocjenu za knjigu: ", i + 1);
		enterNum(*(this->ocjena + i), outputMsg, 1, 10);
	}
}

void Book::generateBookData() {
	std::random_device dev{};
	std::mt19937 rng{ dev() };
	std::uniform_int_distribution<std::mt19937::result_type> ocjeneDist{ 1, 10 };
	std::uniform_int_distribution<std::size_t> bookAuthorAndNameDist(0, std::size(MANGA_NAMES) - 1);

	const std::size_t bookAuthorAndNameIndex{ bookAuthorAndNameDist(rng) };

	strcpy_s(this->imeKnjige, BUFFER_SIZE, MANGA_NAMES[bookAuthorAndNameIndex]);
	strcpy_s(this->imeAutora, BUFFER_SIZE, MANGA_AUTHOR_NAMES[bookAuthorAndNameIndex]);

	this->brojOcjena = ocjeneDist(rng);

	if (this->ocjena) {
		delete[] ocjena;
	}

	this->ocjena = new int[this->brojOcjena] {};

	char outputMsg[BUFFER_SIZE]{};
	for (std::size_t i = 0; i < this->brojOcjena; i++) {
		*(this->ocjena + i) = ocjeneDist(rng);
	}
}

void Book::ispis() const {
	std::cout << "Ime knjige: " << std::quoted(this->imeKnjige) << std::endl;
	std::cout << "Ime autora: " << std::quoted(this->imeAutora) << std::endl;
	std::cout << "Ocjene za knjigu su: ";
	for (std::size_t i = 0; i < this->brojOcjena; i++) {
		std::cout << *(this->ocjena + i);

		if (i + 1 != this->brojOcjena) {
			std::cout << ", ";
		}
	}
	std::cout << std::endl;

	const unsigned int bookNameWordCount{ countWordsInText(this->imeKnjige) };

	std::cout << "Ime knjige ima " << bookNameWordCount << " rijec";
	if (bookNameWordCount != 1) {
		std::cout << 'i';
	}
	std::cout << std::endl;

	std::cout << "Index najbolje ocjene za knjigu je index " << getIndexForBiggestNum(this->ocjena, this->brojOcjena) << std::endl;
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

void printBreakLine() {
	std::cout << "-------------------------------------------\n";
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
			printErrorMsg("Duzina treba biti u intervalu [2, 1000]\n");
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

void enterNum(int& num, const char* const outputMsg, const std::optional<int> min, const std::optional<int> max) {
	bool repeatLoop{};
	char errorMsg[BUFFER_SIZE]{};

	do {
		repeatLoop = false;

		std::cout << outputMsg;
		std::cin >> num;

		if (std::cin.fail()) {
			printErrorMsg("Nevalidan unos\n");
			clearBuffer();
			repeatLoop = true;
		} else if (min && num < min.value()) {
			sprintf_s(errorMsg, BUFFER_SIZE, "Unos ne moze biti manji od %d\n", min.value());
			printErrorMsg(errorMsg);
			clearBuffer();
			repeatLoop = true;
		} else if (max && num > max.value()) {
			sprintf_s(errorMsg, BUFFER_SIZE, "Unos ne moze biti veci od %d\n", max.value());
			printErrorMsg(errorMsg);
			clearBuffer();
			repeatLoop = true;
		}
	} while (repeatLoop);

	clearBuffer();
}

unsigned int countWordsInText(const char* const text) {
	unsigned int counter{ 0u };

	if (!std::isspace(*text)) {
		counter++;
	}

	for (std::size_t i = 1; i < std::strlen(text); i++) {
		if (std::isspace(*(text + i - 1)) && std::isalpha(*(text + i))) {
			counter++;
		}
	}

	return counter;
}

std::size_t getIndexForBiggestNum(const int * const arr, const std::size_t size) {
	std::size_t biggestNumIndex{ 0 };

	for (std::size_t i = 1; i < size; i++) {
		if (*(arr + i) > *(arr + biggestNumIndex)) {
			biggestNumIndex = i;
		}
	}

	return biggestNumIndex;
}