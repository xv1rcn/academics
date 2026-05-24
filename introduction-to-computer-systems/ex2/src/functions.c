#include <limits.h>

int lsbZero(int x) {
    return x & ~1;
}

int byteNot(int x, int n) {
    int mask = 0xFF << (n << 3);
    return x ^ mask;
}

int byteXor(int x, int y, int n) {
    int mask = 0xFF << (n << 3);
    int byteX = (x & mask) >> (n << 3);
    int byteY = (y & mask) >> (n << 3);
    return (byteX ^ byteY) != 0;
}

int logicalAnd(int x, int y) {
    return !(!x | !y);
}

int logicalOr(int x, int y) {
    return !(!x & !y);
}

int rotateLeft(int x, int n) {
    unsigned shift = (unsigned)n & 31u;
    unsigned value = (unsigned)x;

    if (shift == 0) {
        return x;
    }

    return (int)((value << shift) | (value >> ((32u - shift) & 31u)));
}

int parityCheck(int x) {
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return x & 1;
}

int mul2OK(int x) {
    long long result = 2LL * x;
    return result >= INT_MIN && result <= INT_MAX;
}

int mult3div2(int x) {
    return (int)((3LL * x) / 2);
}

int subOK(int x, int y) {
    long long result = (long long)x - (long long)y;
    return result >= INT_MIN && result <= INT_MAX;
}

int absVal(int x) {
    return x >= 0 ? x : (int)(0u - (unsigned)x);
}