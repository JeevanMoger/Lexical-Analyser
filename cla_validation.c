#include "main.h"

/**
 * validate_cla - Validates command-line arguments and opens input/output files.
 * @argv: Array of command-line arguments.
 * @argc: Number of command-line arguments.
 * @input_fname: Pointer to store the input file name.
 * @output_fname: Pointer to store the output file name.
 *
 * This function checks if the correct number of arguments is provided,
 * assigns input and output file names, and attempts to open the files.
 * If any errors occur (insufficient parameters or file errors), it returns FAILURE.
 * Otherwise, it prints success messages and returns SUCCESS.
 *
 * Return: SUCCESS if files are opened successfully, otherwise FAILURE.
 */
Status validate_cla(char *argv[], int argc, char *input_fname, char *output_fname)
{
    FILE *fop, *fdata;

    // Check if at least one argument is provided
    if (argc == 1)
    {
        printf(RED"INSUFFICIENT PARAMETERS\n"RESET);
        printf(RED"./a.out <.c_file> [.text_file]\n"RESET);
        return FAILURE;
    }

    // Copy input file name from command-line argument
    if (argv[1] != NULL)
    {
        strcpy(input_fname, argv[1]);
    }
    else 
    {
        printf(RED"INSUFFICIENT PARAMETERS\n"RESET);
        printf(RED"./a.out <.c_file> [.text_file]\n"RESET);
    }

    // Try opening the input file in read mode
    fdata = fopen(input_fname, "r");
    if (fdata == NULL)
    {
        printf(RED"Error opening input file.\n"RESET);
        return FAILURE;
    }
    else
    {
        printf(GREEN"INFO : OPENED INPUT FILE %s SUCCESSFULLY\n"RESET, input_fname);
    }

    // Copy output file name from command-line argument or use default "output.txt"
    if (argv[2] != NULL)
    {
        strcpy(output_fname, argv[2]);
    }
    else 
    {
        strcpy(output_fname, "output.txt");
    }

    // Try opening the output file in write mode
    fop = fopen(output_fname, "w");
    if (fop == NULL)
    {
        printf("Error opening output file.\n");
        fclose(fdata); // Close input file before returning
        return FAILURE;
    }
    else
    {
        printf(GREEN"INFO : OPENED THE OUTPUT FILE %s SUCCESSFULLY\n"RESET, output_fname);
    }

    return SUCCESS;
}
