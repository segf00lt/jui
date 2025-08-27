#ifndef _LEXER_H
#define _LEXER_H

#define CTOKEN_KEYWORDS \
  X(U8, "u8") \
  X(U16, "u16") \
  X(U32, "u32") \
  X(U64, "u64") \
  X(S8, "s8") \
  X(S16, "s16") \
  X(S32, "s32") \
  X(S64, "s64") \
  X(INT, "int") \
  X(LONG, "long") \
  X(UNSIGNED, "unsigned") \
  X(SIGNED, "signed") \
  X(CHAR, "char") \
  X(SHORT, "short") \
  X(FLOAT, "float") \
  X(DOUBLE, "double") \
  X(VOID, "void") \
  X(FUNC, "func") \
  X(STRUCT, "struct") \
  X(UNION, "union") \
  X(ENUM, "enum") \
  X(TYPEDEF, "typedef") \
  X(INLINE, "inline") \
  X(FORCE_INLINE, "force_inline") \
  X(STATIC, "static") \
  X(EXTERN, "extern") \
  X(FOR, "for") \
  X(WHILE, "while") \
  X(IF, "if") \
  X(ELSE, "else") \
  X(SWITCH, "switch") \
  X(CASE, "case") \
  X(DEFAULT, "default") \
  X(GOTO, "goto") \
  X(BREAK, "break") \
  X(CONTINUE, "continue") \
  X(RETURN, "return") \
  X(SIZEOF, "sizeof") \

#define CTOKEN_OPERATORS \
  X(EQUAL_EQUAL, "==") \
  X(NOT_EQUAL, "!=") \
  X(LESS_OR_EQUAL, "<=") \
  X(GREATER_OR_EQUAL, ">=") \
  X(PLUS_EQUAL, "+=") \
  X(MINUS_EQUAL, "-=") \
  X(TIMES_EQUAL, "*=") \
  X(DIV_EQUAL, "/=") \
  X(MOD_EQUAL, "%=") \
  X(OR_EQUAL, "|=") \
  X(AND_EQUAL, "&=") \
  X(XOR_EQUAL, "^=") \

typedef enum Ctoken_kind {
  CTOKEN_KEYWORDS_BEGIN = 256,
#define X(k, s) CTOKEN_##k,
  CTOKEN_KEYWORDS
#undef X
    CTOKEN_KEYWORDS_END,

  CTOKEN_COMMENT_SINGLE_LINE,
  CTOKEN_COMMENT_MULTI_LINE,
  CTOKEN_PREPROCESSOR_DIRECTIVE,

  CTOKEN_OPERATORS_BEGIN,
#define X(k, s) CTOKEN_##k,
  CTOKEN_OPERATORS
#undef X
    CTOKEN_OPERATORS_END,

  CTOKEN_IDENT,
  CTOKEN_NUMBER,
  CTOKEN_STRING,
  CTOKEN_CHARACTER,

  CTOKEN_COUNT,

} Ctoken_kind;

read_only Str8 ctoken_keywords[CTOKEN_KEYWORDS_END - CTOKEN_KEYWORDS_BEGIN-1] = {
#define X(k, s) str8_lit(s),
  CTOKEN_KEYWORDS
#undef X
};

read_only Str8 ctoken_operators[CTOKEN_OPERATORS_END - CTOKEN_OPERATORS_BEGIN-1] = {
#define X(k, s) str8_lit(s),
  CTOKEN_OPERATORS
#undef X
};

read_only Str8 ctoken_chars = str8_lit("+=-)(*&^%$!~><.,?/:;'\"][{}\\|#");

typedef struct Ctoken Ctoken;
struct Ctoken {
  Str8 str;
  Ctoken_kind kind;
  s32 line;
  s32 col;
  s32 comment_end_line;
  s32 comment_end_col;
};

DECL_ARR_TYPE_NAME(Ctoken, Ctoken_arr);
DECL_SLICE_TYPE_NAME(Ctoken, Ctoken_slice);


Ctoken_slice ctokenize(Arena *arena, Str8 src);

#endif
