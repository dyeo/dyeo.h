#ifndef __TEST_H__
#define __TEST_H__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "inttypes.h"

// ---------------------------------------------------------------------------------------------------------------------

#if !defined(TEST_H_DISABLE_UNICODE) && \
    (defined(__STDC_ISO_10646__) || defined(_UNICODE) || defined(UNICODE))
#define TEST_H_UNICODE
#endif

// ---------------------------------------------------------------------------------------------------------------------

#if !defined(TEST_H_DISABLE_COLORS)
#define _ColStr(CodeStr) "\x1b[" CodeStr "m"
#else
#define _ColStr(CodeStr)
#endif

#define _Col0 _ColStr("0;0")
#define _ColHint _ColStr("3;90")
#define _ColInfo _ColStr("0;90")
#define _ColPass _ColStr("0;32")
#define _ColWarn _ColStr("0;33")
#define _ColFail _ColStr("0;31")
#define _ColPing _ColStr("0;35")

#ifdef TEST_H_UNICODE
#define _LitPass _Col(Pass) "[\u2713] "_Col(0)
#define _LitFail _Col(Fail) "[\u2717] "_Col(0)
#else
#define _LitPass _Col(Pass) "[PASS] "_Col(0)
#define _LitFail _Col(Fail) "[FAIL] "_Col(0)
#endif

// ---------------------------------------------------------------------------------------------------------------------

#define _Col(Name) _Col##Name
#define _Lit(Name) _Lit##Name

// ---------------------------------------------------------------------------------------------------------------------
// macros to genericize fundamental types
// add your own before including to automatically support testing!
// ---------------------------------------------------------------------------------------------------------------------

#define _test_h_t_PFmt ""
#define _test_h_t_PArg(v) v
#define _test_h_t_ExEq(l, r) l == r

// ---------------------------------------------------------------------------------------------------------------------

#define _int8_t_PFmt "%hhd"
#define _int16_t_PFmt "%hd"
#define _int32_t_PFmt "%d"
#define _int64_t_PFmt "%lld"
#define _uint8_t_PFmt "%hhu"
#define _uint16_t_PFmt "%hu"
#define _uint32_t_PFmt "%u"
#define _uint64_t_PFmt "%llu"
#define _char_PFmt "%c"
#define _float_PFmt "%f"
#define _double_PFmt "%lf"
#define _long_double_PFmt "%Lf"
#define _string_PFmt "%s"
#define _pointer_PFmt "%p"
#define _bool_PFmt "%s"
#define __Bool_PFmt "%s"
#define _short_PFmt "%hd"
#define _int_PFmt "%d"
#define _long_PFmt "%lld"

#define _int8_t_PArg(v) _test_h_t_PArg(v)
#define _int16_t_PArg(v) _test_h_t_PArg(v)
#define _int32_t_PArg(v) _test_h_t_PArg(v)
#define _int64_t_PArg(v) _test_h_t_PArg(v)
#define _uint8_t_PArg(v) _test_h_t_PArg(v)
#define _uint16_t_PArg(v) _test_h_t_PArg(v)
#define _uint32_t_PArg(v) _test_h_t_PArg(v)
#define _uint64_t_PArg(v) _test_h_t_PArg(v)
#define _char_PArg(v) _test_h_t_PArg(v)
#define _float_PArg(v) _test_h_t_PArg(v)
#define _double_PArg(v) _test_h_t_PArg(v)
#define _long_double_PArg(v) _test_h_t_PArg(v)
#define _string_PArg(v) _test_h_t_PArg(v)
#define _pointer_PArg(v) _test_h_t_PArg(v)
#define _bool_PArg(v) ((v) ? "true" : "false")
#define __Bool_PArg(v) _bool_PArg(v)
#define _short_PArg(v) _test_h_t_PArg(v)
#define _int_PArg(v) _test_h_t_PArg(v)
#define _long_PArg(v) _test_h_t_PArg(v)

#define _int8_t_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _int16_t_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _int32_t_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _int64_t_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _uint8_t_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _uint16_t_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _uint32_t_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _uint64_t_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _char_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _float_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _double_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _long_double_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _string_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _pointer_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _bool_ExEq(l, r) _test_h_t_ExEq(l, r)
#define __Bool_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _short_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _int_ExEq(l, r) _test_h_t_ExEq(l, r)
#define _long_ExEq(l, r) _test_h_t_ExEq(l, r)

// ---------------------------------------------------------------------------------------------------------------------

#define _PFmt(T) _##T##_PFmt
#define _PArg(T, V) _##T##_PArg(V)
#define _ExEq(T, L, R) _##T##_ExEq(L, R)

// ---------------------------------------------------------------------------------------------------------------------

#define _test_h_indent(S)                           \
    do                                              \
    {                                               \
        for (int i = 0; i < _test_h_level * 2; ++i) \
        {                                           \
            fputc(' ', S);                          \
        }                                           \
    } while (0)

#define test_true(Expr)                                                \
    do                                                                 \
    {                                                                  \
        _test_h_level++;                                               \
        if ((Expr))                                                    \
        {                                                              \
            _test_h_indent(stdout);                                    \
            fprintf(stdout, _Lit(Pass) #Expr "\n");                    \
            _test_h_passes++;                                          \
        }                                                              \
        else                                                           \
        {                                                              \
            _test_h_indent(stderr);                                    \
            fprintf(stderr, _Lit(Fail) _Col(Warn) #Expr _Col(0) "\n"); \
            _test_h_fails++;                                           \
        }                                                              \
        _test_h_level--;                                               \
    } while (0)

#define test_expr(Expr, TRes, Res)                                                                               \
    do                                                                                                           \
    {                                                                                                            \
        _test_h_level++;                                                                                         \
        TRes _res = ((Res));                                                                                     \
        TRes _expr = ((Expr));                                                                                   \
        if (_ExEq(TRes, _expr, _res))                                                                            \
        {                                                                                                        \
            _test_h_indent(stdout);                                                                              \
            fprintf(stdout,                                                                                      \
                    _Lit(Pass) #Expr _Col(Hint) " == " _PFmt(TRes) _Col(0) "\n",                                 \
                    _PArg(TRes, _res));                                                                          \
            _test_h_passes++;                                                                                    \
        }                                                                                                        \
        else                                                                                                     \
        {                                                                                                        \
            _test_h_indent(stderr);                                                                              \
            fprintf(stdout,                                                                                      \
                    _Lit(Fail) _Col(Warn) #Expr _Col(Hint) " != " _PFmt(TRes) " (" _PFmt(TRes) ")" _Col(0) "\n", \
                    _PArg(TRes, _res), _PArg(TRes, _expr));                                                      \
            _test_h_fails++;                                                                                     \
        }                                                                                                        \
        _test_h_level--;                                                                                         \
    } while (0)

#define test_group(Name, Block)                                                                \
    do                                                                                         \
    {                                                                                          \
        size_t _test_h_passes_##Name = _test_h_passes, _test_h_fails_##Name = _test_h_fails;   \
        _test_h_indent(stdout);                                                                \
        fprintf(stdout, _Col(Ping) "[" #Name "]" _Col(0) " Running tests...\n");               \
        _test_h_level++;                                                                       \
        {                                                                                      \
            Block;                                                                             \
        }                                                                                      \
        _test_h_level--;                                                                       \
        _test_h_indent(stdout);                                                                \
        fprintf(stdout,                                                                        \
                _Col(Info) "[" #Name "] %llu passed; %llu failed." _Col(0) "\n",               \
                _test_h_passes - _test_h_passes_##Name, _test_h_fails - _test_h_fails_##Name); \
    } while (0)

// ---------------------------------------------------------------------------------------------------------------------
// test group state
// ---------------------------------------------------------------------------------------------------------------------

static size_t _test_h_passes = 0;
static size_t _test_h_fails = 0;
static size_t _test_h_level = 0;

// ---------------------------------------------------------------------------------------------------------------------

#endif //__TEST_H__