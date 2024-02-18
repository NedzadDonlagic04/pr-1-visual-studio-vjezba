#include<iostream>
#include<memory>

namespace arr {
    constexpr std::size_t ARR_SIZE{ 6 };
    constexpr std::size_t ARR_SHIFT{ 2 };

    void fillArray(int(&arr)[ARR_SIZE]) {
        for (std::size_t i = 0; i < std::size(arr); i++) {
            arr[i] = static_cast<int>(i + 1);
        }
    }

    void printArray(int(&arr)[ARR_SIZE], const char* const outputMsg) {
        std::cout << outputMsg;
        for (const auto num : arr) {
            std::cout << num << ' ';
        }
        std::cout << std::endl;
    }
}

namespace example1 {
    /*
        Saw this implementation for this algorithm here
        Here I just wrote it myself in an attempt to better understand it
        https://en.cppreference.com/w/cpp/algorithm/rotate
        https://github.com/gcc-mirror/gcc/blob/14d8a5ae472ca5743016f37da2dd4770d83dea21/libstdc%2B%2B-v3/include/bits/stl_algo.h#L1213-L1416
    */
    void leftShiftArray(int(&arr)[arr::ARR_SIZE], std::size_t totalShifts) {
        if (totalShifts == 0 || totalShifts >= std::size(arr)) {
            return;
        }

        std::size_t& middle{ totalShifts };
        std::size_t first{ 0 };
        std::size_t first2{ middle };
        std::size_t last{ std::size(arr) };

        do {
            std::swap(arr[first], arr[first2]);
            first++;
            first2++;

            if (first == middle) {
                middle = first2;
            }
            // Debugging
            // printArray(arr, "");
        } while (first2 != last);

        // Debugging
        // std::cout << first << ' ' << middle << std::endl;

        first2 = middle;

        while (first2 != last) {
            std::swap(arr[first], arr[first2]);
            first++;
            first2++;

            if (first == middle) {
                middle = first2;
            }
            else if (first2 == last) {
                first2 = middle;
            }
            // Debugging
            // printArray(arr, "");
        }
    }
}

namespace example2 {
    void leftShiftArray(int(&arr)[arr::ARR_SIZE], std::size_t totalShifts) {
        if (totalShifts == 0 || totalShifts >= std::size(arr)) {
            return;
        }

        auto tempArr = std::make_unique<int[]>(totalShifts);

        for (std::size_t i = 0; i < totalShifts; i++) {
            tempArr[i] = arr[i];
        }

        for (std::size_t i = totalShifts; i < std::size(arr); i++) {
            std::swap(arr[i], arr[i - totalShifts]);
        }

        for (std::size_t i = 0; i < totalShifts; i++) {
            arr[std::size(arr) - totalShifts + i] = tempArr[i];
        }
    }
}

int main() {
    long long idk{ 0u };

    int arr[arr::ARR_SIZE]{};

    arr::fillArray(arr);
    arr::printArray(arr, "Array before rotation: ");

    example1::leftShiftArray(arr, arr::ARR_SHIFT);
    arr::printArray(arr, "Array after example 1 rotation: ");

    example2::leftShiftArray(arr, arr::ARR_SHIFT);
    arr::printArray(arr, "Array after example 2 rotation: ");

    return 0;
}