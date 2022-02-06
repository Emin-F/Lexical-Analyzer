#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Dynamically allocates memory for two strings and concatenates them.
char* concatString (char* s1, char* s2) {
    int size = strlen(s1) + strlen(s2);
    s1 = realloc(s1, sizeof(char) * (size+1));
    strcat(s1, s2);
    return s1;
}

// Finds the character is a bracket.
bool isBracket (char ch) {
    if (ch == '(' || ch == ')' || ch == '[' || ch == ']' ||
        ch == '{' || ch == '}')
        return (true);
    return (false);
}

// Finds if the character is a double quote.
bool isDoubleQuote (char ch) {
    if (ch == '"') return true;
    return false;
}

// Finds if the character is a delimiter.
/* To be able to find 'Identifier's and 'Integer's, I created delimiters and tried to catch them.
 * This way, I was able to distinguish an identifier with another element. */
bool isDelimiter(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
        ch == ':' || ch == ';' || ch == '(' || ch == ')' ||
        ch == '[' || ch == ']' || ch == '{' || ch == '}' ||
        ch == ' ' || ch == '"' || ch == '\n')
        return (true);
    return (false);
}

// Finds if the character is an operator.
bool isOperator(char ch)
{
    if (ch == '+' || ch == '-' || ch == '*' ||
            ch == '/' || ch == ':' || ch == '=')
        return (true);
    return (false);
}

// Finds if the string is a keyword.
bool isKeyword(char* str)
{
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
    if (!strcmp(str, "break") || !strcmp(str, "case") ||
        !strcmp(str, "char") || !strcmp(str, "const") ||
        !strcmp(str, "continue") || !strcmp(str, "do") ||
        !strcmp(str, "else")
        || !strcmp(str, "enum") || !strcmp(str, "float")
        || !strcmp(str, "for") || !strcmp(str, "goto")
        || !strcmp(str, "if") || !strcmp(str, "int")
        || !strcmp(str, "long") || !strcmp(str, "record")
        || !strcmp(str, "return") || !strcmp(str, "static")
        || !strcmp(str, "while"))
        return (true);
    return (false);
}

// Finds if the string is a keyword.
bool isInteger(char* str)
{
    int i, len = strlen(str);

    if (len == 0)
        return (false);
    for (i = 0; i < len; i++) {
        if ((str[i] != '0' && str[i] != '1' && str[i] != '2'
            && str[i] != '3' && str[i] != '4' && str[i] != '5'
            && str[i] != '6' && str[i] != '7' && str[i] != '8'
            && str[i] != '9') || (str[i] == '-'))
            return (false);
    }
    return (true);
}

// Finds if the character is an EndOfLine.
bool isEOL (char ch) {
    if (ch == ';') return true;
}

// Finds if the identifier given is valid.
/* Here I thought identifier should have not start with a number and it should have between alphabet with
 * numbers and '_' character.
 * So first I found any number-starting and longer than 20 identifiers.
 * Then I checked if they contain anything else than alphabet characters and '_'. */
bool validIdentifier(char* str)
{
    for (int i = 0; str[i]; i++) {
        str[i] = toupper(str[i]);
    }
    int len = strlen(str);
    if (str[0] == '0' || str[0] == '1' || str[0] == '2' ||
        str[0] == '3' || str[0] == '4' || str[0] == '5' ||
        str[0] == '6' || str[0] == '7' || str[0] == '8' ||
        str[0] == '9' || len > 20)
        return (false);

    for (int i = 0; str[i]; i++) {
        if (!(str[i] >= 'A' && str[i] <= 'Z')) {
            if (!(str[i] == '0' || str[i] == '1' || str[i] == '2' ||
                str[i] == '3' || str[i] == '4' || str[i] == '5' ||
                str[i] == '6' || str[i] == '7' || str[i] == '8' ||
                str[i] == '9' || str[i] == '_')) {
                return false;
            }
        }
    }
    return (true);
}

// Takes out a substring for examination.
/* I thought how I could identify identifiers. The only way was take them out as a seperate string.
 * They should've between delimiters so this function just cuts them up. */
char* subString(char* str, int left, int right)
{
    int i;
    char* subStr = (char*)malloc(
            sizeof(char) * (right - left + 2));

    for (i = left; i <= right; i++)
        subStr[i - left] = str[i];
    subStr[right - left + 1] = '\0';
    return (subStr);
}

// Parses all the code.
char* parse(char* str)
{
    /* These temporary strings made for expanding the strings along the way.
    ** I put the string in 'tmp2', then concatenate that string with 'tmp' within all the elements.
    ** While concatenating, I used Dynamic Memory Allocation. */
    char* tmp = malloc(sizeof(char) * 15);
    char* tmp2 = malloc(sizeof(char) * 15);

    // Left and right made for iterating through the string.
    int left = 0, right = 0;

    // Found the length of the string for using it with while loop.
    int len = strlen(str) - 1;

    // This while loop iterates through the string until the end.
    while (right <= len && left <= right) {
        // Iterator goes to right until there's a delimiter.
        if (isDelimiter(str[right]) == false)
            right++;

        // Iterator skips whitespaces.
        if (str[right] == ' ')
            right++;

        // When iterator encounters a delimiter and if left iterator is caught up with right iterator,
        // it tries to find its job. If left not caught up yet, it explained below.
        if (isDelimiter(str[right]) == true && left == right) {
            // If iterator encounters an 'Operator'
            if (isOperator(str[right]) == true)
                // First, it tries to find double character operators like '++'.
                if (str[right] == '+' && str[right+1] == '+') {
                    sprintf(tmp2, "Operator(%c%c)\n", str[right], str[right+1]);
                    tmp = concatString(tmp, tmp2);
                    right++;
                } else if (str[right] == '-' && str[right+1] == '-') {
                    sprintf(tmp2, "Operator(%c%c)\n", str[right], str[right+1]);
                    tmp = concatString(tmp, tmp2);
                    right++;
                } else if (str[right] == ':' && str[right+1] == '=') {
                    sprintf(tmp2, "Operator(%c%c)\n", str[right], str[right+1]);
                    tmp = concatString(tmp, tmp2);
                    right++;
                }

                // Then it tries to find single character operators.
                else {
                    sprintf(tmp2, "Operator(%c)\n", str[right]);
                    tmp = concatString(tmp, tmp2);
                }

            // If iterator encounters an 'Left Par', it tries to find '*' next to it.
            // Made to be able to find '#8, Comments'.
            else if (str[right] == '(' && str[right+1] == '*') {
                while (!(str[right-1] == '*' && str[right] == ')')) {
                    right++;

                    // If the comment line doesn't end, it shows an error message.
                    if (right > strlen(str)) {
                        printf("ERROR: COMMENT LINE IS NOT ENDING!", str[right], str[right+1]);
                        exit(1);
                    }
                }
            }

            // If iterator encounters a 'Bracket', it tries to find its kind.
            // Made to be able to find '#4, Brackets'.
            else if (isBracket(str[right])) {
                if (str[right] == '(') {
                    sprintf(tmp2, "LeftPar\n");
                    tmp = concatString(tmp, tmp2);
                } else if (str[right] == ')') {
                    sprintf(tmp2, "RightPar\n");
                    tmp = concatString(tmp, tmp2);
                } else if (str[right] == '[') {
                    sprintf(tmp2, "LeftSquareBracket\n");
                    tmp = concatString(tmp, tmp2);
                } else if (str[right] == ']') {
                    sprintf(tmp2, "RightSquareBracket\n");
                    tmp = concatString(tmp, tmp2);
                } else if (str[right] == '{') {
                    sprintf(tmp2, "LeftCurlyBracket\n");
                    tmp = concatString(tmp, tmp2);
                } else if (str[right] == '}') {
                    sprintf(tmp2, "RightCurlyBracket\n");
                    tmp = concatString(tmp, tmp2);
                }
            }

            // Iterator tries to find a string between 'double quote'.
            // Made to be able to find '#5, String constants.
            else if (isDoubleQuote(str[right])) {
                right++;
                char tmp3[1000] = "String(";
                int cursor = 7;
                while (str[right] != '"') {
                    tmp3[cursor] = str[right];
                    cursor++;
                    right++;
                    if (right > strlen(str)) {
                        printf("ERROR: STRING IS NOT ENDING!");
                        exit(1);
                    }
                }
                tmp3[cursor] = ')';
                tmp3[cursor+1] = '\n';
                tmp3[cursor+2] = '\0';
                concatString(tmp, tmp3);
            }

            // Iterator tries to find a ';'.
            // Made to be able to find '#7, EndOfLine'.
            else if (isEOL(str[right]) == true) {
                if (str[right] != '\n') {
                    sprintf(tmp2, "EndOfLine\n");
                    tmp = concatString(tmp, tmp2);;
                }
            }

            // After executing the block and finding the things, it moves the iterator.
            right++;
            left = right;
        }

        // This is when the left iterator didn't caught up with right.
        // Made to be able to cut a substring and find its type.
        /* I thought about how I could cut out the identifiers and integers.
         * I needed to cut the line to be able to find the identifiers.
         * I needed a start and an end point to cut. Because of that, I made a left iterator and a right iterator.
         * If the right iterator is caught a delimiter, I split a substring from left iterator to right iterator. */
        else if ((isDelimiter(str[right]) == true && left != right)
                   || (right == len && left != right)) {
            char* subStr = subString(str, left, right - 1);

            // Made to be able to find '#6, Keywords'.
            if (isKeyword(subStr) == true) {
                sprintf(tmp2, "Keyword(%s)\n", subStr);
                tmp = concatString(tmp, tmp2);
            }

            // Made to be able to find '#2, Integer constants'.
            else if (isInteger(subStr) == true) {
                if (strlen(subStr) > 10) {
                    printf("ERROR: '%s' IS NOT A CORRECT INTEGER!\n", subStr);
                } else {
                    sprintf(tmp2, "IntConst(%s)\n", subStr);
                    tmp = concatString(tmp, tmp2);
                }
            }

            // Made to be able to find '#1, Identifiers'.
            else if (validIdentifier(subStr) == true
                     && isDelimiter(str[right - 1]) == false) {
                sprintf(tmp2, "Identifier(%s)\n", subStr);
                tmp = concatString(tmp, tmp2);
            }

            // If the identifier is not valid, it displays an error message.
            else if (validIdentifier(subStr) == false
                     && isDelimiter(str[left - 1]) == false) {
                printf("ERROR: '%s' IS NOT A VALID IDENTIFIER!\n", subStr);
            }

            // After cutting the substring, it makes the left iterator catch up with the right iterator.
            left = right;
        }
    }

    // It returns the written string to be able to write into the file faster.
    return tmp;
}

int main()
{
    FILE *codePsiFile, *codeLexFile;
    int size = 0;

    char* contents = malloc(sizeof(char) * 1);

    codePsiFile = fopen("code.psi", "r");
    codeLexFile = fopen("code.lex", "w");

    if (codePsiFile == NULL) {
        printf("ERROR: FILE COULD NOT BE OPENED");
        exit(-1);
    }

    while (fscanf(codePsiFile, "%c", &contents[size]) != EOF) {
        size++;
        contents = realloc(contents, (sizeof(char) * size) + 1);
    }
    fclose(codePsiFile);
    char* codeLex = parse(contents);

    fprintf(codeLexFile, "%s", codeLex);

    return (0);
}
