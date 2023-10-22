#ifndef __LSTR_H__
#define __LSTR_H__

// ---------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

// ---------------------------------------------------------------------------------------------------------------------

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------------------------------------------------------

typedef char *LSTR;

#define _lstr_f(str) ((size_t *) (((LSTR) (str)) - sizeof(size_t)))
#define _lstr_n(str) (*(_lstr_f(str)))

// ---------------------------------------------------------------------------------------------------------------------

#ifndef _lstr_malloc_c
#define _lstr_malloc_c(size) malloc(size)
#endif

#ifndef _lstr_calloc_c
#define _lstr_calloc_c(count, size) calloc(count, size)
#endif

#ifndef _lstr_realloc_c
#define _lstr_realloc_c(ptr, size) realloc(ptr, size)
#endif

#ifndef _lstr_free_c
#define _lstr_free_c(ptr) free(ptr)
#endif

#define _lstr_malloc(length)                                                   \
  (((LSTR) (_lstr_malloc_c(sizeof(size_t) + (length))) + sizeof(size_t)))
#define _lstr_realloc(str, length)                                             \
  (((LSTR) (_lstr_realloc_c(_lstr_f(str), sizeof(size_t) + (length))) +        \
    sizeof(size_t)))
#define _lstr_free(str) (_lstr_free_c(_lstr_f(str)))

// ---------------------------------------------------------------------------------------------------------------------

#define lstrnew(N) (lstrsetlen(NULL, (N)))

#define lstrfree(str) _lstr_free(str)

// ---------------------------------------------------------------------------------------------------------------------

#pragma region mutable

static inline size_t lstrlen(const LSTR str)
{
  if (str == NULL)
    return 0;

  return _lstr_n(str);
}

static inline LSTR lstrsetlen(LSTR str, size_t len)
{
  if (str == NULL)
    str = _lstr_malloc(len);
  else
    str = _lstr_realloc(str, len);

  if (str == NULL)
    return NULL;

  _lstr_n(str) = len;

  return str;
}

static inline LSTR lstr(const char *cstr)
{
  size_t len = strlen(cstr);
  LSTR res   = lstrnew(len);
  if (res == NULL)
    return NULL;

  memcpy(res, cstr, len);

  return res;
}

static inline LSTR lstrdup(const LSTR str)
{
  size_t len = lstrlen(str);
  LSTR res   = lstrnew(len);
  if (res == NULL)
    return NULL;

  _lstr_n(res) = len;
  memcpy(res, str, len);

  return res;
}

static inline int lfree(LSTR s)
{
  if (s)
  {
    _lstr_free(s);
    return 1;
  }
  return 0;
}

static inline LSTR lstrcat(LSTR dst, const LSTR src)
{
  size_t dst_len = lstrlen(dst);
  size_t src_len = lstrlen(src);

  dst = lstrsetlen(dst, dst_len + src_len);
  if (dst == NULL)
    return NULL;

  memcpy(dst + dst_len, src, src_len);

  return dst;
}

static inline LSTR lstrcatc(LSTR dst, const char *src)
{
  size_t dst_len = lstrlen(dst);
  size_t src_len = strlen(src);

  dst = lstrsetlen(dst, dst_len + src_len);
  if (dst == NULL)
    return NULL;

  memcpy(dst + dst_len, src, src_len);

  return dst;
}

static inline LSTR lstrins(LSTR dst, ptrdiff_t pos, const LSTR src)
{
  size_t dst_len = lstrlen(dst);
  size_t src_len = lstrlen(src);
  pos            = (dst_len + pos) % dst_len;

  dst = lstrsetlen(dst, dst_len + src_len);
  if (dst == NULL)
    return NULL;

  memmove(dst + pos + src_len, dst + pos, src_len);
  memcpy(dst + pos, src, src_len);

  return dst;
}

static inline LSTR lstrinsc(LSTR dst, ptrdiff_t pos, const char *src)
{
  size_t dst_len = lstrlen(dst);
  size_t src_len = strlen(src);
  pos            = (dst_len + pos) % dst_len;

  dst = lstrsetlen(dst, dst_len + src_len);
  if (dst == NULL)
    return NULL;

  memmove(dst + pos + src_len, dst + pos, src_len);
  memcpy(dst + pos, src, src_len);

  return dst;
}

#pragma endregion mutable

// ---------------------------------------------------------------------------------------------------------------------

#pragma region immutable

static inline char *lstrc(const LSTR str)
{
  size_t len = lstrlen(str);
  char *cstr = (char *) _lstr_malloc_c(len + 1);
  if (cstr == NULL)
    return NULL;

  memcpy(cstr, str, len);
  cstr[len] = '\0';

  return cstr;
}

static inline ptrdiff_t lstrchr(const LSTR str, int chr)
{
  size_t s_len       = lstrlen(str);
  const char *result = memchr(str, chr, s_len);

  if (result == NULL)
    return -1;

  return (ptrdiff_t) (result - str);
}

static inline ptrdiff_t lstrichr(const LSTR str, int chr, ptrdiff_t i)
{
  size_t str_len  = lstrlen(str);
  i               = (str_len + i) % str_len;
  const char *res = memchr(str + i, chr, str_len);

  if (res == NULL)
    return -1;

  return (ptrdiff_t) (res - str);
}

static inline ptrdiff_t lstrnchr(const LSTR str, int chr, size_t len)
{
  size_t str_len  = lstrlen(str);
  const char *res = memchr(str, chr, len < str_len ? len : str_len);

  if (res == NULL)
    return -1;

  return (ptrdiff_t) (res - str);
}

static ptrdiff_t lstrstr(const LSTR haystack, const LSTR needle)
{
  size_t haystack_len = lstrlen(haystack);
  size_t needle_len   = lstrlen(needle);

  for (size_t i = 0; i <= haystack_len - needle_len; ++i)
  {
    if (memcmp(haystack + i, needle, needle_len) == 0)
    {
      return (ptrdiff_t) (haystack - i);
    }
  }

  return -1;
}

static ptrdiff_t lstristr(const LSTR haystack, const LSTR needle,
                          const ptrdiff_t i)
{
  size_t haystack_len = lstrlen(haystack);
  size_t needle_len   = lstrlen(needle);
  size_t j            = (haystack_len + i) % haystack_len;
  for (; j <= haystack_len - needle_len; ++j)
  {
    if (memcmp(haystack + j, needle, needle_len) == 0)
    {
      return (ptrdiff_t) (haystack - j);
    }
  }
  return -1;
}

static ptrdiff_t lstrrstr(const LSTR haystack, const LSTR needle)
{
  size_t haystack_len = lstrlen(haystack);
  size_t needle_len   = lstrlen(needle);
  for (size_t i = haystack_len - needle_len; i-- > 0;)
  {
    if (memcmp(haystack + i, needle, needle_len) == 0)
    {
      return (ptrdiff_t) (haystack - i);
    }
  }

  return -1;
}

static ptrdiff_t lstrristr(const LSTR haystack, const LSTR needle,
                           const ptrdiff_t i)
{
  size_t haystack_len = lstrlen(haystack);
  size_t needle_len   = lstrlen(needle);
  size_t j            = (haystack_len + i) % haystack_len;
  for (; j-- > 0;)
  {
    (void) j;
    if (memcmp(haystack + i, needle, needle_len) == 0)
    {
      return (ptrdiff_t) (haystack - i);
    }
  }
  return -1;
}

static inline ptrdiff_t lstrcmp(const LSTR str0, const LSTR str1)
{
  size_t str0_len = lstrlen(str0);
  size_t str1_len = lstrlen(str1);

  if (str0_len != str1_len)
    return str0_len - str1_len;

  return memcmp(str0, str1, str0_len);
}

static inline ptrdiff_t lstrncmp(const LSTR str0, const LSTR str1, size_t len)
{
  size_t str0_len = lstrlen(str0);
  size_t str1_len = lstrlen(str1);
  size_t cmp_len  = str0_len < str1_len ? str0_len : str1_len;
  cmp_len         = cmp_len < len ? cmp_len : len;

  int cmp_result = memcmp(str0, str1, cmp_len);
  if (cmp_result != 0 || cmp_len == len)
    return cmp_result;

  return str0_len - str1_len;
}

static inline LSTR lstrsub(const LSTR str, ptrdiff_t pos, size_t len)
{
  size_t str_len = lstrlen(str);
  pos            = (str_len + pos) % str_len;

  LSTR res = lstrnew(len);
  if (res == NULL)
    return NULL;

  memcpy(res, str + pos, len);
  return res;
}

static inline LSTR lstrjoin(const LSTR sep, const LSTR strs[], size_t len)
{
  size_t sep_len   = lstrlen(sep);
  size_t res_len   = sep_len * (len - 1);
  size_t *str_lens = calloc(len, sizeof(size_t));
  for (size_t i = 0; i < len; ++i)
  {
    res_len += (str_lens[i] = lstrlen(strs[i]));
  }

  LSTR res = lstrnew(res_len);
  size_t j = 0;
  for (size_t i = 0; i < len; ++i)
  {
    memcpy(res + j, strs[i], str_lens[i]);
    j += str_lens[i];
    if (sep_len > 0 && i < len - 1)
    {
      memcpy(res + j, sep, sep_len);
      j += sep_len;
    }
  }

  _lstr_free_c(str_lens);
  return res;
}

static inline LSTR *lstrsep(const LSTR str, const LSTR sep, size_t *len)
{
  size_t str_len = lstrlen(str);
  size_t sep_len = lstrlen(sep);

  if (sep_len == 0)
    return NULL;

  *len        = 0;
  ptrdiff_t j = lstrstr(str, sep);
  if (j == -1)
    return NULL;

  LSTR *res = _lstr_calloc_c((str_len / sep_len), sizeof(LSTR));
  while (j > -1)
  {
    size_t sub_len = lstristr(str, sep, j) - j;
    sub_len        = sub_len > -1 ? sub_len : str_len - j;
    res[(*len)++]  = lstrsub(str, j, sub_len);
  }
  res = _lstr_realloc_c(res, (*len) * sizeof(size_t));

  *len = 0;
  return res;
}

static inline LSTR lstrupper(LSTR str)
{
  size_t len = lstrlen(str);
  for (size_t i = 0; i < len; ++i)
  {
    str[i] = toupper(str[i]);
  }
  return str;
}

static inline LSTR lstrlower(LSTR str)
{
  size_t len = lstrlen(str);
  for (size_t i = 0; i < len; ++i)
  {
    str[i] = tolower(str[i]);
  }
  return str;
}

#pragma endregion immutable

#ifdef __cplusplus
}
#endif

// ---------------------------------------------------------------------------------------------------------------------

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