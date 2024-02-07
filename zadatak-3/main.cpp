#include<iostream>
#include<random>

int main() {
	std::random_device dev{};
	std::mt19937 rng{ dev() };
	std::uniform_int_distribution<std::mt19937::result_type> cardDrawDist{ 1, 14 };

	int card1{}, card2{}, card3{};
	unsigned int counter{ 0u };

	do {
		card1 = cardDrawDist(rng);
		card2 = cardDrawDist(rng);
		card3 = cardDrawDist(rng);

		counter++;
	} while (card1 + 5 != card2 || card2 + 5 != card3);

	std::cout << "Ukupno pokusaja: " << counter << std::endl;
	std::cout << "Krajnja kombinacija: " << card1 << ' ' << card2 << ' ' << card3 << std::endl;

	return 0;
}