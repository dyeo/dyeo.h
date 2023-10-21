#define DT_IMPLEMENTATION
#include "../dt.h"
#include "../test.h"

int main()
{
  const char *files[] = {"./res/test.dt", "./res/test.json"};
  test_group(dt, {
    for (int i = 1; i < 2; ++i)
    {
      const char *filepath = files[i];
      dt_node *node        = dt_loadf(filepath);
      test_true(node != NULL);
      dt_dumps_settings_t set = {.force_json = true};
      printf("%s\n", dt_dumps(node));

      size_t len  = 0;
      byte *bytes = dt_dumpb(node, &len);
      test_true(len != 0 && bytes != NULL);
      dt_print_bytes(len, bytes);
      printf("\n");

      dt_node *bnode = dt_loadb(len, bytes);
      test_true(bnode != NULL);
      printf("%s\n", dt_dumps_ex(bnode, &set));
      dt_node *feat = dt_gets(node, "settings", "features", 0);
      test_true(feat != NULL);
      printf("%s\n", feat->string_v);
      dt_free(node);
    }
  });
  return 0;
}