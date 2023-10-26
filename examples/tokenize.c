  
#define DT_IMPLEMENTATION
#include "../dt.h"

#include <string.h>

#define _STR(X) #X
#define STR(X) _STR(X)

#define X_OP_CHARS                                                             \
  X(+)                                                                         \
  X(-)                                                                         \
  X(*)                                                                         \
  X(/)                                                                         \
  X(%)                                                                         \
  X(&)                                                                         \
  X(|)                                                                         \
  X(^)                                                                         \
  X(~)                                                                         \
  X(!)                                                                         \
  X(=)                                                                         \
  X(<)                                                                         \
  X(>)                                                                         \
  X(?)                                                                         \
  X( :)                                                                        \
  X(#)                                                                         \
  X(.) X_COMMA

#define X(V) #V
#define X_COMMA ","
const char *_op_chars = X_OP_CHARS "\0";
#undef X

char *strndup(const char *s, size_t n)
{
  size_t len = strnlen(s, n);
  char *new  = (char *) malloc(len + 1);
  if (new == NULL)
    return NULL;
  new[len] = '\0';
  return (char *) memcpy(new, s, len);
}

bool ischrcls(char c, const char *cls)
{
  while (*cls)
  {
    if (c == *cls)
    {
      return true;
    }
    cls++;
  }
  return false;
}

size_t chrclslen(const char *s, const char *cls)
{
  size_t count = 0;

  while (*s && ischrcls(*s, cls))
  {
    count++;
    s++;
  }

  return count;
}

const char *next_token(const char *s, size_t *start, size_t *len)
{
  size_t i = 0;
  *len     = 0;

  while (s[i])
  {
    if (s[i] == '\n')
    {
      *start = i++;
      *len   = 1;
      return s + i;
    }
    if (strncmp(s + i, "//", 2) == 0)
    {
      *start = i;
      *len   = 2;
      while (s[i] != '\n')
      {
        i++;
      }
      *len = i - *start;
      return s + i;
    }
    if (strncmp(s + i, "/*", 2) == 0)
    {
      *start = i;
      *len   = 2;
      i += 2;
      while (s[i] && !(s[i] == '*' && s[i + 1] == '/'))
      {
        if (s[i] == '\n')
        {
          (*len)++;
        }
        i++;
      }
      if (s[i] == '*' && s[i + 1] == '/')
      {
        i += 2;
        *len += 2;
      }
      return s + i;
    }
    if (isspace(s[i]))
    {
      i++;
      continue;
    }
    if (s[i] == '\'')
    {
      *start = i;
      i++;
      if (s[i] == '\\')
        i++;
      i++;
      if (s[i] == '\'')
        i++;
      *len = i - *start;
      return s + i;
    }
    else if (s[i] == '"')
    {
      *start = i;
      i++;
      while (s[i] && (s[i] != '"' || (s[i] == '"' && s[i - 1] == '\\')))
      {
        i++;
      }
      if (s[i] == '"')
        i++;
      *len = i - *start;
      return s + i;
    }
    else if (isalpha(s[i]) || s[i] == '_')
    {
      *start = i;
      while (s[i] && (isalnum(s[i]) || s[i] == '_'))
      {
        i++;
      }
      *len = i - *start;
      return s + i;
    }
    else if (s[i] == '-' && (isdigit(s[i + 1]) || s[i + 1] == '.'))
    {
      *start = i;
      i++;
      while (s[i] && (isdigit(s[i]) || s[i] == '.'))
      {
        i++;
      }
      while (s[i] == 'e' || isdigit(s[i]))
      {
        i++;
      }
      *len = i - *start;
      return s + i;
    }
    else if (isdigit(s[i]))
    {
      *start = i;
      while (s[i] && (isdigit(s[i]) || s[i] == '.'))
      {
        i++;
      }
      while (s[i] == 'e' || isdigit(s[i]))
      {
        i++;
      }
      *len = i - *start;
      return s + i;
    }
    else if (ischrcls(s[i], _op_chars))
    {
      *start = i;
      *len   = 1;
      while (ischrcls(s[i], _op_chars))
      {
        i++;
      }
      *len = i - *start;
      return s + i;
    }
    else
    {
      *start = i++;
      *len   = 1;
      return s + i;
    }
  }
  return s + i;
}

typedef enum
{
  TT_NEWLINE,    // \n
  TT_CHAR,       // Char token, e.g. 'h'
  TT_STRING,     // String token, e.g. "hello"
  TT_IDENTIFIER, // Identifier token, e.g. variable names
  TT_NUMBER,     // Number token, e.g. 123, 4.56, -78.9
  TT_OPERATOR,   // Operator token, e.g. +, -, *, /, &&, ...
  TT_LPAREN,     // (
  TT_RPAREN,     // )
  TT_LBRACE,     // {
  TT_RBRACE,     // }
  TT_LBRACK,     // [
  TT_RBRACK,     // ]
  TT_COMMA,
  TT_SEMICOLON,
  TT_UNKNOWN // Unknown token type, this will help handle unexpected cases
} token_type_t;

typedef struct
{
  token_type_t type;
  char *value;
  size_t line, col;
} token_t;

token_t **tokenize(const char *s)
{
  token_t **tokens = NULL;

  size_t start;
  size_t len;

  size_t line = 1, col = 1;
  while (s && *s)
  {
    const char *next = next_token(s, &start, &len);
    if (len)
    {
      token_t *token = (token_t *) malloc(sizeof(token_t));

      token->value = strndup(s + start, len);
      token->line  = line;
      token->col   = col;
      col += len;

      if (s[start] == '\n' || strncmp(s + start, "//", 2) == 0 ||
          strncmp(s + start, "/*", 2) == 0)
      {
        for (size_t j = 0; j < len; j++)
        {
          if (s[start + j] == '\n')
          {
            line += s[start + j] == '\n';
          }
        }
        col = 1;
        s   = next;
        free(token);
        continue;
      }

      if (s[start] == '\'')
      {
        token->type = TT_CHAR;
        free(token->value);
        token->value = strndup(s + start + 1, len - 2);
      }
      else if (s[start] == '"')
      {
        token->type = TT_STRING;
        free(token->value);
        token->value = strndup(s + start + 1, len - 2);
      }
      else if (isalpha(s[start]) || s[start] == '_')
        token->type = TT_IDENTIFIER;
      else if (isdigit(s[start]) ||
               (s[start] == '-' &&
                (isdigit(s[start + 1]) || s[start + 1] == '.')))
        token->type = TT_NUMBER;
      else if (ischrcls(s[start], _op_chars))
        token->type = TT_OPERATOR;
      else if (s[start] == '(')
        token->type = TT_LPAREN;
      else if (s[start] == ')')
        token->type = TT_RPAREN;
      else if (s[start] == '{')
        token->type = TT_LBRACE;
      else if (s[start] == '}')
        token->type = TT_RBRACE;
      else if (s[start] == '[')
        token->type = TT_LBRACK;
      else if (s[start] == ']')
        token->type = TT_RBRACK;
      else if (s[start] == ',')
        token->type = TT_COMMA;
      else if (s[start] == ';')
        token->type = TT_SEMICOLON;
      else
        token->type = TT_UNKNOWN;

      dt_arradd(tokens, token);
    }
    s = next;
  }

  return tokens;
}


const char *test =
  "#include <stdio.h>\n"
  "\n"
  "// Preprocessor Macro\n"
  "#define GREETING \"Hello, World!\" // my comment\n"
  "\n"
  "// Global variable\n"
  "int globalVar = 10;\n"
  "\n"
  "// Enumeration\n"
  "typedef enum { SUN, MON, TUE, WED, THU, FRI, SAT } Weekday;\n"
  "\n"
  "// Structure\n"
  "typedef struct {\n"
  "    int x;\n"
  "    int y;\n"
  "} Point;\n"
  "\n"
  "// Union\n"
  "union Data {\n"
  "    int integer;\n"
  "    float floating;\n"
  "};\n"
  "\n"
  "// Function declarations\n"
  "void printGreeting(void);\n"
  "int add(int a, int b);\n"
  "Weekday nextDay(Weekday currentDay);\n"
  "\n"
  "// Main function\n"
  "int main() {\n"
  "    printf(\"%s\\n\", GREETING);\n"
  "\n"
  "    int localVar = 20;\n"
  "    printf(\"Sum: %d\\n\", add(globalVar, localVar));\n"
  "\n"
  "    Weekday today = FRI;\n"
  "    printf(\"Today is %d, Tomorrow is %d\\n\", today, nextDay(today));\n"
  "\n"
  "    Point p = {5, 10};\n"
  "    printf(\"Point: (%d, %d)\\n\", p.x, p.y);\n"
  "\n"
  "    union Data data;\n"
  "    data.integer = 42;\n"
  "    printf(\"Union integer data: %d\\n\", data.integer);\n"
  "\n"
  "    return 0;\n"
  "}\n"
  "\n"
  "// Function definitions\n"
  "void printGreeting(void) {\n"
  "    printf(\"%s\\n\", GREETING);\n"
  "}\n"
  "\n"
  "int add(int a, int b) {\n"
  "    return a + b;\n"
  "}\n"
  "\n"
  "Weekday nextDay(Weekday currentDay) {\n"
  "    return '0' + (currentDay + 1) % 7; // my other comment\n"
  "};\n";
  
int main(void)
{
  token_t **tokens = tokenize(test);
  for (int i = 0; i < dt_arrlenu(tokens); ++i)
  {
    printf("%llu:%llu\t%s     \t%s\n", tokens[i]->line, tokens[i]->col,
           tokens[i]->type == TT_NEWLINE      ? "NEWLINE"
           : tokens[i]->type == TT_CHAR       ? "CHAR"
           : tokens[i]->type == TT_STRING     ? "STRING"
           : tokens[i]->type == TT_IDENTIFIER ? "IDENTIFIER"
           : tokens[i]->type == TT_NUMBER     ? "NUMBER"
           : tokens[i]->type == TT_OPERATOR   ? "OPERATOR"
           : tokens[i]->type == TT_LPAREN     ? "LPAREN"
           : tokens[i]->type == TT_RPAREN     ? "RPAREN"
           : tokens[i]->type == TT_LBRACE     ? "LBRACE"
           : tokens[i]->type == TT_RBRACE     ? "RBRACE"
           : tokens[i]->type == TT_LBRACK     ? "LBRACK"
           : tokens[i]->type == TT_RBRACK     ? "RBRACK"
           : tokens[i]->type == TT_COMMA      ? "COMMA"
           : tokens[i]->type == TT_SEMICOLON  ? "SEMICOLON"
                                              : "UNKNOWN",
           tokens[i]->value);
  }
}