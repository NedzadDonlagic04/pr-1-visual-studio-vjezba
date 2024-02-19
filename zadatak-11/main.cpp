#include<iostream>
#include<limits>
#include<memory>
#include<cmath>

constexpr std::size_t BUFFER_SIZE{ 256 };

void clearBuffer();
void printErrorMsg(const char* const);

void enterArraySize(std::size_t&, const char* const);
void enterNum(int&, const char* const);

void enterArray(int* const, const std::size_t, const std::size_t = 0);
void printArray(const int* const, const std::size_t, const std::size_t = 0);

[[nodiscard]] double aritmeticAverageOfArr(const int* const, const std::size_t, const std::size_t = 0);
[[nodiscard]] double geometricAverageOfArr(const int* const, const std::size_t, const std::size_t = 0);

int main() {
	std::size_t arraySize{};

	enterArraySize(arraySize, "Unesite velicinu niza: ");

	const auto arr{ std::make_unique<int[]>(arraySize) };

	enterArray(arr.get(), arraySize);
	printArray(arr.get(), arraySize);

	std::cout << "Aritmeticka sredina niza je " << aritmeticAverageOfArr(arr.get(), arraySize) << '\n';
	std::cout << "Geometrijska sredina niza je " << geometricAverageOfArr(arr.get(), arraySize) << '\n';

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

void enterArraySize(std::size_t& size, const char* const outputMsg) {
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

void enterNum(int& num, const char* const outputMsg) {
	do {
		if (std::cin.fail()) {
			printErrorMsg("Nevalidan unos\n");
			clearBuffer();
		}

		std::cout << outputMsg;
		std::cin >> num;
	} while (std::cin.fail());

	// To eat the '\n'
	clearBuffer();
}

void enterArray(int* const arr, const std::size_t size, const std::size_t i) {
	if (i == size) {
		return;
	}

	char outputMsg[BUFFER_SIZE]{};
	sprintf_s(outputMsg, BUFFER_SIZE, "Unesite broj na index %llu: ", i);
	enterNum(arr[i], outputMsg);

	enterArray(arr, size, i + 1);
}

void printArray(const int* const arr, const std::size_t size, const std::size_t i) {
	if (i == 0) {
		std::cout << "Niz: ";
	}
	else if (i == size) {
		std::cout << std::endl;
		return;
	}
	else {
		std::cout << ", ";
	}

	std::cout << arr[i];

	printArray(arr, size, i + 1);
}

double aritmeticAverageOfArr(const int* const arr, const std::size_t size, const std::size_t i) {
	if (i == size) {
		return 0.0;
	}

	return static_cast<double>(arr[i]) / size + aritmeticAverageOfArr(arr, size, i + 1);
}

double geometricAverageOfArr(const int* const arr, const std::size_t size, const std::size_t i) {
	if (i == size) {
		return 1.0f;
	}

	return std::pow(arr[i], 1.0 / size) * geometricAverageOfArr(arr, size, i + 1);
}