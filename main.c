#include "main.h"

char lexemes[20];
char num[20];

char input_fname[20];
char output_fname[20];

int hex_flag;
int dec_flag;
int f_flag;
int oct_flag;
int bin_flag;

int main(int argc, char *argv[])
{
	system("clear");
	int i = 0, j = 0;
	int state = 1;
	char ch;
	int char_flag = 0;

	FILE *fop, *fdata;

	// validating the command line arguments
	if(validate_cla(argv,argc,input_fname,output_fname) == FAILURE)
		exit(0);

	//opening the files
	fdata = fopen(input_fname, "r");
	fop = fopen(output_fname, "w");

	//check for valid number of braces
	if(validate_braces(fdata) == FAILURE)
		exit(0);

	
	rewind(fdata);
	while ((ch = fgetc(fdata)) != EOF)
	{
		switch (state)
		{
			case 1:
			// If the character is an alphabet, transition to state 2 (likely identifier or keyword)
			if (isalpha(ch))
			{
				state = 2;
				lexemes[i++] = ch;
			}
			// If the character is a digit, transition to state 3 (likely a number)
			else if (isdigit(ch))
			{
				state = 3;
				num[j++] = ch;
			}
			// If the character is a double quote (") or single quote ('), transition to state 4 (likely a string or character literal)
			else if (ch == '"' || ch == '\'')
			{
				state = 4;
				lexemes[i++] = ch;
			}
			// If the character is whitespace, ignore it (no state change)
			else if (isspace(ch))
			{
			}
			// If the character is a forward slash ('/'), check for comments
			else if (ch == '/')
			{
				ch = fgetc(fdata);
				// Single-line comment (starts with "//"), skip until newline
				if (ch == '/')
				{
					while ((ch = fgetc(fdata)) != EOF)
					{
						if (ch == '\n')
						{
							break;
						}
					}
				}
				// Multi-line comment (starts with "/*"), skip until "*/"
				else if (ch == '*')
				{
					while ((ch = fgetc(fdata)) != EOF)
					{
						if (ch == '*' && (ch = fgetc(fdata)) == '/')
							break;
					}
				}
				// If it's not a comment, put the character back into the stream
				else
					ungetc(ch, fdata);
			}
			// If the character is an operator, transition to state 5
			else if (is_operator(ch) == SUCCESS)
			{
				lexemes[i++] = ch;
				state = 5;
			}
	
			break;

			case 2:
			// If the character is alphanumeric, continue building the lexeme
			if (isalnum(ch))
				lexemes[i++] = ch;
			else
			{
				// Null-terminate the lexeme
				lexemes[i] = '\0';
				// Check if the lexeme is a keyword or an identifier
				if (is_keyword(lexemes) == SUCCESS) 
				{
					fprintf(fop, "%-20s : %s\n", "KEYWORD", lexemes);
				} 
				else 
				{
					fprintf(fop, "%-20s : %s\n", "IDENTIFIER", lexemes);
				}
				// Reset state and lexeme index, push back the last read character
				state = 1;
				i = 0;
				ungetc(ch, fdata);
			}
			break;
		
		case 3:
			// If the character is a digit, continue building the number
			if (isdigit(ch)) 
			{
				num[j++] = ch;
			} 
			// Check for a decimal point, ensuring it's not part of a hex or previously detected decimal
			else if (ch == '.' && dec_flag == 0 && hex_flag == 0)
			{
				dec_flag = 1;
				num[j++] = ch;
		
				// Read the rest of the floating-point number
				while ((ch = fgetc(fdata)) != EOF && ch != ';') 
				{
					if (isdigit(ch)) 
					{
						num[j++] = ch;
					} 
					else if (ch == 'f' || ch == 'F') // Handle floating-point suffix
					{
						num[j++] = ch;
						f_flag = 1; 
						break;
					} 
					else 
					{
						// Invalid floating-point number detected
						num[j++] = ch;
						num[j] = '\0';
						printf(RED"ERROR: NOT A VALID FLOATING POINT %s\n"RESET, num);
						ungetc(ch, fdata);
						state = 1;
						j = 0;
						dec_flag = 0;
						return 1; 
					}
				}
			} 
			// Check for hexadecimal number (starts with 0x or 0X)
			else if ((ch == 'x' || ch == 'X') && num[0] == '0' && j == 1) 
			{
				hex_flag = 1;
				num[j++] = ch;
		
				// Read the rest of the hexadecimal number
				while ((ch = fgetc(fdata)) != EOF && ch != ';') 
				{
					if (isdigit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) 
					{
						num[j++] = ch;
					} 
					else 
					{
						// Invalid hexadecimal number detected
						num[j++] = ch;
						num[j] = '\0'; 
						printf(RED"ERROR: NOT A VALID HEX DIGIT %s\n"RESET,num);
						ungetc(ch, fdata);
						hex_flag = 0;
						state = 1;
						j = 0;
						return 1;
					}
				}
			}
			// Check for binary number (starts with 'b' after an initial digit)
			else if (ch == 'b')
			{
				bin_flag = 1;
				num[j++] = ch;
		
				// Read the rest of the binary number
				while ((ch = fgetc(fdata)) != EOF && ch != ';')
				{
					if (ch == '0' || ch == '1')
					{
						num[j++] = ch;
					}
					else
					{
						// Invalid binary number detected
						num[j++] = ch;
						num[j] = '\0';
						printf(RED"ERROR: NOT A VALID BINARY DIGIT %s\n"RESET,num);
						ungetc(ch, fdata);
						bin_flag = 0;
						state = 1;
						j = 0;
						return 1;
					}
				}
			}
			// Check for octal number (starts with '0' and is not part of hex or binary)
			else if (num[0] == '0' && hex_flag == 0 && bin_flag == 0 && oct_flag == 0)
			{
				num[j] = '\0';
		
				// Validate if it is a proper octal number
				if (validate_octal(num, j) == SUCCESS)
				{
					oct_flag = 1;
				}
				else
				{
					// Invalid octal number detected
					printf(RED"ERROR: NOT A VALID OCTAL DIGIT %s\n"RESET,num);
					ungetc(ch, fdata);
					oct_flag = 0;
					state = 1;
					j = 0;
					return 1;
				}
			}
			else 
			{
				// Null-terminate the number
				num[j] = '\0';
		
				// Classify and print the detected number type
				if (hex_flag) 
				{
					fprintf(fop, "%-20s : %s\n", "HEXADECIMAL CONSTANT", num);
					hex_flag = 0;
				} 
				else if (dec_flag) 
				{
					fprintf(fop, "%-20s : %s\n", "FLOATING CONSTANT", num);
					dec_flag = 0;
				} 
				else if (oct_flag)
				{
					fprintf(fop, "%-20s : %s\n", "OCTAL CONSTANT", num);
					oct_flag = 0;
				}
				else if (bin_flag)
				{
					fprintf(fop, "%-20s : %s\n", "BINARY CONSTANT", num);
					bin_flag = 0;
				}
				// If an invalid character follows the number, print an error
				else if (isalpha(ch))
				{
					num[j++] = ch;
					num[j] = '\0';
					printf(RED"ERROR : NOT A VALID NUMBER %s \n"RESET,num);
					ungetc(ch, fdata);
					state = 1;
					j = 0;
					return 1;
				}
				else 
				{
					// If no special flags, classify it as an integer constant
					fprintf(fop, "%-20s : %s\n", "INTEGER CONSTANT", num);
				}
		
				// Reset state and number index, push back the last read character
				state = 1;
				j = 0;
				ungetc(ch, fdata);
			}
			break;
		
			case 4 :
			{
				char quote = lexemes[0]; // Store the opening quote (single or double)
				lexemes[i++] = ch; // Store the first character of the lexeme
				int q_flag = 1; // Flag to track if closing quote is found
				while ((ch = fgetc(fdata)) != EOF) // Read characters until EOF
				{
					lexemes[i++] = ch; // Store the character in the lexeme array
					if(ch == quote) // If closing quote is found
					{
						q_flag = 0; // Reset flag as closing quote is found
						lexemes[i] = '\0'; // Null-terminate the lexeme string
						if(quote == '"') // Check if it is a string literal
							fprintf(fop, "%-20s : %s\n", "STRING LITERAL", lexemes);
						else // Otherwise, it is a character literal
						{
							if(validate_char(lexemes) == SUCCESS) // Validate character literal
							{
								fprintf(fop, "%-20s : %s\n", "CHARACTER LITERAL", lexemes);
							}
							else // If invalid character constant, print an error
							{
								printf(RED"ERROR : INVALID CHARACTER CONSTANT %s\n"RESET,lexemes);
								i = 0;
								state = 1;
								return 1;
							}
						}
						i = 0;
						state  = 1; // Reset state to default
						break;
					}
				}
				if(q_flag == 1) // If flag is still 1, closing quote is missing
				{
					printf(RED"ERROR : CLOSING QUOTE MISSING\n"RESET);
					i = 0;
					state = 1;
					return 1;
				}
				break;
			}
	
			case 5 :
				// Check if the character is part of a possible operator
				if (ch == '=' || ch == '+' || ch == '-' || ch == '&' || ch == '|')
				{
					lexemes[i++] = ch; // Store the first character of the operator
					lexemes[i] = '\0'; // Null-terminate the lexeme string
					ch = fgetc(fdata); // Read the next character
	
					// Check for multi-character operators like ++, --, >=, <=, ==, &&, ||
					if ((lexemes[0] == '+' && ch == '+') ||
						(lexemes[0] == '-' && ch == '-') ||
						(lexemes[0] == '>' && ch == '=') ||
						(lexemes[0] == '<' && ch == '=') ||
						(lexemes[0] == '=' && ch == '=') ||
						(lexemes[0] == '&' && ch == '&') ||
						(lexemes[0] == '|' && ch == '|'))
					{
						lexemes[i++] = ch; // Store the second character of the operator
						lexemes[i] = '\0'; // Null-terminate the lexeme string
					}
					else // If not a multi-character operator, push back the character
					{
						ungetc(ch, fdata);
					}
				}
				ungetc(ch,fdata); // Push back the last read character
				lexemes[i] = '\0'; // Null-terminate the lexeme string
				fprintf(fop, "%-20s : %s\n", "OPERATOR", lexemes); // Print operator
				state = 1; // Reset state to default
				i = 0;
				break;

		default:
			printf("ERROR: INVALID CHARACTER\n");
			break;
		}
	}
	fclose(fdata);
	fclose(fop);
	printf(GREEN"INFO : LEXICAL ANALYSIS COMPLETED"RESET"\n");
	return 0;
}
