#ifndef _LISP_H
#define _LISP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <headers/mpc.h>

/*===================================forward declarations===================================*/
struct lval;
struct lenv;
typedef struct lval lval;
typedef struct lenv lenv;

/*===================================define===================================*/
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define	MAXLINE	4096			/* max line length */
#define VERSION "0.0.0.0.0.5"   /* my lisp version */

// for debug
#define MPC_AST_PRINT 0x1

#define DEBUG_VERBOSE_SET   0x1


#define LASSERT(args, cond, err) \
    if (!(cond)) { lval_del(args); return lval_err(err); }

/*===================================data type===================================*/
// {0, 1, 2, 3} lisp value type
enum { LVAL_ERR,  LVAL_NUM,   LVAL_SYM, 
       LVAL_FUN,  LVAL_SEXPR, LVAL_QEXPR };

typedef lval* (*lbuiltin)(lenv*, lval*);

// lisp value
struct lval {
    int type;
    double num;
    
    char* err;
    char* sym;
    lbuiltin func;
    
    int count;
    lval** cell;
};

// Each entry in one list has a correspoiding entry in the other list at the same position
// lisp environment
struct lenv {
    int count;
    char** syms;
    lval** vals;
};

/*===================================function===================================*/
// lib/error.c
void	err_msg(const char *, ...);
void	err_dump(const char *, ...);
void	err_quit(const char *, ...);
void	err_cont(int, const char *, ...);
void	err_exit(int, const char *, ...);
void	err_ret(const char *, ...);
void	err_sys(const char *, ...);
// lib/util.c
/* Dynamic storage allocation wrappers */
void    *Malloc(size_t size);
void    *Realloc(void *ptr, size_t size);
void    *Calloc(size_t nmemb, size_t size);
void    Free(void *ptr);

// operation/op.c
lval*   builtin_op(lenv*, lval*, char*);
lval*   builtin_head(lenv*, lval*);
lval*   builtin_tail(lenv*, lval*);
lval*   builtin_list(lenv*, lval*);
lval*   builtin_eval(lenv*, lval*);
lval*   builtin_join(lenv*, lval*);
lval*   builtin_init(lenv*, lval*);
lval*   builtin_len(lenv*, lval*);
lval*   builtin_cons(lenv*, lval*);
lval*   builtin_add(lenv*, lval*);
lval*   builtin_sub(lenv*, lval*);
lval*   builtin_mul(lenv*, lval*);
lval*   builtin_div(lenv*, lval*);
lval*   builtin_mod(lenv*, lval*);
lval*   builtin_max(lenv*, lval*);
lval*   builtin_min(lenv*, lval*);

// expression/expr.c
lval*   lval_join(lval*, lval*);
lval*   lval_num(double);
lval*   lval_err(char*);
lval*   lval_sym(char*);
lval*   lval_sexpr(void);
lval*   lval_qexpr(void);
lval*   lval_func(lbuiltin);
lval*   lval_read_num(mpc_ast_t*);
lval*   lval_read(mpc_ast_t*);
lval*   lval_add(lval*, lval*);
lval*   lval_eval_sexpr(lenv*, lval*);
lval*   lval_eval(lenv*, lval*);
lval*   lval_take(lval*, int);
lval*   lval_pop(lval*, int);
lval*   lval_copy(lval*);
void    lval_del(lval* v);
void    lval_print(lval*);
void    lval_println(lval*);
void    lval_expr_print(lval*, char, char);

// environment/env.c
lenv*   lenv_new(void);
void    lenv_del(lenv*);
lval*   lenv_get(lenv*, lval*);
void    lenv_put(lenv*, lval*, lval*);
void    lenv_add_builtin(lenv*, char*, lbuiltin);
void    lenv_add_builtins(lenv*);

// lib/debug.c
uint64_t debug_printf(uint64_t open_set, const char *format, ...);
uint64_t debug_mpc_ast_print(uint64_t open_set, mpc_ast_t* a);

#endif