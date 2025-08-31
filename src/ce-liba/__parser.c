
#include "parser.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


struct ArithmeticParser* analysis__init_parser( const char* summand, const char* reststr, struct ArithmeticParser* prev ) {
    struct ArithmeticParser* arith_parser = (struct ArithmeticParser*) malloc(sizeof(struct ArithmeticParser));

    arith_parser->summand = summand;
    arith_parser->reststr = reststr;
    arith_parser->prev = prev;
    arith_parser->next = analysis__get_next_parser( reststr, arith_parser );

    return arith_parser;
}

struct ArithmeticParser* analysis__init_parser_default( const char* funcstr ) {
    return analysis__get_next_parser( funcstr, NULL );
}

struct ArithmeticParser* analysis__get_next_parser( const char* reststr, struct ArithmeticParser* prev ) {

    bool complete = false;
    bool found_exponent = false;
    bool leaved_exponent = false;
    unsigned int ch = 0;
    size_t reststr_len = strlen(reststr);

    while ( ! complete ) {
        while ( ! found_exponent && ( strcmp( (char*) reststr[ch], "+" ) != 0 || strcmp( (char*) reststr[ch], "-" ) != 0 ) && ch < reststr_len ) {
            ++ch;
            if ( strcmp( (char*) reststr[ch], "^" ) == 0 ) {
                found_exponent = true;
                ++ch;
                break;
            }
        }
        while ( found_exponent ) {
            ++ch;
            if ( strcmp( (char*) reststr[ch], "}" ) == 0 && ch < reststr_len  ) {
                leaved_exponent = true;
                ++ch;
                break;
            }
        }
        if ( leaved_exponent || ! found_exponent ) complete = true;
    }
    
    while ( ( strcmp( (char*) reststr[ch], "+" ) != 0 || strcmp( (char*) reststr[ch], "-" ) != 0 ) && ch < reststr_len  ) { ++ch; }

    char* summand = (char*) malloc(sizeof(char)*ch);
    strncpy( summand, reststr, ch );
    char* new_reststr = (char*) malloc(sizeof(char)*(reststr_len-ch+1));
    unsigned int c = 0;
    for ( unsigned int i = ch-1; i < reststr_len-ch-1; c++, i++ ) { new_reststr[c] = reststr[i]; }

    return analysis__init_parser( summand, new_reststr, prev );
}
