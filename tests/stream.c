#define STREAM_IMPLEMENTATION
#include "../stream.h"
#include "../test.h"

int main(void)
{
  test_group(stream, {
    STREAM *s = NULL;
    test_true(spushb(s, strlen("Hello, world!"), "Hello, world!"));
    test_expr(slength(s), int, strlen("Hello, world!"));
  });
}