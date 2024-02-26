#include<iostream>
#include<iomanip>
#include<limits>
#include<algorithm>
#include<span>
#include<random>
#include<memory>
#include<numeric>

constexpr std::size_t BUFFER_SIZE{ 256 };
constexpr char EXAMPLE_PHONE_MODELS[][BUFFER_SIZE]{
	"Samsung Galaxy S21 Ultra",
	"iPhone 13 Pro Max",
	"Samsung Galaxy A52",
	"Google Pixel 5a",
	"Samsung Galaxy A12",
	"Xiaomi Redmi 9A",
};

struct Smartphone {
	static constexpr std::size_t MODEL_SIZE{ 256 };
	const char* model{};

	static constexpr std::size_t TYPE_SIZE{ 64 };
	static constexpr const char* const ALLOWED_TYPES[]{
		"flagship",
		"mid-range",
		"budget",
		"foldable",
	};
	const char* type{};
	
	float price{};

	std::size_t reviewsSize{};
	int* reviews{};
	float averageReview{};

	Smartphone();
	Smartphone(Smartphone&&) noexcept;
	~Smartphone();

	Smartphone& operator=(Smartphone&&) noexcept;

	void enterReviews();

	void enterInfo();
	void generateInfo();
	void printInfo() const;

	[[nodiscard]] static void printValidTypes();
	[[nodiscard]] static bool isValidType(const char* const);
};

void printBreakLine();
void printErrorMsg(const char* const);
void clearBuffer();

template<typename NumberType, typename Callback>
void enterNum(NumberType&, const char* const, Callback);
void enterText(char*, const std::size_t, const char* const);

void generatePhonesInfo(std::span<Smartphone> phones);
void enterPhonesInfo(std::span<Smartphone> phones);
void printPhonesInfo(const std::span<const Smartphone> phones);

[[nodiscard]] float twoDecimalRound(const float);

int main() {
	std::size_t phoneCount{};

	enterNum(phoneCount, "Enter number of phones: ",
		[](const std::size_t num) {
			if (num < 2 || num > 1000) {
				printErrorMsg("Entered number has to be in the [2, 1000] interval\n");
				return false;
			}
			return true;
		}
	);

	const auto phones{ std::make_unique<Smartphone[]>(phoneCount) };

	// Enable line below for auto generation
	// enterPhonesInfo({ phones.get(), phoneCount });

	// Enable line for auto generation of phone data
	generatePhonesInfo({ phones.get(), phoneCount });
	printPhonesInfo({ phones.get(), phoneCount });
	
	
	std::sort(phones.get(), phones.get() + phoneCount,
		[](const Smartphone& a, const Smartphone& b) {
			return a.averageReview > b.averageReview;
		}
	);

	std::cout << "\n\nPhone info after sorting phones by average review (DESC)\n";
	printPhonesInfo({ phones.get(), phoneCount });
	

	return 0;
}


// -------------------------------
// Smartphone class members definition starts here
// -------------------------------

Smartphone::Smartphone()
	: model{ new char[this->MODEL_SIZE] {} }
	, type{ new char[this->TYPE_SIZE] {} }
	, reviews{ nullptr }
{}

Smartphone::Smartphone(Smartphone&& rhs) noexcept {
	this->model = std::exchange(rhs.model, nullptr);
	this->type = std::exchange(rhs.type, nullptr);

	this->price = rhs.price;

	this->reviewsSize = rhs.reviewsSize;
	this->reviews = std::exchange(rhs.reviews, nullptr);

	this->averageReview = rhs.averageReview;
}

Smartphone::~Smartphone() {
	// Was used for debugging
	// std::cout << "Delete\n";
	delete[] model;
	this->model = nullptr;

	delete[] type;
	this->type = nullptr;

	delete[] reviews;
	this->reviews = nullptr;
}

Smartphone& Smartphone::operator=(Smartphone&& rhs) noexcept {
	this->model = std::exchange(rhs.model, nullptr);
	this->type = std::exchange(rhs.type, nullptr);

	this->price = rhs.price;

	this->reviewsSize = rhs.reviewsSize;
	this->reviews = std::exchange(rhs.reviews, nullptr);

	this->averageReview = rhs.averageReview;

	return *this;
}

void Smartphone::enterReviews() {
	enterNum(this->reviewsSize, "Enter number of reviews: ",
		[](const std::size_t num) {
			if (num < 2 || num > 1000) {
				printErrorMsg("Entered number has to be in the [2, 1000] interval\n");
				return false;
			}
			return true;
		}
	);

	if (this->reviews) {
		delete[] this->reviews;
	}
	this->reviews = new int[this->reviewsSize] {};

	char forLoopOutputMsg[BUFFER_SIZE]{};
	this->averageReview = 0.0f;

	for (std::size_t i = 0; i < this->reviewsSize; i++) {
		sprintf_s(forLoopOutputMsg, "Enter review number %llu: ", i + 1);
		enterNum(this->reviews[i], forLoopOutputMsg,
			[](const std::size_t num) {
				if (num < 1 || num > 5) {
					printErrorMsg("A review has to be in range [1, 5]\n");
					return false;
				}
				return true;
			}
		);
		this->averageReview += this->reviews[i];
	}

	this->averageReview /= this->reviewsSize;
	this->averageReview = twoDecimalRound(this->averageReview);
}

void Smartphone::generateInfo() {
	std::random_device dev{};
	std::mt19937 rng{ dev() };

	std::uniform_int_distribution<std::size_t> phoneModelDist(0, std::size(EXAMPLE_PHONE_MODELS) - 1);
	std::uniform_int_distribution<std::size_t> phoneTypeDist(0, std::size(Smartphone::ALLOWED_TYPES) - 1);
	std::uniform_real_distribution<float> phonePriceDist(0, 10000);
	std::uniform_int_distribution<std::size_t> reviewsSizeDist(2, 10);
	std::uniform_int_distribution<> reviewDist(1, 5);

	strcpy_s(const_cast<char*>(this->model), Smartphone::MODEL_SIZE, EXAMPLE_PHONE_MODELS[phoneModelDist(rng)]);
	strcpy_s(const_cast<char*>(this->type), Smartphone::TYPE_SIZE, Smartphone::ALLOWED_TYPES[phoneTypeDist(rng)]);
	
	this->price = twoDecimalRound(phonePriceDist(rng));

	this->reviewsSize = reviewsSizeDist(rng);
	this->reviews = new int[this->reviewsSize] {};
	std::generate(this->reviews, this->reviews + this->reviewsSize, [&reviewDist, &rng]() { return reviewDist(rng); });

	this->averageReview = std::accumulate(this->reviews, this->reviews + this->reviewsSize, 0.0f) / this->reviewsSize;
	this->averageReview = twoDecimalRound(this->averageReview);
}

void Smartphone::enterInfo() {
	enterText(const_cast<char*>(this->model), Smartphone::MODEL_SIZE, "Enter phone model: ");
	
	bool repeatLoop{};
	do {
		repeatLoop = false;
		Smartphone::printValidTypes();
		enterText(const_cast<char*>(this->type), Smartphone::TYPE_SIZE, "Enter phone type: ");

		if (!Smartphone::isValidType(this->type)) {
			printErrorMsg("Entered type isn't valid\n");
			repeatLoop = true;
		}
	} while (repeatLoop);

	enterNum(this->price, "Enter phone price: ",
		[](const float price) {
			if (price < 0.0f) {
				printErrorMsg("Phone price can't be a negative number\n");
				return false;
			}
			return true;
		}
	);

	this->price = twoDecimalRound(this->price);

	this->enterReviews();
}

void Smartphone::printInfo() const {
	std::cout << "Phone model: " << std::quoted(this->model) << '\n';
	std::cout << "Phone type: " << std::quoted(this->type) << '\n';
	std::cout << "Phone price: " << price << '\n';
	std::cout << "Phone reviews:\n";
	std::for_each(this->reviews, this->reviews + this->reviewsSize, [](const auto el) { std::cout << '\t' << el << " stars\n"; });
	std::cout << "The average review for the phone is " << this->averageReview << " stars\n";
}

void Smartphone::printValidTypes() {
	std::cout << "Valid types: ";
	for (std::size_t i = 0; i < std::size(Smartphone::ALLOWED_TYPES); i++) {
		std::cout << std::quoted(Smartphone::ALLOWED_TYPES[i]);

		if (i + 1 != std::size(Smartphone::ALLOWED_TYPES)) {
			std::cout << ", ";
		}
	}
	std::cout << std::endl;
}

bool Smartphone::isValidType(const char* const type) {
	for (const auto& ALLOWED_TYPE : Smartphone::ALLOWED_TYPES) {
		if (std::strcmp(ALLOWED_TYPE, type) == 0) {
			return true;
		}
	}

	return false;
}


// -------------------------------
// Global function definitions starts here
// -------------------------------

void printBreakLine() {
	std::cout << "--------------------------------------\n";
}

void printErrorMsg(const char* const errorMsg) {
	std::cout << "\033[31m";
	std::cout << errorMsg;
	std::cout << "\033[37m";
}

void clearBuffer() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

template<typename NumberType, typename Callback>
void enterNum(NumberType& num, const char* const outputMsg, Callback checkIsNumInRange) {
	do {
		if (std::cin.fail()) {
			printErrorMsg("Invalid number\n");
			clearBuffer();
		}

		std::cout << outputMsg;
		std::cin >> num;
	} while (std::cin.fail() || !checkIsNumInRange(num));
	clearBuffer();
}

void enterText(char* text, const std::size_t bufferSize, const char* const outputMsg) {
	do {
		if (std::cin.fail()) {
			printErrorMsg("Entered text is too long\n");
			clearBuffer();
		}

		std::cout << outputMsg;
		std::cin.getline(text, bufferSize);
	} while (std::cin.fail());
}

void generatePhonesInfo(std::span<Smartphone> phones) {
	for (auto& phone : phones) {
		phone.generateInfo();
	}
}

void enterPhonesInfo(std::span<Smartphone> phones) {
	for (std::size_t i = 0; i < phones.size(); i++) {
		printBreakLine();
		std::cout << "Enter information for phone " << i + 1 << "\n";
		phones[i].enterInfo();
		printBreakLine();
	}
}

void printPhonesInfo(const std::span<const Smartphone> phones) {
	for (std::size_t i = 0; i < phones.size(); i++) {
		printBreakLine();
		std::cout << "Print information for phone " << i + 1 << "\n";
		phones[i].printInfo();
		printBreakLine();
	}
}

float twoDecimalRound(const float num) {
	return std::round(num * 100.0f) / 100.0f;
}