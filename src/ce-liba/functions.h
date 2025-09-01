#pragma once



#ifdef __cplusplus
extern "C" {
#endif

#include "parser.h"


typedef union Exponent {
    double exponent;    // For example 2x^3
    char* exponent_str; // For example 2x^exp(x)
} ArithmeticExponent;

const char* toString( const union Exponent aexpo );
double toDouble( const union Exponent aexpo );
double toDoubleCalc( const ArithmeticExponent aexpo, const double ax, const double x );

struct ArithmeticFunction {
    double factorialX;  // For example 2 in front of x
    ArithmeticExponent exponent;
    char* summand_addition;

    struct ArithmeticParser* parser;

    struct ArithmeticFunction* prev_factorization; // For e.g. x^n the 3rd factorization will have n-3 previous ArithmeticFunction factorizations
    struct ArithmeticFunction* next_factorization; // For e.g. x^3 there will follow 3 more exponential factors of x
};

struct ArithmeticFunction* analysis__init_arithmetic_function( double factorialX, ArithmeticExponent expo, char* summand );
struct ArithmeticFunction* analysis__init_arithmetic_function_from_parser( struct ArithmeticParser* parser );
struct ArithmeticFunction* analysis__init_arithmetic_function_default();

const char* analysis_arithmetic_function_to_str( const struct ArithmeticFunction* arith_func );
struct ArithmeticFunction* analysis_arithmetic_function_from_str( const char* funcstr );

void analysis_init_function_from_summand( struct ArithmeticFunction* arith_func, const char* summand );

double analysis_calculate_function_factorial( const struct ArithmeticFunction* func, double a );
double analysis_calculate_function_factorial_with_constant( const struct ArithmeticFunction* func, double a, double c );
double analysis_calculate_factorial_sumproduct( const struct ArithmeticFunction* func, double a, double c );


struct Function {
    char* f;

    struct ArithmeticFunction* arith; // The start of factorization
};

struct Function* analysis__init_function( const char* funcstr );
struct Function* analysis__init_function_default();
void analysis_init_function( struct Function* func );

double analysis_calculate_function_y( const struct Function* func, double x );
double analysis_calculate_function_x( const struct Function* func, double y );
double analysis_calculate_function_x_from_a( const struct Function* func, double a );
double analysis_calculate_function_limes( const struct Function* func, double x );
double analysis_calculate_function_limes_a_towards_y( const struct Function* func, double a, double x );

double analysis_calculate_function_deviation( const struct Function* func, double a );
double analysis_calculate_function_deviation_plus_constant( const struct Function* func, double a, double c );
double analysis_calculate_function_deviationNth( const struct Function* func, unsigned int Nth, double a );
double analysis_calculate_function_deviationNth_plus_constant( const struct Function* func, unsigned int Nth, double a, double c );

struct FunctionAdvanced {
    struct Function* func;
    struct Function* f1;
    struct Function* f2;
    struct Function* f3;
};

struct FunctionAdvanced* analysis__init_function_advanced( const char* funcstr );
struct FunctionAdvanced* analysis__init_function_default_advanced( const char* funcstr );
void analysis_init_function_advanced( struct FunctionAdvanced* func, const char* funcstr );


#ifdef __cplusplus
}
#endif
