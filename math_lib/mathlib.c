#include "mathlib.h"

#include <math.h>

#define EPSILON 1e-14

// A helper function that only exists in this file
// that calculate and return the absolute value of x
static inline double Abs(double x) {
    return (x < 0) ? -x : x;
}

// Sine function is inspired and sited from Prof. Long
// we normalize the input within the range of Y which is 2pi
double Sin(double y) {
    double x = fmod(y, M_PI * 2);
    double num = x;
    double denom = 1;
    double sum = x;
    double k = 3.0;
    while (Abs(num / denom) > EPSILON) {
        num *= -x * x;
        denom *= k * (k - 1.0);
        sum += (num / denom);
        k += 2;
    }
    return sum;
}

// normalized inputs only used for tangent
double Sin_work(double y) {
    double x = fmod(y, M_PI);
    double num = x;
    double denom = 1;
    double sum = x;
    double k = 3.0;
    while (Abs(num / denom) > EPSILON) {
        num *= -x * x;
        denom *= k * (k - 1.0);
        sum += (num / denom);
        k += 2;
    }
    return sum;
}

// The cosine function is similar to the sine function
// with taylor series expansion
double Cos(double y) {
    double x = fmod(y, M_PI * 2);
    double num = 1.0;
    double denom = 1.0;
    double sum = 1.0;
    double k = 2.0;
    while (Abs(num / denom) > EPSILON) {
        num *= -x * x;
        denom *= k * (k - 1.0);
        sum += (num / denom);
        k += 2;
    }
    return sum;
}

// this function is only used for the tangent function
double Cos_work(double y) {
    double x = fmod(y, M_PI);
    double num = 1.0;
    double denom = 1.0;
    double sum = 1.0;
    double k = 2.0;
    while (Abs(num / denom) > EPSILON) {
        num *= -x * x;
        denom *= k * (k - 1.0);
        sum += (num / denom);
        k += 2;
    }
    return sum;
}

// Tangent function following sinx/cosx
double Tan(double x) {
    return Sin_work(x) / Cos_work(x);
}

// The Exp function mimic the exp() in math.h
// and is approached by taylor series expansion
double Exp(double x) {
    double num = 1.0;
    double denom = 1.0;
    double sum = 1.0;
    double k = 1.0;
    while (Abs(num / denom) > EPSILON) {
        num *= x;
        denom *= k;
        sum += (num / denom);
        k += 1;
    }
    return sum;
}

// Cited from Prof.Long
double Log(double x) {
    double y = 1.0;
    double p = Exp(y);
    while (Abs(x - p) > EPSILON) {
        y += (x - p) / p;
        p = Exp(y);
    }
    return y;
}
