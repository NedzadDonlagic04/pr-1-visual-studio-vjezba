#include<iostream>
#include<random>
#include<limits>	
#include<iomanip>
#include<cmath>

void clearBuffer();
void printErrorMsg(const char* const);

void enterSize(std::size_t&, const char* const);

void allocateMatrix(int **&, const std::size_t, const std::size_t);
void deallocateMatrix(int **&, const std::size_t);

void setRandomValues(int* const* const, const std::size_t, const std::size_t, const int, const int);
void printMatrix(const int* const* const, const std::size_t, const std::size_t, const char* const);
void sumMatrix(const int* const* const, const int* const* const, int**, const std::size_t, const std::size_t, const std::size_t, const std::size_t);
[[nodiscard]] int biggestSideDiagonal(const int* const* const, const int* const* const, const std::size_t, const std::size_t, const std::size_t, const std::size_t);

[[nodiscard]] unsigned int getLongestDigitInMatrix(const int* const* const, const std::size_t, const std::size_t);
[[nodiscard]] unsigned int getDigitWidth(const int);

int main() {
	std::size_t rows1{}, cols1{};
	std::size_t rows2{}, cols2{};
	std::size_t rows3{}, cols3{};

	int** matrix1{};
	int** matrix2{};
	int** matrix3{};

	enterSize(rows1, "Unesi duzinu redova matrice 1: ");
	enterSize(cols1, "Unesi duzinu kolona matrice 1: ");

	enterSize(rows2, "Unesi duzinu redova matrice 2: ");
	enterSize(cols2, "Unesi duzinu kolona matrice 2: ");

	rows3 = std::max(rows1, rows2);
	cols3 = std::max(cols1, cols2);

	allocateMatrix(matrix1, rows1, cols1);
	allocateMatrix(matrix2, rows2, cols2);
	allocateMatrix(matrix3, rows3, cols3);

	setRandomValues(matrix1, rows1, cols1, 1, 15);
	setRandomValues(matrix2, rows2, cols2, 1, 15);
	sumMatrix(matrix1, matrix2, matrix3, rows1, cols1, rows2, cols2);

	printMatrix(matrix1, rows1, cols1, "Ispis matrice 1:\n");
	printMatrix(matrix2, rows2, cols2, "Ispis matrice 2:\n");
	printMatrix(matrix3, rows3, cols3, "Ispis matrice 3 (koja je nastala sumom matrice 1 i 2):\n");

	const int matrixWithBiggerSideDiagonalSum{ biggestSideDiagonal(matrix1, matrix2, rows1, cols1, rows2, cols2) };

	if (matrixWithBiggerSideDiagonalSum == -1) {
		std::cout << "Zbog dimenzija matrica ne moze se reci koja ima vecu sumu sporedne dijagonale\n";
	} else {
		std::cout << "Matrica broj " << matrixWithBiggerSideDiagonalSum << " ima vecu sumu na sporednoj dijagonali\n";
	}

	deallocateMatrix(matrix1, rows1);
	deallocateMatrix(matrix2, rows2);
	deallocateMatrix(matrix3, rows3);

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
		} else if (size < 2 || size > 1000) {
			printErrorMsg("Duzina mora biti u intervalu [2, 1000]\n");
			clearBuffer();
			repeatLoop = true;
		}
	} while (repeatLoop);

	// To eat the '\n'
	clearBuffer();
}

void allocateMatrix(int**& matrix, const std::size_t rows, const std::size_t cols) {
	matrix = new int* [rows] {};
	for (std::size_t i = 0; i < rows; i++) {
		matrix[i] = new int[cols] {};
	}
}

void deallocateMatrix(int**& matrix, const std::size_t rows) {
	for (std::size_t i = 0; i < rows; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
	matrix = nullptr;
}

void setRandomValues(int* const* const matrix, const std::size_t rows, const std::size_t cols, const int from, const int to) {
	std::random_device dev{};
	std::mt19937 rng{ dev() };
	std::uniform_int_distribution<std::mt19937::result_type> dist(from, to);

	for (std::size_t i = 0; i < rows; i++) {
		for (std::size_t ii = 0; ii < cols; ii++) {
			matrix[i][ii] = dist(rng);
		}
	}
}

void printMatrix(const int* const* const matrix, const std::size_t rows, const std::size_t cols, const char * const outputMsg) {
	const unsigned int longestDigitWidth{ getLongestDigitInMatrix(matrix, rows, cols) };
	std::cout << outputMsg;

	for (std::size_t i = 0; i < rows; i++) {
		for (std::size_t ii = 0; ii < cols; ii++) {
			std::cout << std::setw(longestDigitWidth) << matrix[i][ii] << ' ';
		}
		std::cout << std::endl;
	}
}

void sumMatrix(const int* const* const matrix1, const int* const* const matrix2, int** matrix3, const std::size_t rows1, const std::size_t cols1, const std::size_t rows2, const std::size_t cols2) {
	const std::size_t rows3{ std::max(rows1, rows2) };
	const std::size_t cols3{ std::max(cols1, cols2) };

	for (std::size_t i = 0; i < rows3; i++) {
		for (std::size_t ii = 0; ii < cols3; ii++) {
			if (i < rows1 && ii < cols1) {
				matrix3[i][ii] += matrix1[i][ii];
			}
			if (i < rows2 && ii < cols2) {
				matrix3[i][ii] += matrix2[i][ii];
			}
		}
	}
}

int biggestSideDiagonal(const int* const* const matrix1, const int* const* const matrix2, const std::size_t rows1, const std::size_t cols1, const std::size_t rows2, const std::size_t cols2) {
	if (rows1 != cols1 || rows2 != cols2) {
		return -1;
	}

	int sumOfSideDiagonal1{ 0 }, sumOfSideDiagonal2{ 0 };
	const std::size_t rows3{ std::max(rows1, rows2) };

	for (std::size_t i = 0; i < rows3; i++) {
		if (i < rows1) {
			sumOfSideDiagonal1 += matrix1[i][rows1 - 1 - i];
		}
		if (i < rows2) {
			sumOfSideDiagonal2 += matrix2[i][rows2 - 1 - i];
		}
	}
	
	// Debugging
	// std::cout << "Suma za matricu 1: " << sumOfSideDiagonal1 << std::endl;
	// std::cout << "Suma za matricu 2: " << sumOfSideDiagonal2 << std::endl;

	return (sumOfSideDiagonal1 > sumOfSideDiagonal2) ? 1 : 2;
}

unsigned int getLongestDigitInMatrix(const int* const* const matrix, const std::size_t rows, const std::size_t cols) {
	unsigned int longestDigitWidth{ 0u };

	for (std::size_t i = 0; i < rows; i++) {
		for (std::size_t ii = 0; ii < cols; ii++) {
			longestDigitWidth = std::max(getDigitWidth(matrix[i][ii]), longestDigitWidth);
		}
	}

	return longestDigitWidth;
}

unsigned int getDigitWidth(const int num) {
	if (!num) {
		return 1;
	}

	bool isNeg{ num < 0 };

	return static_cast<unsigned int>(std::log10(std::abs(num))) + 1 + isNeg;
}