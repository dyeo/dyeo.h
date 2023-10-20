#define STREAM_IMPLEMENTATION
#include "../stream.h"

int main(void) {
  STREAM *s = NULL;
  if (spushb(s, 14, "Hello, world!\0")) {
    STREAMITER i = siter(s);
    while (sihasnext(i)) {
      printf("%c", sicurr(i));
      sinext(i);
    }
    free(i);
    STREAMRITER j = sriter(s);
    while (srihasnext(j)) {
      printf("%c", sricurr(j));
      srinext(j);
    }
    free(j);
    printf("\n");
    sdeqb(s, 3);
    printf("%d\n", (int)s->length);
    sinsbz(s, 5, 4);
    printf("%d\n", (int)s->length);
    spopb(s, 7);
    sclear(s);
    printf("%d\n", (int)s->length);
    sfree(s);
  }
}