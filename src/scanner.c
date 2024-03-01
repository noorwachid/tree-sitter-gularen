#include "tree_sitter/parser.h"
#include <stdio.h>
#include <string.h>

enum TokenType {
	END,
	NEWLINE,
	NEWLINE_PLUS,
	HEAD3,
	HEAD2,
	HEAD1,
	FENCE_OPEN,
	FENCE_CLOSE,
	RAW,
	TEXT,
};

typedef struct {
	unsigned int indentLevel;
} Payload;

void* tree_sitter_gularen_external_scanner_create() {
	Payload* context = malloc(sizeof(Payload));
	context->indentLevel = 0;
	return context;
}

void tree_sitter_gularen_external_scanner_destroy(void *payload) {
	free(payload);
}

unsigned int tree_sitter_gularen_external_scanner_serialize(void* payload, char* buffer) {
	memcpy(buffer, payload, sizeof(Payload));
	return sizeof(Payload);
}

void tree_sitter_gularen_external_scanner_deserialize(void* payload, const char* buffer, unsigned int length) {
	memcpy(payload, buffer, length);
}


bool parse_newline(void* payload, TSLexer* lexer, const bool* valid_symbols) {

	return false;
}

bool parse_text(void* payload, TSLexer* lexer, const bool* valid_symbols) {

	return false;
}

bool tree_sitter_gularen_external_scanner_scan(void* payload, TSLexer* lexer, const bool* valid_symbols) {
	if (valid_symbols[END]) {
		if (lexer->eof(lexer)) {
			lexer->result_symbol = END;
			return true;
		}
	}

	if (valid_symbols[NEWLINE] || valid_symbols[NEWLINE_PLUS]) {
		if (lexer->lookahead == '\n') {
			unsigned int count = 0;
			while (!lexer->eof(lexer) && lexer->lookahead == '\n') {
				count += 1;
				lexer->advance(lexer, false);
			}

			if (count == 1) {
				lexer->result_symbol = NEWLINE;
				return true;
			}

			lexer->result_symbol = NEWLINE_PLUS;
			return true;
		}
	}

	if (valid_symbols[HEAD3] || valid_symbols[HEAD2] || valid_symbols[HEAD1]) {
		if (lexer->lookahead == '>') {
			lexer->advance(lexer, false); 

			if (lexer->lookahead == ' ') {
				lexer->result_symbol = HEAD1;
				return true;
			}

			if (lexer->lookahead == '>') {
				lexer->advance(lexer, false); 

				if (lexer->lookahead == ' ') {
					lexer->result_symbol = HEAD2;
					return true;
				}

				if (lexer->lookahead == '>') {
					lexer->advance(lexer, false); 

					if (lexer->lookahead == ' ') {
						lexer->result_symbol = HEAD3;
						return true;
					}
				}
			}
		}
	}

	if (valid_symbols[FENCE_OPEN]) {
		if (lexer->lookahead == '-') {
			lexer->advance(lexer, false); 

			if (lexer->lookahead == '-') {
				lexer->advance(lexer, false); 

				if (lexer->lookahead == '-') {
					lexer->advance(lexer, false); 
				}
			}
		}
	}

	if (valid_symbols[TEXT]) {

		while (!lexer->eof(lexer)) {
			switch (lexer->lookahead) {
				// fast case
				case ' ':
				case ',':
				case '.':

				case '0': case '1': case '2': case '3': case '4':
				case '5': case '6': case '7': case '8': case '9':

				case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
				case 'H': case 'I': case 'J': case 'K': case 'L': case 'M':
				case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': 
				case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':

				case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
				case 'h': case 'i': case 'j': case 'k': case 'l': case 'm':
				case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't': 
				case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
					lexer->advance(lexer, false);
					break;

				case '*':
				case '_':
				case '`':
				case '~':
				// case '<':
				// case '|':
				// case '{':
				// case '[':
				case '\n':
					lexer->result_symbol = TEXT;
					return true;
				
				default:
					lexer->advance(lexer, false);
					break;
			}
		}

	}

	return false;
}

