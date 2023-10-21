#define DT_IMPLEMENTATION
#include "../dt.h"
#include "../test.h"

char *load_file(const char *filepath)
{
  FILE *file;
  char *buffer;
  size_t file_size;

  file = fopen(filepath, "rb");
  if (file == NULL)
  {
    fprintf(stderr, "ERROR: Couldn't opening file\n");
    return NULL;
  }

  // Get the file size
  fseek(file, 0, SEEK_END);
  file_size = ftell(file);
  rewind(file);

  buffer = (char *) malloc(file_size + 1); // +1 for null terminator
  if (buffer == NULL)
  {
    fprintf(stderr, "ERROR: Memory allocation failure\n");
    fclose(file);
    return NULL;
  }

  if (fread(buffer, 1, file_size, file) != file_size)
  {
    fprintf(stderr, "ERROR: Counldn't read from file\n");
    free(buffer);
    fclose(file);
    return NULL;
  }

  buffer[file_size] = '\0';
  fclose(file);
  return buffer;
}

int main()
{
  const char *files[] = {"./res/test.dt", "./res/test.json"};
  test_group(dt, {
    for (int i = 1; i < 2; ++i)
    {
      const char *filepath = files[i];
      char *dt_file        = load_file(filepath);
      dt_node *node        = dt_loads(dt_file);
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