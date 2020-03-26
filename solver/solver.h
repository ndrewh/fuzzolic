#ifndef SOLVER_H
#define SOLVER_H

#include <stdint.h>
#include <unistd.h>

#include <gmodule.h>

#include <z3.h>
#define Z3_VERSION 487

#define USE_COLOR
#include "debug.h"

#include "../tracer/tcg/symbolic/symbolic-struct.h"

typedef enum ExprAnnotationType {
    COSTANT_AND,
} ExprAnnotationType;

typedef struct ExprAnnotation {
    ExprAnnotationType type;
    uintptr_t          value;
    void*              result;
} ExprAnnotation;

typedef struct CachedExpr {
    Z3_ast      expr;
    GHashTable* inputs;
} CachedExpr;

Z3_ast smt_query_to_z3(Expr* query, uintptr_t is_const, size_t width,
                       GHashTable* inputs);
void   smt_print_ast_sort(Z3_ast e);
Z3_ast smt_new_const(uint64_t value, size_t n_bits);
Z3_ast smt_new_symbol(uintptr_t id, const char* name, size_t n_bits, Expr* e);
Z3_ast smt_bv_extract(Z3_ast e, size_t width);
Z3_ast smt_to_bv(Z3_ast e);
Z3_ast smt_to_bv_n(Z3_ast e, size_t width);
void   smt_bv_resize(Z3_ast* a, Z3_ast* b, ssize_t size);
Z3_ast optimize_z3_query(Z3_ast e);
void get_inputs_from_expr(Z3_ast e, GHashTable* inputs);

void            add_expr_annotation(Expr* e, ExprAnnotation* ea);
ExprAnnotation* get_expr_annotation(Expr* e);

// branch_coverage.c
#define CONTEXT_SENSITIVITY 1
int  is_interesting_branch(uintptr_t pc, uintptr_t taken);
int  is_interesting_memory(uintptr_t addr);
void load_bitmaps();
void save_bitmaps();

typedef struct Config {
    const char* testcase_path;
    const char* testcase_dir;
    const char* testcase_size;
    //
    const char* output_dir;
    //
    const char* branch_bitmap_path;
    const char* context_bitmap_path;
    const char* memory_bitmap_path;
} Config;

// opts.c
void parse_opts(int argc, char* argv[], Config* config);

#endif // SOLVER_H