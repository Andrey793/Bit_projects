#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
 * \brief Auxiliary macro used by \ref VAR_TO_STR
 */
#define STR_2(X) #X
/**
 * \brief Returns a literal with the parameter value.
 */
#define VAR_TO_STR(X) STR_2(X)

/**
* \brief The number of digits after the decimal point for float output.
*/
#define prec1 9
/**
* \brief The number of digits after the decimal point for double output.
*/
#define prec2 16

/**
 * \brief Converts mantissa's bit representation to double.
 *
 * @param u Mantissa's bit representation
 * @param size Size of the mantissa
 * @return Mantissa value.
 */
double decode_mantissa(uint64_t u, int size) {
    double res = 0;

    for (int i = size - 1; i >= 0; --i) {
        if (u >> i & 1) {
            res += 1.0 / ((uint64_t)1 << (size - i));
        }
    }
    return res;
}

/**
 * \brief Prints floating point number bit representation.
 *
 * Highlights Sign, Exp and Frac sections.
 * @param u Floating point number bit representation
 * @param exp Size of exp section
 * @param mant Size of mantissa section
 */
void bit_print(uint64_t const u, int const exp, int const mant) {
    for (int i = exp + mant; i >= 0; --i) {
        if (i == exp + mant)
            printf("Sign:");
        if (i == exp + mant - 1)
            printf("  Exp:");
        if (i == mant-1) {
            printf("  Frac:");
        }
        printf("%lu", (u >> i) & 1);
    }
    printf("\n");
}

/**
 * \brief Prints the canonical representation of a floating point number and its bit representation.
 *
 * @param u Floating point number bit representation
 * @param size The size of the number in bits.
 *
 * \warning This function is only compatible with systems where floating point numbers are 32 and 64 bits in size.
 */
void visualise(uint64_t const u, unsigned long const size) {
    int mant = 0;
    int exp = 0;
    uint64_t mant_mask = 0;
    uint64_t exp_mask = 0;
    char const* format = 0;
    switch (size) {
        case 32:
            exp = 8;
            mant = 23;
            format = "%d * 2^(%"PRIi64") * %." VAR_TO_STR(prec1) "f \n\t";
            break;
        case 64:
            exp = 11;
            mant = 52;
            format = "%d * 2^(%"PRIi64") * %." VAR_TO_STR(prec2) "f \n\t";
            break;
        default:
    }
    mant_mask = ((uint64_t)1 << mant) - 1;
    exp_mask = ((uint64_t)1 << exp) - 1;

    if (!exp) {
        fprintf(stderr, "This size is unsupported");
        exit(1);
    }
    int sign = u >> (size - 1);
    int64_t exponent = u >> mant & exp_mask;
    int64_t const bias = (2 << (exp - 2)) - 1;

    uint64_t mantissa = u & mant_mask;
    double decoded_mantissa = decode_mantissa(mantissa, mant);

    if (exponent != 0 && exponent != (1 << exp) - 1) {
        // normalized values
        exponent -= bias;
        printf(format, sign ? -1 : 1, exponent, decoded_mantissa + 1);
    } else if (exponent == 0) {
        // unnormalized values
        exponent = 1 - bias;
        printf(format, sign==1 ? -1 : 1, exponent, decoded_mantissa);
    } else {
        // special values
        printf(mantissa == 0 ? (sign ? "-Inf" : "+Inf") : "Nan");
        printf("\n\t");
    }

    bit_print(u, exp, mant);
}

/**
 * \brief The function accepts a floating point number and visualizes its float and double representations if the number fits within those data types.
 */
int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: <float number>");
        return 1;
    }
    union {
        float f;
        uint64_t u;
    } a1;
    a1.u = 0;
    a1.f = strtof(argv[1], 0);
    if (errno) {
        perror("float representation:");
        errno = 0;
    } else {
        unsigned long f_size = sizeof(a1.f) * 8;
        printf("Float representation: \n\t");
        visualise(a1.u, f_size);

    }

    union {
        double f;
        uint64_t u;
    } a2;
    a2.u = 0;
    a2.f = strtof(argv[1], 0);

    a2.f = strtod(argv[1], 0);
    if (errno) {
        perror("double representation:");
        errno = 0;
    } else {
        unsigned long f_size = sizeof(a2.f) * 8;
        printf("Double representation: \n\t");
        visualise(a2.u, f_size);
        return 0;
    }
    printf("This number cant be represented as a float or double, so it cant be visualized");

}