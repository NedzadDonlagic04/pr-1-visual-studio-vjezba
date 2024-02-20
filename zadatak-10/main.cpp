#include<iostream>
#include<random>
#include<array>
#include<algorithm>

constexpr std::size_t CARDS_TO_DRAW{ 5 };

[[nodiscard]] bool isSameDistanceBetweenAdjancentElements(std::array<int, CARDS_TO_DRAW>::iterator, const std::array<int, CARDS_TO_DRAW>::iterator);

int main() {
	std::random_device dev{};
	std::mt19937 rng{ dev() };
	std::uniform_int_distribution<std::mt19937::result_type> cardDrawDist{ 1, 14 };

	std::array<int, CARDS_TO_DRAW> cards{};	
	unsigned int counter{ 0u };

	do {
		std::generate(cards.begin(), cards.end(), [&cardDrawDist, &rng]() { return cardDrawDist(rng); });

		counter++;
	} while (!isSameDistanceBetweenAdjancentElements(cards.begin(), cards.end()));

	std::cout << "Ukupno pokusaja: " << counter << std::endl;
	std::cout << "Krajnja kombinacija: ";  
	std::for_each(cards.begin(), cards.end(), [](const int card) { std::cout << card << ' '; });
	std::cout<<std::endl;

	return 0;
}

bool isSameDistanceBetweenAdjancentElements(std::array<int, CARDS_TO_DRAW>::iterator itBegin, const std::array<int, CARDS_TO_DRAW>::iterator itEnd) {
	if (itBegin + 1 == itEnd) {
		return true;
	}

	const unsigned int distance = std::abs(*itBegin - *(itBegin + 1));

	for (itBegin++; itBegin + 1 != itEnd; itBegin++) {
		if (distance != std::abs(*itBegin - *(itBegin + 1))) {
			return false;
		}
	}

	return true;
}