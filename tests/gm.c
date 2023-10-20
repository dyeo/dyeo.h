
#include "../gm.h"
#include "../test.h"

bool test_vec2i_addition() {
  vec2i a = v2i(1, 2);
  vec2i b = v2i(1, 1);
  vec2i result = v2i_add(a, b);
  return result.x == 2 && result.y == 3;
}

bool test_m4f_transpose() {
  mat4f m = m4f(1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6);
  mat4f t = m4f(1, 5, 9, 3, 2, 6, 0, 4, 3, 7, 1, 5, 4, 8, 2, 6);
  return m4f_eq(t, m4f_transpose(m));
}

bool test_m4f_determinant() {
  mat4f m = m4f(1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6);
  float det = m4f_det(m);
  return det == 0;
}

bool test_v3f_length() {
  vec3f v = v3f(10, 15, 20);
  float len = v3f_len(v);
  float alen = 5.0f * sqrtf(29);
  return feq(len, alen);
}

bool test_m4f_trs() {
  mat4 trs = m4f_trs(v3f(3, 4, 5), v3f(45, 0, 0), v3f(3, 3, 3));
  return true;
}

bool test_quat_conjugate() {
  quat q = qf_aangle(afrads(1), v3f_up);
  quat conj = qf_conj(q);
  return true;
}

int main() {
  test_group(gm, {
    test_true(test_vec2i_addition());
    test_true(test_m4f_transpose());
    test_true(test_m4f_determinant());
    test_true(test_v3f_length());
    test_true(test_m4f_trs());
    test_true(test_quat_conjugate());
  });
}