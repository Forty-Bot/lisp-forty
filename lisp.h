#ifndef LISP_H
#define LISP_H

#include <stdbool.h>

#include "mpc.h"

struct lval;
struct lenv;
typedef struct lval lval;
typedef struct lenv lenv;

typedef lval*(*lbuiltin)(lenv*, lval*);

struct lval{
	enum ltype {
		LVAL_NUM,
		LVAL_BOOL,
		LVAL_ERR,
		LVAL_SYM,
		LVAL_SEXPR,
		LVAL_QEXPR,
		LVAL_FUNC} type;

	long num;
	char* err;
	char* sym;

	lbuiltin builtin;
	lenv* env;
	lval* formals;
	lval* body;

	int count;
	struct lval** cell;
};

static lval L_TRUE = {LVAL_BOOL, true};
static lval L_FALSE = {LVAL_BOOL, false};
lval* LVAL_TRUE = &L_TRUE;
lval* LVAL_FALSE = &L_FALSE;

//typedef enum rel {GT, LT, EQ} rel;

typedef struct lentry{
	char* sym;
	lval* v;
} lentry;

//lenvs are hiearchal doubly hashed hashtables
struct lenv{
	lenv* par;
	int max;
	int count;
	lentry* table;
};

/* Initial size of hash table
 * The hash table size should always be a factor of 2
 * This means our double hash needs to be odd, but we can make that work
 */
#define LENV_INIT 32
//Use a smaller value for local scope
#define LENV_LOCAL_INIT 8

//enum { LERR_DIV_0, LERR_BAD_OP, LERR_BAD_NUM};

#define LVAL_ERR_MAX 512

lval* lval_num(long);
lval* lval_err(char*, ...);
lval* lval_sym(char*);
lval* lval_sexp();

void lval_del(lval*);
lval* lval_append(lval*, lval*);
lval* lval_join(lval*, lval*);
lval* lval_take(lval*, int);
lval* lval_pop(lval*, int);
lval* lval_equals(lval*, lval*);
//rel lval_compare(lval*, lval*);

void lval_print(lval*);
void lval_expr_print(lval*, char, char);
void lval_println(lval*);

lval* lval_read(mpc_ast_t*);
lval* lval_read_num(mpc_ast_t*);

lval* lval_eval(lenv*, lval*);
lval* lval_eval_sexpr(lenv*, lval*);

lval* lval_call(lenv*, lval*, lval*);

char* ltype_name(int);

lval* builtin_head(lenv*, lval*);
lval* builtin_tail(lenv*, lval*);
lval* builtin_list(lenv*, lval*);
lval* builtin_eval(lenv*, lval*);
lval* builtin_join(lenv*, lval*);
lval* builtin_lambda(lenv*, lval*);
lval* builtin_def(lenv*, lval*);
lval* builtin_put(lenv*, lval*);
lval* builtin_op(lenv*, lval*, char*);
lval* builtin_eq(lenv*, lval*);
//lval* builtin(lval*, char*);

lenv* lenv_new(int);
void lenv_del(lenv*);
lenv* lenv_copy(lenv*);
void lenv_put(lenv*, lval*, lval*);
void lenv_def(lenv*, lval*, lval*);
lval* lenv_get(lenv*, lval*);
void lenv_resize(lenv*, int);
void lenv_add_builtin(lenv*, char*, lbuiltin);
void lenv_add_builtins(lenv*);
lval* lenv_equals(lenv*, lenv*);

//lval eval(mpc_ast_t*);
//lval eval_op(char*, lval, lval);
//lval divide(long, long);

#endif