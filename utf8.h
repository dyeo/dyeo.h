#ifndef _UTF8_H
#define _UTF8_H

#if __cplusplus
extern "C" {
#endif

#ifndef UNICODE
#define UNICODE 1
#endif
#ifndef _UNICODE
#define _UNICODE 1
#endif
#ifndef UTF8
#define UTF8 1
#endif
#ifndef _UTF8
#define _UTF8 1
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/// @brief Decodes the length of a single UTF-8 codepoint.
/// @param c The codepoint to decode.
/// @return 1 for a single-byte char or a continued codepoint (denoted by
/// 10xxxxxx). Otherwise, the number of bytes for any other length codepoint.
extern size_t cplen(const char c);

/// @brief Compares two UTF-8 codepoints of variable length.
/// @param l The first variable-length UTF-8 codepoint.</param>
/// @param r The second variable-length UTF-8 codepoint.</param>
/// @return 0 if codepoints are equivalent, -1 if l < r, 1 otherwise.
extern int cpcmp(const char *l, const char *r);

/// @brief Compares two UTF-8 strings.
/// @param l The first UTF-8 string.</param>
/// @param r The second UTF-8 string.</param>
/// @return 0 if strings are equivalent, -1 if l < r, 1 otherwise.
extern int utfcmp(const char *l, const char *r);

/// @brief Compares two UTF-8 strings up to a given length.
/// @param l The first UTF-8 string.</param>
/// @param r The second UTF-8 string.</param>
/// @param n The maximum length to compare.</param>
/// @return 0 if strings are equivalent, -1 if l < r, 1 otherwise.
extern int utfncmp(const char *l, const char *r, size_t n);

/// @brief Counts the number of literal characters in a UTF-8 string, not the
/// number of individual codepoints (bytes).
/// @param str The UTF-8 string to count.
/// @return The number of characters in the string. For example,
/// "\xe4\xbd\xa0\xe5\xa5\xbd" (你好) would return 2, not 6.
extern size_t utflen(const char *str);

/// @brief Counts the number of individual codepoints (bytes) in a UTF-8 string,
/// not the number of literal characters.
/// @param str The UTF-8 string to count.
/// @return The number of characters in the string. For example,
/// "\xe4\xbd\xa0\xe5\xa5\xbd" (你好) would return 2, not 6.
extern size_t utfblen(const char *str);

/// @brief Duplicates a UTF-8 string, ensuring it's correctly null-terminated.
/// @param str The UTF-8 string to duplicate.
/// @return The new duplicated string. This string must be freed later.
extern char *utfdup(const char *str);

/// @brief Copies a UTF-8 string.
/// @param dest The destination buffer.
/// @param src The source UTF-8 string.
/// @return A pointer to the destination string.
extern char *utfcpy(char *dest, const char *src);

/// @brief Copies at most n characters from a UTF-8 string.
/// @param dest The destination buffer.
/// @param src The source UTF-8 string.
/// @param n The maximum number of characters to copy.
/// @return A pointer to the destination string.
extern char *utfncpy(char *dest, const char *src, size_t n);

/// @brief Appends one UTF-8 string to another.
/// @param dest The destination string.
/// @param src The source UTF-8 string.
/// @return A pointer to the destination string.
extern char *utfcat(char *dest, const char *src);

/// @brief Appends at most n characters from one UTF-8 string to another.
/// @param dest The destination string.
/// @param src The source UTF-8 string.
/// @param n The maximum number of characters to append.
/// @return A pointer to the destination string.
extern char *utfncat(char *dest, const char *src, size_t n);

/// @brief Finds the first occurrence of a UTF-8 character in a string.
/// @param s The UTF-8 string to search within.
/// @param ch The UTF-8 character to find.
/// @return A pointer to the first occurrence of ch in s, or NULL if ch is not
/// found.
extern char *utfchr(const char *s, int ch);

/// @brief Finds the first occurrence of a UTF-8 substring in another string.
/// @param haystack The UTF-8 string to search within.
/// @param needle The UTF-8 substring to find.
/// @return A pointer to the first occurrence of needle in haystack, or NULL if
/// needle is not found.
extern char *utfstr(const char *haystack, const char *needle);

/// @brief Reverses a UTF-8 string in place.
/// @param str The UTF-8 string to reverse.
/// @return A pointer to the reversed string.
extern char *utfreverse(char *str);

#if __cplusplus
}
#endif

#endif //_UTF8_H

#if defined(UTF8_IMPLEMENTATION) || defined(UTF8_STATIC)
#ifndef _UTF8_C
#define _UTF8_C

#ifdef UTF8_STATIC
#define UTF8_CDECL static
#else
#define UTF8_CDECL
#endif

UTF8_CDECL size_t cplen(const char c)
{
  if ((c & 0x80) == 0)
    return 1;
  else if ((c & 0xE0) == 0xC0)
    return 2;
  else if ((c & 0xF0) == 0xE0)
    return 3;
  else if ((c & 0xF8) == 0xF0)
    return 4;
  return 1;
}

UTF8_CDECL int cpcmp(const char *l, const char *r)
{
  if (l == NULL && r == NULL)
    return 0;
  if (l == NULL)
    return -1;
  if (r == NULL)
    return 1;
  uint8_t ln = cplen(*l);
  uint8_t rn = cplen(*r);
  for (uint8_t i = 0; i < ln && i < rn; ++i)
  {
    if (l[i] < r[i])
      return -1;
    else if (l[i] > r[i])
      return 1;
  }
  if (ln < rn)
    return -1;
  else if (ln > rn)
    return 1;
  return 0;
}

UTF8_CDECL int utfcmp(const char *l, const char *r)
{
  if (!l && !r)
    return 0;
  if (!l)
    return -1;
  if (!r)
    return 1;
  while (*l && *r)
  {
    int cmp = cpcmp(l, r);
    if (cmp != 0)
      return cmp;
    l += cplen(*l);
    r += cplen(*r);
  }
  if (*l)
    return 1;
  if (*r)
    return -1;
  return 0;
}

UTF8_CDECL int utfncmp(const char *l, const char *r, size_t n)
{
  if ((!l && !r) || n == 0)
    return 0;
  if (!l)
    return -1;
  if (!r)
    return 1;
  size_t count = 0;
  while (*l && *r && count < n)
  {
    int cmp = cpcmp(l, r);
    if (cmp != 0)
      return cmp;
    l += cplen(*l);
    r += cplen(*r);
    count++;
  }
  if (count == n)
    return 0;
  if (*l)
    return 1;
  if (*r)
    return -1;
  return 0;
}

UTF8_CDECL size_t utflen(const char *str)
{
  size_t i = 0;
  if (str == NULL)
    return i;
  const char *c = str;
  while (*c != '\0')
  {
    i += 1;
    c += cplen(*c);
  }
  return i;
}

UTF8_CDECL size_t utfblen(const char *str)
{
  size_t i = 0;
  if (str == NULL)
    return 0;
  const char *c = str;
  while (*c != '\0')
  {
    i += 1;
    c += cplen(*c);
  }
  return c - str;
}

UTF8_CDECL char *utfdup(const char *str)
{
  if (str == NULL)
    return NULL;
  size_t blen = utfblen(str) + 1;
  char *dest  = calloc(blen, sizeof(char));
  if (dest == NULL)
    return NULL;
  dest[blen - 1] = '\0';
  memmove(dest, str, blen - 1);
  return dest;
}

UTF8_CDECL char *utfcpy(char *dest, const char *src)
{
  char *d       = dest;
  const char *s = src;
  while ((*d++ = *s++))
    ;
  return dest;
}

UTF8_CDECL char *utfncpy(char *dest, const char *src, size_t n) {
    if (!n) return dest;
    char *d = dest;
    const char *s = src;
    size_t codepoints_copied = 0;
    while (*s && codepoints_copied < n) {
        size_t len = cplen(*s);
        memcpy(d, s, len);
        s += len;
        d += len;
        codepoints_copied++;
    }
    *d = '\0';
    return dest;
}

UTF8_CDECL char *utfcat(char *dest, const char *src)
{
  char *d = dest;
  while (*d)
    d++;
  while ((*d++ = *src++))
    ;
  return dest;
}

UTF8_CDECL char *utfncat(char *dest, const char *src, size_t n) {
    if (!n) return dest;
    char *d = dest;
    while (*d) {
        d++;
    }
    const char *s = src;
    size_t codepoints_copied = 0;
    while (*s && codepoints_copied < n) {
        size_t len = cplen(*s);
        memcpy(d, s, len);
        s += len;
        d += len;
        codepoints_copied++;
    }
    *d = '\0';
    return dest;
}

UTF8_CDECL char *utfchr(const char *s, int ch)
{
  const char *p = s;
  while (*p)
  {
    if (*p == ch)
      return (char *) p;
    p += cplen(*p);
  }
  return NULL;
}

UTF8_CDECL char *utfstr(const char *haystack, const char *needle)
{
  const char *h = haystack;
  const char *n;
  while (*h)
  {
    n                 = needle;
    const char *start = h;
    while (*h && *n && cpcmp(h, n) == 0)
    {
      h += cplen(*h);
      n += cplen(*n);
    }
    if (!*n)
      return (char *) start;
    h = start + cplen(*start);
  }
  return NULL;
}

UTF8_CDECL char *utfreverse(char *str) {
    char *s = str;
    size_t len = utflen(str);  // number of UTF-8 characters
    char **chars = (char **)malloc(len * sizeof(char *));  
    if (!chars) {
        return NULL;  // Memory allocation failed
    }
    for (size_t i = 0; i < len; i++) {
        chars[i] = s;
        s += cplen(*s);
    }

    for (size_t i = 0; i < len / 2; i++) {
        char *a = chars[i];
        char *b = chars[len - 1 - i];
        size_t a_len = cplen(*a);
        size_t b_len = cplen(*b);
        char temp[4];  // Maximum UTF-8 length

        memcpy(temp, a, a_len);
        memmove(a, b, b_len);
        memmove(a + b_len, temp, a_len);
    }

    free(chars);
    return str;
}


#undef UTF_CDECL

#endif
#endif