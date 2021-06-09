//float_parser.c

#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <math.h>
#include <ctype.h>

/* Andy Fleischer - CS201 - Program #3 - 5/11/2021
 * -----------------------------------------------
 * This program takes in exactly 3 command line
 * arguments to parse a hexadecimal number as a
 * floating point number. The first argument is
 * the number of frac bits, the second is the
 * number of exp bits, and the last is the hex
 * number being parsed. */

//checks to see if the given bits and hex num are in a valid range
void
check_data(int frac_bits, int exp_bits, int hex_num)
{
    if (frac_bits < 2 || frac_bits > 10) {
        printf("Invalid number of fraction bits (%d). Should be between 2 and 10\n", frac_bits);
        exit(EXIT_FAILURE);
    }
    if (exp_bits < 3 || exp_bits > 5) {
        printf("Invalid number of exponent bits (%d). Should be between 3 and 5\n", exp_bits);
        exit(EXIT_FAILURE);
    }
    if ((unsigned int)hex_num >> (frac_bits + exp_bits + 1)) { //checks for data beyond bits specified
        printf("Number %x exceeds maximum number of bits. Only %d allowed here.\n", hex_num, frac_bits + exp_bits + 1);
        exit(EXIT_FAILURE);
    }
}

//prints the proper output (normalized, denormalized, +-inf, or NaN)
void
print_output(int sign, float frac_field, int exp_field, int exp_bits, int bias)
{
    if (exp_field == (1 << exp_bits) - 1) { //+-inf or NaN
        if (frac_field == 0) { //+-inf
            printf("%cinf\n", sign ? '-' : '+'); //if sign exists (1), print a -, else +
        } else { //NaN
            printf("NaN\n");
        }
    } else {
        float output;
        if (exp_field == 0) { //denormalized number
            output = pow(-1, sign) * frac_field * pow(2, (1 - bias));
        } else { //normalized number
            output = pow(-1, sign) * (1 + frac_field) * pow(2, (exp_field - bias));
        }
        printf("%f\n", output);
    }
}

int
main(int argc, char* argv[]) //main method
{
    if (argc != 4) { //incorrect number of arguments
        printf("Usage: fp_parse <# of frac_bits> <# of exp_bits> <hex #>\n");    
        exit(EXIT_FAILURE);
    }

    //setup variables and sscanf to convert argv args
    int frac_bits, exp_bits, hex_num;
    if (sscanf(argv[1], "%d", &frac_bits) != 1) {
        printf("Error scanning fraction bits. Should be number between 2 and 10.\n");
        exit(EXIT_FAILURE);
    }
    if (sscanf(argv[2], "%d", &exp_bits) != 1) {
        printf("Error scanning exponent bits. Should be number between 3 and 5.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; argv[3][i] != '\0'; i++) { //go through, if any number is not hex, exit with error
        if (!isxdigit(argv[3][i])) {
            printf("Argument 3 is not a hexadecimal number (%s).\n", argv[3]);
            exit(EXIT_FAILURE);
        }
    }
    sscanf(argv[3], "%x", &hex_num);

    check_data(frac_bits, exp_bits, hex_num); //check if data is valid

    //interpret data
    int sign = (unsigned int)hex_num >> (frac_bits + exp_bits); //logical right shift by all the bits for sign
    int sum = hex_num & ((1 << frac_bits) - 1); //bit mask to get the frac section total
    float frac_field = sum / pow(2, frac_bits); //divide by proper power to get the frac field
    int exp_field = ((unsigned int)hex_num >> frac_bits) & ((1 << exp_bits) - 1); //right shift the num then use bit mask
    int bias = pow(2, (exp_bits - 1)) - 1; //bias = 2^(k-1) - 1

    print_output(sign, frac_field, exp_field, exp_bits, bias);
        
    exit(EXIT_SUCCESS);
}
