#include <string>
#include <optional>
#include <iostream>

#ifndef Token_H
#define Token_H

typedef struct {
    int type;
    std::optional<std::string> value {};
} Token;


std::string token_to_string(Token token) {
    std::string result = "{" + std::to_string(token.type) + " : ";
    
    if (token.value) {
        result += *token.value;
    } else {
        result += "'none'";
    }

    result += "}";
    return result;
}

#define NE		 1		/* '!='  */
#define NOT		 2		/* '!'   */
#define MODE	 3		/* '%='  */
#define MOD		 4		/* '%'   */
#define DAND	 5		/* '&&' (marks beginning of binaries) */
#define ANDE	 6		/* '&='  */
#define AND		 7		/* '&' - And*/
#define ORB		 8		/* (     */
#define CRB		 9		/* )     */
#define STARE	10		/* '*='  */
#define MUL 	11		/* '*' - multiply/
#define INC		12		/* '++'  */
#define ADDE	13		/* '+='  */
#define ADD		14		/* '+'   */
#define COMMA	15		/* ,     */
#define DEC		16		/* '--'  */
#define SUBE	17		/* '-='  */
#define DEREF	18		/* '->'  */
#define SUB		19		/* '-' - subtract & negate */
#define ELIPSE	20		/* '...' Elipse for args */
#define DOT		21		/* '.'   */
#define DIVE	22		/* '/='  */
#define DIV		23		/* '/'   */
#define COLON	24		/* :     */
#define SEMI	25		/* ;     */
#define SHLE 26		/* '<<=' */
#define SHL		27		/* '<<'  */
#define LE		28		/* '<='  */
#define LT		29		/* '<'   */
#define EQ		30		/* '=='  */
#define ASSIGN	31		/* '='   */
#define SHRE 32		/* '>>=' */
#define SHR		33		/* '>>'  */
#define GE		34		/* '>='  */
#define GT		35		/* '>'   */
#define QUEST	36		/* '?' (marks ending of binaries) */
#define OSB		37		/* [     */
#define CSB		38		/* ]     */
#define XORE	39		/* '^='  */
#define XOR		40		/* '^'   */
#define BREAK	41		/* 'break' statement */
#define CASE	42		/* 'case' statement */
#define CHAR	43		/* 'char' */
#define INT8	44		/* 'int8' */
#define CONTIN	45		/* 'continue' statement */
#define DEFAULT	46		/* 'default' statement */
#define INT16   47		/* 'int16' */
#define ELSE	48		/* 'else' modifier */
#define EXTERN	49		/* 'extern' */
#define NOP	50
#define FOR		51		/* 'for' statement */
#define GOTO	52		/* 'goto' statement */
#define UINT8	53		/* Unsiged int8 */
#define INT32	54	    /* 'int32'   */
#define IF		55		/* 'if' statement */
#define INT 	56		/* 'interrupt'   */
#define BYTEOUT	57		/* 'Byte out' */
#define RETURN	58		/* 'return' statement */
#define HEADER	59      /* 'header*/
#define SIZEOF  60		/* 'sizeof' */
#define FUNC    61		/* 'function' */
#define STRUC   62		/* 'struct' */
#define SWITCH	63		/* 'switch' statement */
#define UINT16	64		/* 'uint16' */
#define TYPEDEF 65		/* 'typedef' */
#define REG 	66		/* 'register' */
#define BYTEIN	67		/* 'Byte in' */
#define VOIDD	68		/* 'void' */
#define WHILE	69		/* 'while' statement */
#define OCB		70		/* {    */
#define DOR		71		/* '||' */
#define ORE		72		/* '|=' */
#define OR		73		/* '|'  */
#define CCB		74		/* }    */
#define COM		75		/* '~'  */
#define INT64   76      /* 'int64'    */
#define UINT32  77      /* 'uint32'   */
#define UINT64  78      /* 'uint64'   */
#define HEX 	79
#define DECIM 	80
#define OCTAL 	81
#define BINARY 	82
#define IDENTIFIER 83
#define STRING 84
#define CHARLIT 85
#define DECIMFLOAT 86

#define EOT 100
#if NULL
	static char *tokens[] = {

	/* token         number  priority  optype  ASCII */
		"",          /* 00       0       0        0  */
		"!=",        /* 01       9       2        33 */
		"!",         /* 02       0       1           */
		"%=",        /* 03       2       3        37 */
		"%",         /* 04       13      2           */
		"&&",        /* 05       5       4        38 */
		"&=",        /* 06       2       3           */
		"&",         /* 07       8       2           */
		"(",         /* 08       0       0        40 */
		")",         /* 09       0       0        41 */
		"*=",        /* 10       2       3        42 */
		"*",         /* 11       13      2           */
		"++",        /* 12       15      0        43 */
		"+=",        /* 13       2       3           */
		"+",         /* 14       12      2           */
		",",         /* 15       0       0        44 */
		"--",        /* 16       15      0        45 */
		"-=",        /* 17       2       3           */
		"->",        /* 18       15      2           */
		"-",         /* 19       12      2           */
		"...",       /* 20       0       0        46 */
		".",         /* 21       15      2        46 */
		"/=",        /* 22       2       3        47 */
		"/",         /* 23       13      2           */
		":",         /* 24       0       0        58 */
		";",         /* 25       0       0        59 */
		"<<=",       /* 26       2       3        60 */
		"<<",        /* 27       11      2           */
		"<=",        /* 28       10      2           */
		"<",         /* 29       10      2           */
		"==",        /* 30       9       2        61 */
		"=",         /* 31       2       3           */
		">>=",       /* 32       2       3        62 */
		">>",        /* 33       11      2           */
		">=",        /* 34       10      2           */
		">",         /* 35       10      2           */
		"?",         /* 36       3       4        63 */
		"[",         /* 37       0       0        91 */
		"]",         /* 38       0       0        93 */
		"^=",        /* 39       2       3        94 */
		"^",         /* 40       7       2           */
		"break",     /* 41       0       0        98 */
		"case",      /* 42       0       0        99 */
		"char",      /* 43       0       0           */
        "int8",      /* 44       0       0           */
		"continue",  /* 45       0       0           */
		"default",   /* 46       0       0        100*/
        "int16",     /* 47       0       0           */
		"else",      /* 48       0       0        101*/
		"extern",    /* 49       0       0           */
        "nop",
		"for",       /* 51       0       0           */
		"goto",      /* 52       0       0        103*/
		"int32",     /* 54       0       0           */
		"if",        /* 55       0       0           */
		"int",       /* 56       0       0        108*/
		"byout",     /* 57       0       0        114*/
		"return",    /* 58       0       0           */
        "HEADER",    /* 59       0       0           */
		"sizeof",    /* 60       0       1           */
		"function",  /* 61       0       0           */
		"struct",    /* 62       0       0           */
		"switch",    /* 63       0       0           */
		"short",     /* 64       0       0           */
		"typedef",   /* 65       0       0        116*/
		"reg",       /* 66       0       0        117*/
		"byin",      /* 67       0       0           */
		"void",      /* 68       0       0        118*/
		"while",     /* 69       0       0        119*/
		"{",         /* 70       0       0        123*/
		"||",        /* 71       4       4        124*/
		"|=",        /* 72       2       3           */
		"|",         /* 73       6       2           */
		"}",         /* 74       0       0        125*/
		"~",         /* 75       ?       1        126*/
        "int64",     /* 76       0       0           */
		0,
        0,
        0,
        0,
        0,
        0,
        };           /* End of table */
#endif

#endif // Token_H