#include<iostream>
#include<limits>
#include<optional>
#include<cstring>
#include<random>
#include<iomanip>
#include<cmath>

constexpr std::size_t BUFFER_SIZE{ 4096 };
constexpr std::size_t COUNTRY_NAME_SIZE{ 40 };

struct Fudbaler {
	char* ime{};
	char* prezime{};
	char drzava[COUNTRY_NAME_SIZE]{};
	const char* fudbalskiKlub{};
	int brojUtakmica{};
	int brojKorisnika{};
	int* golova{};
	int** ocjene{};

	Fudbaler();
	~Fudbaler();

	void unos();
	void generateTestData();
	// added [[nodiscard]] because if we allow someone to discard this value
	// then the dynamic memory would allow a memory leak
	[[nodiscard]] float* getAveragesByGame();
	void ispis();

	void alokacija();
	void dealokacija();
};

void clearBuffer();
void printErrorMsg(const char* const);
void printBreakLine();

void enterNum(int &, const char * const, const std::optional<int> = std::nullopt, const std::optional<int> = std::nullopt);
void enterText(char* const, const std::size_t, const char* const);

[[nodiscard]] float roundTo2DecimalPlaces(const float);
[[nodiscard]] std::size_t getIndexOfBiggestNum(const float* const, const std::size_t);

int main() {
	Fudbaler fudbaler{};

	fudbaler.generateTestData();
	fudbaler.ispis();

	const float* const averagesByGames{ fudbaler.getAveragesByGame() };

	printBreakLine();
	std::cout << "Prosjeci po utakmici: ";
	for (std::size_t i = 0; i < fudbaler.brojUtakmica; i++) {
		std::cout << *(averagesByGames + i);

		if (i + 1 != fudbaler.brojUtakmica) {
			std::cout << ", ";
		}
	}
	std::cout << std::endl;
	printBreakLine();

	printBreakLine();
	const std::size_t bestGameIndex{ getIndexOfBiggestNum(averagesByGames, fudbaler.brojUtakmica) };
	// Debug
	// std::cout << "Best index: " << bestGameIndex << std::endl;
	std::cout << "Broj golova na utakmici sa najvecom ocjenom je " << *(fudbaler.golova + bestGameIndex) << std::endl;
	printBreakLine();

	delete[] averagesByGames;
	
	return 0;
}

Fudbaler::Fudbaler() {
	this->alokacija();
}

Fudbaler::~Fudbaler() {
	this->dealokacija();
}

void Fudbaler::unos() {
	enterText(this->ime, BUFFER_SIZE, "Unesi ime fudbalera: ");
	enterText(this->prezime, BUFFER_SIZE, "Unesi prezime fudbalera: ");
	enterText(this->drzava, COUNTRY_NAME_SIZE, "Unesi drzavu fudbalera: ");
	enterText(const_cast<char*>(this->fudbalskiKlub), BUFFER_SIZE, "Unesi klub fudbalera: ");

	if (this->golova) {
		delete[] this->golova;

		for (std::size_t i = 0; i < this->brojUtakmica; i++) {
			delete[] * (this->ocjene + i);
		}
		delete[] this->ocjene;
	}

	std::random_device dev{};
	std::mt19937 rng{ dev() };
	std::uniform_int_distribution<std::mt19937::result_type> goalDist{ 0, 3 };
	std::uniform_int_distribution<std::mt19937::result_type> ocjeneDist{ 1, 10 };

	enterNum(this->brojUtakmica, "Unesi broj utakmica fudbalera: ", 0);
	enterNum(this->brojKorisnika, "Unesi broj korisnika fudbalera: ", 0);

	this->golova = new int[this->brojUtakmica] {};
	this->ocjene = new int* [this->brojUtakmica] {};
	for (std::size_t i = 0; i < this->brojUtakmica; i++) {
		*(this->ocjene + i) = new int[this->brojKorisnika] {};
	}

	for (std::size_t i = 0; i < this->brojUtakmica; i++) {
		*(this->golova + i) = goalDist(rng);
	}

	for (std::size_t i = 0; i < this->brojUtakmica; i++) {
		for (std::size_t ii = 0; ii < this->brojKorisnika; ii++) {
			*(*(this->ocjene + i) + ii) = ocjeneDist(rng);
		}
	}
}

void Fudbaler::generateTestData() {
	strcpy_s(this->ime, BUFFER_SIZE, "Ime");
	strcpy_s(this->prezime, BUFFER_SIZE, "Prezime");
	strcpy_s(this->drzava, COUNTRY_NAME_SIZE, "Drzava");
	strcpy_s(const_cast<char*>(this->fudbalskiKlub), BUFFER_SIZE, "Klub");

	if (this->golova) {
		delete[] this->golova;

		for (std::size_t i = 0; i < this->brojUtakmica; i++) {
			delete[] * (this->ocjene + i);
		}
		delete[] this->ocjene;
	}

	std::random_device dev{};
	std::mt19937 rng{ dev() };
	std::uniform_int_distribution<std::mt19937::result_type> goalDist{ 0, 3 };
	std::uniform_int_distribution<std::mt19937::result_type> ocjeneDist{ 1, 10 };
	std::uniform_int_distribution<std::mt19937::result_type> brojUtakmicaKorisnika{ 1, 20 };
	
	this->brojUtakmica = brojUtakmicaKorisnika(rng);
	this->brojKorisnika = brojUtakmicaKorisnika(rng);

	this->golova = new int[this->brojUtakmica] {};
	this->ocjene = new int* [this->brojUtakmica] {};
	for (std::size_t i = 0; i < this->brojUtakmica; i++) {
		*(this->ocjene + i) = new int[this->brojKorisnika] {};
	}

	for (std::size_t i = 0; i < this->brojUtakmica; i++) {
		*(this->golova + i) = goalDist(rng);
	}

	for (std::size_t i = 0; i < this->brojUtakmica; i++) {
		for (std::size_t ii = 0; ii < this->brojKorisnika; ii++) {
			*(*(this->ocjene + i) + ii) = ocjeneDist(rng);
		}
	}
}

float* Fudbaler::getAveragesByGame() {
	float* const averagesByGames{ new float[this->brojUtakmica] {0.0f} };

	for (std::size_t i = 0; i < this->brojUtakmica; i++) {
		for (std::size_t ii = 0; ii < this->brojKorisnika; ii++) {
			*(averagesByGames + i) += *(*(this->ocjene + i) + ii);
		}
		*(averagesByGames + i) /= this->brojKorisnika;
		*(averagesByGames + i) = roundTo2DecimalPlaces(*(averagesByGames + i));
	}

	return averagesByGames;
}

void Fudbaler::ispis() {
	std::cout << "Ime fudbalera: " << std::quoted(this->ime) << std::endl;
	std::cout << "Prezime fudbalera: " << std::quoted(this->prezime) << std::endl;
	std::cout << "Drzava fudbalera: " << std::quoted(this->drzava) << std::endl;
	std::cout << "Klub fudbalera: " << std::quoted(this->fudbalskiKlub) << std::endl;

	printBreakLine();
	std::cout << "Golovi po utakmici: ";
	for (std::size_t i = 0; i < this->brojUtakmica; i++) {
		std::cout<<*(this->golova + i);

		if (i + 1 != this->brojUtakmica) {
			std::cout << ", ";
		}
	}
	std::cout << std::endl;
	printBreakLine();

	printBreakLine();
	std::cout << "Rezultati igraca na Goal.com\n";
	for (std::size_t i = 0; i < this->brojUtakmica; i++) {
		for (std::size_t ii = 0; ii < this->brojKorisnika; ii++) {
			std::cout<<std::setw(3)<<*(*(this->ocjene + i) + ii)<<' ';
		}
		std::cout << std::endl;
	}
	printBreakLine();
}

void Fudbaler::alokacija() {
	this->ime = new char[BUFFER_SIZE] {};
	this->prezime = new char[BUFFER_SIZE] {};
	this->fudbalskiKlub = new char[BUFFER_SIZE] {};
	this->golova = nullptr;
	this->ocjene = nullptr;
}

void Fudbaler::dealokacija() {
	delete[] this->ime;
	this->ime = nullptr;

	delete[] this->prezime;
	this->prezime = nullptr;

	delete[] this->fudbalskiKlub;
	this->fudbalskiKlub = nullptr;

	delete[] this->golova;
	this->golova = nullptr;

	for (std::size_t i = 0; i < this->brojUtakmica; i++) {
		delete[] * (this->ocjene + i);
	}
	delete[] this->ocjene;
	this->ocjene = nullptr;
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

void printBreakLine() {
	std::cout << "----------------------------------\n";
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
		}
		else if (min && num < min.value()) {
			sprintf_s(errorMsg, BUFFER_SIZE, "Unos ne moze biti manji od %d\n", min.value());
			printErrorMsg(errorMsg);
			clearBuffer();
			repeatLoop = true;
		}
		else if (max && num > max.value()) {
			sprintf_s(errorMsg, BUFFER_SIZE, "Unos ne moze biti veci od %d\n", max.value());
			printErrorMsg(errorMsg);
			clearBuffer();
			repeatLoop = true;
		}
	} while (repeatLoop);

	// To eat the '\n'
	clearBuffer();
}

void enterText(char* const text, const std::size_t bufferSize, const char* const outputMsg) {
	do {
		if (std::cin.fail()) {
			printErrorMsg("Nevalidan unos\n");
			clearBuffer();
		}

		std::cout << outputMsg;
		std::cin.getline(text, bufferSize);
	} while (std::cin.fail());
}

float roundTo2DecimalPlaces(const float num) {
	return std::round(num * 100) / 100;
}

std::size_t getIndexOfBiggestNum(const float* const arr, const std::size_t size) {
	std::size_t biggestNumIndex{ 0 };

	for (std::size_t i = 1; i < size; i++) {
		if (*(arr + biggestNumIndex) < *(arr + i)) {
			biggestNumIndex = i;
		}
	}

	return biggestNumIndex;
}