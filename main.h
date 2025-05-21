#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"

typedef enum
{
    SUCCESS,
    FAILURE
} Status;

Status is_keyword(char *);
Status is_operator(char);
Status validate_octal(char *,int);
Status validate_char(char *);

Status validate_cla(char *argv[],int argc,char *input_fname,char *output_fname);
Status validate_braces(FILE *fdata);


#endif