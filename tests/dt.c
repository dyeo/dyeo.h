#define DT_IMPLEMENTATION
#include "../dt.h"
#include "../test.h"

int main()
{
  const char *files[] = {"./res/test.dt", "./res/test.json", "./res/mid.json",
                         "./res/large.json"};
  test_group(dt, {
    for (int i = 0; i < 1; ++i)
    {
      const char *filepath = files[i];
      dt_node *node        = dt_loadf(filepath);
      test_true(node != NULL);

      size_t len  = 0;
      byte *bytes = dt_dumpb(node, &len);
      test_true(bytes != NULL);

      dt_free(node);
    }
  });
  return 0;
}