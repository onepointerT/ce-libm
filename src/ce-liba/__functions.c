
#include "functions.h"

#include <stdbool.h>
//#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "libm.h"
#include "math.h"
#include "parser.h"


const char* toString( const union Exponent aexpo ) {
    return ( strcmp(aexpo.exponent_str, "") != 0 ) ? aexpo.exponent_str : (char*) &(aexpo.exponent);
}


double toDouble( const union Exponent aexpo ) {
    return ( strcmp(aexpo.exponent_str, "") == 0 ) ? aexpo.exponent : *((double*) aexpo.exponent_str);
}


double toDoubleCalc( const union Exponent aexpo, const double ax, const double x  ) {
    return ax * pow(x, toDouble(aexpo));
}


struct ArithmeticFunction* analysis__init_arithmetic_function( double factorialX, ArithmeticExponent expo, char* summand ) {
    struct ArithmeticFunction* arith_func = (struct ArithmeticFunction*) malloc(sizeof(struct ArithmeticFunction));

    arith_func->factorialX = factorialX;
    arith_func->exponent = expo;
    arith_func->summand_addition = summand;

    arith_func->parser = analysis__init_parser_default( analysis_arithmetic_function_to_str(arith_func) );

    if ( arith_func->parser->next != NULL ) arith_func->next_factorization = analysis__init_arithmetic_function_from_parser( arith_func->parser->next );
    else arith_func->next_factorization = NULL;

    if ( arith_func->parser->prev != NULL ) arith_func->prev_factorization = analysis__init_arithmetic_function_from_parser( arith_func->parser->prev );
    else arith_func->prev_factorization = NULL;

    return arith_func;
}


struct ArithmeticFunction* analysis__init_arithmetic_function_from_parser( struct ArithmeticParser* parser ) {
    struct ArithmeticFunction* arith_func = (struct ArithmeticFunction*) malloc(sizeof(struct ArithmeticFunction));

    analysis_init_function_from_summand( arith_func, parser->summand );

    arith_func->parser = parser;

    if ( parser->next != NULL ) arith_func->next_factorization = analysis__init_arithmetic_function_from_parser( parser->next );
    else arith_func->next_factorization = NULL;

    if ( parser->prev != NULL ) arith_func->prev_factorization = analysis__init_arithmetic_function_from_parser( parser->prev );
    else arith_func->prev_factorization = NULL;
    

    return arith_func;
}


struct ArithmeticFunction* analysis__init_arithmetic_function_default() {
    struct ArithmeticFunction* arith_func = (struct ArithmeticFunction*) malloc(sizeof(struct ArithmeticFunction));

    arith_func->factorialX = 0.0;
    arith_func->exponent.exponent = 1.0;
    arith_func->summand_addition = "";

    arith_func->parser = NULL;

    arith_func->prev_factorization = NULL;
    arith_func->next_factorization = NULL;

    return arith_func;
}



const char* analysis_arithmetic_function_to_str( const struct ArithmeticFunction* arith_func ) {
    size_t aflen = strlen(((char*) &(arith_func->factorialX))) + 2 + strlen(toString(arith_func->exponent)) + 1 + strlen(arith_func->summand_addition);
    char* funcstr = (char*) malloc(sizeof(char)*aflen);

    sprintf( funcstr, "%s%s%s%d%s", arith_func->factorialX,
                strcmp(toString(arith_func->exponent), "") == 0 ?
                  "", "", ""
                : ((char*) "^{"
                , strlen(arith_func->exponent.exponent_str) > 0 ? arith_func->exponent.exponent_str : toString(arith_func->exponent)
                , "}"), arith_func->summand_addition );
    
    return funcstr;
}

struct ArithmeticFunction* analysis_arithmetic_function_from_str( const char* funcstr ) {
    struct ArithmeticParser* parser = analysis__init_parser_default( funcstr );
    struct ArithmeticParser* parser1 = parser;

    struct ArithmeticFunction* arith_func_start = analysis__init_arithmetic_function_from_parser( parser1 );
    if ( parser->next != NULL ) {
        parser = parser->next;

        struct ArithmeticFunction* arith_func_current = arith_func_start;

        while ( parser != NULL ) {
            arith_func_current->next_factorization = analysis__init_arithmetic_function_from_parser( parser );
            arith_func_current->next_factorization->prev_factorization = arith_func_current;
            arith_func_current = arith_func_current->next_factorization;
            parser = parser->next;
        }
    }
    
    return arith_func_start;
}

void analysis_init_function_from_summand( struct ArithmeticFunction* arith_func, const char* summand ) {

    unsigned int ch = 0;
    size_t aflen = strlen(summand);

    unsigned int facX = 0;
    while ( strcmp((char*) summand[ch], "x") != 0  && strcmp((char*) summand[ch], "a") != 0 && ch < aflen ) { ++facX; ++ch; }
    char* facXstr = (char*) malloc(sizeof(char)*(facX+1));
    strncpy( facXstr, summand, facX+1 );
    arith_func->factorialX = (double) *facXstr;

    unsigned int ch_before = ch;
    unsigned int expo_idx = ch;
    while ( strcmp((char*) summand[ch], "^") != 0 && (strcmp((char*) summand[ch], "+") != 0 || strcmp((char*) summand[ch], "-")) && ch < aflen ) {
        ++expo_idx; ++ch;
    }
    arith_func->factorialX = *((double*) facXstr);
    
    char* expostr = (char*) malloc(sizeof(char)*(expo_idx+1-ch_before));
    unsigned int str_i = 0;
    for ( unsigned int eidx = ch_before+1; eidx < expo_idx && eidx < aflen; eidx++, str_i++ ) {
        expostr[str_i] = summand[eidx];
    }
    bool has_braces = false;
    for ( unsigned int expo_idx = 0; expo_idx < strlen(expostr); expo_idx++ ) {
        if ( strcmp( (char*) expostr[expo_idx], "{" ) ) { has_braces = true; break; }
    }
    if ( has_braces ) arith_func->exponent.exponent_str = expostr;
    else arith_func->exponent.exponent = *((double*) expostr);

    // Now find the next brace closing or the next plus/minus
    ch_before = ch;
    while ( strcmp( (char*) summand[ch], "+") != 0 || strcmp( (char*) summand[ch], "-" ) || strcmp( (char*) summand[ch], "}") ) {
        if ( ch == aflen ) break;
        ++ch;
    }

    // Now find the next summand index
    if ( ch_before < ch ) { // we found an additional summand constant function-similar
        char* summand_addition = (char*) malloc(sizeof(char*)*(aflen-ch));
        for ( unsigned int sadd_i = ch; ch < aflen; ch++, sadd_i++ ) {
            summand_addition[sadd_i] = summand[ch];
        }
        arith_func->summand_addition = summand_addition;
    }
}


double analysis_calculate_function_factorial( const struct ArithmeticFunction* func, double a ) {
    return toDoubleCalc( func->exponent, func->factorialX, a );
}

double analysis_calculate_function_factorial_with_constant( const struct ArithmeticFunction* func, double a, double c ) {

    double result = analysis_calculate_function_factorial( func, a );

    double contant_summand = cos(a) + pow( sin(a), 2 ) + c;

    return result + contant_summand;
}

double analysis_calculate_factorial_sumproduct( const struct ArithmeticFunction* func, double a, double c ) {

    double result = 0;

    struct ArithmeticFunction* func_now = (struct ArithmeticFunction*) func;
    while ( func_now != NULL) {
        result += analysis_calculate_function_factorial_with_constant( func_now, a, c );
    }

    return result;
}




struct Function* analysis__init_function( const char* funcstr ) {
    struct Function* func = (struct Function*) malloc(sizeof(struct Function));

    func->f = funcstr;
    func->arith = analysis_arithmetic_function_from_str( func->f );

    return func;
}

struct Function* analysis__init_function_default() {
    struct Function* func = (struct Function*) malloc(sizeof(struct Function));

    func->f = "";
    func->arith = NULL;

    return func;
}

void analysis_init_function( struct Function* func ) {
    func->arith = analysis_arithmetic_function_from_str( func->f );
}


double analysis_calculate_function_y( const struct Function* func, double x ) {
    return analysis_calculate_function_factorial( func->arith, x );
}

double analysis_calculate_function_x( const struct Function* func, double y ) {
    double x_start = ftaylorXofY( y );
    double x = ftaylorXofY( analysis_calculate_function_x( func, remainder( x_start, y ) ) );
    return x;
}

double analysis_calculate_function_x_from_a( const struct Function* func, double a ) {
    double y_from_a = ftaylorCompleteY( a );
    double x_from_a = ftaylorXduo( a );
    double y = analysis_calculate_function_x( func, y_from_a );
    double x = analysis_calculate_function_y( func, x_from_a );
    return remainder( x, y );
}

double analysis_calculate_function_limes( const struct Function* func, double x ) {
    return - tan( analysis_calculate_function_y(func, x)/x );
}

double analysis_calculate_function_limes_a_towards_y( const struct Function* func, double a, double x ) {
    return - tan( analysis_calculate_function_limes(func, x) / analysis_calculate_function_x_from_a(func, a) );
}


double analysis_calculate_function_deviation( const struct Function* func, double a ) {
    // The f'(a) = lim _a->~x ((f(x)-f(~x))/(x-~x))
    // So we need an approximated additional divergenting x
    double x_approx = ftaylorXduo( a );

    return - tan( (analysis_calculate_function_x_from_a( func, a ) - analysis_calculate_function_x_from_a( func, x_approx )) / a - x_approx );
}


double analysis_calculate_function_deviation_plus_constant( const struct Function* func, double a, double c ) {
    return analysis_calculate_function_deviation(func, a) + c;
}

double analysis_calculate_function_deviationNth( const struct Function* func, unsigned int Nth, double a ) {
    double N1start = analysis_calculate_function_deviation( func, a );

    unsigned int Nth_k = 1;
    double result = N1start;
    while ( Nth_k <= Nth ) {
        result = analysis_calculate_function_deviation( func, result );
        ++Nth_k;
    }

    return result;
}

double analysis_calculate_function_deviationNth_plus_constant( const struct Function* func, unsigned int Nth, double a, double c ) {
    double N1start = analysis_calculate_function_deviation_plus_constant( func, a, c );

    unsigned int Nth_k = 1;
    double result = N1start;
    while ( Nth_k <= Nth ) {
        result = analysis_calculate_function_deviation_plus_constant( func, result, c );
        ++Nth_k;
    }

    return result;
}


struct FunctionAdvanced* analysis__init_function_advanced( const char* funcstr ) {
    struct FunctionAdvanced* func_adv = analysis__init_function_default_advanced( funcstr );

    func_adv->f1 = analysis__init_function( "lim_a->x (func(a)-func(x)/a-x)");
    func_adv->f2 = analysis__init_function( "lim_a->x (f1(a)-f1(x)/a-x)");
    func_adv->f3 = analysis__init_function( "lim_a->x (f2(a)-f2(x)/a-x)");

    return func_adv;
}

struct FunctionAdvanced* analysis__init_function_default_advanced( const char* funcstr ) {
    struct FunctionAdvanced* func_adv = (struct FunctionAdvanced*) malloc(sizeof(struct FunctionAdvanced));

    func_adv->func = analysis__init_function( funcstr );
    func_adv->f1 = 0;
    func_adv->f2 = 0;
    func_adv->f3 = 0;

    return func_adv;
}

void analysis_init_function_advanced( struct FunctionAdvanced* func, const char* funcstr ) {
    func = analysis__init_function_advanced( funcstr );
}

