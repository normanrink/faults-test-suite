
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint64_t is_digit(uint64_t) __attribute__((always_inline));
uint64_t is_digit(uint64_t c) {
  if ((c >= (long)'0') && (c <= (long)'9')) {
    return 1L;
  }
   return 0L;
}

uint64_t is_white(uint64_t) __attribute__((always_inline));
uint64_t is_white(uint64_t c) {
  switch (c) {
  case (long)' ':
  case (long)'\t':
  case (long)'\n':
      return 1L;
  default:
      return 0L;
  }
}


typedef struct {
  enum {
    T_LPAREN,
    T_RPAREN,
    T_PLUS,
    T_MINUS,
    T_ASTERISK,
    T_SLASH,
    T_LITERAL,
    T_EOF,
  } kind;

  uint64_t value;
} Token;

Token *cur;
uint64_t *buffer;

void lex() __attribute__((noinline));
void lex() {
  while (is_white(*buffer)) {
    ++buffer;
  }

  if (is_digit(*buffer)) {
    uint64_t val = 0;
    while(is_digit(*buffer)) {
      val *= 10L;
      val += (*buffer) - (uint64_t)'0';
      ++buffer;
    }
    cur->kind = T_LITERAL;
    cur->value = val;
    return;
  }

  switch(*buffer) {
  case (uint64_t)'(':
    cur->kind = T_LPAREN;
    break;
  case (uint64_t)')':
    cur->kind = T_RPAREN;
    break;
  case (uint64_t)'+':
    cur->kind = T_PLUS;
    break;
  case (uint64_t)'-':
    cur->kind = T_MINUS;
    break;
  case (uint64_t)'*':
    cur->kind = T_ASTERISK;
    break;
  case (uint64_t)'/':
    cur->kind = T_SLASH;
    break;
  case (uint64_t)'\0':
    cur->kind = T_EOF;
    break;
  default:
    exit(-1);
  }
  ++buffer;
}

/* Grammar:
 *
 * expr := term
 *
 * term := PLUS factor factor
 *         | MINUS factor factor
 *         | factor
 *
 * factor := ASTERISK unary unary
 *           | SLASH unary unary
 *           | unary
 *
 * unary := paren
 *          | MINUS unary
 *          | LITERAL
 *
 * paren := LPAREN expr RPAREN
 *
 */

typedef struct _Node{
  enum {
    N_EXPR,
    N_TERM,
    N_FACTOR,
    N_UNARY,
    N_PAREN,
  } kind;

  union {
    struct {
      struct _Node *term;
    } expr;

    struct {
      enum {
        TRM_FACTOR,
        TRM_ARITH
      } kind;

      union {
        struct _Node *factor;
        struct {
          Token op;
          struct _Node *f0;
          struct _Node *f1;
        } arith;
      } v;
    } term;

    struct {
      enum {
        FAC_UNARY,
        FAC_ARITH
      } kind;

      union {
        struct _Node *unary;
        struct {
          Token op;
          struct _Node *u0;
          struct _Node *u1;
        } arith;
      } v;
    } factor;

    struct {
      enum {
        UNR_LITERAL,
        UNR_ARITH,
        UNR_PAREN
      } kind;

      union {
        Token lit;
        struct {
          Token sign;
          struct _Node *u0;
        } arith;
        struct _Node *paren;
      } v;
    } unary;

    struct {
      struct _Node *expr;
    } paren;
  } u;
} Node;

const unsigned long max_nodes = 128;
static Node nodes[max_nodes];
static unsigned long cnt_nodes = 0;

Node *createNode() __attribute__((always_inline));
Node *createNode() {
  // The fast version does not rely on system calls:
  /* return (Node*)malloc(sizeof(Node)); */
  if (cnt_nodes == max_nodes) {
    exit(-4);
  }
  return &nodes[cnt_nodes++];
}

void destroyNode(Node *n) __attribute__((always_inline));
void destroyNode(Node *n) {
  /* The fast version does not rely on system calls,
   * nodes are never free'd:
   */
  /* free(n) */;
}

Node *parse() __attribute__((noinline));
Node *parseExpr() __attribute__((noinline));
Node *parseTerm() __attribute__((noinline));
Node *parseFactor() __attribute__((noinline));
Node *parseUnary() __attribute__((noinline));
Node *parseParen() __attribute__((noinline));

Node *parse() {
  lex(); // Read the first token.

  Node *n = parseExpr();
  if (cur->kind != T_EOF) {
    exit(-1);
  }
  return n;
}

Node *parseExpr() {
  Node *n = createNode();
  n->kind = N_EXPR;

  n->u.expr.term = parseTerm();
  return n;
}

Node *parseTerm() {
  Node *n = createNode();
  n->kind = N_TERM;

  switch(cur->kind) {
  case T_PLUS:
  case T_MINUS: {
    Token op = *cur;
    lex(); // Consume current token.

    n->u.term.kind = TRM_ARITH;
    n->u.term.v.arith.op = op;
    n->u.term.v.arith.f0 = parseFactor();
    n->u.term.v.arith.f1 = parseFactor();
    break;
  }
  default: {
    n->u.term.kind = TRM_FACTOR;
    n->u.term.v.factor = parseFactor();
    break;
  }
  }
  return n;
}

Node *parseFactor() {
  Node *n = createNode();
  n->kind = N_FACTOR;

  switch(cur->kind) {
  case T_ASTERISK:
  case T_SLASH: {
    Token op = *cur;
    lex(); // Consume current token.

    n->u.factor.kind = FAC_ARITH;
    n->u.factor.v.arith.op = op;
    n->u.factor.v.arith.u0 = parseUnary();
    n->u.factor.v.arith.u1 = parseUnary();
    break;
  }
  default: {
    n->u.factor.kind = FAC_UNARY;
    n->u.factor.v.unary = parseUnary();
    break;
  }
  }
  return n;
}

Node *parseUnary() {
  Node *n = createNode();
  n->kind = N_UNARY;

  switch(cur->kind) {
  case T_MINUS: {
    Token sign = *cur;
    lex(); // Consume current token.

    n->u.unary.kind = UNR_ARITH;
    n->u.unary.v.arith.sign = sign;
    n->u.unary.v.arith.u0 = parseUnary();
    break;
  }
  case T_LITERAL: {
    Token lit = *cur;
    lex(); // Consume current token.

    n->u.unary.kind = UNR_LITERAL;
    n->u.unary.v.lit = lit;
    break;
  }
  case T_LPAREN: {
    n->u.unary.kind = UNR_PAREN;
    n->u.unary.v.paren = parseParen();
    break;
  }
  default:
    exit(-1);
  }
  return n;
}

Node *parseParen() {
  Node *n = createNode();
  n->kind = N_PAREN;

  if (cur->kind != T_LPAREN) {
    exit(-1);
  }
  lex(); // Consume LPAREN.

  n->u.paren.expr = parseExpr();
  if (cur->kind != T_RPAREN) {
      exit(-1);
  }
  lex(); // Consume RPAREN.
  return n;
}

void printNode(const Node *n) {
  switch(n->kind) {
  case N_EXPR:
    printNode(n->u.expr.term);
    break;
  case N_TERM:
    if (n->u.term.kind == TRM_FACTOR) {
      printNode(n->u.term.v.factor);
    } else if (n->u.term.kind == TRM_ARITH) {
      printNode(n->u.term.v.arith.f0);
      printf("%c ", (n->u.term.v.arith.op.kind == T_PLUS) ? '+' : '-');
      printNode(n->u.term.v.arith.f1);
    } else {
      exit(-3);
    }
    break;
  case N_FACTOR:
    if (n->u.factor.kind == FAC_UNARY) {
      printNode(n->u.factor.v.unary);
    } else if (n->u.factor.kind == FAC_ARITH) {
      printNode(n->u.factor.v.arith.u0);
      printf("%c ", (n->u.factor.v.arith.op.kind == T_ASTERISK) ? '*' : '/');
      printNode(n->u.factor.v.arith.u1);
    } else {
      exit(-3);
    }
    break;
  case N_UNARY:
    if (n->u.unary.kind == UNR_LITERAL) {
      printf("%lu ", n->u.unary.v.lit.value);
    } else if (n->u.unary.kind == UNR_ARITH) {
      printf("-");
      printNode(n->u.unary.v.arith.u0);
    } else if (n->u.unary.kind == UNR_PAREN) {
      printNode(n->u.unary.v.paren);
    } else {
      exit(-3);
    }
    break;
  case N_PAREN:
    printf("(");
    printNode(n->u.paren.expr);
    printf(") ");
    break;
  }
}

int64_t evalNode(const Node *) __attribute__((noinline));
int64_t evalNode(const Node *n) {
  switch(n->kind) {
  case N_EXPR:
    return evalNode(n->u.expr.term);
  case N_TERM:
    if (n->u.term.kind == TRM_FACTOR) {
      return evalNode(n->u.term.v.factor);
    } else if (n->u.term.kind == TRM_ARITH) {
      int64_t t0 = evalNode(n->u.term.v.arith.f0);
      int64_t t1 = evalNode(n->u.term.v.arith.f1);
      return (n->u.term.v.arith.op.kind == T_PLUS) ? (t0 + t1) : (t0 - t1);
    } else {
      exit(-3);
    }
    break;
  case N_FACTOR:
    if (n->u.factor.kind == FAC_UNARY) {
      return evalNode(n->u.factor.v.unary);
    } else if (n->u.factor.kind == FAC_ARITH) {
      int64_t f0 = evalNode(n->u.factor.v.arith.u0);
      int64_t f1 = evalNode(n->u.factor.v.arith.u1);
      return (n->u.factor.v.arith.op.kind == T_ASTERISK) ? (f0 * f1) : (f0 / f1);
    } else {
      exit(-3);
    }
    break;
  case N_UNARY:
    if (n->u.unary.kind == UNR_LITERAL) {
      return n->u.unary.v.lit.value;
    } else if (n->u.unary.kind == UNR_ARITH) {
      return -evalNode(n->u.unary.v.arith.u0);
    } else if (n->u.unary.kind == UNR_PAREN) {
      return evalNode(n->u.unary.v.paren);
    } else {
      exit(-3);
    }
    break;
  case N_PAREN:
    return evalNode(n->u.paren.expr);
  }
}

int64_t ___enc_repl(uint64_t *input) __attribute__((noinline));
int64_t ___enc_repl(uint64_t *input) {
  Token t;

  buffer = input;
  cnt_nodes = 0;
  // Avoid system calls for performance ...
  // cur = (Token*)malloc(sizeof(Token));
  // ... use a stack variable instead:
  cur = &t;
  Node *root = parse();

  // Avoid prinintg for performance:
  /* printNode(root);
     printf("\n"); */

  int64_t res = evalNode(root);
  // Avoid prinintg for performance:
  /* printf("%ld\n", res); */
  return res;
}
