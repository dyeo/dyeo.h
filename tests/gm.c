
#include "../gm.h"

#include <stdio.h>

int main() {
  vec2i a = v2i(1, 2);
  vec2i b = v2i(1, 1);
  vec2i c = v2i_add(a, b);
  printf(GM_VEC_FMT(2, int) "\n", GM_VEC_VALS(2, c));
  mat4f m = m4f_mul(m4f(1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6),
                    m4f(1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2, 3, 4, 5, 6));
  mat4 l, u;
  m4f_ludecomp(m, &l, &u);
  mat4f t = m4f_transpose(l);
  printf(GM_MAT_FMT(4, float) "\n", GM_MAT_VALS(4, t));
  printf(GM_MAT_FMT(4, float) "\n", GM_MAT_VALS(4, u));
  printf("%g\n", m4f_det(m));

  vec4i xyzw = {1, 2, 3, 4};
  vec3i wyx = GM_SWIZZLE(xyzw, wyx);
  printf(GM_VEC_FMT(3, int) "\n", GM_VEC_VALS(3, wyx));

  printf("%f\n", v3f_len(v3f(10, 15, 20)));

  mat4 trs = m4f_trs(v3f(3, 4, 5), v3f(45, 0, 0), v3f(3, 3, 3));
  printf(GM_MAT_FMT(4, float) "\n", GM_MAT_VALS(4, trs));

  for
    GM_FOREACH(i, trs) {
      printf("%g", (trs.a[i]));
      printf("\n");
    }

  quat q = qf_aangle(afrads(1), v3f_up);
  quat cj = qf_conj(q);
  printf(GM_QUAT_FMT(float) "\n", GM_QUAT_VALS(cj));
}