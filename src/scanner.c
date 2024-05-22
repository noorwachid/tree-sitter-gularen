#include "tree_sitter/parser.h"
#include <stdio.h>
#include <string.h>

enum TokenType {
	NEWLINE,
	NEWLINE_PLUS,

	HEAD3,
	HEAD2,
	HEAD1,

	BULLET,
	INDEX,
	BLANK,
	X,

	FENCE_OPEN,
	FENCE_CLOSE,
	CODE_BLOCK_LABEL,
	CODE_BLOCK_LINE,

	BACKTICK,
	CODE_INLINE_CONTENT,

	EXCLAMATION,
	QUESTION,
	CARET,
	AMPERSAND,

	SQUARE_OPEN,
	SQUARE_CLOSE,
	RESOURCE,

	PAREN_OPEN,
	PAREN_CLOSE,
	LABEL,

	ACCOUNT_TAG,
	HASH_TAG,

	TEXT,
};

typedef struct {
	int indent_level;
	int fence_dash_count;
	bool indent_opening;
	bool code_inline;
	bool resource;
	bool label;
	bool previous_alphanumeric;
} Context;

void* tree_sitter_gularen_external_scanner_create() {
	Context* context = malloc(sizeof(Context));
	context->indent_level = 0;
	context->indent_opening = false;
	context->fence_dash_count = 0;
	context->code_inline = false;
	context->resource = false;
	context->label = false;
	context->previous_alphanumeric = false;
	return context;
}

void tree_sitter_gularen_external_scanner_destroy(void *payload) {
	free(payload);
}

unsigned int tree_sitter_gularen_external_scanner_serialize(void* payload, char* buffer) {
	memcpy(buffer, payload, sizeof(Context));
	return sizeof(Context);
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
	Context* context = payload;

	if (valid_symbols[NEWLINE] || valid_symbols[NEWLINE_PLUS]) {

		// if (lexer->eof(lexer)) {
		// 	lexer->result_symbol = NEWLINE;
		// 	return true;
		// }

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

	if (lexer->get_column(lexer) == 0) {
		while (!lexer->eof(lexer) && lexer->lookahead == '\t') {
			lexer->advance(lexer, true);
		}

		if (valid_symbols[HEAD3] || valid_symbols[HEAD2] || valid_symbols[HEAD1] || valid_symbols[TEXT]) {
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

		if (valid_symbols[INDEX] || valid_symbols[TEXT]) {
			if (lexer->lookahead >= '0' && lexer->lookahead <= '9') {
				while (!lexer->eof(lexer) && lexer->lookahead >= '0' && lexer->lookahead <= '9') {
					lexer->advance(lexer, false);
				}

				if (!lexer->eof(lexer) && lexer->lookahead == '.') {
					lexer->advance(lexer, false);

					if (!lexer->eof(lexer) && lexer->lookahead == ' ') {
						lexer->advance(lexer, false);
						lexer->result_symbol = INDEX;
						return true;
					}
				}
			}
		}

		if (valid_symbols[AMPERSAND] || valid_symbols[TEXT]) {
			if (lexer->lookahead == '&') {
				lexer->advance(lexer, false);

				if (!lexer->eof(lexer)) {
					if (lexer->lookahead == ' ') {
						lexer->result_symbol = AMPERSAND;
						return true;
					}

					if (lexer->lookahead == '[') {
						lexer->result_symbol = AMPERSAND;
						return true;
					}
				}

				lexer->result_symbol = TEXT;
				return true;
			}
		}

		if (valid_symbols[FENCE_OPEN] || valid_symbols[BULLET] || valid_symbols[TEXT]) {
			if (lexer->lookahead == '-') {
				lexer->advance(lexer, false);

				if (lexer->lookahead == ' ') {
					lexer->advance(lexer, false);
					lexer->result_symbol = BULLET;
					return true;
				}

				if (lexer->lookahead == '-') {
					lexer->advance(lexer, false);

					if (lexer->lookahead == '-') {
						lexer->advance(lexer, false);
						int dashCount = 3;
						while (!lexer->eof(lexer) && lexer->lookahead == '-') {
							dashCount += 1;
							lexer->advance(lexer, false);
						}

						if (lexer->lookahead == ' ') {
							lexer->advance(lexer, false);
							lexer->result_symbol = FENCE_OPEN;
							context->fence_dash_count = dashCount;
							return true;
						}

						if (lexer->lookahead == '\n') {
							lexer->result_symbol = FENCE_OPEN;
							context->fence_dash_count = dashCount;
							return true;
						}
					}
				}
			}
		}

		if (valid_symbols[CODE_BLOCK_LINE] || valid_symbols[FENCE_CLOSE]) {
			if (valid_symbols[FENCE_CLOSE] && lexer->eof(lexer)) {
				lexer->result_symbol = FENCE_CLOSE;
				context->fence_dash_count = 0;
				return true;
			}

			if (lexer->lookahead == '-') {
				int dashCount = 0;
				while (!lexer->eof(lexer) && lexer->lookahead == '-') {
					dashCount += 1;
					lexer->advance(lexer, false);
				}

				if (lexer->eof(lexer) || lexer->lookahead == '\n') {
					while (!lexer->eof(lexer) && lexer->lookahead == '\n')  {
						lexer->advance(lexer, false);
					}

					if (dashCount == context->fence_dash_count) {
						lexer->result_symbol = FENCE_CLOSE;
						context->fence_dash_count = 0;
						return true;
					}
				}
			}

			while (!lexer->eof(lexer) && lexer->lookahead != '\n') {
				lexer->advance(lexer, false);
			}

			if (lexer->lookahead == '\n') {
				lexer->advance(lexer, false);
			}

			lexer->result_symbol = CODE_BLOCK_LINE;
			return true;
		}
	}

	if (valid_symbols[CODE_BLOCK_LABEL]) {
		if (context->fence_dash_count != 0) {
			while (!lexer->eof(lexer) && lexer->lookahead != '\n')  {
				lexer->advance(lexer, false);
			}
			lexer->result_symbol = CODE_BLOCK_LABEL;
			return true;
		}
	}

	if (valid_symbols[CODE_INLINE_CONTENT] || valid_symbols[BACKTICK]) {
		if (context->code_inline) {
			if (lexer->lookahead == '`') {
				lexer->advance(lexer, false);
				lexer->result_symbol = BACKTICK;
				context->code_inline = false;
				return true;
			}

			while (!lexer->eof(lexer) && lexer->lookahead != '`') {
				lexer->advance(lexer, false);
			}

			lexer->result_symbol = CODE_INLINE_CONTENT;
			return true;
		}

		if (lexer->lookahead == '`') {
			lexer->advance(lexer, false);
			lexer->result_symbol = BACKTICK;
			context->code_inline = true;
			return true;
		}
	}

	if (valid_symbols[EXCLAMATION] || valid_symbols[TEXT]) {
		if (lexer->lookahead == '!') {
			lexer->advance(lexer, false);
			lexer->mark_end(lexer);
			if (!lexer->eof(lexer)) {
				if (lexer->lookahead == '[') {
					lexer->result_symbol = EXCLAMATION;
					return true;
				}
			}
			lexer->result_symbol = TEXT;
			return true;
		}
	}

	if (valid_symbols[QUESTION] || valid_symbols[TEXT]) {
		if (lexer->lookahead == '?') {
			lexer->advance(lexer, false);
			lexer->mark_end(lexer);
			if (!lexer->eof(lexer)) {
				if (lexer->lookahead == '[') {
					lexer->result_symbol = QUESTION;
					return true;
				}
			}
			lexer->result_symbol = TEXT;
			return true;
		}
	}

	if (valid_symbols[CARET] || valid_symbols[TEXT]) {
		if (lexer->lookahead == '^') {
			lexer->advance(lexer, false);
			lexer->mark_end(lexer);
			if (!lexer->eof(lexer)) {
				if (lexer->lookahead == '[') {
					lexer->result_symbol = CARET;
					return true;
				}
			}
			lexer->result_symbol = TEXT;
			return true;
		}
	}

	if (valid_symbols[AMPERSAND] || valid_symbols[TEXT]) {
		if (lexer->lookahead == '&') {
			lexer->advance(lexer, false);

			if (!lexer->eof(lexer)) {
				if (lexer->lookahead == '[') {
					lexer->result_symbol = AMPERSAND;
					return true;
				}
			}

			lexer->result_symbol = TEXT;
			return true;
		}
	}

	if (valid_symbols[SQUARE_OPEN]) {
		if (lexer->lookahead == '[') {
			lexer->advance(lexer, false);
			lexer->result_symbol = SQUARE_OPEN;
			context->resource = true;
			return true;
		}
	}

	if (valid_symbols[RESOURCE] || valid_symbols[BLANK] || valid_symbols[X] || valid_symbols[SQUARE_CLOSE]) {
		if (context->resource) {
			if (lexer->lookahead == ' ') {
				lexer->advance(lexer, false);
				if (lexer->lookahead == ']') {
					lexer->result_symbol = BLANK;
					return true;
				}
			}
			if (lexer->lookahead == 'x') {
				lexer->advance(lexer, false);
				if (lexer->lookahead == ']') {
					lexer->result_symbol = X;
					return true;
				}
			}

			if (lexer->lookahead == ']') {
				lexer->advance(lexer, false);
				lexer->result_symbol = SQUARE_CLOSE;
				context->resource = false;
				return true;
			}

			while (!lexer->eof(lexer) && lexer->lookahead != ']') {
				lexer->advance(lexer, false);
			}

			lexer->mark_end(lexer);
			if (!lexer->eof(lexer)) {
				lexer->advance(lexer, false);
				if (lexer->lookahead == '(') {
					context->label = true;
				}
			}

			lexer->result_symbol = RESOURCE;
			return true;
		}
	}

	if (valid_symbols[PAREN_OPEN]) {
		if (lexer->lookahead == '(') {
			lexer->advance(lexer, false);
			lexer->result_symbol = PAREN_OPEN;
			return true;
		}
	}

	if (valid_symbols[LABEL] || valid_symbols[PAREN_CLOSE]) {
		if (context->label) {
			if (lexer->lookahead == ')') {
				lexer->advance(lexer, false);
				lexer->result_symbol = PAREN_CLOSE;
				context->label = false;
				return true;
			}

			while (!lexer->eof(lexer) && lexer->lookahead != ')') {
				lexer->advance(lexer, false);
			}

			lexer->result_symbol = LABEL;
			return true;
		}
	}

	if (valid_symbols[ACCOUNT_TAG] || valid_symbols[TEXT]) {
		if (lexer->lookahead == '@') {
			lexer->advance(lexer, false);
			while (!lexer->eof(lexer) && (
				(lexer->lookahead >= '0' && lexer->lookahead <= '9') ||
				(lexer->lookahead >= 'A' && lexer->lookahead <= 'Z') ||
				(lexer->lookahead >= 'a' && lexer->lookahead <= 'z') ||
				lexer->lookahead == '_')
			) {
				lexer->advance(lexer, false);
			}
			lexer->result_symbol = ACCOUNT_TAG;
			return true;
		}
	}

	if (valid_symbols[HASH_TAG] || valid_symbols[TEXT]) {
		if (lexer->lookahead == '#') {
			lexer->advance(lexer, false);
			while (!lexer->eof(lexer) && (
				(lexer->lookahead >= '0' && lexer->lookahead <= '9') ||
				(lexer->lookahead >= 'A' && lexer->lookahead <= 'Z') ||
				(lexer->lookahead >= 'a' && lexer->lookahead <= 'z') ||
				lexer->lookahead == '_')
			) {
				lexer->advance(lexer, false);
			}
			lexer->result_symbol = HASH_TAG;
			return true;
		}
	}

	if (valid_symbols[TEXT]) {

		switch (lexer->lookahead) {
			case '*':
			case '/':
			case '_':
			case '`':
			case '~':
			case '<':
			case '{':
			case '[':
			case '!':
			case '?':
			case '^':
			case '&':
			case '=':
			case '+':
			case '-':
			case '"':
			case '\\':
			case '\n':
				return false;
				break;
		}

		while (!lexer->eof(lexer)) {
			switch (lexer->lookahead) {
				// fast case
				case ' ':
				case ',':
				case '.':
					context->previous_alphanumeric = false;
					lexer->advance(lexer, false);
					break;

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
					context->previous_alphanumeric = true;
					lexer->advance(lexer, false);
					break;

				case '*':
				case '/':
				case '_':
				case '=':
				case '`':
				case '~':
				case '<':
				case '{':
				case '[':
				case '!':
				case '?':
				case '^':
				case '&':
				case '-':
				case '+':
				case '"':
				case '\\':
				case '\n':
					lexer->result_symbol = TEXT;
					return true;

				case '#':
				case '@':
					if (!context->previous_alphanumeric) {
						lexer->result_symbol = TEXT;
						return true;
					}
					lexer->advance(lexer, false);
					break;

				default:
					context->previous_alphanumeric = false;
					lexer->advance(lexer, false);
					break;
			}
		}

	}

	return false;
}

