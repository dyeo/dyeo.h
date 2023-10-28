#define UTF8_IMPLEMENTATION
#include "../utf8.h"
#include "../test.h"

int main()
{
    test_group(utf8, {
        test_group(utfcpy, {
            char dest[20];
            char *result = utfcpy(dest, "你好");
            test_true(result == dest);
            test_true(strcmp(dest, "你好") == 0);
        });

        test_group(utfncpy, {
            char dest[20];
            char *result = utfncpy(dest, "你好世界", 2);
            test_true(result == dest);
            test_true(strcmp(dest, "你好") == 0);
        });

        test_group(utfcat, {
            char dest[20] = "你";
            char *result  = utfcat(dest, "好");
            test_true(result == dest);
            test_true(strcmp(dest, "你好") == 0);
        });

        test_group(utfncat, {
            char dest[20] = "你";
            char *result  = utfncat(dest, "好世界", 1);
            test_true(result == dest);
            test_true(strcmp(dest, "你好") == 0);
        });

        test_group(utfstr, {
            const char *haystack = "你好世界";
            char *result         = utfstr(haystack, "好世");
            test_true(result != NULL);
            test_true(strcmp(result, "好世界") == 0);
        });

        test_group(utfreverse, {
            char str[20] = "你好";
            char *result = utfreverse(str);
            test_true(result == str);
            test_true(strcmp(str, "好你") == 0); // Checking if the string is reversed correctly
        });
    });
}
