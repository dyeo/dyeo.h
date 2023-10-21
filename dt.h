#ifndef _DT_H
#define _DT_H

#ifdef _MSC_VER
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#define strdup _strdup
#endif

// -----------------------------------------------------------------------------

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------------------------------------------------------

// redefine these for your own allocation
#ifndef _dt_malloc
#define _dt_malloc(_Size) malloc(_Size)
#endif
#ifndef _dt_calloc
#define _dt_calloc(_Count, _Size) calloc(_Count, _Size)
#endif
#ifndef _dt_realloc
#define _dt_realloc(_BlockPtr, _Size) realloc(_BlockPtr, _Size)
#endif
#ifndef _dt_free
#define _dt_free(_BlockPtr) free(_BlockPtr)
#endif

// -----------------------------------------------------------------------------

#ifndef _dt_memcmp
#define _dt_memcmp(_LPtr, _RPtr, _Size) memcmp(_LPtr, _RPtr, _Size)
#endif
#ifndef _dt_memset
#define _dt_memset(_DstPtr, _Val, _Size) memset(_DstPtr, _Val, _Size)
#endif
#ifndef _dt_memcpy
#define _dt_memcpy(_DstPtr, _SrcPtr, _Size) memcpy(_DstPtr, _SrcPtr, _Size)
#endif

#ifndef byte
typedef unsigned char byte;
#endif

// for security against attackers, seed the library with a random number, at
// least time() but stronger is better
extern void dt_rand_seed(size_t seed);

// these are the hash functions used internally if you want to test them or use
// them for other purposes
extern size_t dt_hash_bytes(void *p, size_t len, size_t seed);
extern size_t dt_hash_string(char *str, size_t seed);

// this is a simple string arena allocator, initialize with e.g.
// 'dt_strarena_t my_arena={0}'.
typedef struct dt_strarena_t dt_strarena_t;
extern char *dt_stralloc(dt_strarena_t *a, char *str);
extern void dt_strreset(dt_strarena_t *a);

// have to #define DT_UNIT_TESTS to call this
extern void dt_unit_tests(void);

///////////////
//
// Everything below here is implementation details
//

extern void *
dt_arrgrowf(void *a, size_t elemsize, size_t addlen, size_t min_cap);
extern void *dt_arrtonulltermf(void *a, size_t vallen);
extern void dt_arrfreef(void *a);
extern void dt_mapfree_impl(void *p, size_t elemsize);
extern void *
dt_mapget_key(void *a, size_t elemsize, void *key, size_t keysize, int mode);
extern void *dt_mapget_key_ts(void *a,
                              size_t elemsize,
                              void *key,
                              size_t keysize,
                              ptrdiff_t *temp,
                              int mode);
extern void *dt_mapput_default(void *a, size_t elemsize);
extern void *
dt_mapput_key(void *a, size_t elemsize, void *key, size_t keysize, int mode);
extern void *dt_mapdel_key(void *a,
                           size_t elemsize,
                           void *key,
                           size_t keysize,
                           size_t keyoffset,
                           int mode);
extern void *dt_smpmode_impl(size_t elemsize, int mode);

#if !defined(__cplusplus)
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#define DT_HAS_LITERAL_ARRAY
#endif
#endif

#define dt_tuple(Type0, Type1)                                                 \
  struct                                                                       \
  {                                                                            \
    Type0 item0;                                                               \
    Type1 item1;                                                               \
  }

#define dt_thruple(Type0, Type1, Type2)                                        \
  struct                                                                       \
  {                                                                            \
    Type0 item0;                                                               \
    Type1 item1;                                                               \
    Type2 item2;                                                               \
  }

// this macro takes the address of the argument, but on gcc/clang can accept
// rvalues
#if defined(DT_HAS_LITERAL_ARRAY) && defined(DT_HAS_TYPEOF)
#if __clang__
#define DT_ADDRESSOF(TypeVar, Value) ((__typeof__(TypeVar)[1]){Value})
#else
#define DT_ADDRESSOF(TypeVar, Value) ((typeof(TypeVar)[1]){Value})
#endif
#else
#define DT_ADDRESSOF(TypeVar, Value) &(Value)
#endif

#define DT_OFFSETOF(Var, Field) ((char *) &(Var)->Field - (char *) (var))

#define dt_arrhead(t) ((dt_arrhead_t *) (t) -1)
#define dt_temp(t) dt_arrhead(t)->tmp
#define dt_temp_key(t) (*(char **) dt_arrhead(t)->tbl)

#define dt_arrsetcap(ArrPtr, Capacity) (dt_arrgrow(ArrPtr, 0, Capacity))
#define dt_arrsetlen(ArrPtr, Len)                                              \
  ((dt_arrcap(ArrPtr) < (size_t) (Len)                                         \
    ? dt_arrsetcap((ArrPtr), (size_t) (Len)),                                  \
    0                                                                          \
    : 0),                                                                      \
   (ArrPtr) ? dt_arrhead(ArrPtr)->len = (size_t) (Len) : 0)
#define dt_arrcap(ArrPtr) ((ArrPtr) ? dt_arrhead(ArrPtr)->cap : 0)
#define dt_arrlen(ArrPtr) ((ArrPtr) ? (ptrdiff_t) dt_arrhead(ArrPtr)->len : 0)
#define dt_arrlenu(ArrPtr) ((ArrPtr) ? dt_arrhead(ArrPtr)->len : 0)
#define dt_arrput(ArrPtr, Value)                                               \
  (dt_arrmaygrow(ArrPtr, 1), (ArrPtr)[dt_arrhead(ArrPtr)->len++] = (Value))
#define dt_arradd dt_arrput  // synonym
#define dt_arrenq dt_arrput  // synonym
#define dt_arrpush dt_arrput // synonym
#define dt_arrpop(ArrPtr)                                                      \
  (dt_arrhead(ArrPtr)->len--, (ArrPtr)[dt_arrhead(ArrPtr)->len])
#define dt_arrpopn(ArrPtr, Index)                                              \
  (dt_arrswap(ArrPtr, 0, dt_arrlen(ArrPtr) + 1),                               \
   dt_arrdel(ArrPtr, 0),                                                       \
   (ArrPtr)[dt_arrlen(ArrPtr) + 1])
#define dt_arrdeq(ArrPtr) dt_arrpopn(ArrPtr, 0)
#define dt_arraddn(ArrPtr, Len)                                                \
  ((void) (dt_arraddnindex(                                                    \
    ArrPtr, Len))) // deprecated, use one of the following instead:
#define dt_arraddnptr(ArrPtr, Len)                                             \
  (dt_arrmaygrow(ArrPtr, Len),                                                 \
   (Len) ? (dt_arrhead(ArrPtr)->len += (Len),                                  \
            &(ArrPtr)[dt_arrhead(ArrPtr)->len - (Len)])                        \
         : (ArrPtr))
#define dt_arraddnindex(ArrPtr, Len)                                           \
  (dt_arrmaygrow(ArrPtr, Len),                                                 \
   (Len) ? (dt_arrhead(ArrPtr)->len += (Len), dt_arrhead(ArrPtr)->len - (Len)) \
         : dt_arrlen(ArrPtr))
#define dt_arraddnoff dt_arraddnindex
#define dt_arrlast(ArrPtr) ((ArrPtr)[dt_arrhead(ArrPtr)->len - 1])
#define dt_arrfree(ArrPtr)                                                     \
  ((void) ((ArrPtr) ? _dt_free(dt_arrhead(ArrPtr)) : (void) 0), (ArrPtr) = NULL)
#define dt_arrdel(ArrPtr, Index) dt_arrdeln(ArrPtr, Index, 1)
#define dt_arrdeln(ArrPtr, Index, Len)                                         \
  (memmove(&(ArrPtr)[Index],                                                   \
           &(ArrPtr)[(Index) + (Len)],                                         \
           sizeof *(ArrPtr) * (dt_arrhead(ArrPtr)->len - (Len) - (Index))),    \
   dt_arrhead(ArrPtr)->len -= (Len))
#define dt_arrdelswap(ArrPtr, Index)                                           \
  ((ArrPtr)[Index] = dt_arrlast(ArrPtr), dt_arrhead(ArrPtr)->len -= 1)
#define dt_arrinsn(ArrPtr, Index, Len)                                         \
  (dt_arraddn((ArrPtr), (Len)),                                                \
   memmove(&(ArrPtr)[(Index) + (Len)],                                         \
           &(ArrPtr)[Index],                                                   \
           sizeof *(ArrPtr) * (dt_arrhead(ArrPtr)->len - (Len) - (Index))))
#define dt_arrins(ArrPtr, Index, v)                                            \
  (dt_arrinsn((ArrPtr), (Index), 1), (ArrPtr)[Index] = (v))

#define dt_arrswap(ArrPtr, Index0, Index1)                                     \
  (dt_arrmaygrow(ArrPtr, 1),                                                   \
   (ArrPtr)[dt_arrlenu(ArrPtr)] = (ArrPtr)[Index0],                            \
   (ArrPtr)[Index0]             = (ArrPtr)[Index1],                            \
   (ArrPtr)[Index1]             = (ArrPtr)[dt_arrlenu(ArrPtr)])

#define dt_arrmaygrow(ArrPtr, Len)                                             \
  ((!(ArrPtr) || dt_arrhead(ArrPtr)->len + (Len) > dt_arrhead(ArrPtr)->cap)    \
     ? (dt_arrgrow(ArrPtr, Len, 0), 0)                                         \
     : 0)

#define dt_arrgrow(ArrPtr, Len, Capacity)                                      \
  ((ArrPtr) = dt_arrgrowf((ArrPtr), sizeof *(ArrPtr), (Len), (Capacity)))

#define dt_arraddcstr(ArrPtr, CStr)                                            \
  do                                                                           \
  {                                                                            \
    const char *__cstr = CStr;                                                 \
    size_t __cstrl     = strlen(__cstr);                                       \
    dt_arrmaygrow(ArrPtr, __cstrl);                                            \
    _dt_memcpy(&(ArrPtr)[dt_arrlen(ArrPtr)], __cstr, __cstrl);                 \
    dt_arraddnoff(ArrPtr, __cstrl);                                            \
  } while (0)

#define dt_arrinscstr(ArrPtr, Index, CStr)                                     \
  do                                                                           \
  {                                                                            \
    const char *__cstr = CStr;                                                 \
    size_t __cstrl     = strlen(__cstr);                                       \
    size_t __cstri     = (dt_arrlen(ArrPtr) + (Index)) % dt_arrlen(ArrPtr);    \
    dt_arrinsn(ArrPtr, __cstri, __cstrl);                                      \
    _dt_memcpy(&(ArrPtr)[__cstri], __cstr, __cstrl);                           \
  } while (0)

#define dt_arraddbytes(ArrPtr, Bytes, Len)                                     \
  do                                                                           \
  {                                                                            \
    dt_arrmaygrow(ArrPtr, Len);                                                \
    _dt_memcpy(&(ArrPtr)[dt_arrlen(ArrPtr)], Bytes, Len);                      \
    dt_arraddnoff(ArrPtr, Len);                                                \
  } while (0)

#define dt_arrinsbytes(ArrPtr, Index, Bytes, Len)                              \
  do                                                                           \
  {                                                                            \
    size_t __cstri = (dt_arrlen(ArrPtr) + (Index)) % dt_arrlen(ArrPtr);        \
    dt_arrinsn(ArrPtr, __cstri, Len);                                          \
    _dt_memcpy(&(ArrPtr)[__cstri], Bytes, Len);                                \
  } while (0)

#define dt_arrconcat(DstArrPtr, SrcArrPtr)                                     \
  do                                                                           \
  {                                                                            \
    size_t dstlen = dt_arrlenu(DstArrPtr);                                     \
    size_t srclen = dt_arrlenu(SrcArrPtr);                                     \
    dt_arrmaygrow(DstArrPtr, srclen);                                          \
    _dt_memcpy(&(DstArrPtr)[dstlen], SrcArrPtr, srclen);                       \
    dt_arrsetlen(DstArrPtr, dstlen + srclen);                                  \
  } while (0)

#define dt_arrtonullterm(ArrPtr) dt_arrtonulltermf((ArrPtr), sizeof *(ArrPtr))

#define dt_kvp(TKey, TValue)                                                   \
  struct                                                                       \
  {                                                                            \
    TKey key;                                                                  \
    TValue value;                                                              \
  }

#define dt_mapput(MapPtr, Key, Value)                                                                \
  ((MapPtr)                             = dt_mapput_key((MapPtr),                                    \
                            sizeof *(MapPtr),                            \
                            (void *) DT_ADDRESSOF((MapPtr)->key, (Key)), \
                            sizeof(MapPtr)->key,                         \
                            0),                                          \
   (MapPtr)[dt_temp((MapPtr) -1)].key   = (Key),                                                     \
   (MapPtr)[dt_temp((MapPtr) -1)].value = (Value))

#define dt_mapadd dt_mapput // synonym

#define dt_mapputs(MapPtr, Pair)                                               \
  ((MapPtr)                       = dt_mapput_key((MapPtr),                    \
                            sizeof *(MapPtr),            \
                            &(Pair).key,                 \
                            sizeof(Pair).key,            \
                            DT_MAP_BINARY),              \
   (MapPtr)[dt_temp((MapPtr) -1)] = (Pair))

#define dt_mapadds dt_mapputs // synonym

#define dt_mapgeti(MapPtr, Key)                                                \
  ((MapPtr) = dt_mapget_key((MapPtr),                                          \
                            sizeof *(MapPtr),                                  \
                            (void *) DT_ADDRESSOF((MapPtr)->key, (Key)),       \
                            sizeof(MapPtr)->key,                               \
                            DT_MAP_BINARY),                                    \
   dt_temp((MapPtr) -1))

#define dt_maphask(MapPtr, Key) ((bool) (dt_mapgeti(MapPtr, Key) >= 0))

#define dt_mapgeti_ts(MapPtr, Key, temp)                                       \
  ((MapPtr) = dt_mapget_key_ts((MapPtr),                                       \
                               sizeof *(MapPtr),                               \
                               (void *) DT_ADDRESSOF((MapPtr)->key, (Key)),    \
                               sizeof(MapPtr)->key,                            \
                               &(temp),                                        \
                               DT_MAP_BINARY),                                 \
   (temp))

#define dt_mapgetp(MapPtr, Key)                                                \
  ((void) dt_mapgeti(MapPtr, Key), &(MapPtr)[dt_temp((MapPtr) -1)])

#define dt_mapgetp_ts(MapPtr, Key, temp)                                       \
  ((void) dt_mapgeti_ts(MapPtr, Key, temp), &(MapPtr)[temp])

#define dt_mapdel(MapPtr, Key)                                                 \
  (((MapPtr) = dt_mapdel_key((MapPtr),                                         \
                             sizeof *(MapPtr),                                 \
                             (void *) DT_ADDRESSOF((MapPtr)->key, (Key)),      \
                             sizeof(MapPtr)->key,                              \
                             DT_OFFSETOF((MapPtr), key),                       \
                             DT_MAP_BINARY)),                                  \
   (MapPtr) ? dt_temp((MapPtr) -1) : 0)

#define dt_mapdefault(MapPtr, Value)                                           \
  ((MapPtr)           = dt_mapput_default((MapPtr), sizeof *(MapPtr)),         \
   (MapPtr)[-1].value = (Value))

#define dt_mapdefaults(MapPtr, Pair)                                           \
  ((MapPtr)     = dt_mapput_default((MapPtr), sizeof *(MapPtr)),               \
   (MapPtr)[-1] = (Pair))

#define dt_mapfree(p)                                                          \
  ((void) ((p) != NULL ? dt_mapfree_impl((p) -1, sizeof *(p)), 0 : 0),         \
   (p) = NULL)

#define dt_mapgets(MapPtr, Key) (*dt_mapgetp(MapPtr, Key))
#define dt_mapget(MapPtr, Key) (dt_mapgetp(MapPtr, Key)->value)
#define dt_mapget_ts(MapPtr, Key, temp)                                        \
  (dt_mapgetp_ts(MapPtr, Key, temp)->value)
#define dt_mapget_ts(MapPtr, Key, temp)                                        \
  (dt_mapgetp_ts(MapPtr, Key, temp)->value)
#define dt_maplen(MapPtr)                                                      \
  ((MapPtr) ? (ptrdiff_t) dt_arrhead((MapPtr) -1)->len - 1 : 0)
#define dt_maplenu(MapPtr) ((MapPtr) ? dt_arrhead((MapPtr) -1)->len - 1 : 0)
#define dt_mapgetp_null(MapPtr, Key)                                           \
  (dt_mapgeti(MapPtr, Key) == -1 ? NULL : &(MapPtr)[dt_temp((MapPtr) -1)])

#define dt_smpput(MapPtr, Key, Value)                                          \
  ((MapPtr)                             = dt_mapput_key((MapPtr),              \
                            sizeof *(MapPtr),      \
                            (void *) (Key),        \
                            sizeof(MapPtr)->key,   \
                            DT_MAP_STRING),        \
   (MapPtr)[dt_temp((MapPtr) -1)].value = (Value))

#define dt_smpadd dt_smpput // synonym

#define dt_smpputi(MapPtr, Key, Value)                                         \
  ((MapPtr)                             = dt_mapput_key((MapPtr),              \
                            sizeof *(MapPtr),      \
                            (void *) (Key),        \
                            sizeof(MapPtr)->key,   \
                            DT_MAP_STRING),        \
   (MapPtr)[dt_temp((MapPtr) -1)].value = (Value),                             \
   dt_temp((MapPtr) -1))

#define dt_smpputs(MapPtr, Pair)                                               \
  ((MapPtr)                       = dt_mapput_key((MapPtr),                    \
                            sizeof *(MapPtr),            \
                            (void *) (Pair).key,         \
                            sizeof(Pair).key,            \
                            DT_MAP_STRING),              \
   (MapPtr)[dt_temp((MapPtr) -1)] = (Pair),                                    \
   (MapPtr)[dt_temp((MapPtr) -1)].key =                                        \
     dt_temp_key((MapPtr) -1)) // above line overwrites whole structure, so
                               // must rewrite key
                               // here if it was allocated internally

#define dt_pshput(MapPtr, p)                                                   \
  ((MapPtr)                       = dt_mapput_key((MapPtr),                    \
                            sizeof *(MapPtr),            \
                            (void *) (p)->key,           \
                            sizeof(p)->key,              \
                            DT_MAP_PTR_TO_STRING),       \
   (MapPtr)[dt_temp((MapPtr) -1)] = (p))

#define dt_smpgeti(MapPtr, Key)                                                \
  ((MapPtr) = dt_mapget_key((MapPtr),                                          \
                            sizeof *(MapPtr),                                  \
                            (void *) (Key),                                    \
                            sizeof(MapPtr)->key,                               \
                            DT_MAP_STRING),                                    \
   dt_temp((MapPtr) -1))

#define dt_smphask(MapPtr, Key) ((bool) (dt_smpgeti(MapPtr, Key) >= 0))

#define dt_pshgeti(MapPtr, Key)                                                \
  ((MapPtr) = dt_mapget_key((MapPtr),                                          \
                            sizeof *(MapPtr),                                  \
                            (void *) (Key),                                    \
                            sizeof(*(MapPtr))->key,                            \
                            DT_MAP_PTR_TO_STRING),                             \
   dt_temp((MapPtr) -1))

#define dt_smpgetp(MapPtr, Key)                                                \
  ((void) dt_smpgeti(MapPtr, Key), &(MapPtr)[dt_temp((MapPtr) -1)])

#define dt_pshget(MapPtr, Key)                                                 \
  ((void) dt_pshgeti(MapPtr, Key), (MapPtr)[dt_temp((MapPtr) -1)])

#define dt_smpdel(MapPtr, Key)                                                 \
  (((MapPtr) = dt_mapdel_key((MapPtr),                                         \
                             sizeof *(MapPtr),                                 \
                             (void *) (Key),                                   \
                             sizeof(MapPtr)->key,                              \
                             DT_OFFSETOF((MapPtr), key),                       \
                             DT_MAP_STRING)),                                  \
   (MapPtr) ? dt_temp((MapPtr) -1) : 0)
#define dt_pshdel(MapPtr, Key)                                                 \
  (((MapPtr) = dt_mapdel_key((MapPtr),                                         \
                             sizeof *(MapPtr),                                 \
                             (void *) (Key),                                   \
                             sizeof(*(MapPtr))->key,                           \
                             DT_OFFSETOF(*(MapPtr), key),                      \
                             DT_MAP_PTR_TO_STRING)),                           \
   (MapPtr) ? dt_temp((MapPtr) -1) : 0)

#define dt_smp_new_arena(MapPtr)                                               \
  ((MapPtr) = dt_smpmode_impl(sizeof *(MapPtr), DT_SMP_ARENA))
#define dt_smp_new_strdup(MapPtr)                                              \
  ((MapPtr) = dt_smpmode_impl(sizeof *(MapPtr), DT_SMP_STRDUP))

#define dt_smpdefault(MapPtr, Value) dt_mapdefault(MapPtr, Value)
#define dt_smpdefaults(MapPtr, Pair) dt_mapdefaults(MapPtr, Pair)

#define dt_smpfree dt_mapfree
#define dt_smplenu dt_maplenu

#define dt_smpgets(MapPtr, Key) (*dt_smpgetp(MapPtr, Key))
#define dt_smpget(MapPtr, Key) (dt_smpgetp(MapPtr, Key)->value)
#define dt_smpgetp_null(MapPtr, Key)                                           \
  (dt_smpgeti(MapPtr, Key) == -1 ? NULL : &(MapPtr)[dt_temp((MapPtr) -1)])
#define dt_smplen dt_maplen

typedef struct
{
  size_t len;
  size_t cap;
  void *tbl;
  ptrdiff_t tmp;
} dt_arrhead_t;

typedef struct dt_strblock_t
{
  struct dt_strblock_t *next;
  char storage[8];
} dt_strblock_t;

struct dt_strarena_t
{
  dt_strblock_t *storage;
  size_t remaining;
  unsigned char block;
  unsigned char mode; // this isn't used by the string arena itself
};

#define DT_MAP_BINARY 0
#define DT_MAP_STRING 1

enum
{
  DT_SMP_NONE,
  DT_SMP_DEFAULT,
  DT_SMP_STRDUP,
  DT_SMP_ARENA
};

// -----------------------------------------------------------------------------

#define dt_default(Value)                                                      \
  (_Generic((Value),                                                           \
    unsigned char: 0u,                                                         \
    char: '\0',                                                                \
    unsigned short: 0u,                                                        \
    short: 0,                                                                  \
    unsigned int: 0u,                                                          \
    int: 0,                                                                    \
    unsigned long: 0lu,                                                        \
    long: 0l,                                                                  \
    unsigned long long: 0llu,                                                  \
    long long: 0ll,                                                            \
    float: 0.0f,                                                               \
    double: 0.0,                                                               \
    default: NULL))

// -----------------------------------------------------------------------------

typedef dt_kvp(char *, struct dt_node *) dt_nodekvp;

#define DT_TYPES_LIST                                                          \
  X(null, void *)                                                              \
  X(bool, _Bool)                                                               \
  X(int, long)                                                                 \
  X(float, double)                                                             \
  X(arr, struct dt_node **)                                                    \
  X(map, dt_nodekvp *)                                                         \
  X(string, char *)

typedef enum dt_type
{
  dt_invalid = -1,
#define X(NAME, _, ...) dt_##NAME,
  DT_TYPES_LIST
#undef X
    dt_type_count
} dt_type;

const char *dt_type_names[dt_type_count] = {
#define X(NAME, ...) #NAME,
  DT_TYPES_LIST
#undef X
};

// -----------------------------------------------------------------------------

typedef struct dt_node
{
  dt_type type;
  union
  {
#define X(NAME, TYPE, ...) TYPE NAME##_v;
    DT_TYPES_LIST
#undef X
  };
} dt_node;

#define dt_get(Node, KeyOrIndex) dt_get_impl(Node, (void *) KeyOrIndex)

typedef struct dt_end_t
{
  byte _;
} dt_end_t;

dt_end_t _dt_end = {0};
#define DT_END (&_dt_end)

#define dt_gets(Node, ...) dt_gets_impl(Node, __VA_ARGS__, (void *) -1)

static dt_node *dt_get_impl(dt_node *node, void *keyorindex)
{
  switch (node->type)
  {
    case dt_arr:
      return node->arr_v[(size_t) keyorindex];
    case dt_map:
      return dt_smpget(node->map_v, (char *) keyorindex);
    default:
      return NULL;
  }
}

static inline dt_node *dt_gets_impl(const dt_node *node, ...)
{
  va_list args;
  va_start(args, node);
  void *key;
  dt_node *res = (dt_node *) node;
  while ((key = va_arg(args, void *)) != (void *) -1)
  {
    res = dt_get(res, key);
    if (res == NULL)
    {
      return NULL;
    }
  }
  return res;
}

// -----------------------------------------------------------------------------

typedef struct dt_dumps_settings_t
{
  bool force_json;
} dt_dumps_settings_t;

const static dt_dumps_settings_t _dt_dumps_settings_default = {.force_json =
                                                                 false};

// -----------------------------------------------------------------------------

extern bool isescape(const char c);
extern bool isendchar(const char c);
extern bool islongstring(const char *str);

extern char *stresc(const char *s);
extern char *strunesc(const char *s);

extern char *sprintfx(const char *format, ...);
extern size_t strnchrn(const char *string, const char c, const size_t n);
extern char *strrchre(const char *string, const char c, const char *end);

extern dt_node *dt_loadf(const char *filepath);
extern dt_node *dt_loads(const char *string);
extern dt_node *dt_loads_impl(const char *string, size_t *offset);
extern bool dt_dumpf(const dt_node *node, const char *filepath);
extern char *dt_dumps(const dt_node *node);
extern char *dt_dumps_ex(const dt_node *node, const dt_dumps_settings_t *set);

extern dt_node *dt_loadb(const size_t len, const byte *bytes);
extern dt_node *
dt_loadb_impl(const size_t len, const byte *bytes, size_t *offset);
extern byte *dt_dumpb(const dt_node *node, size_t *len);
extern void dt_dumpb_impl(const dt_node *node, byte *bytes);

extern void dt_free(dt_node *node);

extern char *dt_loads_raw_string(const char *string, size_t *offset);
extern char *dt_dumps_raw_string(const char *string,
                                 const dt_dumps_settings_t *set);
extern void dt_dumpb_raw_string(const char *string, byte *bytes);

extern void dt_loads_space(const char *string, size_t *offset);
extern void dt_loads_comment(const char *string, size_t *offset);

extern void dt_consume_token(const char *string, size_t *offset, char token);

// -----------------------------------------------------------------------------

#ifdef NDEBUG
#define dt_test(...) ((void *) 0)
#else
#define dt_assert dt_test
#endif

typedef struct dt_linecol
{
  size_t line;
  size_t col;
} dt_linecol;

static inline dt_linecol dt_get_linecol(const char *string, size_t offset)
{
  size_t line = strnchrn(string, '\n', offset);
  size_t col  = line == 0
                  ? offset
                  : strrchre(string, '\n', string + offset) - (string + offset);
  return (dt_linecol){line + 1, col};
}

#define dt_testf(CONDITION, STRING, OFFSET, FORMAT, ...)                       \
  do                                                                           \
  {                                                                            \
    if ((CONDITION))                                                           \
    {                                                                          \
      break;                                                                   \
    }                                                                          \
    dt_linecol lc = dt_get_linecol(STRING, OFFSET);                            \
    fprintf(                                                                   \
      stderr, "ERROR: %llu:%lld: " FORMAT "\n", lc.line, lc.col, __VA_ARGS__); \
    exit(1);                                                                   \
  } while (0)

#define dt_test(CONDITION, MESSAGE, STRING, OFFSET)                            \
  dt_testf(CONDITION, STRING, OFFSET, "%s\n", MESSAGE)

#define X(NAME, TYPE, ...)                                                     \
  static inline dt_node *dt_new_##NAME(TYPE val)                               \
  {                                                                            \
    dt_node *r = (dt_node *) _dt_malloc(sizeof(dt_node));                      \
    if (r == NULL)                                                             \
    {                                                                          \
      return NULL;                                                             \
    }                                                                          \
    r->type     = dt_##NAME;                                                   \
    r->NAME##_v = val;                                                         \
    return r;                                                                  \
  }                                                                            \
  extern void dt_free_##NAME(dt_node *node);                                   \
  extern bool dt_test_##NAME(const char *string, size_t *offset);              \
  extern dt_node *dt_loads_##NAME(const char *string, size_t *offset);         \
  extern char *dt_dumps_##NAME(const dt_node *node,                            \
                               const dt_dumps_settings_t *set);                \
  extern void dt_dumpb_##NAME(const dt_node *node, byte *bytes);               \
  extern dt_node *dt_loadb_##NAME(                                             \
    size_t len, const byte *bytes, size_t *offset);
DT_TYPES_LIST
#undef X

#define dt_print_bytes(Len, Bytes)                                             \
  do                                                                           \
  {                                                                            \
    byte *raw = (void *) Bytes;                                                \
    for (size_t i = 0; i < (Len); ++i)                                         \
    {                                                                          \
      printf("%x", (raw)[i]);                                                  \
    }                                                                          \
  } while (0)

#include <stdint.h>
#define IS_LITTLE_ENDIAN (*(uint16_t *) "\xff\0" < 0x100)

#define dt_pushval(Type, Bytes, Value)                                         \
  do                                                                           \
  {                                                                            \
    size_t len         = sizeof(Type);                                         \
    const byte *buffer = ((void *) &(Value));                                  \
    dt_arrmaygrow(bytes, len);                                                 \
    for (size_t i = 0; i < len; ++i)                                           \
    {                                                                          \
      dt_arradd(bytes, buffer[i]);                                             \
    }                                                                          \
  } while (0)

#define dt_popval(Type, Bytes, Offset, Value)                                  \
  do                                                                           \
  {                                                                            \
    size_t len = sizeof(Type);                                                 \
    size_t res = 0llu;                                                         \
    for (size_t i = 0; i < len; ++i)                                           \
    {                                                                          \
      byte bval = bytes[*(Offset) + i];                                        \
      res |= (((size_t) bval) << (8 * i));                                     \
    }                                                                          \
    *(Offset) += len;                                                          \
    Value = (Type) res;                                                        \
  } while (0)

#endif //_DT_H

// -----------------------------------------------------------------------------

// include-only-once flag to include the datatag implementation
#ifdef DT_IMPLEMENTATION
#ifndef _DT_C
#define _DT_C

#ifndef DT_ASSERT
#define DT_ASSERT_WAS_UNDEFINED
#define DT_ASSERT(x) ((void) 0)
#endif

//
// dt_arr implementation
//

void *dt_arrgrowf(void *a, size_t elemsize, size_t addlen, size_t min_cap)
{
  (void) elemsize;
  dt_arrhead_t temp = {0}; // force debugging
  void *b;
  size_t min_len = dt_arrlen(a) + addlen;
  (void) sizeof(temp);

  if (min_len > min_cap)
  {
    min_cap = min_len;
  }

  if (min_cap <= dt_arrcap(a))
  {
    return a;
  }

  if (min_cap < 2 * dt_arrcap(a))
  {
    min_cap = 2 * dt_arrcap(a);
  }
  else if (min_cap < 4)
  {
    min_cap = 4;
  }

  b = _dt_realloc((a) ? dt_arrhead(a) : NULL,
                  elemsize * min_cap + sizeof(dt_arrhead_t));
  b = (char *) b + sizeof(dt_arrhead_t);
  if (a == NULL)
  {
    dt_arrhead(b)->len = 0;
    dt_arrhead(b)->tbl = 0;
    dt_arrhead(b)->tmp = 0;
  }
  dt_arrhead(b)->cap = min_cap;

  return b;
}

void *dt_arrtonulltermf(void *a, size_t vallen)
{
  size_t arrlen = dt_arrlen(a);
  if (a == NULL || arrlen == 0)
  {
    return NULL;
  }

  char *b = _dt_realloc(NULL, vallen * (arrlen + 1));
  _dt_memcpy(b, a, vallen * arrlen);
  _dt_memset(&(b)[vallen * arrlen], 0, vallen);
  return b;
}

void dt_arrfreef(void *a)
{
  _dt_free(dt_arrhead(a));
}

//
// dt_map hash table implementation
//

#if !defined(DT_HASHFUNC_SIPHAASH_2_4) || !defined(DT_HASHFUNC_JENKIN) ||      \
  !defined(DT_HASHFUNC_JENKIN_WANG) || !defined(DT_HASHFUNC_MURMUR) ||         \
  !defined(DT_HASHFUNC_FNV)
#define DT_HASHFUNC_FNV
#endif

#if defined(_WIN64) || defined(_LP64) || defined(__LP64__) ||                  \
  defined(__amd64__) || defined(__x86_64__) || defined(_M_AMD_64) ||           \
  defined(__aarch64__)
static const size_t _DT_FNV_PRIME  = 1099511628211llu;
static const size_t _DT_FNV_OFFSET = 14695981039346656037llu;
#else
#if defined(DT_HASHFUNC_SIPHAASH_2_4)
#undef DT_HASHFUNC_SIPHAASH_2_4
#define DT_HASHFUNC_FNV
#endif
static const size_t _DT_FNV_PRIME  = 16777619;
static const size_t _DT_FNV_OFFSET = 2166136261;
#endif

#ifndef DT_BUCKET_LENGTH
#define DT_BUCKET_LENGTH 8
#endif

#define DT_BUCKET_SHIFT (DT_BUCKET_LENGTH == 8 ? 3 : 2)
#define DT_BUCKET_MASK (DT_BUCKET_LENGTH - 1)
#define DT_CACHE_LINE_SIZE 64

#define DT_ALIGN_FWD(n, a) (((n) + (a) -1) & ~((a) -1))

typedef struct
{
  size_t hash[DT_BUCKET_LENGTH];
  ptrdiff_t index[DT_BUCKET_LENGTH];
} dt_hshbucket_t; // in 32-bit, this is one 64-byte cache line; in 64-bit,
                  // each array is one 64-byte cache line

typedef struct
{
  char *temp_key; // this MUST be the first field of the hash table
  size_t slot_count;
  size_t used_count;
  size_t used_count_threshold;
  size_t used_count_shrink_threshold;
  size_t tombstone_count;
  size_t tombstone_count_threshold;
  size_t seed;
  size_t slot_count_log2;
  dt_strarena_t string;
  dt_hshbucket_t *storage; // not a separate allocation, just 64-byte
                           // aligned storage after this struct
} dt_hshindex_t;

#define DT_INDEX_EMPTY -1
#define DT_INDEX_DELETED -2
#define DT_INDEX_IN_USE(x) ((x) >= 0)

#define DT_HASH_EMPTY 0
#define DT_HASH_DELETED 1

static size_t dt_hash_seed = 0x31415926;

void dt_rand_seed(size_t seed)
{
  dt_hash_seed = seed;
}

#define dt_load_32_or_64(var, temp, v32, v64_hi, v64_lo)                       \
  temp = v64_lo ^ v32, temp <<= 16, temp <<= 16, temp >>= 16,                  \
  temp >>= 16,                            /* discard if 32-bit */              \
    var = v64_hi, var <<= 16, var <<= 16, /* discard if 32-bit */              \
    var ^= temp ^ v32

#define DT_SIZE_T_BITS ((sizeof(size_t)) * 8)

static size_t
dt_probe_position(size_t hash, size_t slot_count, size_t slot_log2)
{
  (void) slot_log2;
  size_t pos;
  pos = hash & (slot_count - 1);
#ifdef DT_INTERNAL_BUCKET_START
  pos &= ~DT_BUCKET_MASK;
#endif
  return pos;
}

static size_t dt_log2(size_t slot_count)
{
  size_t n = 0;
  while (slot_count > 1)
  {
    slot_count >>= 1;
    ++n;
  }
  return n;
}

static dt_hshindex_t *dt_make_hash_index(size_t slot_count, dt_hshindex_t *ot)
{
  dt_hshindex_t *t;
  t = (dt_hshindex_t *) _dt_realloc(
    NULL,
    (slot_count >> DT_BUCKET_SHIFT) * sizeof(dt_hshbucket_t) +
      sizeof(dt_hshindex_t) + DT_CACHE_LINE_SIZE - 1);
  t->storage =
    (dt_hshbucket_t *) DT_ALIGN_FWD((size_t) (t + 1), DT_CACHE_LINE_SIZE);
  t->slot_count      = slot_count;
  t->slot_count_log2 = dt_log2(slot_count);
  t->tombstone_count = 0;
  t->used_count      = 0;

#if 0   // A1
  t->used_count_threshold        = slot_count*12/16; // if 12/16th of table is occupied, grow
  t->tombstone_count_threshold   = slot_count* 2/16; // if tombstones are 2/16th of table, rebuild
  t->used_count_shrink_threshold = slot_count* 4/16; // if table is only 4/16th full, shrink
#elif 1 // A2
        // t->used_count_threshold        = slot_count*12/16; // if 12/16th of
        // table is occupied, grow t->tombstone_count_threshold   = slot_count*
        // 3/16; // if tombstones are 3/16th of table, rebuild
        // t->used_count_shrink_threshold = slot_count* 4/16; // if table is
        // only 4/16th full, shrink

  // compute without overflowing
  t->used_count_threshold        = slot_count - (slot_count >> 2);
  t->tombstone_count_threshold   = (slot_count >> 3) + (slot_count >> 4);
  t->used_count_shrink_threshold = slot_count >> 2;

#elif 0 // B1
  t->used_count_threshold =
    slot_count * 13 / 16; // if 13/16th of table is occupied, grow
  t->tombstone_count_threshold =
    slot_count * 2 / 16; // if tombstones are 2/16th of table, rebuild
  t->used_count_shrink_threshold =
    slot_count * 5 / 16; // if table is only 5/16th full, shrink
#else   // C1
  t->used_count_threshold =
    slot_count * 14 / 16; // if 14/16th of table is occupied, grow
  t->tombstone_count_threshold =
    slot_count * 2 / 16; // if tombstones are 2/16th of table, rebuild
  t->used_count_shrink_threshold =
    slot_count * 6 / 16; // if table is only 6/16th full, shrink
#endif
  // Following statistics were measured on a Core i7-6700 @ 4.00Ghz,
  // compiled with clang 7.0.1 -O2 Note that the larger tables have high
  // variance as they were run fewer times
  //     A1            A2          B1           C1
  //    0.10ms :     0.10ms :     0.10ms :     0.11ms :      2,000 inserts
  //    creating 2K table 0.96ms :     0.95ms :     0.97ms :     1.04ms :
  //    20,000 inserts creating 20K table
  //   14.48ms :    14.46ms :    10.63ms :    11.00ms :    200,000 inserts
  //   creating 200K table
  //  195.74ms :   196.35ms :   203.69ms :   214.92ms :  2,000,000 inserts
  //  creating 2M table
  // 2193.88ms :  2209.22ms :  2285.54ms :  2437.17ms : 20,000,000 inserts
  // creating 20M table
  //   65.27ms :    53.77ms :    65.33ms :    65.47ms : 500,000 inserts &
  //   deletes in 2K table 72.78ms :    62.45ms :    71.95ms :    72.85ms
  //   : 500,000 inserts & deletes in 20K table 89.47ms :    77.72ms
  //   :    96.49ms :    96.75ms : 500,000 inserts & deletes in 200K table
  //   97.58ms :    98.14ms :    97.18ms :    97.53ms : 500,000 inserts &
  //   deletes in 2M table
  //  118.61ms :   119.62ms :   120.16ms :   118.86ms : 500,000 inserts &
  //  deletes in 20M table 192.11ms :   194.39ms :   196.38ms :   195.73ms
  //  : 500,000 inserts & deletes in 200M table

  if (slot_count <= DT_BUCKET_LENGTH)
    t->used_count_shrink_threshold = 0;
  // to avoid infinite loop, we need to guarantee that at least one slot
  // is empty and will terminate probes
  DT_ASSERT(t->used_count_threshold + t->tombstone_count_threshold <
            t->slot_count);
  if (ot)
  {
    t->string = ot->string;
    // reuse old seed so we can reuse old hashes so below "copy out old
    // data" doesn't do any hashing
    t->seed = ot->seed;
  }
  else
  {
    size_t a, b, temp;
    _dt_memset(&t->string, 0, sizeof(t->string));
    t->seed = dt_hash_seed;
    // LCG
    // in 32-bit, a =          2147001325   b =  715136305
    // in 64-bit, a = 2862933555777941757   b = 3037000493
    dt_load_32_or_64(a, temp, 2147001325, 0x27bb2ee6, 0x87b0b0fd);
    dt_load_32_or_64(b, temp, 715136305, 0, 0xb504f32d);
    dt_hash_seed = dt_hash_seed * a + b;
  }

  {
    size_t i, j;
    for (i = 0; i < slot_count >> DT_BUCKET_SHIFT; ++i)
    {
      dt_hshbucket_t *b = &t->storage[i];
      for (j = 0; j < DT_BUCKET_LENGTH; ++j)
        b->hash[j] = DT_HASH_EMPTY;
      for (j = 0; j < DT_BUCKET_LENGTH; ++j)
        b->index[j] = DT_INDEX_EMPTY;
    }
  }

  // copy out the old data, if any
  if (ot)
  {
    size_t i, j;
    t->used_count = ot->used_count;
    for (i = 0; i < ot->slot_count >> DT_BUCKET_SHIFT; ++i)
    {
      dt_hshbucket_t *ob = &ot->storage[i];
      for (j = 0; j < DT_BUCKET_LENGTH; ++j)
      {
        if (DT_INDEX_IN_USE(ob->index[j]))
        {
          size_t hash = ob->hash[j];
          size_t pos =
            dt_probe_position(hash, t->slot_count, t->slot_count_log2);
          size_t step = DT_BUCKET_LENGTH;
          for (;;)
          {
            size_t limit, z;
            dt_hshbucket_t *bucket;
            bucket = &t->storage[pos >> DT_BUCKET_SHIFT];

            for (z = pos & DT_BUCKET_MASK; z < DT_BUCKET_LENGTH; ++z)
            {
              if (bucket->hash[z] == 0)
              {
                bucket->hash[z]  = hash;
                bucket->index[z] = ob->index[j];
                goto done;
              }
            }

            limit = pos & DT_BUCKET_MASK;
            for (z = 0; z < limit; ++z)
            {
              if (bucket->hash[z] == 0)
              {
                bucket->hash[z]  = hash;
                bucket->index[z] = ob->index[j];
                goto done;
              }
            }

            pos += step; // quadratic probing
            step += DT_BUCKET_LENGTH;
            pos &= (t->slot_count - 1);
          }
        }
      done:;
      }
    }
  }

  return t;
}

#define DT_ROTATE_LEFT(val, n)                                                 \
  (((val) << (n)) | ((val) >> (DT_SIZE_T_BITS - (n))))
#define DT_ROTATE_RIGHT(val, n)                                                \
  (((val) >> (n)) | ((val) << (DT_SIZE_T_BITS - (n))))

size_t dt_hash_string(char *str, size_t seed)
{
  size_t hash = seed;
  while (*str)
    hash = DT_ROTATE_LEFT(hash, 9) + (byte) *str++;

  // Thomas Wang 64-to-32 bit mix function, hopefully also works in 32
  // bits
  hash ^= seed;
  hash = (~hash) + (hash << 18);
  hash ^= hash ^ DT_ROTATE_RIGHT(hash, 31);
  hash = hash * 21;
  hash ^= hash ^ DT_ROTATE_RIGHT(hash, 11);
  hash += (hash << 6);
  hash ^= DT_ROTATE_RIGHT(hash, 22);
  return hash + seed;
}

#ifdef DT_HASHFUNC_SIPHAASH_2_4
#define DT_SIPHASH_C_ROUNDS 2
#define DT_SIPHASH_D_ROUNDS 4
typedef int DT_HASHFUNC_SIPHAASH_2_4_can_only_be_used_in_64_bit_builds
  [sizeof(size_t) == 8 ? 1 : -1];
#endif

#ifndef DT_SIPHASH_C_ROUNDS
#define DT_SIPHASH_C_ROUNDS 1
#endif
#ifndef DT_SIPHASH_D_ROUNDS
#define DT_SIPHASH_D_ROUNDS 1
#endif

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4127) // conditional expression is constant, for
                                // do..while(0) and sizeof()==
#endif

static size_t dt_siphash_bytes(void *p, size_t len, size_t seed)
{
  byte *d = (byte *) p;
  size_t i, j;
  size_t v0, v1, v2, v3, data;

  // hash that works on 32- or 64-bit registers without knowing which we
  // have (computes different results on 32-bit and 64-bit platform)
  // derived from siphash, but on 32-bit platforms very different as it
  // uses 4 32-bit state not 4 64-bit
  v0 = ((((size_t) 0x736f6d65 << 16) << 16) + 0x70736575) ^ seed;
  v1 = ((((size_t) 0x646f7261 << 16) << 16) + 0x6e646f6d) ^ ~seed;
  v2 = ((((size_t) 0x6c796765 << 16) << 16) + 0x6e657261) ^ seed;
  v3 = ((((size_t) 0x74656462 << 16) << 16) + 0x79746573) ^ ~seed;

#ifdef DT_TEST_SIPHASH_2_4
  // hardcoded with key material in the siphash test vectors
  v0 ^= 0x0706050403020100ull ^ seed;
  v1 ^= 0x0f0e0d0c0b0a0908ull ^ ~seed;
  v2 ^= 0x0706050403020100ull ^ seed;
  v3 ^= 0x0f0e0d0c0b0a0908ull ^ ~seed;
#endif

#define DT_SIPROUND()                                                          \
  do                                                                           \
  {                                                                            \
    v0 += v1;                                                                  \
    v1 = DT_ROTATE_LEFT(v1, 13);                                               \
    v1 ^= v0;                                                                  \
    v0 = DT_ROTATE_LEFT(v0, DT_SIZE_T_BITS / 2);                               \
    v2 += v3;                                                                  \
    v3 = DT_ROTATE_LEFT(v3, 16);                                               \
    v3 ^= v2;                                                                  \
    v2 += v1;                                                                  \
    v1 = DT_ROTATE_LEFT(v1, 17);                                               \
    v1 ^= v2;                                                                  \
    v2 = DT_ROTATE_LEFT(v2, DT_SIZE_T_BITS / 2);                               \
    v0 += v3;                                                                  \
    v3 = DT_ROTATE_LEFT(v3, 21);                                               \
    v3 ^= v0;                                                                  \
  } while (0)

  for (i = 0; i + sizeof(size_t) <= len;
       i += sizeof(size_t), d += sizeof(size_t))
  {
    data = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
    data |= (size_t) (d[4] | (d[5] << 8) | (d[6] << 16) | (d[7] << 24))
            << 16 << 16; // discarded if size_t == 4

    v3 ^= data;
    for (j = 0; j < DT_SIPHASH_C_ROUNDS; ++j)
      DT_SIPROUND();
    v0 ^= data;
  }
  data = len << (DT_SIZE_T_BITS - 8);
  switch (len - i)
  {
    case 7:
      data |= ((size_t) d[6] << 24) << 24; // fall through
    case 6:
      data |= ((size_t) d[5] << 20) << 20; // fall through
    case 5:
      data |= ((size_t) d[4] << 16) << 16; // fall through
    case 4:
      data |= (d[3] << 24); // fall through
    case 3:
      data |= (d[2] << 16); // fall through
    case 2:
      data |= (d[1] << 8); // fall through
    case 1:
      data |= d[0]; // fall through
    case 0:
      break;
  }
  v3 ^= data;
  for (j = 0; j < DT_SIPHASH_C_ROUNDS; ++j)
    DT_SIPROUND();
  v0 ^= data;
  v2 ^= 0xff;
  for (j = 0; j < DT_SIPHASH_D_ROUNDS; ++j)
    DT_SIPROUND();

#ifdef DT_HASHFUNC_SIPHAASH_2_4
  return v0 ^ v1 ^ v2 ^ v3;
#else
  return v1 ^ v2 ^ v3;   // slightly stronger since v0^v3 in above cancels out
                         // final round operation? I tweeted at the authors of
                         // SipHash about this but they didn't reply
#endif
}

// -----------------------------------------------------------------------------

size_t dt_hash_bytes(void *p, size_t len, size_t seed)
{
#ifdef DT_HASHFUNC_SIPHAASH_2_4
  return dt_siphash_bytes(p, len, seed);
#else
  byte *d = (byte *) p;

  if (len == 4)
  {
    unsigned int hash = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
#ifdef DT_HASHFUNC_JENKIN
    // Bob Jenkin's hash function w/o large constants
    hash ^= seed;
    hash -= (hash << 6);
    hash ^= (hash >> 17);
    hash -= (hash << 9);
    hash ^= seed;
    hash ^= (hash << 4);
    hash -= (hash << 3);
    hash ^= (hash << 10);
    hash ^= (hash >> 15);
#endif
#ifdef DT_HASHFUNC_JENKIN_WANG
    // Bob Jenkin's presumably-accidental version of Thomas Wang
    // hash with rotates turned into shifts. Note that converting these
    // back to rotates makes it run a lot slower, presumably due to
    // collisions, so I'm not really sure what's going on.
    hash ^= seed;
    hash = (hash ^ 61) ^ (hash >> 16);
    hash = hash + (hash << 3);
    hash = hash ^ (hash >> 4);
    hash = hash * 0x27d4eb2d;
    hash ^= seed;
    hash = hash ^ (hash >> 15);
#endif
#ifdef DT_HASHFUNC_MURMUR
    // Murmur3
    hash ^= seed;
    hash *= 0xcc9e2d51;
    hash = (hash << 17) | (hash >> 15);
    hash *= 0x1b873593;
    hash ^= seed;
    hash = (hash << 19) | (hash >> 13);
    hash = hash * 5 + 0xe6546b64;
    hash ^= hash >> 16;
    hash *= 0x85ebca6b;
    hash ^= seed;
    hash ^= hash >> 13;
    hash *= 0xc2b2ae35;
    hash ^= hash >> 16;
#endif
#ifdef DT_HASHFUNC_FNV
    // Danny's Numerically Optimized Fowler–Noll–Vo
    hash ^= seed;
    hash *= _DT_FNV_OFFSET;
    hash *= _DT_FNV_PRIME;
    hash ^= (hash << 0);
    hash *= _DT_FNV_PRIME;
    hash ^= (hash << 8);
    hash *= _DT_FNV_PRIME;
    hash ^= (hash << 16);
    hash *= _DT_FNV_PRIME;
    hash ^= (hash << 24);
#endif
    // Following statistics were measured on a Core i7-6700 @ 4.00Ghz,
    // compiled with clang 7.0.1 -O2 Note that the larger tables have
    // high variance as they were run fewer times
    //  HASH32-A   //  HASH32-BB  //  HASH32-C
    //    0.10ms   //    0.10ms   //    0.10ms :      2,000 inserts
    //    creating 2K table 0.96ms   //    0.95ms   //    0.99ms :
    //    20,000 inserts creating 20K table
    //   14.69ms   //   14.43ms   //   14.97ms :    200,000 inserts
    //   creating 200K table
    //  199.99ms   //  195.36ms   //  202.05ms :  2,000,000 inserts
    //  creating 2M table
    // 2234.84ms   // 2187.74ms   // 2240.38ms : 20,000,000 inserts
    // creating 20M table
    //   55.68ms   //   53.72ms   //   57.31ms : 500,000 inserts &
    //   deletes in 2K table 63.43ms   //   61.99ms   //   65.73ms :
    //   500,000 inserts & deletes in 20K table 80.04ms   //   77.96ms
    //   //   81.83ms : 500,000 inserts & deletes in 200K table
    //  100.42ms   //   97.40ms   //  102.39ms : 500,000 inserts &
    //  deletes in 2M table 119.71ms   //  120.59ms   //  121.63ms :
    //  500,000 inserts & deletes in 20M table 185.28ms   //  195.15ms
    //  //  187.74ms : 500,000 inserts & deletes in 200M table
    //   15.58ms   //   14.79ms   //   15.52ms : 200,000 inserts
    //   creating 200K table with varying key spacing

    return (((size_t) hash << 16 << 16) | hash) ^ seed;
  }
  else if (len == 8 && sizeof(size_t) == 8)
  {
    size_t hash = d[0] | (d[1] << 8) | (d[2] << 16) | (d[3] << 24);
    hash |= (size_t) (d[4] | (d[5] << 8) | (d[6] << 16) | (d[7] << 24))
            << 16 << 16; // avoid warning if size_t == 4
    hash ^= seed;
    hash = (~hash) + (hash << 21);
    hash ^= DT_ROTATE_RIGHT(hash, 24);
    hash *= 265;
    hash ^= DT_ROTATE_RIGHT(hash, 14);
    hash ^= seed;
    hash *= 21;
    hash ^= DT_ROTATE_RIGHT(hash, 28);
    hash += (hash << 31);
    hash = (~hash) + (hash << 18);
    return hash;
  }
  else
  {
    return dt_siphash_bytes(p, len, seed);
  }
#endif
}
#ifdef _MSC_VER
#pragma warning(pop)
#endif

static int dt_is_key_equal(void *a,
                           size_t elemsize,
                           void *key,
                           size_t keysize,
                           size_t keyoffset,
                           int mode,
                           size_t i)
{
  if (mode >= DT_MAP_STRING)
    return 0 == strcmp((char *) key,
                       *(char **) ((char *) a + elemsize * i + keyoffset));
  else
    return 0 == _dt_memcmp(key, (char *) a + elemsize * i + keyoffset, keysize);
}

#define DT_HASH_TO_ARR(x, elemsize) ((char *) (x) - (elemsize))
#define DT_ARR_TO_HASH(x, elemsize) ((char *) (x) + (elemsize))

#define dt_hash_table(a) ((dt_hshindex_t *) dt_arrhead(a)->tbl)

void dt_mapfree_impl(void *a, size_t elemsize)
{
  if (a == NULL)
    return;
  if (dt_hash_table(a) != NULL)
  {
    if (dt_hash_table(a)->string.mode == DT_SMP_STRDUP)
    {
      size_t i;
      // skip 0th element, which is default
      for (i = 1; i < dt_arrhead(a)->len; ++i)
        _dt_free(*(char **) ((char *) a + elemsize * i));
    }
    dt_strreset(&dt_hash_table(a)->string);
  }
  _dt_free(dt_arrhead(a)->tbl);
  _dt_free(dt_arrhead(a));
}

static ptrdiff_t dt_map_find_slot(void *a,
                                  size_t elemsize,
                                  void *key,
                                  size_t keysize,
                                  size_t keyoffset,
                                  int mode)
{
  void *raw_a          = DT_HASH_TO_ARR(a, elemsize);
  dt_hshindex_t *table = dt_hash_table(raw_a);
  size_t hash          = mode >= DT_MAP_STRING
                           ? dt_hash_string((char *) key, table->seed)
                           : dt_hash_bytes(key, keysize, table->seed);
  size_t step          = DT_BUCKET_LENGTH;
  size_t limit, i;
  size_t pos;
  dt_hshbucket_t *bucket;

  if (hash < 2)
    hash += 2; // stored hash values are forbidden from being 0, so we
               // can detect empty slots

  pos = dt_probe_position(hash, table->slot_count, table->slot_count_log2);

  for (;;)
  {
    bucket = &table->storage[pos >> DT_BUCKET_SHIFT];

    // start searching from pos to end of bucket, this should help
    // performance on small hash tables that fit in cache
    for (i = pos & DT_BUCKET_MASK; i < DT_BUCKET_LENGTH; ++i)
    {
      if (bucket->hash[i] == hash)
      {
        if (dt_is_key_equal(
              a, elemsize, key, keysize, keyoffset, mode, bucket->index[i]))
        {
          return (pos & ~DT_BUCKET_MASK) + i;
        }
      }
      else if (bucket->hash[i] == DT_HASH_EMPTY)
      {
        return -1;
      }
    }

    // search from beginning of bucket to pos
    limit = pos & DT_BUCKET_MASK;
    for (i = 0; i < limit; ++i)
    {
      if (bucket->hash[i] == hash)
      {
        if (dt_is_key_equal(
              a, elemsize, key, keysize, keyoffset, mode, bucket->index[i]))
        {
          return (pos & ~DT_BUCKET_MASK) + i;
        }
      }
      else if (bucket->hash[i] == DT_HASH_EMPTY)
      {
        return -1;
      }
    }

    // quadratic probing
    pos += step;
    step += DT_BUCKET_LENGTH;
    pos &= (table->slot_count - 1);
  }
  /* NOTREACHED */
}

void *dt_mapget_key_ts(void *a,
                       size_t elemsize,
                       void *key,
                       size_t keysize,
                       ptrdiff_t *temp,
                       int mode)
{
  size_t keyoffset = 0;
  if (a == NULL)
  {
    // make it non-empty so we can return a temp
    a = dt_arrgrowf(0, elemsize, 0, 1);
    dt_arrhead(a)->len += 1;
    _dt_memset(a, 0, elemsize);
    *temp = DT_INDEX_EMPTY;
    // adjust a to point after the default element
    return DT_ARR_TO_HASH(a, elemsize);
  }
  else
  {
    dt_hshindex_t *table;
    void *raw_a = DT_HASH_TO_ARR(a, elemsize);
    // adjust a to point to the default element
    table = (dt_hshindex_t *) dt_arrhead(raw_a)->tbl;
    if (table == 0)
    {
      *temp = -1;
    }
    else
    {
      ptrdiff_t slot =
        dt_map_find_slot(a, elemsize, key, keysize, keyoffset, mode);
      if (slot < 0)
      {
        *temp = DT_INDEX_EMPTY;
      }
      else
      {
        dt_hshbucket_t *b = &table->storage[slot >> DT_BUCKET_SHIFT];
        *temp             = b->index[slot & DT_BUCKET_MASK];
      }
    }
    return a;
  }
}

void *
dt_mapget_key(void *a, size_t elemsize, void *key, size_t keysize, int mode)
{
  ptrdiff_t temp;
  void *p = dt_mapget_key_ts(a, elemsize, key, keysize, &temp, mode);
  dt_temp(DT_HASH_TO_ARR(p, elemsize)) = temp;
  return p;
}

void *dt_mapput_default(void *a, size_t elemsize)
{
  // three cases:
  //   a is NULL <- allocate
  //   a has a hash table but no entries, because of shmode <- grow
  //   a has entries <- do nothing
  if (a == NULL || dt_arrhead(DT_HASH_TO_ARR(a, elemsize))->len == 0)
  {
    a = dt_arrgrowf(a ? DT_HASH_TO_ARR(a, elemsize) : NULL, elemsize, 0, 1);
    dt_arrhead(a)->len += 1;
    _dt_memset(a, 0, elemsize);
    a = DT_ARR_TO_HASH(a, elemsize);
  }
  return a;
}

static char *dt_strdup(char *str);

void *
dt_mapput_key(void *a, size_t elemsize, void *key, size_t keysize, int mode)
{
  size_t keyoffset = 0;
  void *raw_a;
  dt_hshindex_t *table;

  if (a == NULL)
  {
    a = dt_arrgrowf(0, elemsize, 0, 1);
    _dt_memset(a, 0, elemsize);
    dt_arrhead(a)->len += 1;
    // adjust a to point AFTER the default element
    a = DT_ARR_TO_HASH(a, elemsize);
  }

  // adjust a to point to the default element
  raw_a = a;
  a     = DT_HASH_TO_ARR(a, elemsize);

  table = (dt_hshindex_t *) dt_arrhead(a)->tbl;

  if (table == NULL || table->used_count >= table->used_count_threshold)
  {
    dt_hshindex_t *nt;
    size_t slot_count;

    slot_count = (table == NULL) ? DT_BUCKET_LENGTH : table->slot_count * 2;
    nt         = dt_make_hash_index(slot_count, table);
    if (table)
      _dt_free(table);
    else
      nt->string.mode = mode >= DT_MAP_STRING ? DT_SMP_DEFAULT : 0;
    dt_arrhead(a)->tbl = table = nt;
  }

  // we iterate hash table explicitly because we want to track if we saw a
  // tombstone
  {
    size_t hash = mode >= DT_MAP_STRING
                    ? dt_hash_string((char *) key, table->seed)
                    : dt_hash_bytes(key, keysize, table->seed);
    size_t step = DT_BUCKET_LENGTH;
    size_t pos;
    ptrdiff_t tombstone = -1;
    dt_hshbucket_t *bucket;

    // stored hash values are forbidden from being 0, so we can detect
    // empty slots to early out quickly
    if (hash < 2)
      hash += 2;

    pos = dt_probe_position(hash, table->slot_count, table->slot_count_log2);

    for (;;)
    {
      size_t limit, i;
      bucket = &table->storage[pos >> DT_BUCKET_SHIFT];

      // start searching from pos to end of bucket
      for (i = pos & DT_BUCKET_MASK; i < DT_BUCKET_LENGTH; ++i)
      {
        if (bucket->hash[i] == hash)
        {
          if (dt_is_key_equal(raw_a,
                              elemsize,
                              key,
                              keysize,
                              keyoffset,
                              mode,
                              bucket->index[i]))
          {
            dt_temp(a) = bucket->index[i];
            if (mode >= DT_MAP_STRING)
              dt_temp_key(a) =
                *(char **) ((char *) raw_a + elemsize * bucket->index[i] +
                            keyoffset);
            return DT_ARR_TO_HASH(a, elemsize);
          }
        }
        else if (bucket->hash[i] == 0)
        {
          pos = (pos & ~DT_BUCKET_MASK) + i;
          goto found_empty_slot;
        }
        else if (tombstone < 0)
        {
          if (bucket->index[i] == DT_INDEX_DELETED)
            tombstone = (ptrdiff_t) ((pos & ~DT_BUCKET_MASK) + i);
        }
      }

      // search from beginning of bucket to pos
      limit = pos & DT_BUCKET_MASK;
      for (i = 0; i < limit; ++i)
      {
        if (bucket->hash[i] == hash)
        {
          if (dt_is_key_equal(raw_a,
                              elemsize,
                              key,
                              keysize,
                              keyoffset,
                              mode,
                              bucket->index[i]))
          {
            dt_temp(a) = bucket->index[i];
            return DT_ARR_TO_HASH(a, elemsize);
          }
        }
        else if (bucket->hash[i] == 0)
        {
          pos = (pos & ~DT_BUCKET_MASK) + i;
          goto found_empty_slot;
        }
        else if (tombstone < 0)
        {
          if (bucket->index[i] == DT_INDEX_DELETED)
            tombstone = (ptrdiff_t) ((pos & ~DT_BUCKET_MASK) + i);
        }
      }

      // quadratic probing
      pos += step;
      step += DT_BUCKET_LENGTH;
      pos &= (table->slot_count - 1);
    }
  found_empty_slot:
    if (tombstone >= 0)
    {
      pos = tombstone;
      --table->tombstone_count;
    }
    ++table->used_count;

    {
      ptrdiff_t i = (ptrdiff_t) dt_arrlen(a);
      // we want to do dt_arraddn(1), but we can't use the macros
      // since we don't have something of the right type
      if ((size_t) i + 1 > dt_arrcap(a))
        *(void **) &a = dt_arrgrowf(a, elemsize, 1, 0);
      raw_a = DT_ARR_TO_HASH(a, elemsize);

      DT_ASSERT((size_t) i + 1 <= dt_arrcap(a));
      dt_arrhead(a)->len = i + 1;
      bucket             = &table->storage[pos >> DT_BUCKET_SHIFT];
      bucket->hash[pos & DT_BUCKET_MASK]  = hash;
      bucket->index[pos & DT_BUCKET_MASK] = i - 1;
      dt_temp(a)                          = i - 1;

      switch (table->string.mode)
      {
        case DT_SMP_STRDUP:
          dt_temp_key(a) = *(char **) ((char *) a + elemsize * i) =
            dt_strdup((char *) key);
          break;
        case DT_SMP_ARENA:
          dt_temp_key(a) = *(char **) ((char *) a + elemsize * i) =
            dt_stralloc(&table->string, (char *) key);
          break;
        case DT_SMP_DEFAULT:
          dt_temp_key(a) = *(char **) ((char *) a + elemsize * i) =
            (char *) key;
          break;
        default:
          _dt_memcpy((char *) a + elemsize * i, key, keysize);
          break;
      }
    }
    return DT_ARR_TO_HASH(a, elemsize);
  }
}

void *dt_smpmode_impl(size_t elemsize, int mode)
{
  void *a = dt_arrgrowf(0, elemsize, 0, 1);
  dt_hshindex_t *h;
  _dt_memset(a, 0, elemsize);
  dt_arrhead(a)->len = 1;
  dt_arrhead(a)->tbl = h =
    (dt_hshindex_t *) dt_make_hash_index(DT_BUCKET_LENGTH, NULL);
  h->string.mode = (byte) mode;
  return DT_ARR_TO_HASH(a, elemsize);
}

void *dt_mapdel_key(void *a,
                    size_t elemsize,
                    void *key,
                    size_t keysize,
                    size_t keyoffset,
                    int mode)
{
  if (a == NULL)
  {
    return 0;
  }
  else
  {
    dt_hshindex_t *table;
    void *raw_a    = DT_HASH_TO_ARR(a, elemsize);
    table          = (dt_hshindex_t *) dt_arrhead(raw_a)->tbl;
    dt_temp(raw_a) = 0;
    if (table == 0)
    {
      return a;
    }
    else
    {
      ptrdiff_t slot;
      slot = dt_map_find_slot(a, elemsize, key, keysize, keyoffset, mode);
      if (slot < 0)
        return a;
      else
      {
        dt_hshbucket_t *b   = &table->storage[slot >> DT_BUCKET_SHIFT];
        int i               = slot & DT_BUCKET_MASK;
        ptrdiff_t old_index = b->index[i];
        ptrdiff_t final_index =
          (ptrdiff_t) dt_arrlen(raw_a) - 1 - 1; // minus one for the raw_a vs
                                                // a, and minus one for 'last'
        DT_ASSERT(slot < (ptrdiff_t) table->slot_count);
        --table->used_count;
        ++table->tombstone_count;
        dt_temp(raw_a) = 1;
        DT_ASSERT(table->used_count >= 0);
        // DT_ASSERT(table->tombstone_count < table->slot_count/4);
        b->hash[i]  = DT_HASH_DELETED;
        b->index[i] = DT_INDEX_DELETED;

        if (mode == DT_MAP_STRING && table->string.mode == DT_SMP_STRDUP)
          _dt_free(*(char **) ((char *) a + elemsize * old_index));

        // if indices are the same, _dt_memcpy is a no-op, but
        // back-pointer-fixup will fail, so skip
        if (old_index != final_index)
        {
          // swap delete
          memmove((char *) a + elemsize * old_index,
                  (char *) a + elemsize * final_index,
                  elemsize);

          // now find the slot for the last element
          if (mode == DT_MAP_STRING)
            slot = dt_map_find_slot(
              a,
              elemsize,
              *(char **) ((char *) a + elemsize * old_index + keyoffset),
              keysize,
              keyoffset,
              mode);
          else
            slot =
              dt_map_find_slot(a,
                               elemsize,
                               (char *) a + elemsize * old_index + keyoffset,
                               keysize,
                               keyoffset,
                               mode);
          DT_ASSERT(slot >= 0);
          b = &table->storage[slot >> DT_BUCKET_SHIFT];
          i = slot & DT_BUCKET_MASK;
          DT_ASSERT(b->index[i] == final_index);
          b->index[i] = old_index;
        }
        dt_arrhead(raw_a)->len -= 1;

        if (table->used_count < table->used_count_shrink_threshold &&
            table->slot_count > DT_BUCKET_LENGTH)
        {
          dt_arrhead(raw_a)->tbl =
            dt_make_hash_index(table->slot_count >> 1, table);
          _dt_free(table);
        }
        else if (table->tombstone_count > table->tombstone_count_threshold)
        {
          dt_arrhead(raw_a)->tbl = dt_make_hash_index(table->slot_count, table);
          _dt_free(table);
        }

        return a;
      }
    }
  }
  /* NOTREACHED */
}

static char *dt_strdup(char *str)
{
  // to keep replaceable allocator simple, we don't want to use strdup.
  // rolling our own also avoids problem of strdup vs _strdup
  size_t len = strlen(str) + 1;
  char *p    = (char *) _dt_realloc(0, len);
  memmove(p, str, len);
  return p;
}

#ifndef DT_STRING_ARENA_BLOCKSIZE_MIN
#define DT_STRING_ARENA_BLOCKSIZE_MIN 512u
#endif
#ifndef DT_STRING_ARENA_BLOCKSIZE_MAX
#define DT_STRING_ARENA_BLOCKSIZE_MAX (1u << 20)
#endif

char *dt_stralloc(dt_strarena_t *a, char *str)
{
  char *p;
  size_t len = strlen(str) + 1;
  if (len > a->remaining)
  {
    // compute the next blocksize
    size_t blocksize = a->block;

    // size is 512, 512, 1024, 1024, 2048, 2048, 4096, 4096, etc., so
    // that there are log(SIZE) allocations to free when we destroy the
    // table
    blocksize = (size_t) (DT_STRING_ARENA_BLOCKSIZE_MIN) << (blocksize >> 1);

    // if size is under 1M, advance to next blocktype
    if (blocksize < (size_t) (DT_STRING_ARENA_BLOCKSIZE_MAX))
      ++a->block;

    if (len > blocksize)
    {
      // if string is larger than blocksize, then just allocate the
      // full size. note that we still advance string_block so block
      // size will continue increasing, so e.g. if somebody only calls
      // this with 1000-long strings, eventually the arena will start
      // doubling and handling those as well
      dt_strblock_t *sb =
        (dt_strblock_t *) _dt_realloc(0, sizeof(*sb) - 8 + len);
      memmove(sb->storage, str, len);
      if (a->storage)
      {
        // insert it after the first element, so that we don't waste
        // the space there
        sb->next         = a->storage->next;
        a->storage->next = sb;
      }
      else
      {
        sb->next     = 0;
        a->storage   = sb;
        a->remaining = 0; // this is redundant, but good for clarity
      }
      return sb->storage;
    }
    else
    {
      dt_strblock_t *sb =
        (dt_strblock_t *) _dt_realloc(0, sizeof(*sb) - 8 + blocksize);
      sb->next     = a->storage;
      a->storage   = sb;
      a->remaining = blocksize;
    }
  }

  DT_ASSERT(len <= a->remaining);
  p = a->storage->storage + a->remaining - len;
  a->remaining -= len;
  memmove(p, str, len);
  return p;
}

void dt_strreset(dt_strarena_t *a)
{
  dt_strblock_t *x, *y;
  x = a->storage;
  while (x)
  {
    y = x->next;
    _dt_free(x);
    x = y;
  }
  _dt_memset(a, 0, sizeof(*a));
}

// -----------------------------------------------------------------------------

extern size_t memlen(const void *buffer, const size_t elem_len)
{
  if (!buffer)
  {
    return 0;
  }
  byte *zero = (byte *) _dt_calloc(1, elem_len);
  _dt_memset(zero, 0, elem_len);
  size_t i      = 0;
  byte *rawdata = (byte *) buffer;
  while (_dt_memcmp(rawdata + (elem_len * i), zero, elem_len) != 0)
  {
    i++;
  }
  _dt_free(zero);
  return i;
}

// -----------------------------------------------------------------------------

dt_node *dt_loadf(const char *filepath)
{
  FILE *file = fopen(filepath, "rb");
  if (file == NULL)
  {
    fprintf(stderr, "ERROR: Could not read file '%s'\n", filepath);
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  size_t len = ftell(file);
  fseek(file, 0, SEEK_SET);
  uint8_t *data = (uint8_t *) malloc(len);
  if (data == NULL)
  {
    fclose(file);
    return NULL;
  }
  size_t bytesRead = fread(data, 1, len, file);
  if (bytesRead != len)
  {
    fprintf(stderr, "ERROR: Could not read entire file '%s'\n", filepath);
    free(data);
    fclose(file);
    return NULL;
  }
  dt_node *node = dt_loads((char *) data);
  fclose(file);
  return node;
}

dt_node *dt_loads(const char *string)
{
  size_t offset = 0;
  dt_node *res  = dt_loads_impl(string, &offset);
  return res;
}

dt_node *dt_loads_impl(const char *string, size_t *offset)
{
  dt_assert(string, "String is null", string, *offset);
  dt_test(string[*offset], "Unexpected end of string", string, *offset);
  dt_loads_comment(string, offset);
#define X(NAME, ...)                                                           \
  if (dt_test_##NAME(string, offset))                                          \
  {                                                                            \
    dt_loads_comment(string, offset);                                          \
    dt_node *res = dt_loads_##NAME(string, offset);                            \
    dt_loads_comment(string, offset);                                          \
    return res;                                                                \
  }
  DT_TYPES_LIST
#undef X
  dt_test(false, "Unknown type", string, *offset);
  return NULL;
}

// -----------------------------------------------------------------------------

void (*_dt_free_ptrs[dt_type_count])(dt_node *) = {
#define X(NAME, ...) dt_free_##NAME,
  DT_TYPES_LIST
#undef X
};

void dt_free(dt_node *node)
{
  if (node == NULL)
  {
    return;
  }
  if (node->type < 0 || node->type >= dt_type_count)
  {
    fprintf(stderr, "ERROR: Unknown node type");
    exit(1);
    return;
  }
  _dt_free_ptrs[node->type](node);
}

// -----------------------------------------------------------------------------

char *(*_dt_dumps_ptrs[dt_type_count])(const dt_node *,
                                       const dt_dumps_settings_t *) = {
#define X(NAME, ...) dt_dumps_##NAME,
  DT_TYPES_LIST
#undef X
};

bool dt_dumpf(const dt_node *node, const char *filepath)
{
  FILE *file = fopen(filepath, "wb");
  if (file == NULL)
  {
    fprintf(stderr, "ERROR: Could not write to file '%s'\n", filepath);
    return false;
  }
  char *data = dt_dumps(node);
  size_t len    = strlen(data);
  fwrite(data, sizeof(char), len, file);
  fclose(file);
  return true;
}

char *dt_dumps(const dt_node *node)
{
  return dt_dumps_ex(node, &_dt_dumps_settings_default);
}

extern char *dt_dumps_ex(const dt_node *node, const dt_dumps_settings_t *set)
{
  if (node == NULL)
  {
    return NULL;
  }
  if (node->type < 0 || node->type >= dt_type_count)
  {
    fprintf(stderr, "ERROR: Unknown node type");
    exit(1);
    return NULL;
  }
  return _dt_dumps_ptrs[node->type](node, set);
}
// -----------------------------------------------------------------------------

char *dt_loads_raw_string(const char *string, size_t *offset)
{
  size_t start = *offset;
  size_t end   = start;
  if (string[end] == '"')
  {
    start++;
    end++;

    bool escaped = false;
    while (string[end] && string[end] != '"')
    {
      if (escaped)
      {
        escaped = false;
      }
      if (string[end] == '\\')
      {
        escaped = true;
      }
      end++;
    }
    *offset += 2;
  }
  else
  {
    while (string[end] && !isendchar(string[end]) && !isspace(string[end]))
    {
      end++;
    }
  }

  size_t len = end - start;
  char *res0 = _dt_malloc(len + 1);
  _dt_memcpy(res0, string + start, len);
  res0[len] = '\0';
  char *res = strunesc(res0);
  _dt_free(res0);
  *offset += len;
  return res;
}

char *dt_dumps_raw_string(const char *string, const dt_dumps_settings_t *set)
{
  char *res     = NULL;
  char *escrepr = stresc(string);
  if (islongstring(string) || set->force_json)
  {
    size_t qlen = strlen(escrepr) + 2;
    res         = (char *) _dt_malloc(qlen + 1);
    if (res == NULL)
    {
      _dt_free(escrepr);
      exit(1);
    }
    res[0] = '"';
    _dt_memcpy(&(res)[1], escrepr, qlen - 2);
    res[qlen - 1] = '"';
    res[qlen]     = '\0';
    _dt_free(escrepr);
  }
  else
  {
    res = escrepr;
  }
  return res;
}

// -----------------------------------------------------------------------------

dt_node *(*_dt_loadb_ptrs[dt_type_count])(const size_t,
                                          const byte *,
                                          size_t *) = {
#define X(NAME, ...) dt_loadb_##NAME,
  DT_TYPES_LIST
#undef X
};

dt_node *dt_loadb(const size_t len, const byte *bytes)
{
  dt_test(len > 3, "Invalid binary header", (char *) bytes, 0);
  dt_test(bytes[0] == 'd' && bytes[1] == 't',
          "Invalid binary header",
          (char *) bytes,
          0);
  byte version = bytes[3];
  (void) version;
  size_t offset = 3;
  return dt_loadb_impl(len, bytes, &offset);
}

dt_node *dt_loadb_impl(const size_t len, const byte *bytes, size_t *offset)
{
  dt_type type;
  dt_popval(byte, bytes, offset, type);
  dt_test(
    type >= 0 && type < dt_type_count, "Invalid type", (char *) bytes, *offset);
  return _dt_loadb_ptrs[type](len, bytes, offset);
}

// -----------------------------------------------------------------------------

void (*_dt_dumpb_ptrs[dt_type_count])(const dt_node *, byte *) = {
#define X(NAME, ...) dt_dumpb_##NAME,
  DT_TYPES_LIST
#undef X
};

byte *dt_dumpb(const dt_node *node, size_t *len)
{
  if (node == NULL)
  {
    return NULL;
  }
  byte *bytes = NULL;
  dt_arrsetcap(bytes, 1024);
  dt_arradd(bytes, 'd');
  dt_arradd(bytes, 't');
  dt_arradd(bytes, 0);
  dt_dumpb_impl(node, bytes);
  *len += dt_arrlenu(bytes);
  return bytes;
}

void dt_dumpb_impl(const dt_node *node, byte *bytes)
{
  byte type = node->type;
  dt_pushval(byte, bytes, type);
  _dt_dumpb_ptrs[node->type](node, bytes);
}

// -----------------------------------------------------------------------------

char *dt_loadb_raw_string(const size_t len, const byte *bytes, size_t *offset)
{
  dt_test(len > *offset, "Not enough bytes", (char *) bytes, *offset);
  size_t slen;
  dt_popval(size_t, bytes, offset, slen);
  char *string = _dt_realloc(NULL, slen + 1);
  _dt_memcpy(string, &(bytes)[*offset], slen);
  string[slen]   = '\0';
  char *unescstr = strunesc(string);
  free(string);
  *offset += slen;
  return unescstr;
}

void dt_dumpb_raw_string(const char *string, byte *bytes)
{
  char *escstr = stresc(string);
  size_t slen  = strlen(escstr);
  dt_arrmaygrow(bytes, sizeof(size_t) + slen);
  dt_pushval(size_t, bytes, slen);
  dt_arraddbytes(bytes, escstr, slen);
  free(escstr);
}

// -----------------------------------------------------------------------------

void dt_loads_space(const char *string, size_t *offset)
{
  if (!string[*offset] || !isspace(string[*offset]))
  {
    return;
  }
  do
  {
    *offset += 1;
  } while (string[*offset] && isspace(string[*offset]));
}

void dt_loads_comment(const char *string, size_t *offset)
{
  dt_loads_space(string, offset);

  if (!string[*offset])
  {
    return;
  }

  if (string[*offset] == '/' && string[*offset + 1] == '/')
  {
    do
    {
      *offset += 1;
    } while (string[*offset] && string[*offset] != '\n');
  }
  else if (string[*offset] == '/' && string[*offset + 1] == '*')
  {
    bool commentEndFound = false;
    *offset += 2;

    while (string[*offset])
    {
      if (string[*offset] == '*' && string[*offset + 1] == '/')
      {
        *offset += 2;
        commentEndFound = true;
        break;
      }
      *offset += 1;
    }

    dt_test(commentEndFound, "Expected token '*/'", string, *offset);
  }

  dt_loads_space(string, offset);
}

void dt_consume_token(const char *string, size_t *offset, char token)
{
  dt_loads_comment(string, offset);
  dt_testf(
    string[*offset] == token, string, *offset, "Expected token '%c'", token);
  *offset += 1;
  dt_loads_comment(string, offset);
}

// -----------------------------------------------------------------------------

bool dt_test_null(const char *string, size_t *offset)
{
  const char *next = (string + *offset);
  return strstr(next, "null") == next;
}

dt_node *dt_loads_null(const char *string, size_t *offset)
{
  dt_loads_comment(string, offset);
  const char *next = (string + *offset);
  dt_test(strstr(next, "null") == next, "Expected null", string, *offset);
  *offset += 4;
  dt_loads_comment(string, offset);
  return dt_new_null(NULL);
}

char *dt_dumps_null(const dt_node *node, const dt_dumps_settings_t *set)
{
  (void) node;
  (void) set;
  return strdup("null");
}

dt_node *dt_loadb_null(const size_t len, const byte *bytes, size_t *offset)
{
  dt_test(len > *offset, "Not enough bytes", (char *) bytes, *offset);
  (void) len;
  return dt_new_null(NULL);
}

void dt_dumpb_null(const dt_node *node, byte *bytes)
{
  (void) node;
  (void) bytes;
}

void dt_free_null(dt_node *node)
{
  _dt_free(node);
}

// -----------------------------------------------------------------------------

bool dt_test_bool(const char *string, size_t *offset)
{
  const char *next = (string + *offset);
  return strstr(next, "true") == next || strstr(next, "false") == next;
}

dt_node *dt_loads_bool(const char *string, size_t *offset)
{
  const char *next = (string + *offset);
  if (strstr(next, "true") == next)
  {
    *offset += 4;
    return dt_new_bool(true);
  }
  else if (strstr(next, "false") == next)
  {
    *offset += 5;
    return dt_new_bool(false);
  }
  dt_test(false, "Expected bool", string, *offset);
  return NULL;
}

char *dt_dumps_bool(const dt_node *node, const dt_dumps_settings_t *set)
{
  (void) set;
  return strdup(node->bool_v ? "true" : "false");
}

dt_node *dt_loadb_bool(const size_t len, const byte *bytes, size_t *offset)
{
  dt_test(len > *offset, "Not enough bytes", (char *) bytes, *offset);
  bool result;
  dt_popval(bool, bytes, offset, result);
  return dt_new_bool(result);
}

void dt_dumpb_bool(const dt_node *node, byte *bytes)
{
  dt_pushval(bool, bytes, node->bool_v);
}

void dt_free_bool(dt_node *node)
{
  _dt_free(node);
}

// -----------------------------------------------------------------------------

bool dt_test_int(const char *string, size_t *offset)
{
  const char *next = (string + *offset);
  char *end        = (char *) next;
  strtol(next, &end, 0);
  return next != end && end && (*end == '\0' || *end != '.');
}

dt_node *dt_loads_int(const char *string, size_t *offset)
{
  const char *next = (string + *offset);
  char *end        = (char *) next;
  long val         = strtol(next, &end, 0);
  dt_test(end && next != end, "Expected int", string, *offset);
  *offset += end - next;
  return dt_new_int(val);
}

char *dt_dumps_int(const dt_node *node, const dt_dumps_settings_t *set)
{
  (void) set;
  return sprintfx("%ld", node->int_v);
}

dt_node *dt_loadb_int(const size_t len, const byte *bytes, size_t *offset)
{
  dt_test(len > *offset, "Not enough bytes", (char *) bytes, *offset);
  long result;
  dt_popval(long, bytes, offset, result);
  return dt_new_int(result);
}

void dt_dumpb_int(const dt_node *node, byte *bytes)
{
  dt_pushval(long, bytes, node->int_v);
}

void dt_free_int(dt_node *node)
{
  _dt_free(node);
}

// -----------------------------------------------------------------------------

bool dt_test_float(const char *string, size_t *offset)
{
  const char *next = (string + *offset);
  char *end        = (char *) next;
  strtod(next, &end);
  return next != end && end && (isspace(*end) || isendchar(*end));
}

dt_node *dt_loads_float(const char *string, size_t *offset)
{
  const char *next = (string + *offset);
  char *end        = (char *) next;
  double val       = strtod(next, &end);
  dt_test(end && next != end, "Expected float", string, *offset);
  (void) offset;
  *offset += end - next;
  return dt_new_float(val);
}

char *dt_dumps_float(const dt_node *node, const dt_dumps_settings_t *set)
{
  (void) set;
  char *repr = sprintfx("%lf", node->float_v);
  char *dotp = strrchr(repr, '.');
  if (dotp)
  {
    size_t dot = dotp - repr;
    size_t len = strlen(repr) - 1;
    while (len > dot && repr[len] == '0')
    {
      len--;
    }
    if (len == dot)
    {
      len += 1;
    }
    repr          = _dt_realloc(repr, len + 2);
    repr[len + 1] = '\0';
  }
  return repr;
}

dt_node *dt_loadb_float(const size_t len, const byte *bytes, size_t *offset)
{
  dt_test(len > *offset, "Not enough bytes", (char *) bytes, *offset);
  double result;
  _dt_memcpy(&result, &(bytes)[*offset], sizeof(double));
  *offset += sizeof(double);
  return dt_new_float(result);
}

void dt_dumpb_float(const dt_node *node, byte *bytes)
{
  dt_pushval(double, bytes, node->float_v);
}

void dt_free_float(dt_node *node)
{
  _dt_free(node);
}

// -----------------------------------------------------------------------------

bool dt_test_arr(const char *string, size_t *offset)
{
  const char *next = (string + *offset);
  return *next == '[';
}

dt_node *dt_loads_arr(const char *string, size_t *offset)
{
  dt_consume_token(string, offset, '[');
  dt_node *res = dt_new_arr(NULL);
  while (string[*offset] && string[*offset] != ']')
  {
    dt_loads_comment(string, offset);
    while (string[*offset] == ',')
    {
      *offset += 1;
    }
    dt_node *elem = dt_loads_impl(string, offset);
    dt_arradd(res->arr_v, elem);
    while (string[*offset] == ',')
    {
      *offset += 1;
    }
    dt_loads_comment(string, offset);
    dt_test(string[*offset] != '}', "Expected token ']'", string, *offset);
  }
  dt_consume_token(string, offset, ']');
  return res;
}

char *dt_dumps_arr(const dt_node *node, const dt_dumps_settings_t *set)
{
  char *res = NULL;
  dt_arradd(res, '[');
  size_t len = dt_arrlen(node->arr_v);
  for (size_t i = 0; i < len; ++i)
  {
    dt_arradd(res, ' ');
    char *erepr = dt_dumps_ex(node->arr_v[i], set);
    dt_arraddcstr(res, erepr);
    if (set->force_json && i < len - 1)
    {
      dt_arradd(res, ',');
    }
    _dt_free(erepr);
  }
  dt_arraddcstr(res, " ]");
  char *repr = (char *) dt_arrtonullterm(res);
  return repr;
}

dt_node *dt_loadb_arr(const size_t len, const byte *bytes, size_t *offset)
{
  size_t alen;
  dt_popval(size_t, bytes, offset, alen);
  dt_node *node = dt_new_arr(NULL);
  dt_arrmaygrow(node->arr_v, alen);
  for (size_t i = 0; i < alen; ++i)
  {
    dt_node *value = dt_loadb_impl(len, bytes, offset);
    dt_arradd(node->arr_v, value);
  }
  return node;
}

void dt_dumpb_arr(const dt_node *node, byte *bytes)
{
  size_t alen = dt_arrlenu(node->arr_v);
  dt_pushval(size_t, bytes, alen);
  for (size_t i = 0; i < alen; ++i)
  {
    dt_dumpb_impl(node->arr_v[i], bytes);
  }
}

void dt_free_arr(dt_node *node)
{
  size_t len = dt_arrlen(node->arr_v);
  for (size_t i = 0; i < len; ++i)
  {
    dt_free(node->arr_v[i]);
  }
  dt_arrfree(node->arr_v);
  _dt_free(node);
}

// -----------------------------------------------------------------------------

bool dt_test_map(const char *string, size_t *offset)
{
  const char *next = (string + *offset);
  return *next == '{';
}

dt_node *dt_loads_map(const char *string, size_t *offset)
{
  dt_consume_token(string, offset, '{');
  dt_node *res = dt_new_map(NULL);
  while (string[*offset] && string[*offset] != '}')
  {
    dt_loads_comment(string, offset);
    dt_test(string[*offset] != '{', "Expected key", string, *offset);
    dt_test(string[*offset] != '[', "Expected key", string, *offset);
    while (string[*offset] == ',')
    {
      *offset += 1;
    }
    dt_loads_comment(string, offset);
    char *key = dt_loads_raw_string(string, offset);
    dt_loads_comment(string, offset);
    dt_consume_token(string, offset, ':');
    dt_node *value = dt_loads_impl(string, offset);
    dt_smpadd(res->map_v, key, value);
    while (string[*offset] == ',')
    {
      *offset += 1;
    }
    dt_loads_comment(string, offset);
    dt_test(string[*offset] != ']', "Expected token '}'", string, *offset);
  }
  dt_consume_token(string, offset, '}');
  return res;
}

char *dt_dumps_map(const dt_node *node, const dt_dumps_settings_t *set)
{
  char *res = NULL;
  dt_arradd(res, '{');
  size_t len = dt_smplenu(node->map_v);
  for (size_t i = 0; i < len; ++i)
  {
    if (node->map_v[i].key == NULL || node->map_v[i].value == NULL)
    {
      continue;
    }
    dt_arradd(res, ' ');
    char *krepr = dt_dumps_raw_string(node->map_v[i].key, set);
    dt_arraddcstr(res, krepr);
    _dt_free(krepr);
    dt_arradd(res, ':');
    char *vrepr = dt_dumps_ex(node->map_v[i].value, set);
    dt_arraddcstr(res, vrepr);
    if (set->force_json && i < len - 1)
    {
      dt_arradd(res, ',');
    }
    _dt_free(vrepr);
  }
  dt_arraddcstr(res, " }");
  char *repr = dt_arrtonullterm(res);
  return repr;
}

dt_node *dt_loadb_map(const size_t len, const byte *bytes, size_t *offset)
{
  size_t mlen;
  dt_popval(size_t, bytes, offset, mlen);
  dt_node *node = dt_new_map(NULL);
  for (size_t i = 0; i < mlen; ++i)
  {
    char *key      = dt_loadb_raw_string(len, bytes, offset);
    dt_node *value = dt_loadb_impl(len, bytes, offset);
    dt_smpadd(node->map_v, key, value);
  }
  return node;
}

void dt_dumpb_map(const dt_node *node, byte *bytes)
{
  size_t mlen = dt_smplenu(node->map_v);
  dt_pushval(size_t, bytes, mlen);
  for (size_t i = 0; i < mlen; ++i)
  {
    dt_dumpb_raw_string(node->map_v[i].key, bytes);
    dt_dumpb_impl(node->map_v[i].value, bytes);
  }
}

void dt_free_map(dt_node *node)
{
  size_t len = dt_smplenu(node->map_v);
  for (size_t i = 0; i < len; ++i)
  {
    _dt_free(node->map_v[i].key);
    _dt_free(node->map_v[i].value);
  }
  dt_smpfree(node->map_v);
  _dt_free(node);
}

// -----------------------------------------------------------------------------

bool dt_test_string(const char *string, size_t *offset)
{
  const char *next = (string + *offset);
  return *next == '\"' || !isspace(*next);
}

dt_node *dt_loads_string(const char *string, size_t *offset)
{
  char *res = dt_loads_raw_string(string, offset);
  dt_test(res, "String is invalid", string, *offset);
  return dt_new_string(res);
}

char *dt_dumps_string(const dt_node *node, const dt_dumps_settings_t *set)
{
  (void) set;
  return dt_dumps_raw_string(node->string_v, set);
}

dt_node *dt_loadb_string(const size_t len, const byte *bytes, size_t *offset)
{
  return dt_new_string(dt_loadb_raw_string(len, bytes, offset));
}

void dt_dumpb_string(const dt_node *node, byte *bytes)
{
  dt_dumpb_raw_string(node->string_v, bytes);
}

void dt_free_string(dt_node *node)
{
  _dt_free(node->string_v);
  _dt_free(node);
}

// -----------------------------------------------------------------------------

char *sprintfx(const char *format, ...)
{
  va_list args, args_copy;
  va_start(args, format);
  va_copy(args_copy, args);
  int len = vsnprintf(NULL, 0, format, args);
  va_end(args);
  if (len < 0)
  {
    va_end(args_copy);
    return NULL;
  }

  char *result = (char *) _dt_malloc(len + 1);
  if (!result)
  {
    va_end(args_copy);
    return NULL;
  }

  vsnprintf(result, len + 1, format, args_copy);
  va_end(args_copy);
  return result;
}

size_t strnchrn(const char *string, const char c, const size_t n)
{
  if (string == NULL)
  {
    return 0;
  }
  size_t i = 0;
  size_t j = 0;
  while (i < n && string[i])
  {
    i++;
    if (string[i] == c)
      j++;
  }
  return j;
}

char *strrchre(const char *string, const char c, const char *end)
{
  if (!end || end < string || end > (string + strlen(string)))
  {
    return (char *) 0;
  }

  size_t i = 0;
  while ((end - i) >= string)
  {
    if (end[i] == c)
    {
      return (char *) (end - i);
    }
    i--;
  }

  return (char *) 0;
}

// -----------------------------------------------------------------------------

bool isescape(const char c)
{
  return c == '\n' || c == '\r' || c == '\t' || c == '\0' || c == '\\' ||
         c == '\"';
}

bool isendchar(const char c)
{
  return c == '{' || c == '}' || c == '[' || c == ']' || c == ':' || c == ',';
}

bool islongstring(const char *str)
{
  size_t i = 0;
  while (str[i])
  {
    if (isspace(str[i]) || isescape(str[i]) || str[i] == '"' || str[i] == ':' ||
        str[i] == '[' || str[i] == ']' || str[i] == '{' || str[i] == '}')
    {
      return true;
    }
    i++;
  }
  return false;
}

// -----------------------------------------------------------------------------

char *stresc(const char *s)
{
  int len = 0;
  for (int i = 0; s[i]; i++)
  {
    if (isescape(s[i]))
    {
      len++;
    }
    len++;
  }

  char *result = (char *) _dt_malloc(len + 1);
  if (!result)
  {
    return NULL;
  }

  int j = 0;
  for (int i = 0; s[i]; i++)
  {
    if (s[i] == '\n')
    {
      result[j++] = '\\';
      result[j++] = 'n';
    }
    else if (s[i] == '\r')
    {
      result[j++] = '\\';
      result[j++] = 'r';
    }
    else if (s[i] == '\t')
    {
      result[j++] = '\\';
      result[j++] = 't';
    }
    else if (s[i] == '\0')
    {
      result[j++] = '\\';
      result[j++] = '0';
    }
    else if (s[i] == '\\')
    {
      result[j++] = '\\';
      result[j++] = '\\';
    }
    else if (s[i] == '\"')
    {
      result[j++] = '\\';
      result[j++] = '\"';
    }
    else
    {
      result[j++] = s[i];
    }
  }
  result[j] = '\0';

  return result;
}

char *strunesc(const char *s)
{
  int len      = strlen(s);
  char *result = (char *) _dt_malloc(len + 1);
  if (!result)
  {
    return NULL;
  }

  int j = 0;
  for (int i = 0; s[i]; i++)
  {
    if (s[i] == '\\' && s[i + 1])
    {
      if (s[i + 1] == 'n')
      {
        result[j++] = '\n';
        i++;
      }
      else if (s[i + 1] == 'r')
      {
        result[j++] = '\r';
        i++;
      }
      else if (s[i + 1] == 't')
      {
        result[j++] = '\t';
        i++;
      }
      else if (s[i + 1] == '0')
      {
        result[j++] = '\0';
        i++;
      }
      else if (s[i + 1] == '\\')
      {
        result[j++] = '\\';
        i++;
      }
      else if (s[i + 1] == '\"')
      {
        result[j++] = '\"';
        i++;
      }
      else
      {
        result[j++] = s[i];
      }
    }
    else
    {
      result[j++] = s[i];
    }
  }
  result[j] = '\0';

  return result;
}

// -----------------------------------------------------------------------------

#endif
#endif
/*
This software is served under two licenses - pick which you prefer.
--------------------------------------------------------------------------------
MIT License
Copyright 2023 Dani Yeomans

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
--------------------------------------------------------------------------------
Public Domain
Dedicated 2023 Dani Yeomans

The author of this software and associated documentation files (the "Software")
also dedicates any and all copyright interest in the Software to the public
domain. The author makes this dedication for the benefit of the public at large
and to the detriment of the author's heirs and successors. The author intends
this dedication to be an overt act of relinquishment in perpetuity of all
present and future rights to the Software under copyright law.

Any person obtaining a copy of the Software and associated documentation files
(the "Software") is free to to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to no conditions.
--------------------------------------------------------------------------------
*/
