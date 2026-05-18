#include <random>

// returns a random number from to min-to-max
struct RandomNum {
    std::random_device dev;
    std::mt19937 rng{dev()};
    
    int getRandomNum(int min, int max) {
        std::uniform_int_distribution<std::mt19937::result_type> dist(min, max);
        return dist(rng);
    }
};

// 70% probablity of being right
inline bool correctNumber() {
    RandomNum random;
    return random.getRandomNum(1, 10) <= 7;
}
