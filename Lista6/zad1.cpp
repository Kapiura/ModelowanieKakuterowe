#include <iostream>
#include <cstdint>
#include <vector>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

class MersenneTwister {
private:
    uint32_t MT[624];
    int index = 624;
public:
    MersenneTwister(uint32_t seed) {
        MT[0] = seed;
        for (int i = 1; i < 624; i++)
            MT[i] = 0x6c078965 * (MT[i - 1] ^ (MT[i - 1] >> 30)) + i;
    }

    void generateNumbers() {
        for (int i = 0; i < 624; i++) {
            int y = (MT[i] & 0x80000000) + (MT[(i + 1) % 624] & 0x7FFFFFFF);
            MT[i] = MT[(i + 397) % 624] ^ (y >> 1);
            if (y & 1)
                MT[i] ^= 0x9908b0df;
        }
    }

    int extractNumber() {
        if (index == 0)
            generateNumbers();
        int y = MT[index];
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680;
        y ^= (y << 15) & 0xefc60000;
        y ^= (y >> 18);
        index = (index + 1) % 624;
        return y;
    }
};

int lcg(uint64_t modulus, int a, int c, int seed) {
    return (a * seed + c) % modulus;
}

int main() {
    int buckets = 10'000;
    int n = 1'000'000;
    int seed = 2137;

    MersenneTwister generator(seed);
    std::vector<int> x_MersenneTwister(buckets, 0);
    std::vector<int> x_modulo(buckets, 0);
    std::vector<int> x(buckets);

    for (int i = 0; i < buckets; ++i)
        x[i] = i;

    uint64_t modulus = uint64_t(1) << 32;
    int a = 1664525;
    int c = 1013904223;

    for (int i = 0; i < n; i++) {
        uint32_t randTemp =  generator.extractNumber();
        x_MersenneTwister[randTemp % buckets]++;

        seed = lcg(modulus, a, c, seed);
        x_modulo[seed % buckets]++;
    }

    plt::figure_size(1920, 1080);
    plt::bar(x, x_MersenneTwister);
    plt::title("Histogram (Mersenne Twister)");
    plt::xlabel("Kubełek");
    plt::ylabel("Ilość");
    plt::tight_layout();
    plt::save("z1_Mersenne_hist.png");

    plt::figure_size(1920, 1080);
    plt::bar(x, x_modulo);
    plt::title("Histogram (LCG)");
    plt::xlabel("Kubełek");
    plt::ylabel("Ilość");
    plt::tight_layout();
    plt::save("z1_lcg_hist.png");
}
