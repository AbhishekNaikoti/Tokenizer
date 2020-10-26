#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

typedef struct Node { // each Node holds characters that have been processed, creating one token at the end
    char data;
    struct Node *next;
} Node;

Node *headToken; // current token being created
char *input; // input string read in through argv[1]
int currIndex; // current index of pointer in input
int currTokenType;
int prevTokenType;

void insertRearLL (char c) {
    Node *new_node = (Node*)malloc(sizeof(Node)); // allocating space for a new node
    Node *curr_node = NULL;

    new_node->data = c;
    new_node->next = NULL;

    if (headToken == NULL) {
        headToken = new_node;
    } else {
        curr_node = headToken;

        while (curr_node->next != NULL) {
            curr_node = curr_node->next;
        }
        curr_node->next = new_node;
    }
}

void printLL () {
    Node* curr_ptr = headToken;

    if (headToken == NULL) {
        printf("Empty List");
    }

    printf("\"");
    while (curr_ptr != NULL) {
        printf("%c", curr_ptr->data);
        curr_ptr = curr_ptr->next;
    }
    printf("\"");

    printf("\n");
}

void freeLL () {
    Node *curr_ptr;

    while (headToken !=  NULL) {
        if ((headToken)->next == NULL) {
            free(headToken);
            headToken = NULL;
        } else {
            curr_ptr = headToken;
            headToken = (headToken)->next;
            free(curr_ptr);
        }
    }

    headToken = NULL;
}

void getWordToken () {
    prevTokenType = currTokenType;
    currTokenType = 1;

    insertRearLL(input[currIndex]);
    currIndex++;

    while (isalnum(input[currIndex])) {
        insertRearLL(input[currIndex]);
        currIndex++;
    }
}

void getDecimalToken () {
    prevTokenType = currTokenType;
    currTokenType = 2;

    insertRearLL(input[currIndex]);
    currIndex++;

    while (isdigit(input[currIndex])) {
        insertRearLL(input[currIndex]);
        currIndex++;
    }
}

void getHexToken () {
    prevTokenType = currTokenType;
    currTokenType = 4;

    insertRearLL(input[currIndex]); // inserting x or X
    currIndex++;

    while (isxdigit(input[currIndex])) {
        insertRearLL(input[currIndex]);
        currIndex++;
    }
}

int validOctal (char c) {
    if (c >= '0' && c <= '7') {
        return 1;
    }

    return 0;
}

void getOctalToken () {
    prevTokenType = currTokenType;
    currTokenType = 3;

    insertRearLL(input[currIndex]);
    currIndex++;

    if ((input[currIndex] == 'x') || (input[currIndex] == 'X')) { // second character after '0' is (x or X) indicating potential hexadecimal token
        getHexToken();
    } else if (validOctal(input[currIndex]) == 1) {
        while (validOctal(input[currIndex]) == 1) {
            printf("Came in getOctalToken\n");
            insertRearLL(input[currIndex]);
            currIndex++;
        }

        if (input[currIndex] >= '8' && input[currIndex] <= '9') { // current character is out of OCTAL range
            // printf("Token type BEFORE is: %d", currTokenType);
            getDecimalToken();
            // printf("Token type AFTER is: %d", currTokenType);
        }
    } else if (input[currIndex] >= '8' && input[currIndex] <= '9') {
        getDecimalToken();
    }
}

int getToken (int i) {
    prevTokenType = currTokenType; // currTokenType from previous token
    currTokenType = 0; // re-initialize currTokenType

    // printf("i: %d\n", i);
    while (isspace(input[i])) {
        printf("Came in isspace\n");
        i++;
    }
    currIndex = i; // setting global index to local incrementer (i)

    if (isalpha(input[i])) { // regex: [alpha followed by 0 or more occurrences of alphanum]
        getWordToken();
    } else if (input[i] == '0') { // if first character is a 0, then assume octal
        getOctalToken();
    } else if (isdigit(input[i])) {
        getDecimalToken();
    }

    return currTokenType;
}

// getting proper token type name from tokenType integer
char* getTokenTypeName (int tokenType) {
    switch (tokenType) {
        case 0:
            return NULL;
        case 1:
            return "WORD";
        case 2:
            return "DECIMAL";
        case 3:
            return "OCTAL";
        case 4:
            return "HEX";
        case 5:
            return "FLOAT";
        case 6:
            return "OPERATOR (TBD)";
        default:
            return NULL;
    }
}

int main (int argc, char **argv) {
    int argvlen = 0;
    int i = 0;
    int currPos = 0;
    int tokenCount = 0;

    if (argc == 1) { // error checking for invalid input
        printf("Error: invalid input\n");
        return EXIT_SUCCESS;
    }

    // initializing global variables
    argvlen = strlen(argv[1]);
    // printf("\nargvlen: %d", argvlen);
    input = (char*) malloc (argvlen + 1);
    strcpy(input, argv[1]);
    // printf("%s\n", input);
    headToken = NULL;
    currIndex = 0;
    // printf("%c", *currChar);

    /* Testing LL implentation (creation, printing, freeing) */
    // insertRearLL('4');
    // insertRearLL('5');
    // insertRearLL('6');
    // printLL();
    // freeLL();
    // if (headToken == NULL) {
    //     printf("Properly freed.");
    // }
    // printLL();
    /* Testing LL implentation (creation, printing, freeing) */

    while (currIndex < strlen(input) && input[currIndex] != '\0') {
        currPos = currIndex;
        if (getToken(currPos) != 0) { // got the current token and token type
            // debugging purposes
            tokenCount++;
            printf("Token #%d is of type %s ", tokenCount, getTokenTypeName(currTokenType));

            printLL();
            freeLL();
        } else {
            freeLL();
            currIndex = strlen(input);
            printf("Invalid character.\n");
        }
    }

    if (headToken != NULL && headToken->next != NULL) {
        printf("Exited WHILE and LL freed successfully. No wandering nodes either.\n");
    }

    return EXIT_SUCCESS;
}
