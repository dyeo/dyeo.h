/*
# stream.h

## Description

This is a single-header C library providing stream operations for bytes.
It is designed to handle streams of bytes with performance and flexibility.
This header provides both stream handling and iteration capabilities over
those streams.

## Usage

1. Simply include this header in your project.
2. Define STREAM_IMPLEMENTATION in one C or C++ file before inclusion to
create the implementation.

## API Documentation

### Streams

- `stream_length(STREAM*)`: Returns the current length of the stream.
- `stream_capacity(STREAM*)`: Returns the current capacity of the stream.
- `stream_empty(STREAM*)`: Checks if the stream is empty.
- `stream_begptr(STREAM*)`: Returns a pointer to the beginning of the stream
buffer.
- `stream_endptr(STREAM*)`: Returns a pointer to the end of the stream buffer.
- `stream_free(STREAM*)`: Frees the stream and its buffer.
- `stream_clear(STREAM*)`: Clears the stream content.
- `stream_setcap(STREAM*, size_t)`: Sets the stream's capacity.
- `stream_growcap(STREAM*, size_t)`: Ensures the stream has at least a given
capacity.
- `stream_pushb(STREAM*, size_t, BYTE*)`: Pushes bytes onto the end of the
stream.
- `stream_enqb(STREAM*, size_t, BYTE*)`: Enqueues bytes onto the end of the
stream (alias to pushb).
- `stream_insb(STREAM*, size_t, BYTE*, size_t)`: Inserts N bytes at a specific
index in the stream.
- `stream_pushbz(STREAM*, size_t)`: Pushes zero-bytes onto the end of the
stream.
- `stream_insbz(STREAM*, size_t, size_t)`: Inserts zero-bytes at a specific
index in the stream.
- `stream_popb(STREAM*, size_t)`: Pops bytes from the end of the stream.
- `stream_deqb(STREAM*, size_t)`: Dequeues bytes from the beginning of the
stream.
- `stream_seek(STREAM*, ptrdiff_t, int)`: Seeks to a position in the stream.
- `stream_tell(STREAM*)`: Tells the current position in the stream.
- `stream_read(STREAM*, void*, size_t, size_t)`: Reads bytes from the stream
into a buffer.
- `stream_write(STREAM*, const void*, size_t, size_t)`: Writes bytes to the
stream from a buffer.

### Stream Iterators

- `streamiter(STREAM*)`: Creates an iterator for the stream.
- `streamiter_reset(STREAMITER)`: Resets the iterator to the beginning.
- `streamiter_curr(STREAMITER)`: Gets the current byte in the iterator.
- `streamiter_next(STREAMITER)`: Moves to the next byte and returns it.
- `streamiter_hasnext(STREAMITER)`: Checks if there are more bytes in the
iterator.
- `streamriter(STREAM*)`: Creates a reverse iterator for the stream.
- `streamriter_reset(STREAMRITER)`: Resets the reverse iterator.
- `streamriter_curr(STREAMRITER)`: Gets the current byte in the reverse
iterator.
- `streamriter_next(STREAMRITER)`: Moves to the next byte and returns it.
- `streamriter_hasnext(STREAMRITER)`: Checks if there are more bytes in the
reverse iterator.

### Note

- Internal functions are prefixed with "_", and they should not be used
directly.
- In case of any function that modifies the stream (like pushb, insb, etc.),
the stream pointer might change because of potential reallocations.
- For platform-specific handling, _CRT_SECURE_NO_WARNINGS is defined for
Windows.
- Short names are available as long as STREAM_NO_SHORT_NAMES is not
defined. It uses the following syntax:
  - stream_length -> slength
  - stream_pushb -> spushb
and so on.
- deq and pop functions return the pointer to the start of the memory that was
popped. The memory is volatile. Please ensure to copy out the values if you
need them before using the stream further.
*/
#ifndef _STREAM_H
#define _STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BYTE
#include <stdint.h>
#define BYTE uint8_t
#endif

typedef struct STREAM
{
  BYTE *buffer;
  size_t offset;
  size_t length;
  size_t capacity;
} STREAM;

typedef struct STREAMITER
{
  STREAM *stream;
  size_t offset;
  size_t i;
} *STREAMITER;

typedef struct STREAMITER *STREAMRITER;

#ifndef STREAM_MIN_CAPACITY
#define STREAM_MIN_CAPACITY 32
#endif

#ifndef STREAM_NO_SHORT_NAMES
#define slength stream_length
#define slengthu stream_lengthu
#define scapacity stream_capacity
#define scapacityu stream_capacityu
#define sempty stream_empty
#define sbegptr stream_begptr
#define sendptr stream_endptr
#define ssetcap stream_setcap
#define ssetlen stream_setlen
#define spushb stream_pushb
#define sinsb stream_insb
#define sinsbz stream_insbz
#define senqb stream_enqb
#define spopb stream_popb
#define sdeqb stream_deqb
#define sseek stream_seek
#define stell stream_tell
#define sread stream_read
#define sclear stream_clear
#define sfree stream_free
#define siter streamiter
#define sireset streamiter_reset
#define sicurr streamiter_curr
#define sinext streamiter_next
#define sihasnext streamiter_hasnext
#define sriter streamriter
#define srireset streamiter_reset
#define sricurr streamriter_curr
#define srinext streamriter_next
#define srihasnext streamriter_hasnext
#endif

#define stream_length(stream) ((stream) ? ((ptrdiff_t) (stream)->length) : 0l)
#define stream_lengthu(stream) ((stream) ? ((size_t) (stream)->length) : 0)
#define stream_capacity(stream)                                                \
  ((stream) ? ((ptrdiff_t) (stream)->capacity) : 0)
#define stream_capacityu(stream) ((stream) ? ((size_t) (stream)->capacity) : 0)
#define stream_empty(stream) (stream_lengthu(stream) == 0)

#define stream_begptr(stream) ((stream)->buffer + (stream)->offset)
#define stream_endptr(stream)                                                  \
  ((stream)->buffer + (stream)->offset + (stream)->length)

#define stream_free(Stream) (_stream_free(Stream), (Stream) = 0)
#define stream_clear(Stream) (_stream_clear(Stream))
#define stream_setcap(Stream, Capacity)                                        \
  ((Stream) = _stream_setcap(stream, Capacity))
#define stream_growcap(Stream, Capacity)                                       \
  ((Stream) = _stream_growcap(stream, Capacity))
#define stream_pushb(Stream, Len, Bytes)                                       \
  ((Stream) = _stream_pushb(Stream, Len, (BYTE *) Bytes))
#define stream_enqb stream_pushb
#define stream_insb(Stream, Len, Bytes, Idx)                                   \
  ((Stream) = _stream_insb(Stream, Len, (BYTE *) Bytes, Idx))
#define stream_pushbz(Stream, Len) ((Stream) = _stream_pushbz(Stream, Len))
#define stream_insbz(Stream, Len, Idx)                                         \
  ((Stream) = _stream_insbz(Stream, Len, Idx))
#define stream_popb(Stream, Len) ((BYTE *) _stream_popb(Stream, Len))
#define stream_deqb(Stream, Len) ((BYTE *) _stream_deqb(Stream, Len))
#define stream_seek(Stream, Idx) (_stream_seek(Stream, Idx))
#define stream_tell(Stream) ((Stream) ? (ptrdiff_t) (Stream)->offset : 0)
#define stream_read(Stream, DataPtr, Size, Count)                              \
  (_stream_read(Stream, DataPtre, Size, Count))
#define stream_write(Stream, DataPtr, Size, Count)                             \
  (_stream_write(Stream, DataPtre, Size, Count))
#define streamiter(Stream) (_streamiter(Stream))
#define streamiter_reset(StreamIter) (_streamiter_reset(StreamIter))
#define streamiter_curr(StreamIter) (_streamiter_curr(StreamIter))
#define streamiter_next(StreamIter) (_streamiter_next(StreamIter))
#define streamiter_hasnext(StreamIter) (_streamiter_hasnext(StreamIter))
#define streamiter_free(StreamIter) (_streamiter_free(StreamIter))
#define streamriter(Stream) (_streamriter(Stream))
#define streamriter_curr(StreamRiter) (_streamriter_curr(StreamRiter))
#define streamriter_next(StreamRiter) (_streamriter_next(StreamRiter))
#define streamriter_hasnext(StreamRiter) (_streamriter_hasnext(StreamRiter))

#ifndef STREAM_STATIC
extern int _stream_free(STREAM *stream);
extern int _stream_clear(STREAM *stream);
extern STREAM *_stream_setcap(STREAM *stream, const size_t capacity);
extern STREAM *_stream_growcap(STREAM *stream, const size_t mincap);
extern STREAM *_stream_reoffset(STREAM *stream, const size_t pos);
extern STREAM *_stream_pushbz(STREAM *stream, const size_t length);
extern STREAM *_stream_insbz(STREAM *stream, const size_t length,
                             const size_t i);
extern STREAM *_stream_pushb(STREAM *stream, const size_t length,
                             const BYTE *Bytes);
extern STREAM *_stream_insb(STREAM *stream, const size_t length,
                            const BYTE *Bytes, const size_t i);
extern BYTE *_stream_popb(STREAM *stream, const size_t length);
extern BYTE *_stream_deqb(STREAM *stream, const size_t length);
extern int _stream_seek(STREAM *stream, const ptrdiff_t offset, int whence);
extern size_t _stream_read(STREAM *stream, void *ptr, size_t size,
                           size_t count);
extern size_t _stream_write(STREAM *stream, const void *ptr, size_t size,
                            size_t count);
extern STREAMITER _streamiter(STREAM *stream);
extern void _streamiter_reset(STREAMITER iter);
extern BYTE _streamiter_next(STREAMITER iter);
extern int _streamiter_hasnext(STREAMITER iter);
extern STREAMRITER _streamriter(STREAM *stream);
extern void _streamriter_reset(STREAMRITER iter);
extern BYTE _streamriter_next(STREAMRITER iter);
extern int _streamriter_hasnext(STREAMRITER iter);
#endif

#ifdef __cplusplus
}
#endif

#endif

#if defined(STREAM_IMPLEMENTATION) || defined(STREAM_STATIC)
#ifndef _STREAM_C
#define _STREAM_C

#ifdef STREAM_STATIC
#ifdef STREAM_CDECL
#define STREAM_CDECL static inline
#endif
#else
#define STREAM_CDECL
#endif

// this value may be subsequently rewritten by the STREAM. please copy out to a
// safe location
#ifndef STREAM_VOlATILE
#define STREAM_VOlATILE
#endif

#ifndef min
#define min(L, R) ((L) < (R) ? (L) : (R))
#endif
#ifndef max
#define max(L, R) ((L) > (R) ? (L) : (R))
#endif

STREAM_CDECL int _stream_free(STREAM *stream)
{
  if (!stream || !stream->buffer)
  {
    return 0;
  }
  free(stream->buffer);
  free(stream);
  return 1;
}

STREAM_CDECL int _stream_clear(STREAM *stream)
{
  if (!stream || !stream->buffer)
  {
    return 0;
  }
  stream->offset = stream->length = 0;
  return 1;
}

STREAM_CDECL STREAM *_stream_setcap(STREAM *stream, const size_t capacity)
{
  size_t mincap =
    capacity < STREAM_MIN_CAPACITY ? STREAM_MIN_CAPACITY : capacity;
  if (!stream)
  {
    stream = (STREAM *) malloc(sizeof(STREAM));
    if (!stream)
    {
      fprintf(stderr, "ERROR: Memory allocation error");
      return NULL;
    }
    stream->offset = 0;
    stream->length = 0;
    stream->buffer = (BYTE *) malloc(mincap);
    if (!stream->buffer)
    {
      fprintf(stderr, "ERROR: Memory allocation error");
      return NULL;
    }
  }
  if (stream->capacity == mincap)
  {
    return stream;
  }
  stream->buffer = (BYTE *) realloc(stream->buffer, mincap);
  if (!stream->buffer)
  {
    fprintf(stderr, "ERROR: Memory allocation error");
    return NULL;
  }
  return stream;
}

STREAM_CDECL STREAM *_stream_growcap(STREAM *stream, const size_t mincap)
{
  if (!stream || (stream_lengthu(stream) < mincap))
  {
    return _stream_setcap(stream, mincap);
  }
  return stream;
}

STREAM_CDECL STREAM *_stream_reoffset(STREAM *stream, const size_t pos)
{
  if (!stream)
  {
    return stream;
  }
  size_t i = min(pos, stream->capacity - stream->length);
  if (i == stream->offset)
  {
    return stream;
  }
  memmove(stream->buffer + stream->offset, stream->buffer + i, stream->length);
  stream->offset = i;
  if (stream->offset > stream->length)
  {
    stream = _stream_reoffset(stream, 0);
  }
  return stream;
}

STREAM_CDECL STREAM *_stream_pushbz(STREAM *stream, const size_t length)
{
  size_t slen = stream_lengthu(stream);
  stream      = _stream_growcap(stream, slen + length);
  memset(stream->buffer + stream->offset + slen, 0, length);
  stream->length += length;
  return stream;
}

STREAM_CDECL STREAM *_stream_insbz(STREAM *stream, const size_t length,
                                   const size_t i)
{
  size_t slen = stream_lengthu(stream);
  stream      = _stream_growcap(stream, slen + length);
  memmove(stream->buffer + stream->offset + i + length,
          stream->buffer + stream->offset + i, length);
  memset(stream->buffer + stream->offset + i, 0, length);
  stream->length += length;
  return stream;
}

STREAM_CDECL STREAM *_stream_pushb(STREAM *stream, const size_t length,
                                   const BYTE *Bytes)
{
  size_t slen = stream_lengthu(stream);
  stream      = _stream_growcap(stream, slen + length);
  memcpy(stream->buffer + stream->offset + slen, Bytes, length);
  stream->length += length;
  return stream;
}

STREAM_CDECL STREAM *_stream_insb(STREAM *stream, const size_t length,
                                  const BYTE *Bytes, const size_t i)
{
  size_t slen = stream_lengthu(stream);
  stream      = _stream_growcap(stream, slen + length);
  memmove(stream->buffer + stream->offset + i + length,
          stream->buffer + stream->offset + i, length);
  memcpy(stream->buffer + stream->offset + i, Bytes, length);
  stream->length += length;
  return stream;
}

STREAM_CDECL STREAM_VOlATILE BYTE *_stream_popb(STREAM *stream,
                                                const size_t length)
{
  size_t maxlen = min(length, stream->length);
  BYTE *result  = stream->buffer + stream->offset + stream->length - maxlen;
  stream->length -= maxlen;
  return result;
}

STREAM_CDECL STREAM_VOlATILE BYTE *_stream_deqb(STREAM *stream,
                                                const size_t length)
{
  size_t maxlen = min(length, stream->length);
  stream->length -= maxlen;
  BYTE *result = stream->buffer + stream->offset;
  stream->offset += maxlen;
  if (stream->offset > stream->length)
  {
    stream = _stream_reoffset(stream, 0);
  }
  return result;
}

STREAM_CDECL int _stream_seek(STREAM *stream, ptrdiff_t offset, int whence)
{
  if (!stream || !stream->buffer)
  {
    return 0;
  }
  size_t newOffset = 0;
  switch (whence)
  {
    case SEEK_SET:
      newOffset = offset;
      break;
    case SEEK_CUR:
      newOffset = stream->offset + offset;
      break;
    case SEEK_END:
      newOffset = stream->length + offset;
      break;
    default:
      return 0;
  }
  if (newOffset > stream->length)
  {
    return 0;
  }
  stream->offset = newOffset;
  return 1;
}

STREAM_CDECL size_t _stream_read(STREAM *stream, void *ptr, size_t size,
                                 size_t count)
{
  if (!stream || !stream->buffer || !ptr || !size || !count)
  {
    return 0;
  }
  size_t bytesToRead = size * count;
  if (bytesToRead > stream->length)
  {
    count       = stream->length / size;
    bytesToRead = size * count;
  }
  memcpy(ptr, (void *) stream->buffer, bytesToRead);
  return count;
}

STREAM_CDECL size_t _stream_write(STREAM *stream, const void *ptr, size_t size,
                                  size_t count)
{
  if (!stream || !ptr || !size || !count)
  {
    return 0;
  };
  size_t bytesToWrite = size * count;
  if (!stream_growcap(stream, bytesToWrite))
  {
    return 0;
  }
  memcpy((void *) stream->buffer, ptr, bytesToWrite);
  stream->length += bytesToWrite;
  return count;
}

STREAM_CDECL STREAMITER _streamiter(STREAM *stream)
{
  if (!stream)
  {
    return NULL;
  }
  STREAMITER iter = malloc(sizeof(struct STREAMITER));
  iter->stream    = stream;
  iter->offset    = stream->offset;
  iter->i         = 0;
  return iter;
}

STREAM_CDECL void _streamiter_reset(STREAMITER iter)
{
  if (iter->stream)
  {
    iter->offset = iter->stream->offset;
    iter->i      = 0;
  }
}

STREAM_CDECL BYTE _streamiter_curr(STREAMITER iter)
{
  if (!_streamiter_hasnext(iter))
  {
    return 0;
  }
  BYTE curr = (iter->stream->buffer)[iter->offset + iter->i];
  return curr;
}

STREAM_CDECL BYTE _streamiter_next(STREAMITER iter)
{
  if (!_streamiter_hasnext(iter))
  {
    return 0;
  }
  BYTE curr = iter->stream->buffer[iter->offset + iter->i];
  iter->i++;
  return curr;
}

STREAM_CDECL int _streamiter_hasnext(STREAMITER iter)
{
  return (iter && iter->stream &&
          (iter->offset + iter->i) < iter->stream->length);
}

STREAM_CDECL STREAMRITER _streamriter(STREAM *stream)
{
  if (!stream)
  {
    return NULL;
  }
  STREAMRITER iter = malloc(sizeof(struct STREAMITER));
  iter->stream     = stream;
  iter->offset     = stream->offset;
  iter->i          = 0;
  return iter;
}

STREAM_CDECL BYTE _streamriter_curr(STREAMRITER iter)
{
  if (!_streamriter_hasnext(iter))
  {
    return 0;
  }
  BYTE curr =
    (iter->stream->buffer)[iter->offset + iter->stream->length - iter->i];
  return curr;
}

STREAM_CDECL BYTE _streamriter_next(STREAMRITER iter)
{
  if (!_streamriter_hasnext(iter))
  {
    return 0;
  }
  BYTE curr =
    iter->stream->buffer[iter->offset + iter->stream->length - iter->i];
  iter->i++;
  return curr;
}

STREAM_CDECL int _streamriter_hasnext(STREAMRITER iter)
{
  return (iter && iter->stream &&
          (iter->offset + iter->i) <= iter->stream->length);
}

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
