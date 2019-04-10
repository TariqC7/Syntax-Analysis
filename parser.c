/* Syntax Assignment */
/* Author: Tariq Cranston */

/* front.c - a lexical analyzer system for simple
arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
int lineNumber = 0;
int currentIndexCount;
size_t len = 0;
ssize_t read;
char * line = NULL;
FILE *in_fp, *fopen();

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
void term();
void factor();
void expr();
void error();
int getErrorChar();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define NEWLINE 101

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/******************************************************/
/* main driver */
int main(int argc, char *argv[]) {
  /* Open the input data file and process its contents */
  fp = fopen("front.in", "r");
  if (fp == NULL) {
    fprintf(stderr,"fopen() failed in file %s at line # %d", __FILE__,__LINE__);
    exit(EXIT_FAILURE);
  }
  while ((read = getline(&line, &len, fp)) != -1) {
    lineNumber += 1;
    currentIndexCount = 0;  
    getChar();
    do {
        lex();
        expr();
    } while (nextToken != EOF);
    printf("\n\n");
  }
}
/*****************************************************/
/* lookup - a function to lookup operators and parentheses
and return the token */
int lookup(char ch) {
  switch (ch) {
        case '(':
          addChar();
          nextToken = LEFT_PAREN;
          break;
        case ')':
          addChar();
          nextToken = RIGHT_PAREN;
          break;
        case '+':
          addChar();
          nextToken = ADD_OP;
          break;
        case '-':
          addChar();
          nextToken = SUB_OP;
          break;
        case '*':
          addChar();
          nextToken = MULT_OP;
          break;
        case '/':
          addChar();
          nextToken = DIV_OP;
          break;
      
        case '\n':
          addChar();
          nextToken = NEWLINE;
          break;

        default:
          addChar();
          nextToken = EOF;
          break;
  }
  return nextToken;
}
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar() {
  if (lexLen <= 98) {
      lexeme[lexLen++] = nextChar;
      lexeme[lexLen] = 0;
  }
  else
      printf("Error - lexeme is too long \n");
}
/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */
void getChar() {
  if (line[currentIndexCount] == '\n' || line[currentIndexCount] == '\0') {
    charClass = EOF;
  } else {
    nextChar = line[currentIndexCount];
    currentIndexCount += 1;
    if (isalpha(nextChar))
      charClass = LETTER;
    else if (isdigit(nextChar))
      charClass = DIGIT;
    else charClass = UNKNOWN;
  }
}
       
/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-
whitespace
character */
void getNonBlank() {
    while (isspace(nextChar))
    getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
              addChar();
              getChar();
        }
        nextToken = IDENT;
        break;
        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
        nextToken = INT_LIT;
        break;

        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
        
        /* NEWLINE */
        case NEWLINE:
          nextToken = NEWLINE;
          lexeme[0] = 'N';
          lexeme[1] = 'E';
          lexeme[2] = 'W';
          lexeme[3] = 'L';
          lexeme[4] = 'I';
          lexeme[5] = 'E';
    } /* End of switch */
  
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */

void expr() {
  printf("Enter <expr>\n");
 /* Parse the first term */
  term();
  while (nextToken == ADD_OP || nextToken == SUB_OP) {
    lex();
    term();
  }
  printf("Exit <expr>\n");
}  /* End of function expr */

void term() {
  printf("Enter <term>\n");
/* Parse the first factor */
  factor();
  while (nextToken == MULT_OP || nextToken == DIV_OP) {
    lex();
    factor();
  }
  printf("Exit <term>\n");
}  /* End of function term */
