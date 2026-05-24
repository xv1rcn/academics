#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int lsbZero(int x);
int byteNot(int x, int n);
int byteXor(int x, int y, int n);
int logicalAnd(int x, int y);
int logicalOr(int x, int y);
int rotateLeft(int x, int n);
int parityCheck(int x);
int mul2OK(int x);
int mult3div2(int x);
int subOK(int x, int y);
int absVal(int x);

static void expectEqual(int actual, int expected, const char *name) {
    if (actual != expected) {
        printf("%s failed: expected %d, got %d\n", name, expected, actual);
        exit(EXIT_FAILURE);
    }
}

static void testLsbZero(void) {
    expectEqual(lsbZero(0), 0, "lsbZero(0)");
    expectEqual(lsbZero(1), 0, "lsbZero(1)");
    expectEqual(lsbZero(2), 2, "lsbZero(2)");
    expectEqual(lsbZero(-1), -2, "lsbZero(-1)");
}

static void testByteNot(void) {
    expectEqual(byteNot(0x12345678, 0), 0x12345687, "byteNot byte0");
    expectEqual(byteNot(0x12345678, 1), 0x1234A978, "byteNot byte1");
    expectEqual(byteNot(0x12345678, 2), 0x12CB5678, "byteNot byte2");
    expectEqual(byteNot(0x12345678, 3), 0xED345678, "byteNot byte3");
}

static void testByteXor(void) {
    expectEqual(byteXor(0x12345678, 0x12345678, 2), 0, "byteXor same");
    expectEqual(byteXor(0x12345678, 0x12AA5678, 2), 1, "byteXor different");
    expectEqual(byteXor(0x00000000, 0xFF000000, 3), 1, "byteXor top byte");
}

static void testLogical(void) {
    expectEqual(logicalAnd(0, 0), 0, "logicalAnd 0 0");
    expectEqual(logicalAnd(1, 0), 0, "logicalAnd 1 0");
    expectEqual(logicalAnd(3, -7), 1, "logicalAnd nonzero");

    expectEqual(logicalOr(0, 0), 0, "logicalOr 0 0");
    expectEqual(logicalOr(1, 0), 1, "logicalOr 1 0");
    expectEqual(logicalOr(3, -7), 1, "logicalOr nonzero");
}

static void testRotateLeft(void) {
    expectEqual(rotateLeft(0x12345678, 0), 0x12345678, "rotateLeft 0");
    expectEqual(rotateLeft(0x12345678, 4), 0x23456781, "rotateLeft 4");
    expectEqual(rotateLeft(0x12345678, 8), 0x34567812, "rotateLeft 8");
    expectEqual(rotateLeft(0x12345678, 32), 0x12345678, "rotateLeft 32");
    expectEqual(rotateLeft(0x12345678, 36), 0x23456781, "rotateLeft 36");
}

static void testParityCheck(void) {
    expectEqual(parityCheck(0), 0, "parityCheck 0");
    expectEqual(parityCheck(1), 1, "parityCheck 1");
    expectEqual(parityCheck(3), 0, "parityCheck 3");
    expectEqual(parityCheck(-1), 0, "parityCheck all ones");
    expectEqual(parityCheck(0x80000000), 1, "parityCheck sign bit");
}

static void testMul2OK(void) {
    expectEqual(mul2OK(0), 1, "mul2OK 0");
    expectEqual(mul2OK(1), 1, "mul2OK 1");
    expectEqual(mul2OK(0x20000000), 1, "mul2OK safe");
    expectEqual(mul2OK(0x40000000), 0, "mul2OK overflow positive");
    expectEqual(mul2OK(INT_MIN), 0, "mul2OK overflow negative");
}

static void testMult3div2(void) {
    expectEqual(mult3div2(0), 0, "mult3div2 0");
    expectEqual(mult3div2(2), 3, "mult3div2 2");
    expectEqual(mult3div2(3), 4, "mult3div2 3");
    expectEqual(mult3div2(-2), -3, "mult3div2 -2");
    expectEqual(mult3div2(-3), -4, "mult3div2 -3");
}

static void testSubOK(void) {
    expectEqual(subOK(10, 3), 1, "subOK safe");
    expectEqual(subOK(INT_MAX, -1), 0, "subOK positive overflow");
    expectEqual(subOK(INT_MIN, 1), 0, "subOK negative overflow");
    expectEqual(subOK(INT_MIN, INT_MIN), 1, "subOK equal min");
}

static void testAbsVal(void) {
    expectEqual(absVal(0), 0, "absVal 0");
    expectEqual(absVal(12), 12, "absVal positive");
    expectEqual(absVal(-12), 12, "absVal negative");
    expectEqual(absVal(INT_MIN), INT_MIN, "absVal INT_MIN");
}

int main(void) {
    testLsbZero();
    testByteNot();
    testByteXor();
    testLogical();
    testRotateLeft();
    testParityCheck();
    testMul2OK();
    testMult3div2();
    testSubOK();
    testAbsVal();

    puts("all tests passed");
    return 0;
}