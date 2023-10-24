#ifndef _GM_H
#define _GM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <float.h>
#include <math.h>
#include <stdint.h>

// universal macros do not touch

#define GM_VAL(X) GM_VAL_IMPL(V)
#define GM_VAL_IMPL(V) V
#define GM_CONCAT(X, Y) GM_CONCAT_IMPL(X, Y)
#define GM_CONCAT_IMPL(X, Y) X##Y
#define GM_CONCAT_1(X, Y, Z) GM_CONCAT_1_IMPL(X, Y, Z)
#define GM_CONCAT_1_IMPL(X, Y, Z) X##Y##Z

// macros for renaming types and functions
// pretty easy to make invalid names, so probably dont mess with this

#ifndef GM_ANG_TYPENAME
#define GM_ANG_TYPENAME(BASETYPE, TYPEPREFIX) GM_CONCAT(ang, TYPEPREFIX)
#endif
#ifndef GM_ANG_SHORTNAME
#define GM_ANG_SHORTNAME(BASETYPE, TYPEPREFIX) GM_CONCAT(a, TYPEPREFIX)
#endif

#ifndef GM_VEC_DEFAULT_TYPE
#define GM_VEC_DEFAULT_BASETYPE float
#define GM_VEC_DEFAULT_TYPEPREFIX f
#endif
#ifndef GM_VEC_TYPENAME
#define GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N) GM_CONCAT_1(vec, N, TYPEPREFIX)
#endif
#ifndef GM_VEC_SHORTNAME
#define GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, N) GM_CONCAT_1(v, N, TYPEPREFIX)
#endif
#ifndef GM_VEC_DEFAULTNAME
#define GM_VEC_DEFAULTNAME(BASETYPE, TYPEPREFIX, N) GM_CONCAT(vec, N)
#endif

#ifndef GM_QUAT_DEFAULT_TYPE
#define GM_QUAT_DEFAULT_BASETYPE float
#define GM_QUAT_DEFAULT_TYPEPREFIX f
#endif
#ifndef GM_QUAT_TYPENAME
#define GM_QUAT_TYPENAME(BASETYPE, TYPEPREFIX) GM_CONCAT(quat, TYPEPREFIX)
#endif
#ifndef GM_QUAT_SHORTNAME
#define GM_QUAT_SHORTNAME(BASETYPE, TYPEPREFIX) GM_CONCAT(q, TYPEPREFIX)
#endif
#ifndef GM_QUAT_DEFAULTNAME
#define GM_QUAT_DEFAULTNAME(BASETYPE, TYPEPREFIX) quat
#endif

#ifndef GM_MAT_DEFAULT_TYPE
#define GM_MAT_DEFAULT_BASETYPE float
#define GM_MAT_DEFAULT_TYPEPREFIX f
#endif
#ifndef GM_MAT_TYPENAME
#define GM_MAT_TYPENAME(BASETYPE, TYPEPREFIX, M, N)                            \
  GM_CONCAT_1(mat, N, TYPEPREFIX)
#endif
#ifndef GM_MAT_SHORTNAME
#define GM_MAT_SHORTNAME(BASETYPE, TYPEPREFIX, M, N)                           \
  GM_CONCAT_1(m, N, TYPEPREFIX)
#endif
#ifndef GM_MAT_DEFAULTNAME
#define GM_MAT_DEFAULTNAME(BASETYPE, TYPEPREFIX, M, N) GM_CONCAT(mat, N)
#endif

#ifndef GM_OPERNAME
#define GM_OPERNAME(SHORTNAME, OPER) GM_CONCAT_1(SHORTNAME, _, OPER)
#endif
#ifndef GM_OPNAME
#define GM_OPNAME(BASETYPE, OP) _gm_##BASETYPE##_##OP
#endif

#define GM_MAT2X2_TYPENAME(T, P) GM_MAT_TYPENAME(T, P, 4, 2)
#define GM_MAT3X3_TYPENAME(T, P) GM_MAT_TYPENAME(T, P, 9, 3)
#define GM_MAT4X4_TYPENAME(T, P) GM_MAT_TYPENAME(T, P, 16, 4)
#define GM_MAT2X2_SHORTNAME(T, P) GM_MAT_SHORTNAME(T, P, 4, 2)
#define GM_MAT3X3_SHORTNAME(T, P) GM_MAT_SHORTNAME(T, P, 9, 3)
#define GM_MAT4X4_SHORTNAME(T, P) GM_MAT_SHORTNAME(T, P, 16, 4)

#define GM_VEC2_TYPENAME(T, P) GM_VEC_TYPENAME(T, P, 2)
#define GM_VEC3_TYPENAME(T, P) GM_VEC_TYPENAME(T, P, 3)
#define GM_VEC4_TYPENAME(T, P) GM_VEC_TYPENAME(T, P, 4)
#define GM_VEC2_SHORTNAME(T, P) GM_VEC_SHORTNAME(T, P, 2)
#define GM_VEC3_SHORTNAME(T, P) GM_VEC_SHORTNAME(T, P, 3)
#define GM_VEC4_SHORTNAME(T, P) GM_VEC_SHORTNAME(T, P, 4)

// macros for renaming constructors and operators
// NAME and SHORTNAME below are from
// GM_VEC_TYPENAME mnd GM_VEC_SHORTNAME
// or
// GM_MAT_TYPENAME mnd GM_MAT_SHORTNAME
// for uniformity

#ifndef GM_CDECL
#define GM_CDECL static inline
#endif
#ifndef GM_CONST
#define GM_CONST static const
#endif

//

#ifndef bool
typedef unsigned char bool;
#endif
#ifndef true
#define true ((bool) 1)
#endif
#ifndef false
#define false ((bool) 0)
#endif
#ifndef sbyte
typedef signed char sbyte;
#endif
#ifndef byte
typedef unsigned char byte;
#endif
#ifndef ushort
typedef unsigned short int ushort;
#endif
#ifndef uint
typedef unsigned int uint;
#endif
#ifndef ulong
typedef long unsigned int ulong;
#endif
#ifndef ldouble
typedef long double ldouble;
#endif

// for any of these, define GM_(TYPE)_CUSTOM_X_LIST as necessary
// also define:
// _gm_(BASETYPE)_fmt, _gm_(BASETYPE)_sqrt, _gm_(BASETYPE)_dot,
// _gm_(BASETYPE)_add, _gm_(BASETYPE)_sub, _gm_(BASETYPE)_mul,
// _gm_(BASETYPE)_cmul, _gm_(BASETYPE)_div, _gm_(BASETYPE)_mod

#ifndef GM_ANG_T_CUSTOM_X_LIST
#define GM_ANG_T_CUSTOM_X_LIST
#endif
#define GM_ANG_T_X_LIST                                                        \
  X(float, f)                                                                  \
  X(double, d)                                                                 \
  X(ldouble, ld)                                                               \
  GM_ANG_T_CUSTOM_X_LIST

#ifndef GM_VEC2I_T_CUSTOM_X_LIST
#define GM_VEC2I_T_CUSTOM_X_LIST
#endif
#define GM_VEC2I_T_X_LIST                                                      \
  X(sbyte, sb, 2, x, y)                                                        \
  X(byte, b, 2, x, y)                                                          \
  X(short, s, 2, x, y)                                                         \
  X(ushort, us, 2, x, y)                                                       \
  X(int, i, 2, x, y)                                                           \
  X(uint, u, 2, x, y)                                                          \
  X(long, l, 2, x, y)                                                          \
  X(ulong, lu, 2, x, y)                                                        \
  GM_VEC2I_T_CUSTOM_X_LIST

#ifndef GM_VEC3I_T_CUSTOM_X_LIST
#define GM_VEC3I_T_CUSTOM_X_LIST
#endif
#define GM_VEC3I_T_X_LIST                                                      \
  X(sbyte, sb, 3, x, y, z)                                                     \
  X(byte, b, 3, x, y, z)                                                       \
  X(short, s, 3, x, y, z)                                                      \
  X(ushort, us, 3, x, y, z)                                                    \
  X(int, i, 3, x, y, z)                                                        \
  X(uint, u, 3, x, y, z)                                                       \
  X(long, l, 3, x, y, z)                                                       \
  X(ulong, lu, 3, x, y, z)                                                     \
  GM_VEC3I_T_CUSTOM_X_LIST

#ifndef GM_VEC4I_T_CUSTOM_X_LIST
#define GM_VEC4I_T_CUSTOM_X_LIST
#endif
#define GM_VEC4I_T_X_LIST                                                      \
  X(sbyte, sb, 4, x, y, z, w)                                                  \
  X(byte, b, 4, x, y, z, w)                                                    \
  X(short, s, 4, x, y, z, w)                                                   \
  X(ushort, us, 4, x, y, z, w)                                                 \
  X(int, i, 4, x, y, z, w)                                                     \
  X(uint, u, 4, x, y, z, w)                                                    \
  X(long, l, 4, x, y, z, w)                                                    \
  X(ulong, lu, 4, x, y, z, w)                                                  \
  GM_VEC4I_T_CUSTOM_X_LIST

#ifndef GM_VEC2F_T_CUSTOM_X_LIST
#define GM_VEC2F_T_CUSTOM_X_LIST
#endif
#define GM_VEC2F_T_X_LIST                                                      \
  X(float, f, 2, x, y)                                                         \
  X(double, d, 2, x, y)                                                        \
  X(ldouble, ld, 2, x, y)                                                      \
  GM_VEC2F_T_CUSTOM_X_LIST

#ifndef GM_VEC3F_T_CUSTOM_X_LIST
#define GM_VEC3F_T_CUSTOM_X_LIST
#endif
#define GM_VEC3F_T_X_LIST                                                      \
  X(float, f, 3, x, y, z)                                                      \
  X(double, d, 3, x, y, z)                                                     \
  X(ldouble, ld, 3, x, y, z)                                                   \
  GM_VEC3F_T_CUSTOM_X_LIST

#ifndef GM_VEC4F_T_CUSTOM_X_LIST
#define GM_VEC4F_T_CUSTOM_X_LIST
#endif
#define GM_VEC4F_T_X_LIST                                                      \
  X(float, f, 4, x, y, z, w)                                                   \
  X(double, d, 4, x, y, z, w)                                                  \
  X(ldouble, ld, 4, x, y, z, w)                                                \
  GM_VEC4F_T_CUSTOM_X_LIST

#ifndef GM_QUAT_T_CUSTOM_X_LIST
#define GM_QUAT_T_CUSTOM_X_LIST
#endif
#define GM_QUAT_T_X_LIST                                                       \
  X(float, f, 4, w, x, y, z)                                                   \
  X(double, d, 4, w, x, y, z)                                                  \
  X(ldouble, ld, 4, w, x, y, z)                                                \
  GM_QUAT_T_CUSTOM_X_LIST

#ifndef GM_MAT2X2I_T_CUSTOM_X_LIST
#define GM_MAT2X2I_T_CUSTOM_X_LIST
#endif
#define GM_MAT2X2I_T_X_LIST                                                    \
  X(sbyte, sb, 4, 2, 1, 1)                                                     \
  X(byte, b, 4, 2, 1, 1)                                                       \
  X(short, s, 4, 2, 1, 1)                                                      \
  X(ushort, us, 4, 2, 1, 1)                                                    \
  X(int, i, 4, 2, 1, 1)                                                        \
  X(uint, u, 4, 2, 1, 1)                                                       \
  X(long, l, 4, 2, 1, 1)                                                       \
  X(ulong, lu, 4, 2, 1, 1)                                                     \
  GM_MAT2X2I_T_CUSTOM_X_LIST

#ifndef GM_MAT3X3I_T_CUSTOM_X_LIST
#define GM_MAT3X3I_T_CUSTOM_X_LIST
#endif
#define GM_MAT3X3I_T_X_LIST                                                    \
  X(sbyte, sb, 9, 3, 4, 2)                                                     \
  X(byte, b, 9, 3, 4, 2)                                                       \
  X(short, s, 9, 3, 4, 2)                                                      \
  X(ushort, us, 9, 3, 4, 2)                                                    \
  X(int, i, 9, 3, 4, 2)                                                        \
  X(uint, u, 9, 3, 4, 2)                                                       \
  X(long, l, 9, 3, 4, 2)                                                       \
  X(ulong, lu, 9, 3, 4, 2)                                                     \
  GM_MAT3X3I_T_CUSTOM_X_LIST

#ifndef GM_MAT4X4I_T_CUSTOM_X_LIST
#define GM_MAT4X4I_T_CUSTOM_X_LIST
#endif
#define GM_MAT4X4I_T_X_LIST                                                    \
  X(sbyte, sb, 16, 4, 9, 3)                                                    \
  X(byte, b, 16, 4, 9, 3)                                                      \
  X(short, s, 16, 4, 9, 3)                                                     \
  X(ushort, us, 16, 4, 9, 3)                                                   \
  X(int, i, 16, 4, 9, 3)                                                       \
  X(uint, u, 16, 4, 9, 3)                                                      \
  X(long, l, 16, 4, 9, 3)                                                      \
  X(ulong, lu, 16, 4, 9, 3)                                                    \
  GM_MAT4X4I_T_CUSTOM_X_LIST

#ifndef GM_MAT2X2F_T_CUSTOM_X_LIST
#define GM_MAT2X2F_T_CUSTOM_X_LIST
#endif
#define GM_MAT2X2F_T_X_LIST                                                    \
  X(float, f, 4, 2, 1, 1)                                                      \
  X(double, d, 4, 2, 1, 1)                                                     \
  X(ldouble, ld, 4, 2, 1, 1)                                                   \
  GM_MAT2X2F_T_CUSTOM_X_LIST

#ifndef GM_MAT3X3F_T_CUSTOM_X_LIST
#define GM_MAT3X3F_T_CUSTOM_X_LIST
#endif
#define GM_MAT3X3F_T_X_LIST                                                    \
  X(float, f, 9, 3, 4, 2)                                                      \
  X(double, d, 9, 3, 4, 2)                                                     \
  X(ldouble, ld, 9, 3, 4, 2)                                                   \
  GM_MAT3X3F_T_CUSTOM_X_LIST

#ifndef GM_MAT4X4F_T_CUSTOM_X_LIST
#define GM_MAT4X4F_T_CUSTOM_X_LIST
#endif
#define GM_MAT4X4F_T_X_LIST                                                    \
  X(float, f, 16, 4, 9, 3)                                                     \
  X(double, d, 16, 4, 9, 3)                                                    \
  X(ldouble, ld, 16, 4, 9, 3)                                                  \
  GM_MAT4X4F_T_CUSTOM_X_LIST

//
GM_CONST float GM_OPERNAME(gm, epsilon) = FLT_EPSILON;
GM_CONST float GM_OPERNAME(gm, small)   = 1.e-5f;
GM_CONST float GM_OPERNAME(gm, e) = 2.718281828459045235360287471352662497757f;
GM_CONST float GM_OPERNAME(gm,
                           log2e) = 1.442695040888963407359924681001892137427f;
GM_CONST float
  GM_OPERNAME(gm, log10e) = 0.4342944819032518276511289189166050822944f;
GM_CONST float GM_OPERNAME(gm, pi) = 3.141592653589793238462643383279502884197f;
GM_CONST float GM_OPERNAME(gm,
                           sqrtpi) = 1.772453850905516027298167483341145182798f;
GM_CONST float
  GM_OPERNAME(gm, inv_pi) = 0.3183098861837906715377675267450287240689f;
GM_CONST float
  GM_OPERNAME(gm, inv_sqrtpi)   = 0.5641895835477562869480794515607725858441f;
GM_CONST float GM_OPERNAME(gm,
                           tau) = 6.283185307179586476925286766559005768394f;
GM_CONST float
  GM_OPERNAME(gm, sqrttau) = 2.506628274631000502415765284811045253007f;
GM_CONST float
  GM_OPERNAME(gm, inv_tau) = 0.1591549430918953357688837633725143620345f;
GM_CONST float
  GM_OPERNAME(gm, inv_sqrttau)    = 0.3989422804014326779399460599343818684759f;
GM_CONST float GM_OPERNAME(gm,
                           ln2)   = 0.6931471805599453094172321214581765680755f;
GM_CONST float GM_OPERNAME(gm,
                           ln10)  = 2.302585092994045684017991454684364207601f;
GM_CONST float GM_OPERNAME(gm,
                           sqrt2) = 1.414213562373095048801688724209698078570f;
GM_CONST float
  GM_OPERNAME(gm, inv_sqrt2)      = 0.7071067811865475244008443621048490392848f;
GM_CONST float GM_OPERNAME(gm,
                           sqrt3) = 1.732050807568877293527446341505872366943f;
GM_CONST float
  GM_OPERNAME(gm, inv_sqrt3)    = 0.5773502691896257645091487805019574556476f;
GM_CONST float GM_OPERNAME(gm,
                           phi) = 1.618033988749894848204586834365638117720f;
GM_CONST float
  GM_OPERNAME(gm, egamma) = 0.5772156649015328606065120900824024310422f;
GM_CONST float
  GM_OPERNAME(gm, catalan)        = 0.9159655941772190150546035149323841107741f;
GM_CONST float GM_OPERNAME(gm,
                           apery) = 1.202056903159594285399738161511449990765f;
GM_CONST float
  GM_OPERNAME(gm, glaisher) = 1.282427129100622636875342568869791727768f;
GM_CONST float
  GM_OPERNAME(gm, deg2rad) = 57.29577951308232087679815481410517033241f;
GM_CONST float
  GM_OPERNAME(gm, rad2deg) = 0.01745329251994329576923690768488612713443f;

GM_CONST double GM_OPERNAME(gm, epsilon_d) = DBL_EPSILON;
GM_CONST double GM_OPERNAME(gm, small_d)   = 1.e-6;
GM_CONST double GM_OPERNAME(gm,
                            e_d) = 2.718281828459045235360287471352662497757;
GM_CONST double
  GM_OPERNAME(gm, log2e_d) = 1.442695040888963407359924681001892137427;
GM_CONST double
  GM_OPERNAME(gm, log10e_d)       = 0.4342944819032518276511289189166050822944;
GM_CONST double GM_OPERNAME(gm,
                            pi_d) = 3.141592653589793238462643383279502884197;
GM_CONST double
  GM_OPERNAME(gm, sqrtpi_d) = 1.772453850905516027298167483341145182798;
GM_CONST double
  GM_OPERNAME(gm, inv_pi_d) = 0.3183098861837906715377675267450287240689;
GM_CONST double
  GM_OPERNAME(gm, inv_sqrtpi_d)    = 0.5641895835477562869480794515607725858441;
GM_CONST double GM_OPERNAME(gm,
                            tau_d) = 6.283185307179586476925286766559005768394;
GM_CONST double
  GM_OPERNAME(gm, sqrttau_d) = 2.506628274631000502415765284811045253007;
GM_CONST double
  GM_OPERNAME(gm, inv_tau_d) = 0.1591549430918953357688837633725143620345;
GM_CONST double
  GM_OPERNAME(gm, inv_sqrttau_d)   = 0.3989422804014326779399460599343818684759;
GM_CONST double GM_OPERNAME(gm,
                            ln2_d) = 0.6931471805599453094172321214581765680755;
GM_CONST double GM_OPERNAME(gm,
                            ln10_d) = 2.302585092994045684017991454684364207601;
GM_CONST double
  GM_OPERNAME(gm, sqrt2_d) = 1.414213562373095048801688724209698078570;
GM_CONST double
  GM_OPERNAME(gm, inv_sqrt2_d) = 0.7071067811865475244008443621048490392848;
GM_CONST double
  GM_OPERNAME(gm, sqrt3_d) = 1.732050807568877293527446341505872366943;
GM_CONST double
  GM_OPERNAME(gm, inv_sqrt3_d)     = 0.5773502691896257645091487805019574556476;
GM_CONST double GM_OPERNAME(gm,
                            phi_d) = 1.618033988749894848204586834365638117720;
GM_CONST double
  GM_OPERNAME(gm, egamma_d) = 0.5772156649015328606065120900824024310422;
GM_CONST double
  GM_OPERNAME(gm, catalan_d) = 0.9159655941772190150546035149323841107741;
GM_CONST double
  GM_OPERNAME(gm, apery_d) = 1.202056903159594285399738161511449990765;
GM_CONST double
  GM_OPERNAME(gm, glaisher_d) = 1.282427129100622636875342568869791727768;
GM_CONST double
  GM_OPERNAME(gm, deg2rad_d) = 57.29577951308232087679815481410517033241;
GM_CONST double
  GM_OPERNAME(gm, rad2deg_d) = 0.01745329251994329576923690768488612713443;

GM_CONST ldouble GM_OPERNAME(gm, epsilon_ld) = LDBL_EPSILON;
GM_CONST ldouble GM_OPERNAME(gm, small_ld)   = 1.e-7L;
GM_CONST ldouble GM_OPERNAME(gm,
                             e_ld) = 2.718281828459045235360287471352662497757L;
GM_CONST ldouble GM_OPERNAME(gm, log2e_ld) =
  1.442695040888963407359924681001892137427L;
GM_CONST ldouble GM_OPERNAME(gm, log10e_ld) =
  0.4342944819032518276511289189166050822944L;
GM_CONST
ldouble GM_OPERNAME(gm, pi_ld) = 3.141592653589793238462643383279502884197L;
GM_CONST ldouble GM_OPERNAME(gm, sqrtpi_ld) =
  1.772453850905516027298167483341145182798L;
GM_CONST ldouble GM_OPERNAME(gm, inv_pi_ld) =
  0.3183098861837906715377675267450287240689L;
GM_CONST ldouble GM_OPERNAME(gm, inv_sqrtpi_ld) =
  0.5641895835477562869480794515607725858441L;
GM_CONST
ldouble GM_OPERNAME(gm, tau_ld) = 6.283185307179586476925286766559005768394L;
GM_CONST ldouble GM_OPERNAME(gm, sqrttau_ld) =
  2.506628274631000502415765284811045253007L;
GM_CONST ldouble GM_OPERNAME(gm, inv_tau_ld) =
  0.1591549430918953357688837633725143620345L;
GM_CONST ldouble GM_OPERNAME(gm, inv_sqrttau_ld) =
  0.3989422804014326779399460599343818684759L;
GM_CONST
ldouble GM_OPERNAME(gm, ln2_ld) = 0.6931471805599453094172321214581765680755L;
GM_CONST
ldouble GM_OPERNAME(gm, ln10_ld) = 2.302585092994045684017991454684364207601L;
GM_CONST ldouble GM_OPERNAME(gm, sqrt2_ld) =
  1.414213562373095048801688724209698078570L;
GM_CONST ldouble GM_OPERNAME(gm, inv_sqrt2_ld) =
  0.7071067811865475244008443621048490392848L;
GM_CONST ldouble GM_OPERNAME(gm, sqrt3_ld) =
  1.732050807568877293527446341505872366943L;
GM_CONST ldouble GM_OPERNAME(gm, inv_sqrt3_ld) =
  0.5773502691896257645091487805019574556476L;
GM_CONST
ldouble GM_OPERNAME(gm, phi_ld) = 1.618033988749894848204586834365638117720L;
GM_CONST ldouble GM_OPERNAME(gm, egamma_ld) =
  0.5772156649015328606065120900824024310422L;
GM_CONST ldouble GM_OPERNAME(gm, catalan_ld) =
  0.9159655941772190150546035149323841107741L;
GM_CONST ldouble GM_OPERNAME(gm, apery_ld) =
  1.202056903159594285399738161511449990765L;
GM_CONST ldouble GM_OPERNAME(gm, glaisher_ld) =
  1.282427129100622636875342568869791727768L;
GM_CONST ldouble GM_OPERNAME(gm, deg2rad_ld) =
  57.29577951308232087679815481410517033241L;
GM_CONST ldouble GM_OPERNAME(gm, rad2deg_ld) =
  0.01745329251994329576923690768488612713443L;

#define gm_min(A, B) ((A) < (B) ? (A) : (B))
#define gm_max(A, B) ((A) > (B) ? (A) : (B))
#define gm_clamp(V, MIN, MAX) ((V) < (MIN) ? (MIN) : (V) > (MAX) ? (MAX) : (V))

GM_CDECL sbyte clampsb(const sbyte v, const sbyte min, const sbyte max)
{
  return gm_clamp(v, min, max);
}

GM_CDECL byte clampb(const byte v, const byte min, const byte max)
{
  return gm_clamp(v, min, max);
}

GM_CDECL short clamps(const short v, const short min, const short max)
{
  return gm_clamp(v, min, max);
}

GM_CDECL ushort clampus(const ushort v, const ushort min, const ushort max)
{
  return gm_clamp(v, min, max);
}

GM_CDECL int clampi(const int v, const int min, const int max)
{
  return gm_clamp(v, min, max);
}

GM_CDECL uint clampu(const uint v, const uint min, const uint max)
{
  return gm_clamp(v, min, max);
}

GM_CDECL long clampl(const long v, const long min, const long max)
{
  return gm_clamp(v, min, max);
}

GM_CDECL ulong clamplu(const ulong v, const ulong min, const ulong max)
{
  return gm_clamp(v, min, max);
}

GM_CDECL float clampf(const float v, const float min, const float max)
{
  return fmaxf(min, fminf(v, max));
}

GM_CDECL double clampd(const double v, const double min, const double max)
{
  return fmax(min, fmin(v, max));
}

GM_CDECL ldouble clampld(const ldouble v, const ldouble min, const ldouble max)
{
  return fmaxl(min, fminl(v, max));
}

GM_CDECL bool feq(const float l, const float r)
{
  const double v = l - r;
  return -GM_OPERNAME(gm, small) < v && v < GM_OPERNAME(gm, small);
}

GM_CDECL bool deq(const double l, const double r)
{
  const double v = l - r;
  return -GM_OPERNAME(gm, small_d) < v && v < GM_OPERNAME(gm, small_d);
}

GM_CDECL bool ldeq(const ldouble l, const ldouble r)
{
  const double v = l - r;
  return -GM_OPERNAME(gm, small_ld) < v && v < GM_OPERNAME(gm, small_ld);
}

GM_CDECL bool fneq(const float l, const float y)
{
  const double v = l - y;
  return -GM_OPERNAME(gm, small) >= v || v >= GM_OPERNAME(gm, small);
}

GM_CDECL bool dneq(const double l, const double y)
{
  const double v = l - y;
  return -GM_OPERNAME(gm, small_d) >= v || v >= GM_OPERNAME(gm, small_d);
}

GM_CDECL bool ldneq(const ldouble l, const ldouble y)
{
  const double v = l - y;
  return -GM_OPERNAME(gm, small_ld) >= v || v >= GM_OPERNAME(gm, small_ld);
}

GM_CDECL sbyte lerpsb(const sbyte a, const sbyte b, const sbyte t)
{
  return a + t * (b - a);
}

GM_CDECL byte lerpb(const byte a, const byte b, const byte t)
{
  return a + t * (b - a);
}

GM_CDECL ushort lerpus(const ushort a, const ushort b, const ushort t)
{
  return a + t * (b - a);
}

GM_CDECL short lerps(const short a, const short b, const short t)
{
  return a + t * (b - a);
}

GM_CDECL uint lerpu(const uint a, const uint b, const uint t)
{
  return a + t * (b - a);
}

GM_CDECL int lerpi(const int a, const int b, const int t)
{
  return a + t * (b - a);
}

GM_CDECL ulong lerplu(const ulong a, const ulong b, const ulong t)
{
  return a + t * (b - a);
}

GM_CDECL float lerpl(const long a, const long b, const long t)
{
  return a + t * (b - a);
}

GM_CDECL float lerpf(const float a, const float b, const float t)
{
  return a + t * (b - a);
}

GM_CDECL double lerpd(const double a, const double b, const double t)
{
  return a + t * (b - a);
}

GM_CDECL ldouble lerpld(const ldouble a, const ldouble b, const ldouble t)
{
  return a + t * (b - a);
}

GM_CDECL sbyte invlerpsb(const sbyte a, const sbyte b, const sbyte l)
{
  return (l - a) / (b - a);
}

GM_CDECL byte invlerpb(const byte a, const byte b, const byte l)
{
  return (l - a) / (b - a);
}

GM_CDECL ushort invlerpus(const ushort a, const ushort b, const ushort l)
{
  return (l - a) / (b - a);
}

GM_CDECL short invlerps(const short a, const short b, const short l)
{
  return (l - a) / (b - a);
}

GM_CDECL uint invlerpu(const uint a, const uint b, const uint l)
{
  return (l - a) / (b - a);
}

GM_CDECL int invlerpi(const int a, const int b, const int l)
{
  return (l - a) / (b - a);
}

GM_CDECL ulong invlerplu(const ulong a, const ulong b, const ulong l)
{
  return (l - a) / (b - a);
}

GM_CDECL float invlerpl(const long a, const long b, const long l)
{
  return (l - a) / (float) (b - a);
}

GM_CDECL float invlerpf(const float a, const float b, const float l)
{
  return (l - a) / (b - a);
}

GM_CDECL double invlerpd(const double a, const double b, const double l)
{
  return (l - a) / (b - a);
}

GM_CDECL ldouble invlerpld(const ldouble a, const ldouble b, const ldouble l)
{
  return (l - a) / (b - a);
}

GM_CDECL uint sqrtui(const uint i)
{
  if (i == 0 || i == 1)
  {
    return i;
  }
  uint x = i / 2;
  while (1)
  {
    uint y = (x + i / x) / 2;

    if (y >= x)
    {
      return x;
    }
    x = y;
  }
}

GM_CDECL int sqrti(const int i)
{
  if (i == 0 || i == 1)
  {
    return i;
  }
  int x = i / 2;
  while (1)
  {
    int y = (x + i / x) / 2;

    if (y >= x)
    {
      return x;
    }
    x = y;
  }
}

GM_CDECL ulong sqrtlu(const ulong i)
{
  if (i == 0 || i == 1)
  {
    return i;
  }
  ulong x = i / 2;
  while (1)
  {
    ulong y = (x + i / x) / 2;

    if (y >= x)
    {
      return x;
    }
    x = y;
  }
}

GM_CDECL long sqrtli(const long i)
{
  if (i == 0 || i == 1)
  {
    return i;
  }
  long x = i / 2;
  while (1)
  {
    long y = (x + i / x) / 2;

    if (y >= x)
    {
      return x;
    }
    x = y;
  }
}

#define sqrtf sqrtf
#define sqrtd sqrt
#define sqrtld sqrtl

#define _gm_sbyte_fmt "%x"
#define _gm_sbyte_sqrt(X) ((sbyte) sqrtui((uint) (X)))
#define _gm_sbyte_dot(X, Y) (X) * (Y)
#define _gm_sbyte_add(X, Y) (X) + (Y)
#define _gm_sbyte_sub(X, Y) (X) - (Y)
#define _gm_sbyte_mul(X, Y) (X) * (Y)
#define _gm_sbyte_cmul(X, Y) (X) * (Y)
#define _gm_sbyte_div(X, Y) (X) / (Y)
#define _gm_sbyte_mod(X, Y) (X) % (Y)
#define _gm_sbyte_lt(X, Y) (X) < (Y)
#define _gm_sbyte_le(X, Y) (X) <= (Y)
#define _gm_sbyte_eq(X, Y) (X) == (Y)
#define _gm_sbyte_ne(X, Y) (X) != (Y)
#define _gm_sbyte_ge(X, Y) (X) >= (Y)
#define _gm_sbyte_gt(X, Y) (X) > (Y)
#define _gm_sbyte_lshift(X, Y) (X) << (Y)
#define _gm_sbyte_rshift(X, Y) (X) >> (Y)
#define _gm_sbyte_band(X, Y) (X) & (Y)
#define _gm_sbyte_bor(X, Y) (X) | (Y)
#define _gm_sbyte_xor(X, Y) (X) ^ (Y)
#define _gm_sbyte_and(X, Y) (X) && (Y)
#define _gm_sbyte_or(X, Y) (X) || (Y)

#define _gm_byte_fmt "%X"
#define _gm_byte_sqrt(X) ((byte) sqrti((int) (X)))
#define _gm_byte_dot(X, Y) (X) * (Y)
#define _gm_byte_add(X, Y) (X) + (Y)
#define _gm_byte_sub(X, Y) (X) - (Y)
#define _gm_byte_mul(X, Y) (X) * (Y)
#define _gm_byte_cmul(X, Y) (X) * (Y)
#define _gm_byte_div(X, Y) (X) / (Y)
#define _gm_byte_mod(X, Y) (X) % (Y)
#define _gm_byte_lt(X, Y) (X) < (Y)
#define _gm_byte_le(X, Y) (X) <= (Y)
#define _gm_byte_eq(X, Y) (X) == (Y)
#define _gm_byte_ne(X, Y) (X) != (Y)
#define _gm_byte_ge(X, Y) (X) >= (Y)
#define _gm_byte_gt(X, Y) (X) > (Y)
#define _gm_byte_lshift(X, Y) (X) << (Y)
#define _gm_byte_rshift(X, Y) (X) >> (Y)
#define _gm_byte_band(X, Y) (X) & (Y)
#define _gm_byte_bor(X, Y) (X) | (Y)
#define _gm_byte_xor(X, Y) (X) ^ (Y)
#define _gm_byte_and(X, Y) (X) && (Y)
#define _gm_byte_or(X, Y) (X) || (Y)

#define _gm_ushort_fmt "%u"
#define _gm_ushort_sqrt(X) ((ushort) sqrtui((uint) (X)))
#define _gm_ushort_dot(X, Y) (X) * (Y)
#define _gm_ushort_add(X, Y) (X) + (Y)
#define _gm_ushort_sub(X, Y) (X) - (Y)
#define _gm_ushort_mul(X, Y) (X) * (Y)
#define _gm_ushort_cmul(X, Y) (X) * (Y)
#define _gm_ushort_div(X, Y) (X) / (Y)
#define _gm_ushort_mod(X, Y) (X) % (Y)
#define _gm_ushort_lt(X, Y) (X) < (Y)
#define _gm_ushort_le(X, Y) (X) <= (Y)
#define _gm_ushort_eq(X, Y) (X) == (Y)
#define _gm_ushort_ne(X, Y) (X) != (Y)
#define _gm_ushort_ge(X, Y) (X) >= (Y)
#define _gm_ushort_gt(X, Y) (X) > (Y)
#define _gm_ushort_lshift(X, Y) (X) << (Y)
#define _gm_ushort_rshift(X, Y) (X) >> (Y)
#define _gm_ushort_band(X, Y) (X) & (Y)
#define _gm_ushort_bor(X, Y) (X) | (Y)
#define _gm_ushort_xor(X, Y) (X) ^ (Y)
#define _gm_ushort_and(X, Y) (X) && (Y)
#define _gm_ushort_or(X, Y) (X) || (Y)

#define _gm_short_fmt "%lu"
#define _gm_short_sqrt(X) ((short) sqrti((int) (X)))
#define _gm_short_dot(X, Y) (X) * (Y)
#define _gm_short_add(X, Y) (X) + (Y)
#define _gm_short_sub(X, Y) (X) - (Y)
#define _gm_short_mul(X, Y) (X) * (Y)
#define _gm_short_cmul(X, Y) (X) * (Y)
#define _gm_short_div(X, Y) (X) / (Y)
#define _gm_short_mod(X, Y) (X) % (Y)
#define _gm_short_lt(X, Y) (X) < (Y)
#define _gm_short_le(X, Y) (X) <= (Y)
#define _gm_short_eq(X, Y) (X) == (Y)
#define _gm_short_ne(X, Y) (X) != (Y)
#define _gm_short_ge(X, Y) (X) >= (Y)
#define _gm_short_gt(X, Y) (X) > (Y)
#define _gm_short_lshift(X, Y) (X) << (Y)
#define _gm_short_rshift(X, Y) (X) >> (Y)
#define _gm_short_band(X, Y) (X) & (Y)
#define _gm_short_bor(X, Y) (X) | (Y)
#define _gm_short_xor(X, Y) (X) ^ (Y)
#define _gm_short_and(X, Y) (X) && (Y)
#define _gm_short_or(X, Y) (X) || (Y)

#define _gm_uint_fmt "%u"
#define _gm_uint_sqrt(X) ((uint) sqrtui((uint) (X)))
#define _gm_uint_dot(X, Y) (X) * (Y)
#define _gm_uint_add(X, Y) (X) + (Y)
#define _gm_uint_sub(X, Y) (X) - (Y)
#define _gm_uint_mul(X, Y) (X) * (Y)
#define _gm_uint_cmul(X, Y) (X) * (Y)
#define _gm_uint_div(X, Y) (X) / (Y)
#define _gm_uint_mod(X, Y) (X) % (Y)
#define _gm_uint_lt(X, Y) (X) < (Y)
#define _gm_uint_le(X, Y) (X) <= (Y)
#define _gm_uint_eq(X, Y) (X) == (Y)
#define _gm_uint_ne(X, Y) (X) != (Y)
#define _gm_uint_ge(X, Y) (X) >= (Y)
#define _gm_uint_gt(X, Y) (X) > (Y)
#define _gm_uint_lshift(X, Y) (X) << (Y)
#define _gm_uint_rshift(X, Y) (X) >> (Y)
#define _gm_uint_band(X, Y) (X) & (Y)
#define _gm_uint_bor(X, Y) (X) | (Y)
#define _gm_uint_xor(X, Y) (X) ^ (Y)
#define _gm_uint_and(X, Y) (X) && (Y)
#define _gm_uint_or(X, Y) (X) || (Y)

#define _gm_int_fmt "%d"
#define _gm_int_sqrt(X) ((int) sqrti((int) (X)))
#define _gm_int_dot(X, Y) (X) * (Y)
#define _gm_int_add(X, Y) (X) + (Y)
#define _gm_int_sub(X, Y) (X) - (Y)
#define _gm_int_mul(X, Y) (X) * (Y)
#define _gm_int_cmul(X, Y) (X) * (Y)
#define _gm_int_div(X, Y) (X) / (Y)
#define _gm_int_mod(X, Y) (X) % (Y)
#define _gm_int_lt(X, Y) (X) < (Y)
#define _gm_int_le(X, Y) (X) <= (Y)
#define _gm_int_eq(X, Y) (X) == (Y)
#define _gm_int_ne(X, Y) (X) != (Y)
#define _gm_int_ge(X, Y) (X) >= (Y)
#define _gm_int_gt(X, Y) (X) > (Y)
#define _gm_int_lshift(X, Y) (X) << (Y)
#define _gm_int_rshift(X, Y) (X) >> (Y)
#define _gm_int_band(X, Y) (X) & (Y)
#define _gm_int_bor(X, Y) (X) | (Y)
#define _gm_int_xor(X, Y) (X) ^ (Y)
#define _gm_int_and(X, Y) (X) && (Y)
#define _gm_int_or(X, Y) (X) || (Y)

#define _gm_ulong_fmt "%lu"
#define _gm_ulong_sqrt(X) ((ulong) sqrtli((long) (X)))
#define _gm_ulong_dot(X, Y) (X) * (Y)
#define _gm_ulong_add(X, Y) (X) + (Y)
#define _gm_ulong_sub(X, Y) (X) - (Y)
#define _gm_ulong_mul(X, Y) (X) * (Y)
#define _gm_ulong_cmul(X, Y) (X) * (Y)
#define _gm_ulong_div(X, Y) (X) / (Y)
#define _gm_ulong_mod(X, Y) (X) % (Y)
#define _gm_ulong_lt(X, Y) (X) < (Y)
#define _gm_ulong_le(X, Y) (X) <= (Y)
#define _gm_ulong_eq(X, Y) (X) == (Y)
#define _gm_ulong_ne(X, Y) (X) != (Y)
#define _gm_ulong_ge(X, Y) (X) >= (Y)
#define _gm_ulong_gt(X, Y) (X) > (Y)
#define _gm_ulong_lshift(X, Y) (X) << (Y)
#define _gm_ulong_rshift(X, Y) (X) >> (Y)
#define _gm_ulong_band(X, Y) (X) & (Y)
#define _gm_ulong_bor(X, Y) (X) | (Y)
#define _gm_ulong_xor(X, Y) (X) ^ (Y)
#define _gm_ulong_and(X, Y) (X) && (Y)
#define _gm_ulong_or(X, Y) (X) || (Y)

#define _gm_long_fmt "%ld"
#define _gm_long_sqrt(X) ((long) sqrtlu((ulong) (X)))
#define _gm_long_dot(X, Y) (X) * (Y)
#define _gm_long_add(X, Y) (X) + (Y)
#define _gm_long_sub(X, Y) (X) - (Y)
#define _gm_long_mul(X, Y) (X) * (Y)
#define _gm_long_cmul(X, Y) (X) * (Y)
#define _gm_long_div(X, Y) (X) / (Y)
#define _gm_long_mod(X, Y) (X) % (Y)
#define _gm_long_lt(X, Y) (X) < (Y)
#define _gm_long_le(X, Y) (X) <= (Y)
#define _gm_long_eq(X, Y) (X) == (Y)
#define _gm_long_ne(X, Y) (X) != (Y)
#define _gm_long_ge(X, Y) (X) >= (Y)
#define _gm_long_gt(X, Y) (X) > (Y)
#define _gm_long_lshift(X, Y) (X) << (Y)
#define _gm_long_rshift(X, Y) (X) >> (Y)
#define _gm_long_band(X, Y) (X) & (Y)
#define _gm_long_bor(X, Y) (X) | (Y)
#define _gm_long_xor(X, Y) (X) ^ (Y)
#define _gm_long_and(X, Y) (X) && (Y)
#define _gm_long_or(X, Y) (X) || (Y)

#define _gm_uint8_t_fmt "%x"
#define _gm_uint8_sqrt(X) ((uint8_t) sqrtui((uint) (X)))
#define _gm_uint8_t_dot(X, Y) (X) * (Y)
#define _gm_uint8_t_add(X, Y) (X) + (Y)
#define _gm_uint8_t_sub(X, Y) (X) - (Y)
#define _gm_uint8_t_mul(X, Y) (X) * (Y)
#define _gm_uint8_t_cmul(X, Y) (X) * (Y)
#define _gm_uint8_t_div(X, Y) (X) / (Y)
#define _gm_uint8_t_mod(X, Y) (X) % (Y)
#define _gm_uint8_t_lt(X, Y) (X) < (Y)
#define _gm_uint8_t_le(X, Y) (X) <= (Y)
#define _gm_uint8_t_eq(X, Y) (X) == (Y)
#define _gm_uint8_t_ne(X, Y) (X) != (Y)
#define _gm_uint8_t_ge(X, Y) (X) >= (Y)
#define _gm_uint8_t_gt(X, Y) (X) > (Y)
#define _gm_uint8_t_lshift(X, Y) (X) << (Y)
#define _gm_uint8_t_rshift(X, Y) (X) >> (Y)
#define _gm_uint8_t_band(X, Y) (X) & (Y)
#define _gm_uint8_t_bor(X, Y) (X) | (Y)
#define _gm_uint8_t_xor(X, Y) (X) ^ (Y)
#define _gm_uint8_t_and(X, Y) (X) && (Y)
#define _gm_uint8_t_or(X, Y) (X) || (Y)

#define _gm_int8_t_fmt "%c"
#define _gm_int8_sqrt(X) ((int8_t) sqrti((uint) (X)))
#define _gm_int8_t_add(X, Y) (X) + (Y)
#define _gm_int8_t_sub(X, Y) (X) - (Y)
#define _gm_int8_t_mul(X, Y) (X) * (Y)
#define _gm_int8_t_cmul(X, Y) (X) * (Y)
#define _gm_int8_t_div(X, Y) (X) / (Y)
#define _gm_int8_t_mod(X, Y) (X) % (Y)
#define _gm_int8_t_lt(X, Y) (X) < (Y)
#define _gm_int8_t_le(X, Y) (X) <= (Y)
#define _gm_int8_t_eq(X, Y) (X) == (Y)
#define _gm_int8_t_ne(X, Y) (X) != (Y)
#define _gm_int8_t_ge(X, Y) (X) >= (Y)
#define _gm_int8_t_gt(X, Y) (X) > (Y)
#define _gm_int8_t_lshift(X, Y) (X) << (Y)
#define _gm_int8_t_rshift(X, Y) (X) >> (Y)
#define _gm_int8_t_band(X, Y) (X) & (Y)
#define _gm_int8_t_bor(X, Y) (X) | (Y)
#define _gm_int8_t_xor(X, Y) (X) ^ (Y)
#define _gm_int8_t_and(X, Y) (X) && (Y)
#define _gm_int8_t_or(X, Y) (X) || (Y)

#define _gm_uint16_t_fmt "%ld"
#define _gm_uint16_sqrt(X) ((uint16_t) sqrtui((uint) (X)))
#define _gm_uint16_t_dot(X, Y) (X) * (Y)
#define _gm_uint16_t_add(X, Y) (X) + (Y)
#define _gm_uint16_t_sub(X, Y) (X) - (Y)
#define _gm_uint16_t_mul(X, Y) (X) * (Y)
#define _gm_uint16_t_cmul(X, Y) (X) * (Y)
#define _gm_uint16_t_div(X, Y) (X) / (Y)
#define _gm_uint16_t_mod(X, Y) (X) % (Y)
#define _gm_uint16_t_lt(X, Y) (X) < (Y)
#define _gm_uint16_t_le(X, Y) (X) <= (Y)
#define _gm_uint16_t_eq(X, Y) (X) == (Y)
#define _gm_uint16_t_ne(X, Y) (X) != (Y)
#define _gm_uint16_t_ge(X, Y) (X) >= (Y)
#define _gm_uint16_t_gt(X, Y) (X) > (Y)
#define _gm_uint16_t_lshift(X, Y) (X) << (Y)
#define _gm_uint16_t_rshift(X, Y) (X) >> (Y)
#define _gm_uint16_t_band(X, Y) (X) & (Y)
#define _gm_uint16_t_bor(X, Y) (X) | (Y)
#define _gm_uint16_t_xor(X, Y) (X) ^ (Y)
#define _gm_uint16_t_and(X, Y) (X) && (Y)
#define _gm_uint16_t_or(X, Y) (X) || (Y)

#define _gm_int16_t_fmt "%d"
#define _gm_int16_sqrt(X) ((int16_t) sqrti((uint) (X)))
#define _gm_int16_t_add(X, Y) (X) + (Y)
#define _gm_int16_t_sub(X, Y) (X) - (Y)
#define _gm_int16_t_mul(X, Y) (X) * (Y)
#define _gm_int16_t_cmul(X, Y) (X) * (Y)
#define _gm_int16_t_div(X, Y) (X) / (Y)
#define _gm_int16_t_mod(X, Y) (X) % (Y)
#define _gm_int16_t_lt(X, Y) (X) < (Y)
#define _gm_int16_t_le(X, Y) (X) <= (Y)
#define _gm_int16_t_eq(X, Y) (X) == (Y)
#define _gm_int16_t_ne(X, Y) (X) != (Y)
#define _gm_int16_t_ge(X, Y) (X) >= (Y)
#define _gm_int16_t_gt(X, Y) (X) > (Y)
#define _gm_int16_t_lshift(X, Y) (X) << (Y)
#define _gm_int16_t_rshift(X, Y) (X) >> (Y)
#define _gm_int16_t_band(X, Y) (X) & (Y)
#define _gm_int16_t_bor(X, Y) (X) | (Y)
#define _gm_int16_t_xor(X, Y) (X) ^ (Y)
#define _gm_int16_t_and(X, Y) (X) && (Y)
#define _gm_int16_t_or(X, Y) (X) || (Y)

#define _gm_uint32_t_fmt "%ld"
#define _gm_uint32_sqrt(X) ((uint32_t) sqrtui((uint) (X)))
#define _gm_uint32_t_dot(X, Y) (X) * (Y)
#define _gm_uint32_t_add(X, Y) (X) + (Y)
#define _gm_uint32_t_sub(X, Y) (X) - (Y)
#define _gm_uint32_t_mul(X, Y) (X) * (Y)
#define _gm_uint32_t_cmul(X, Y) (X) * (Y)
#define _gm_uint32_t_div(X, Y) (X) / (Y)
#define _gm_uint32_t_mod(X, Y) (X) % (Y)
#define _gm_uint32_t_lt(X, Y) (X) < (Y)
#define _gm_uint32_t_le(X, Y) (X) <= (Y)
#define _gm_uint32_t_eq(X, Y) (X) == (Y)
#define _gm_uint32_t_ne(X, Y) (X) != (Y)
#define _gm_uint32_t_ge(X, Y) (X) >= (Y)
#define _gm_uint32_t_gt(X, Y) (X) > (Y)
#define _gm_uint32_t_lshift(X, Y) (X) << (Y)
#define _gm_uint32_t_rshift(X, Y) (X) >> (Y)
#define _gm_uint32_t_band(X, Y) (X) & (Y)
#define _gm_uint32_t_bor(X, Y) (X) | (Y)
#define _gm_uint32_t_xor(X, Y) (X) ^ (Y)
#define _gm_uint32_t_and(X, Y) (X) && (Y)
#define _gm_uint32_t_or(X, Y) (X) || (Y)

#define _gm_int32_t_fmt "%d"
#define _gm_int32_sqrt(X) ((int32_t) sqrti((uint) (X)))
#define _gm_int32_t_dot(X, Y) (X) * (Y)
#define _gm_int32_t_add(X, Y) (X) + (Y)
#define _gm_int32_t_sub(X, Y) (X) - (Y)
#define _gm_int32_t_mul(X, Y) (X) * (Y)
#define _gm_int32_t_cmul(X, Y) (X) * (Y)
#define _gm_int32_t_div(X, Y) (X) / (Y)
#define _gm_int32_t_mod(X, Y) (X) % (Y)
#define _gm_int32_t_lt(X, Y) (X) < (Y)
#define _gm_int32_t_le(X, Y) (X) <= (Y)
#define _gm_int32_t_eq(X, Y) (X) == (Y)
#define _gm_int32_t_ne(X, Y) (X) != (Y)
#define _gm_int32_t_ge(X, Y) (X) >= (Y)
#define _gm_int32_t_gt(X, Y) (X) > (Y)
#define _gm_int32_t_lshift(X, Y) (X) << (Y)
#define _gm_int32_t_rshift(X, Y) (X) >> (Y)
#define _gm_int32_t_band(X, Y) (X) & (Y)
#define _gm_int32_t_bor(X, Y) (X) | (Y)
#define _gm_int32_t_xor(X, Y) (X) ^ (Y)
#define _gm_int32_t_and(X, Y) (X) && (Y)
#define _gm_int32_t_or(X, Y) (X) || (Y)

#define _gm_uint64_t_fmt "%lu"
#define _gm_uint64_sqrt(X) ((uint64_t) sqrtlu((uint) (X)))
#define _gm_uint64_t_dot(X, Y) (X) * (Y)
#define _gm_uint64_t_add(X, Y) (X) + (Y)
#define _gm_uint64_t_sub(X, Y) (X) - (Y)
#define _gm_uint64_t_mul(X, Y) (X) * (Y)
#define _gm_uint64_t_cmul(X, Y) (X) * (Y)
#define _gm_uint64_t_div(X, Y) (X) / (Y)
#define _gm_uint64_t_mod(X, Y) (X) % (Y)
#define _gm_uint64_t_lt(X, Y) (X) < (Y)
#define _gm_uint64_t_le(X, Y) (X) <= (Y)
#define _gm_uint64_t_eq(X, Y) (X) == (Y)
#define _gm_uint64_t_ne(X, Y) (X) != (Y)
#define _gm_uint64_t_ge(X, Y) (X) >= (Y)
#define _gm_uint64_t_gt(X, Y) (X) > (Y)
#define _gm_uint64_t_lshift(X, Y) (X) << (Y)
#define _gm_uint64_t_rshift(X, Y) (X) >> (Y)
#define _gm_uint64_t_band(X, Y) (X) & (Y)
#define _gm_uint64_t_bor(X, Y) (X) | (Y)
#define _gm_uint64_t_xor(X, Y) (X) ^ (Y)
#define _gm_uint64_t_and(X, Y) (X) && (Y)
#define _gm_uint64_t_or(X, Y) (X) || (Y)

#define _gm_int64_t_fmt "%ld"
#define _gm_int64_sqrt(X) ((int64_t) sqrtli((uint) (X)))
#define _gm_int64_t_dot(X, Y) (X) * (Y)
#define _gm_int64_t_add(X, Y) (X) + (Y)
#define _gm_int64_t_sub(X, Y) (X) - (Y)
#define _gm_int64_t_mul(X, Y) (X) * (Y)
#define _gm_int64_t_cmul(X, Y) (X) * (Y)
#define _gm_int64_t_div(X, Y) (X) / (Y)
#define _gm_int64_t_mod(X, Y) (X) % (Y)
#define _gm_int64_t_lt(X, Y) (X) < (Y)
#define _gm_int64_t_le(X, Y) (X) <= (Y)
#define _gm_int64_t_eq(X, Y) (X) == (Y)
#define _gm_int64_t_ne(X, Y) (X) != (Y)
#define _gm_int64_t_ge(X, Y) (X) >= (Y)
#define _gm_int64_t_gt(X, Y) (X) > (Y)
#define _gm_int64_t_lshift(X, Y) (X) << (Y)
#define _gm_int64_t_rshift(X, Y) (X) >> (Y)
#define _gm_int64_t_band(X, Y) (X) & (Y)
#define _gm_int64_t_bor(X, Y) (X) | (Y)
#define _gm_int64_t_xor(X, Y) (X) ^ (Y)
#define _gm_int64_t_and(X, Y) (X) && (Y)
#define _gm_int64_t_or(X, Y) (X) || (Y)

#define _gm_float_fmt "%g"
#define _gm_float_sqrt(X) ((float) sqrtf(X))
#define _gm_float_dot(X, Y) (X) * (Y)
#define _gm_float_add(X, Y) (X) + (Y)
#define _gm_float_sub(X, Y) (X) - (Y)
#define _gm_float_mul(X, Y) (X) * (Y)
#define _gm_float_cmul(X, Y) (X) * (Y)
#define _gm_float_div(X, Y) (X) / (Y)
#define _gm_float_mod(X, Y) (fmodf(X, Y))
#define _gm_float_lt(X, Y) (X) < (Y)
#define _gm_float_le(X, Y) (X) <= (Y)
#define _gm_float_eq(X, Y) (feq(X, Y))
#define _gm_float_ne(X, Y) (fneq(X, Y))
#define _gm_float_ge(X, Y) (X) >= (Y)
#define _gm_float_gt(X, Y) (X) > (Y)
#define _gm_float_lshift(X, Y) (NAN)
#define _gm_float_rshift(X, Y) (NAN)
#define _gm_float_band(X, Y) (NAN)
#define _gm_float_bor(X, Y) (NAN)
#define _gm_float_xor(X, Y) (NAN)
#define _gm_float_and(X, Y) (NAN)
#define _gm_float_or(X, Y) (NAN)

#define _gm_double_fmt "%lg"
#define _gm_double_sqrt(X) ((double) sqrt(X))
#define _gm_double_dot(X, Y) (X) * (Y)
#define _gm_double_add(X, Y) (X) + (Y)
#define _gm_double_sub(X, Y) (X) - (Y)
#define _gm_double_mul(X, Y) (X) * (Y)
#define _gm_double_cmul(X, Y) (X) * (Y)
#define _gm_double_div(X, Y) (X) / (Y)
#define _gm_double_mod(X, Y) (fmod(X, Y))
#define _gm_double_lt(X, Y) (X) < (Y)
#define _gm_double_le(X, Y) (X) <= (Y)
#define _gm_double_eq(X, Y) (deq(X, Y))
#define _gm_double_ne(X, Y) (dneq(X, Y))
#define _gm_double_ge(X, Y) (X) >= (Y)
#define _gm_double_gt(X, Y) (X) > (Y)
#define _gm_double_lshift(X, Y) (NAN)
#define _gm_double_rshift(X, Y) (NAN)
#define _gm_double_band(X, Y) (NAN)
#define _gm_double_bor(X, Y) (NAN)
#define _gm_double_xor(X, Y) (NAN)
#define _gm_double_and(X, Y) (NAN)
#define _gm_double_or(X, Y) (NAN)

#define _gm_ldouble_fmt "%lg"
#define _gm_ldouble_sqrt(X) ((double) sqrtl(X))
#define _gm_ldouble_dot(X, Y) (X) * (Y)
#define _gm_ldouble_add(X, Y) (X) + (Y)
#define _gm_ldouble_sub(X, Y) (X) - (Y)
#define _gm_ldouble_mul(X, Y) (X) * (Y)
#define _gm_ldouble_cmul(X, Y) (X) * (Y)
#define _gm_ldouble_div(X, Y) (X) / (Y)
#define _gm_ldouble_mod(X, Y) (fmodl(X, Y))
#define _gm_ldouble_lt(X, Y) (X) < (Y)
#define _gm_ldouble_le(X, Y) (X) <= (Y)
#define _gm_ldouble_eq(X, Y) (ldeq(X, Y))
#define _gm_ldouble_ne(X, Y) (ldneq(X, Y))
#define _gm_ldouble_ge(X, Y) (X) >= (Y)
#define _gm_ldouble_gt(X, Y) (X) > (Y)
#define _gm_ldouble_lshift(X, Y) (NAN)
#define _gm_ldouble_rshift(X, Y) (NAN)
#define _gm_ldouble_band(X, Y) (NAN)
#define _gm_ldouble_bor(X, Y) (NAN)
#define _gm_ldouble_xor(X, Y) (NAN)
#define _gm_ldouble_and(X, Y) (NAN)
#define _gm_ldouble_or(X, Y) (NAN)

#define GM_ARGC(...)                                                           \
  GM_ARGC_IMPL(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3,   \
               2, 1, 0)
#define GM_ARGC_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13,   \
                     _14, _15, _16, N, ...)                                    \
  N

#define GM_MACROCALL_V(MFUNC, ...)                                             \
  GM_MACROCALL_N(GM_ARGC(__VA_ARGS__), MFUNC, __VA_ARGS__)
#define GM_MACROCALL_N(N, MFUNC, ...)                                          \
  GM_CONCAT(GM_MACROCALL_, N)(MFUNC, __VA_ARGS__)
#define GM_MACROCALL_1(MFUNC, _0, ...) MFUNC(_0)
#define GM_MACROCALL_2(MFUNC, _0, _1, ...) MFUNC(_0), MFUNC(_1)
#define GM_MACROCALL_3(MFUNC, _0, _1, _2, ...) MFUNC(_0), MFUNC(_1), MFUNC(_2)
#define GM_MACROCALL_4(MFUNC, _0, _1, _2, _3, ...)                             \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3)
#define GM_MACROCALL_5(MFUNC, _0, _1, _2, _3, _4, ...)                         \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4)
#define GM_MACROCALL_6(MFUNC, _0, _1, _2, _3, _4, _5, ...)                     \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5)
#define GM_MACROCALL_7(MFUNC, _0, _1, _2, _3, _4, _5, _6, ...)                 \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5), MFUNC(_6)
#define GM_MACROCALL_8(MFUNC, _0, _1, _2, _3, _4, _5, _6, _7, ...)             \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5), MFUNC(_6), \
    MFUNC(_7)
#define GM_MACROCALL_9(MFUNC, _0, _1, _2, _3, _4, _5, _6, _7, _8, ...)         \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5), MFUNC(_6), \
    MFUNC(_7), MFUNC(_8)
#define GM_MACROCALL_10(MFUNC, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...)    \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5), MFUNC(_6), \
    MFUNC(_7), MFUNC(_8), MFUNC(_9)
#define GM_MACROCALL_11(MFUNC, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,    \
                        ...)                                                   \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5), MFUNC(_6), \
    MFUNC(_7), MFUNC(_8), MFUNC(_9), MFUNC(_10)
#define GM_MACROCALL_12(MFUNC, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,    \
                        _11, ...)                                              \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5), MFUNC(_6), \
    MFUNC(_7), MFUNC(_8), MFUNC(_9), MFUNC(_10), MFUNC(_11)
#define GM_MACROCALL_13(MFUNC, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,    \
                        _11, _12, ...)                                         \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5), MFUNC(_6), \
    MFUNC(_7), MFUNC(_8), MFUNC(_9), MFUNC(_10), MFUNC(_11), MFUNC(_12)
#define GM_MACROCALL_14(MFUNC, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,    \
                        _11, _12, _13, ...)                                    \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5), MFUNC(_6), \
    MFUNC(_7), MFUNC(_8), MFUNC(_9), MFUNC(_10), MFUNC(_11), MFUNC(_12),       \
    MFUNC(_13)
#define GM_MACROCALL_15(MFUNC, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,    \
                        _11, _12, _13, _14, ...)                               \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5), MFUNC(_6), \
    MFUNC(_7), MFUNC(_8), MFUNC(_9), MFUNC(_10), MFUNC(_11), MFUNC(_12),       \
    MFUNC(_13), MFUNC(_14)
#define GM_MACROCALL_16(MFUNC, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,    \
                        _11, _12, _13, _14, _15)                               \
  MFUNC(_0), MFUNC(_1), MFUNC(_2), MFUNC(_3), MFUNC(_4), MFUNC(_5), MFUNC(_6), \
    MFUNC(_7), MFUNC(_8), MFUNC(_9), MFUNC(_10), MFUNC(_11), MFUNC(_12),       \
    MFUNC(_13), MFUNC(_14), MFUNC(_15)

#define GM_REPEAT_N(N, X) GM_CONCAT(GM_REPEAT_, N)(X)
#define GM_REPEAT_1(X) X
#define GM_REPEAT_2(X) X X
#define GM_REPEAT_3(X) X X X
#define GM_REPEAT_4(X) X X X X
#define GM_REPEAT_5(X) X X X X X
#define GM_REPEAT_6(X) X X X X X X
#define GM_REPEAT_7(X) X X X X X X X
#define GM_REPEAT_8(X) X X X X X X X X
#define GM_REPEAT_9(X) X X X X X X X X X
#define GM_REPEAT_10(X) X X X X X X X X X X
#define GM_REPEAT_11(X) X X X X X X X X X X X
#define GM_REPEAT_12(X) X X X X X X X X X X X X
#define GM_REPEAT_13(X) X X X X X X X X X X X X X
#define GM_REPEAT_14(X) X X X X X X X X X X X X X X
#define GM_REPEAT_15(X) X X X X X X X X X X X X X X X
#define GM_REPEAT_16(X) X X X X X X X X X X X X X X X X

#define GM_COMMASEP_N(N, X) GM_CONCAT(GM_COMMASEP_, N)(X)
#define GM_COMMASEP_1(X) X
#define GM_COMMASEP_2(X) X, X
#define GM_COMMASEP_3(X) X, X, X
#define GM_COMMASEP_4(X) X, X, X, X
#define GM_COMMASEP_5(X) X, X, X, X, X
#define GM_COMMASEP_6(X) X, X, X, X, X, X
#define GM_COMMASEP_7(X) X, X, X, X, X, X, X
#define GM_COMMASEP_8(X) X, X, X, X, X, X, X, X
#define GM_COMMASEP_9(X) X, X, X, X, X, X, X, X, X
#define GM_COMMASEP_10(X) X, X, X, X, X, X, X, X, X, X
#define GM_COMMASEP_11(X) X, X, X, X, X, X, X, X, X, X, X
#define GM_COMMASEP_12(X) X, X, X, X, X, X, X, X, X, X, X, X
#define GM_COMMASEP_13(X) X, X, X, X, X, X, X, X, X, X, X, X, X
#define GM_COMMASEP_14(X) X, X, X, X, X, X, X, X, X, X, X, X, X, X
#define GM_COMMASEP_15(X) X, X, X, X, X, X, X, X, X, X, X, X, X, X, X
#define GM_COMMASEP_16(X) X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X

#define GM_SEP_N(N, DELIM, TERM) GM_CONCAT(GM_SEP_, N)(DELIM, TERM)
#define GM_SEP_1(DELIM, TERM) TERM
#define GM_SEP_2(DELIM, TERM) TERM DELIM TERM
#define GM_SEP_3(DELIM, TERM) TERM DELIM TERM DELIM TERM
#define GM_SEP_4(DELIM, TERM) TERM DELIM TERM DELIM TERM DELIM TERM
#define GM_SEP_5(DELIM, TERM) TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM
#define GM_SEP_6(DELIM, TERM)                                                  \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM
#define GM_SEP_7(DELIM, TERM)                                                  \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM
#define GM_SEP_8(DELIM, TERM)                                                  \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM \
    TERM
#define GM_SEP_9(DELIM, TERM)                                                  \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM \
    TERM DELIM TERM
#define GM_SEP_10(DELIM, TERM)                                                 \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM \
    TERM DELIM TERM DELIM TERM
#define GM_SEP_11(DELIM, TERM)                                                 \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM \
    TERM DELIM TERM DELIM TERM DELIM TERM
#define GM_SEP_12(DELIM, TERM)                                                 \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM \
    TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM
#define GM_SEP_13(DELIM, TERM)                                                 \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM \
    TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM
#define GM_SEP_14(DELIM, TERM)                                                 \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM \
    TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM
#define GM_SEP_15(DELIM, TERM)                                                 \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM \
    TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM     \
      DELIM TERM
#define GM_SEP_16(DELIM, TERM)                                                 \
  TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM \
    TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM DELIM TERM     \
      DELIM TERM DELIM TERM

#define GM_PARAMLIST_V(TYPE, ...)                                              \
  GM_ARGLIST_N(GM_ARGC(__VA_ARGS__), TYPE, __VA_ARGS__)
#define GM_PARAMLIST_N(N, TYPE, ...)                                           \
  GM_CONCAT(GM_PARAMLIST_, N)(TYPE, __VA_ARGS__)
#define GM_PARAMLIST_0(TYPE, ...)
#define GM_PARAMLIST_1(TYPE, _0, ...) TYPE _0
#define GM_PARAMLIST_2(TYPE, _0, _1, ...) TYPE _0, TYPE _1
#define GM_PARAMLIST_3(TYPE, _0, _1, _2, ...) TYPE _0, TYPE _1, TYPE _2
#define GM_PARAMLIST_4(TYPE, _0, _1, _2, _3, ...)                              \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3
#define GM_PARAMLIST_5(TYPE, _0, _1, _2, _3, _4, ...)                          \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4
#define GM_PARAMLIST_6(TYPE, _0, _1, _2, _3, _4, _5, ...)                      \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5
#define GM_PARAMLIST_7(TYPE, _0, _1, _2, _3, _4, _5, _6, ...)                  \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6
#define GM_PARAMLIST_8(TYPE, _0, _1, _2, _3, _4, _5, _6, _7, ...)              \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7
#define GM_PARAMLIST_9(TYPE, _0, _1, _2, _3, _4, _5, _6, _7, _8, ...)          \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8
#define GM_PARAMLIST_10(TYPE, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...)     \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9
#define GM_PARAMLIST_11(TYPE, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,     \
                        ...)                                                   \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10
#define GM_PARAMLIST_12(TYPE, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,     \
                        _11, ...)                                              \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10, TYPE _11
#define GM_PARAMLIST_13(TYPE, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,     \
                        _11, _12, ...)                                         \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10, TYPE _11, TYPE _12
#define GM_PARAMLIST_14(TYPE, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,     \
                        _11, _12, _13, ...)                                    \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10, TYPE _11, TYPE _12, TYPE _13
#define GM_PARAMLIST_15(TYPE, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,     \
                        _11, _12, _13, _14, ...)                               \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10, TYPE _11, TYPE _12, TYPE _13, TYPE _14
#define GM_PARAMLIST_16(TYPE, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10,     \
                        _11, _12, _13, _14, _15, ...)                          \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10, TYPE _11, TYPE _12, TYPE _13, TYPE _14,        \
    TYPE _15

#define GM_ARGLIST_N(N) GM_CONCAT(GM_ARGLIST_, N)()
#define GM_ARGLIST_0()
#define GM_ARGLIST_1() _0
#define GM_ARGLIST_2() _0, _1
#define GM_ARGLIST_3() _0, _1, _2
#define GM_ARGLIST_4() _0, _1, _2, _3
#define GM_ARGLIST_5() _0, _1, _2, _3, _4
#define GM_ARGLIST_6() _0, _1, _2, _3, _4, _5
#define GM_ARGLIST_7() _0, _1, _2, _3, _4, _5, _6
#define GM_ARGLIST_8() _0, _1, _2, _3, _4, _5, _6, _7
#define GM_ARGLIST_9() _0, _1, _2, _3, _4, _5, _6, _7, _8
#define GM_ARGLIST_10() _0, _1, _2, _3, _4, _5, _6, _7, _8, _9
#define GM_ARGLIST_11() _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10
#define GM_ARGLIST_12() _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11
#define GM_ARGLIST_13() _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12
#define GM_ARGLIST_14()                                                        \
  _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13
#define GM_ARGLIST_15()                                                        \
  _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14
#define GM_ARGLIST_16()                                                        \
  _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15

#define GM_ARGPARAMS_N(N, TYPE) GM_CONCAT(GM_ARGPARAMS_, N)(TYPE)
#define GM_ARGPARAMS_0(TYPE)
#define GM_ARGPARAMS_1(TYPE) TYPE _0
#define GM_ARGPARAMS_2(TYPE) TYPE _0, TYPE _1
#define GM_ARGPARAMS_3(TYPE) TYPE _0, TYPE _1, TYPE _2
#define GM_ARGPARAMS_4(TYPE) TYPE _0, TYPE _1, TYPE _2, TYPE _3
#define GM_ARGPARAMS_5(TYPE) TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4
#define GM_ARGPARAMS_6(TYPE)                                                   \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5
#define GM_ARGPARAMS_7(TYPE)                                                   \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6
#define GM_ARGPARAMS_8(TYPE)                                                   \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7
#define GM_ARGPARAMS_9(TYPE)                                                   \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8
#define GM_ARGPARAMS_10(TYPE)                                                  \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9
#define GM_ARGPARAMS_11(TYPE)                                                  \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10
#define GM_ARGPARAMS_12(TYPE)                                                  \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10, TYPE _11
#define GM_ARGPARAMS_13(TYPE)                                                  \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10, TYPE _11, TYPE _12
#define GM_ARGPARAMS_14(TYPE)                                                  \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10, TYPE _11, TYPE _12, TYPE _13
#define GM_ARGPARAMS_15(TYPE)                                                  \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10, TYPE _11, TYPE _12, TYPE _13, TYPE _14
#define GM_ARGPARAMS_16(TYPE)                                                  \
  TYPE _0, TYPE _1, TYPE _2, TYPE _3, TYPE _4, TYPE _5, TYPE _6, TYPE _7,      \
    TYPE _8, TYPE _9, TYPE _10, TYPE _11, TYPE _12, TYPE _13, TYPE _14,        \
    TYPE _15

#define GM_LENGTH(ARR) (sizeof(ARR) / sizeof *(ARR))
#define GM_FOREACH(INAME, VEC_MAT_QUAT)                                        \
  (size_t INAME = 0; INAME < GM_LENGTH((VEC_MAT_QUAT).a); ++INAME)

#define GM_VEC_FMT(N, TYPE)                                                    \
  "(" GM_SEP_N(N, ",", GM_CONCAT_1(_gm_, TYPE, _fmt)) ")"
#define GM_VEC_VALS(N, TYPE) GM_CONCAT_1(GM_VEC, N, _VALS)(TYPE)
#define GM_VEC_PVALS(N, TYPE) GM_CONCAT_1(GM_VEC, N, _PVALS)(TYPE)
#define GM_VEC2_VALS(V) (V).a[0], (V).a[1]
#define GM_VEC2_PVALS(V) (V)->a[0], (V)->a[1]
#define GM_VEC3_VALS(V) (V).a[0], (V).a[1], (V).a[2]
#define GM_VEC3_PVALS(V) (V)->a[0], (V)->a[1], (V)->a[2]
#define GM_VEC4_VALS(V) (V).a[0], (V).a[1], (V).a[2], (V).a[3]
#define GM_VEC4_PVALS(V) (V)->a[0], (V)->a[1], (V)->a[2], (V)->a[3]

#define GM_MAT_FMT(N, TYPE)                                                    \
  "[" GM_SEP_N(N, ",",                                                         \
               "(" GM_SEP_N(N, ",", GM_CONCAT_1(_gm_, TYPE, _fmt)) ")") "]"
#define GM_MAT_VALS(N, V) GM_CONCAT_1(GM_MAT, GM_CONCAT_1(N, X, N), _VALS)(V)
#define GM_MAT_PVALS(N, TYPE)                                                  \
  GM_CONCAT_1(GM_MAT, GM_CONCAT_1(N, X, N), _PVALS)(V)
#define GM_MAT2X2_VALS(V) (V).a[0], (V).a[1], (V).a[2], (V).a[3]
#define GM_MAT2X2_PVALS(V) (V)->a[0], (V)->a[1], (V)->a[2], (V)->a[3]
#define GM_MAT3X3_VALS(V)                                                      \
  (V).a[0], (V).a[1], (V).a[2], (V).a[3], (V).a[4], (V).a[5], (V).a[6],        \
    (V).a[7], (V).a[8]
#define GM_MAT3X3_PVALS(V)                                                     \
  (V).a[0], (V).a[1], (V).a[2], (V).a[3], (V).a[4], (V).a[5], (V).a[6],        \
    (V).a[7], (V).a[8]
#define GM_MAT4X4_VALS(V)                                                      \
  (V).a[0], (V).a[1], (V).a[2], (V).a[3], (V).a[4], (V).a[5], (V).a[6],        \
    (V).a[7], (V).a[8], (V).a[9], (V).a[10], (V).a[11], (V).a[12], (V).a[13],  \
    (V).a[14], (V).a[15]
#define GM_MAT4X4_PVALS(V)                                                     \
  (V)->a[0], (V)->a[1], (V)->a[2], (V)->a[3], (V)->a[4], (V)->a[5], (V)->a[6], \
    (V)->a[7], (V)->a[8], (V)->a[9], (V)->a[10], (V)->a[11], (V)->a[12],       \
    (V)->a[13], (V)->a[14], (V)->a[15]

#define GM_QUAT_FMT(TYPE)                                                      \
  "{" GM_SEP_N(4, ",", GM_CONCAT_1(_gm_, TYPE, _fmt)) "}"
#define GM_QUAT_VALS(TYPE) GM_CONCAT_1(GM_VEC, 4, _VALS)(TYPE)

#define GM_VEC_IDX(SYM) GM_CONCAT(_gm_vec_idx_, SYM)
#define _gm_vec_idx_x 0
#define _gm_vec_idx_y 1
#define _gm_vec_idx_z 2
#define _gm_vec_idx_w 3

#define GM_VEC_GETI(VEC, X) (VEC).a[(X)]
#define GM_VEC_GET(VEC, X) GM_VEC_GETI(VEC, GM_VEC_IDX(X))

#define _gm_swz_ww(VEC) (VEC).a[3], (VEC).a[3]
#define _gm_swz_wx(VEC) (VEC).a[3], (VEC).a[0]
#define _gm_swz_wy(VEC) (VEC).a[3], (VEC).a[1]
#define _gm_swz_wz(VEC) (VEC).a[3], (VEC).a[2]
#define _gm_swz_xw(VEC) (VEC).a[0], (VEC).a[3]
#define _gm_swz_xx(VEC) (VEC).a[0], (VEC).a[0]
#define _gm_swz_xy(VEC) (VEC).a[0], (VEC).a[1]
#define _gm_swz_xz(VEC) (VEC).a[0], (VEC).a[2]
#define _gm_swz_yw(VEC) (VEC).a[1], (VEC).a[3]
#define _gm_swz_yx(VEC) (VEC).a[1], (VEC).a[0]
#define _gm_swz_yy(VEC) (VEC).a[1], (VEC).a[1]
#define _gm_swz_yz(VEC) (VEC).a[1], (VEC).a[2]
#define _gm_swz_zw(VEC) (VEC).a[2], (VEC).a[3]
#define _gm_swz_zx(VEC) (VEC).a[2], (VEC).a[0]
#define _gm_swz_zy(VEC) (VEC).a[2], (VEC).a[1]
#define _gm_swz_zz(VEC) (VEC).a[2], (VEC).a[2]

#define _gm_swz_www(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[3]
#define _gm_swz_wwx(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[0]
#define _gm_swz_wwy(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[1]
#define _gm_swz_wwz(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[2]
#define _gm_swz_wxx(VEC) (VEC).a[3], (VEC).a[0], (VEC).a[0]
#define _gm_swz_wxy(VEC) (VEC).a[3], (VEC).a[0], (VEC).a[1]
#define _gm_swz_wxz(VEC) (VEC).a[3], (VEC).a[0], (VEC).a[2]
#define _gm_swz_wyx(VEC) (VEC).a[3], (VEC).a[1], (VEC).a[0]
#define _gm_swz_wyy(VEC) (VEC).a[3], (VEC).a[1], (VEC).a[1]
#define _gm_swz_wyz(VEC) (VEC).a[3], (VEC).a[1], (VEC).a[2]
#define _gm_swz_wzx(VEC) (VEC).a[3], (VEC).a[2], (VEC).a[0]
#define _gm_swz_wzy(VEC) (VEC).a[3], (VEC).a[2], (VEC).a[1]
#define _gm_swz_wzz(VEC) (VEC).a[3], (VEC).a[2], (VEC).a[2]
#define _gm_swz_xww(VEC) (VEC).a[0], (VEC).a[3], (VEC).a[3]
#define _gm_swz_xwy(VEC) (VEC).a[0], (VEC).a[3], (VEC).a[1]
#define _gm_swz_xwz(VEC) (VEC).a[0], (VEC).a[3], (VEC).a[2]
#define _gm_swz_xxw(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[3]
#define _gm_swz_xxx(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[0]
#define _gm_swz_xxy(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[1]
#define _gm_swz_xxz(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[2]
#define _gm_swz_xyw(VEC) (VEC).a[0], (VEC).a[1], (VEC).a[3]
#define _gm_swz_xyy(VEC) (VEC).a[0], (VEC).a[1], (VEC).a[1]
#define _gm_swz_xyz(VEC) (VEC).a[0], (VEC).a[1], (VEC).a[2]
#define _gm_swz_xzw(VEC) (VEC).a[0], (VEC).a[2], (VEC).a[3]
#define _gm_swz_xzy(VEC) (VEC).a[0], (VEC).a[2], (VEC).a[1]
#define _gm_swz_xzz(VEC) (VEC).a[0], (VEC).a[2], (VEC).a[2]
#define _gm_swz_yww(VEC) (VEC).a[1], (VEC).a[3], (VEC).a[3]
#define _gm_swz_ywx(VEC) (VEC).a[1], (VEC).a[3], (VEC).a[0]
#define _gm_swz_ywz(VEC) (VEC).a[1], (VEC).a[3], (VEC).a[2]
#define _gm_swz_yxw(VEC) (VEC).a[1], (VEC).a[0], (VEC).a[3]
#define _gm_swz_yxx(VEC) (VEC).a[1], (VEC).a[0], (VEC).a[0]
#define _gm_swz_yxz(VEC) (VEC).a[1], (VEC).a[0], (VEC).a[2]
#define _gm_swz_yyw(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[3]
#define _gm_swz_yyx(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[0]
#define _gm_swz_yyy(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[1]
#define _gm_swz_yyz(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[2]
#define _gm_swz_yzw(VEC) (VEC).a[1], (VEC).a[2], (VEC).a[3]
#define _gm_swz_yzx(VEC) (VEC).a[1], (VEC).a[2], (VEC).a[0]
#define _gm_swz_yzz(VEC) (VEC).a[1], (VEC).a[2], (VEC).a[2]
#define _gm_swz_zww(VEC) (VEC).a[2], (VEC).a[3], (VEC).a[3]
#define _gm_swz_zwx(VEC) (VEC).a[2], (VEC).a[3], (VEC).a[0]
#define _gm_swz_zwy(VEC) (VEC).a[2], (VEC).a[3], (VEC).a[1]
#define _gm_swz_zxw(VEC) (VEC).a[2], (VEC).a[0], (VEC).a[3]
#define _gm_swz_zxx(VEC) (VEC).a[2], (VEC).a[0], (VEC).a[0]
#define _gm_swz_zxy(VEC) (VEC).a[2], (VEC).a[0], (VEC).a[1]
#define _gm_swz_zyw(VEC) (VEC).a[2], (VEC).a[1], (VEC).a[3]
#define _gm_swz_zyx(VEC) (VEC).a[2], (VEC).a[1], (VEC).a[0]
#define _gm_swz_zyy(VEC) (VEC).a[2], (VEC).a[1], (VEC).a[1]
#define _gm_swz_zzw(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[3]
#define _gm_swz_zzx(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[0]
#define _gm_swz_zzy(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[1]
#define _gm_swz_zzz(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[2]

#define _gm_swz_wwww(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[3], (VEC).a[3]
#define _gm_swz_wwwx(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[3], (VEC).a[0]
#define _gm_swz_wwwy(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[3], (VEC).a[1]
#define _gm_swz_wwwz(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[3], (VEC).a[2]
#define _gm_swz_wwxx(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[0], (VEC).a[0]
#define _gm_swz_wwxy(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[0], (VEC).a[1]
#define _gm_swz_wwxz(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[0], (VEC).a[2]
#define _gm_swz_wwyx(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[1], (VEC).a[0]
#define _gm_swz_wwyy(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[1], (VEC).a[1]
#define _gm_swz_wwyz(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[1], (VEC).a[2]
#define _gm_swz_wwzx(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[2], (VEC).a[0]
#define _gm_swz_wwzy(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[2], (VEC).a[1]
#define _gm_swz_wwzz(VEC) (VEC).a[3], (VEC).a[3], (VEC).a[2], (VEC).a[2]
#define _gm_swz_wxxx(VEC) (VEC).a[3], (VEC).a[0], (VEC).a[0], (VEC).a[0]
#define _gm_swz_wxxy(VEC) (VEC).a[3], (VEC).a[0], (VEC).a[0], (VEC).a[1]
#define _gm_swz_wxxz(VEC) (VEC).a[3], (VEC).a[0], (VEC).a[0], (VEC).a[2]
#define _gm_swz_wxyy(VEC) (VEC).a[3], (VEC).a[0], (VEC).a[1], (VEC).a[1]
#define _gm_swz_wxyz(VEC) (VEC).a[3], (VEC).a[0], (VEC).a[1], (VEC).a[2]
#define _gm_swz_wxzy(VEC) (VEC).a[3], (VEC).a[0], (VEC).a[2], (VEC).a[1]
#define _gm_swz_wxzz(VEC) (VEC).a[3], (VEC).a[0], (VEC).a[2], (VEC).a[2]
#define _gm_swz_wyxx(VEC) (VEC).a[3], (VEC).a[1], (VEC).a[0], (VEC).a[0]
#define _gm_swz_wyxz(VEC) (VEC).a[3], (VEC).a[1], (VEC).a[0], (VEC).a[2]
#define _gm_swz_wyyx(VEC) (VEC).a[3], (VEC).a[1], (VEC).a[1], (VEC).a[0]
#define _gm_swz_wyyy(VEC) (VEC).a[3], (VEC).a[1], (VEC).a[1], (VEC).a[1]
#define _gm_swz_wyyz(VEC) (VEC).a[3], (VEC).a[1], (VEC).a[1], (VEC).a[2]
#define _gm_swz_wyzx(VEC) (VEC).a[3], (VEC).a[1], (VEC).a[2], (VEC).a[0]
#define _gm_swz_wyzz(VEC) (VEC).a[3], (VEC).a[1], (VEC).a[2], (VEC).a[2]
#define _gm_swz_wzxx(VEC) (VEC).a[3], (VEC).a[2], (VEC).a[0], (VEC).a[0]
#define _gm_swz_wzxy(VEC) (VEC).a[3], (VEC).a[2], (VEC).a[0], (VEC).a[1]
#define _gm_swz_wzyx(VEC) (VEC).a[3], (VEC).a[2], (VEC).a[1], (VEC).a[0]
#define _gm_swz_wzyy(VEC) (VEC).a[3], (VEC).a[2], (VEC).a[1], (VEC).a[1]
#define _gm_swz_wzzx(VEC) (VEC).a[3], (VEC).a[2], (VEC).a[2], (VEC).a[0]
#define _gm_swz_wzzy(VEC) (VEC).a[3], (VEC).a[2], (VEC).a[2], (VEC).a[1]
#define _gm_swz_wzzz(VEC) (VEC).a[3], (VEC).a[2], (VEC).a[2], (VEC).a[2]
#define _gm_swz_xwww(VEC) (VEC).a[0], (VEC).a[3], (VEC).a[3], (VEC).a[3]
#define _gm_swz_xwwy(VEC) (VEC).a[0], (VEC).a[3], (VEC).a[3], (VEC).a[1]
#define _gm_swz_xwwz(VEC) (VEC).a[0], (VEC).a[3], (VEC).a[3], (VEC).a[2]
#define _gm_swz_xwyy(VEC) (VEC).a[0], (VEC).a[3], (VEC).a[1], (VEC).a[1]
#define _gm_swz_xwyz(VEC) (VEC).a[0], (VEC).a[3], (VEC).a[1], (VEC).a[2]
#define _gm_swz_xwzy(VEC) (VEC).a[0], (VEC).a[3], (VEC).a[2], (VEC).a[1]
#define _gm_swz_xwzz(VEC) (VEC).a[0], (VEC).a[3], (VEC).a[2], (VEC).a[2]
#define _gm_swz_xxww(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[3], (VEC).a[3]
#define _gm_swz_xxwy(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[3], (VEC).a[1]
#define _gm_swz_xxwz(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[3], (VEC).a[2]
#define _gm_swz_xxxw(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[0], (VEC).a[3]
#define _gm_swz_xxxx(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[0], (VEC).a[0]
#define _gm_swz_xxxy(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[0], (VEC).a[1]
#define _gm_swz_xxxz(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[0], (VEC).a[2]
#define _gm_swz_xxyw(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[1], (VEC).a[3]
#define _gm_swz_xxyy(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[1], (VEC).a[1]
#define _gm_swz_xxyz(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[1], (VEC).a[2]
#define _gm_swz_xxzw(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[2], (VEC).a[3]
#define _gm_swz_xxzy(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[2], (VEC).a[1]
#define _gm_swz_xxzz(VEC) (VEC).a[0], (VEC).a[0], (VEC).a[2], (VEC).a[2]
#define _gm_swz_xyww(VEC) (VEC).a[0], (VEC).a[1], (VEC).a[3], (VEC).a[3]
#define _gm_swz_xywz(VEC) (VEC).a[0], (VEC).a[1], (VEC).a[3], (VEC).a[2]
#define _gm_swz_xyyw(VEC) (VEC).a[0], (VEC).a[1], (VEC).a[1], (VEC).a[3]
#define _gm_swz_xyyy(VEC) (VEC).a[0], (VEC).a[1], (VEC).a[1], (VEC).a[1]
#define _gm_swz_xyyz(VEC) (VEC).a[0], (VEC).a[1], (VEC).a[1], (VEC).a[2]
#define _gm_swz_xyzw(VEC) (VEC).a[0], (VEC).a[1], (VEC).a[2], (VEC).a[3]
#define _gm_swz_xyzz(VEC) (VEC).a[0], (VEC).a[1], (VEC).a[2], (VEC).a[2]
#define _gm_swz_xzww(VEC) (VEC).a[0], (VEC).a[2], (VEC).a[3], (VEC).a[3]
#define _gm_swz_xzwy(VEC) (VEC).a[0], (VEC).a[2], (VEC).a[3], (VEC).a[1]
#define _gm_swz_xzyw(VEC) (VEC).a[0], (VEC).a[2], (VEC).a[1], (VEC).a[3]
#define _gm_swz_xzyy(VEC) (VEC).a[0], (VEC).a[2], (VEC).a[1], (VEC).a[1]
#define _gm_swz_xzzw(VEC) (VEC).a[0], (VEC).a[2], (VEC).a[2], (VEC).a[3]
#define _gm_swz_xzzy(VEC) (VEC).a[0], (VEC).a[2], (VEC).a[2], (VEC).a[1]
#define _gm_swz_xzzz(VEC) (VEC).a[0], (VEC).a[2], (VEC).a[2], (VEC).a[2]
#define _gm_swz_ywww(VEC) (VEC).a[1], (VEC).a[3], (VEC).a[3], (VEC).a[3]
#define _gm_swz_ywwx(VEC) (VEC).a[1], (VEC).a[3], (VEC).a[3], (VEC).a[0]
#define _gm_swz_ywwz(VEC) (VEC).a[1], (VEC).a[3], (VEC).a[3], (VEC).a[2]
#define _gm_swz_ywxx(VEC) (VEC).a[1], (VEC).a[3], (VEC).a[0], (VEC).a[0]
#define _gm_swz_ywxz(VEC) (VEC).a[1], (VEC).a[3], (VEC).a[0], (VEC).a[2]
#define _gm_swz_ywzx(VEC) (VEC).a[1], (VEC).a[3], (VEC).a[2], (VEC).a[0]
#define _gm_swz_ywzz(VEC) (VEC).a[1], (VEC).a[3], (VEC).a[2], (VEC).a[2]
#define _gm_swz_yxww(VEC) (VEC).a[1], (VEC).a[0], (VEC).a[3], (VEC).a[3]
#define _gm_swz_yxwz(VEC) (VEC).a[1], (VEC).a[0], (VEC).a[3], (VEC).a[2]
#define _gm_swz_yxxw(VEC) (VEC).a[1], (VEC).a[0], (VEC).a[0], (VEC).a[3]
#define _gm_swz_yxxx(VEC) (VEC).a[1], (VEC).a[0], (VEC).a[0], (VEC).a[0]
#define _gm_swz_yxxz(VEC) (VEC).a[1], (VEC).a[0], (VEC).a[0], (VEC).a[2]
#define _gm_swz_yxzw(VEC) (VEC).a[1], (VEC).a[0], (VEC).a[2], (VEC).a[3]
#define _gm_swz_yxzz(VEC) (VEC).a[1], (VEC).a[0], (VEC).a[2], (VEC).a[2]
#define _gm_swz_yyww(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[3], (VEC).a[3]
#define _gm_swz_yywx(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[3], (VEC).a[0]
#define _gm_swz_yywz(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[3], (VEC).a[2]
#define _gm_swz_yyxw(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[0], (VEC).a[3]
#define _gm_swz_yyxx(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[0], (VEC).a[0]
#define _gm_swz_yyxz(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[0], (VEC).a[2]
#define _gm_swz_yyyw(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[1], (VEC).a[3]
#define _gm_swz_yyyx(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[1], (VEC).a[0]
#define _gm_swz_yyyy(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[1], (VEC).a[1]
#define _gm_swz_yyyz(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[1], (VEC).a[2]
#define _gm_swz_yyzw(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[2], (VEC).a[3]
#define _gm_swz_yyzx(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[2], (VEC).a[0]
#define _gm_swz_yyzz(VEC) (VEC).a[1], (VEC).a[1], (VEC).a[2], (VEC).a[2]
#define _gm_swz_yzww(VEC) (VEC).a[1], (VEC).a[2], (VEC).a[3], (VEC).a[3]
#define _gm_swz_yzwx(VEC) (VEC).a[1], (VEC).a[2], (VEC).a[3], (VEC).a[0]
#define _gm_swz_yzxw(VEC) (VEC).a[1], (VEC).a[2], (VEC).a[0], (VEC).a[3]
#define _gm_swz_yzxx(VEC) (VEC).a[1], (VEC).a[2], (VEC).a[0], (VEC).a[0]
#define _gm_swz_yzzw(VEC) (VEC).a[1], (VEC).a[2], (VEC).a[2], (VEC).a[3]
#define _gm_swz_yzzx(VEC) (VEC).a[1], (VEC).a[2], (VEC).a[2], (VEC).a[0]
#define _gm_swz_yzzz(VEC) (VEC).a[1], (VEC).a[2], (VEC).a[2], (VEC).a[2]
#define _gm_swz_zwww(VEC) (VEC).a[2], (VEC).a[3], (VEC).a[3], (VEC).a[3]
#define _gm_swz_zwwx(VEC) (VEC).a[2], (VEC).a[3], (VEC).a[3], (VEC).a[0]
#define _gm_swz_zwwy(VEC) (VEC).a[2], (VEC).a[3], (VEC).a[3], (VEC).a[1]
#define _gm_swz_zwxx(VEC) (VEC).a[2], (VEC).a[3], (VEC).a[0], (VEC).a[0]
#define _gm_swz_zwxy(VEC) (VEC).a[2], (VEC).a[3], (VEC).a[0], (VEC).a[1]
#define _gm_swz_zwyx(VEC) (VEC).a[2], (VEC).a[3], (VEC).a[1], (VEC).a[0]
#define _gm_swz_zwyy(VEC) (VEC).a[2], (VEC).a[3], (VEC).a[1], (VEC).a[1]
#define _gm_swz_zxww(VEC) (VEC).a[2], (VEC).a[0], (VEC).a[3], (VEC).a[3]
#define _gm_swz_zxwy(VEC) (VEC).a[2], (VEC).a[0], (VEC).a[3], (VEC).a[1]
#define _gm_swz_zxxw(VEC) (VEC).a[2], (VEC).a[0], (VEC).a[0], (VEC).a[3]
#define _gm_swz_zxxx(VEC) (VEC).a[2], (VEC).a[0], (VEC).a[0], (VEC).a[0]
#define _gm_swz_zxxy(VEC) (VEC).a[2], (VEC).a[0], (VEC).a[0], (VEC).a[1]
#define _gm_swz_zxyw(VEC) (VEC).a[2], (VEC).a[0], (VEC).a[1], (VEC).a[3]
#define _gm_swz_zxyy(VEC) (VEC).a[2], (VEC).a[0], (VEC).a[1], (VEC).a[1]
#define _gm_swz_zyww(VEC) (VEC).a[2], (VEC).a[1], (VEC).a[3], (VEC).a[3]
#define _gm_swz_zywx(VEC) (VEC).a[2], (VEC).a[1], (VEC).a[3], (VEC).a[0]
#define _gm_swz_zyxw(VEC) (VEC).a[2], (VEC).a[1], (VEC).a[0], (VEC).a[3]
#define _gm_swz_zyxx(VEC) (VEC).a[2], (VEC).a[1], (VEC).a[0], (VEC).a[0]
#define _gm_swz_zyyw(VEC) (VEC).a[2], (VEC).a[1], (VEC).a[1], (VEC).a[3]
#define _gm_swz_zyyx(VEC) (VEC).a[2], (VEC).a[1], (VEC).a[1], (VEC).a[0]
#define _gm_swz_zyyy(VEC) (VEC).a[2], (VEC).a[1], (VEC).a[1], (VEC).a[1]
#define _gm_swz_zzww(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[3], (VEC).a[3]
#define _gm_swz_zzwx(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[3], (VEC).a[0]
#define _gm_swz_zzwy(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[3], (VEC).a[1]
#define _gm_swz_zzxw(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[0], (VEC).a[3]
#define _gm_swz_zzxx(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[0], (VEC).a[0]
#define _gm_swz_zzxy(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[0], (VEC).a[1]
#define _gm_swz_zzyw(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[1], (VEC).a[3]
#define _gm_swz_zzyx(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[1], (VEC).a[0]
#define _gm_swz_zzyy(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[1], (VEC).a[1]
#define _gm_swz_zzzw(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[2], (VEC).a[3]
#define _gm_swz_zzzx(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[2], (VEC).a[0]
#define _gm_swz_zzzy(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[2], (VEC).a[1]
#define _gm_swz_zzzz(VEC) (VEC).a[2], (VEC).a[2], (VEC).a[2], (VEC).a[2]

#define GM_SWIZZLE(VEC, args_t)                                                \
  {                                                                            \
    GM_CONCAT(_gm_swz_, args_t)(VEC)                                           \
  }

#define GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, DIRECTION, \
                      ...)                                                     \
  GM_CONST TYPENAME GM_OPERNAME(SHORTNAME, DIRECTION) = {__VA_ARGS__};

#define GM_NEW_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N)                \
  GM_CDECL TYPENAME SHORTNAME(GM_ARGPARAMS_N(N, const BASETYPE))               \
  {                                                                            \
    TYPENAME r = {GM_ARGLIST_N(N)};                                            \
    return r;                                                                  \
  }

#define GM_NEW_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, ...)         \
  GM_CDECL TYPENAME SHORTNAME(GM_PARAMLIST_N(N, const BASETYPE, __VA_ARGS__))  \
  {                                                                            \
    TYPENAME r = {__VA_ARGS__};                                                \
    return r;                                                                  \
  }

#define GM_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N)                \
  GM_CONST TYPENAME GM_OPERNAME(SHORTNAME,                                     \
                                zero) = {GM_COMMASEP_N(N, (BASETYPE) 0)};      \
  GM_CONST TYPENAME GM_OPERNAME(SHORTNAME,                                     \
                                one)  = {GM_COMMASEP_N(N, (BASETYPE) 1)};

#define GM_IDENTITY_2X2(BASETYPE)                                              \
  {                                                                            \
    (BASETYPE)(1), (BASETYPE) (0), (BASETYPE) (0), (BASETYPE) (1)              \
  }
#define GM_IDENTITY_3X3(BASETYPE)                                              \
  {                                                                            \
    (BASETYPE)(1), (BASETYPE) (0), (BASETYPE) (0), (BASETYPE) (0),             \
      (BASETYPE) (1), (BASETYPE) (0), (BASETYPE) (0), (BASETYPE) (0),          \
      (BASETYPE) (1)                                                           \
  }
#define GM_IDENTITY_4X4(BASETYPE)                                              \
  {                                                                            \
    (BASETYPE)(1), (BASETYPE) (0), (BASETYPE) (0), (BASETYPE) (0),             \
      (BASETYPE) (0), (BASETYPE) (1), (BASETYPE) (0), (BASETYPE) (0),          \
      (BASETYPE) (0), (BASETYPE) (0), (BASETYPE) (1), (BASETYPE) (0),          \
      (BASETYPE) (0), (BASETYPE) (0), (BASETYPE) (0), (BASETYPE) (1)           \
  }

#define GM_IDENTITY(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N)           \
  const static TYPENAME GM_OPERNAME(SHORTNAME, ident) =                        \
    GM_CONCAT(GM_IDENTITY_, GM_CONCAT_1(N, X, N))(BASETYPE)

#define GM_SCL_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)          \
  GM_CDECL BASETYPE GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,             \
                                                 const TYPENAME r)             \
  {                                                                            \
    BASETYPE v = (BASETYPE) 0;                                                 \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      v += GM_OPNAME(BASETYPE, OPER)(l.a[i], r.a[i]);                          \
    }                                                                          \
    return v;                                                                  \
  }

#define GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)        \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, s##OPER)(const TYPENAME l,          \
                                                    const BASETYPE r)          \
  {                                                                            \
    TYPENAME v = GM_OPERNAME(SHORTNAME, zero);                                 \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      v.a[i] = GM_OPNAME(BASETYPE, OPER)(l.a[i], r);                           \
    }                                                                          \
    return v;                                                                  \
  }

#define GM_CMP_OP_AND(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)      \
  GM_CDECL bool GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,                 \
                                             const TYPENAME r)                 \
  {                                                                            \
    bool v = true;                                                             \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      v = v && GM_OPNAME(BASETYPE, OPER)(l.a[i], r.a[i]);                      \
    }                                                                          \
    return v;                                                                  \
  }

#define GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)       \
  GM_CDECL bool GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,                 \
                                             const TYPENAME r)                 \
  {                                                                            \
    bool v = false;                                                            \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      v = v || GM_OPNAME(BASETYPE, OPER)(l.a[i], r.a[i]);                      \
    }                                                                          \
    return v;                                                                  \
  }

#define GM_VEC3_CROSS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, OPER)         \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,             \
                                                 const TYPENAME r)             \
  {                                                                            \
    return (TYPENAME){                                                         \
      GM_OPNAME(BASETYPE, sub)(GM_OPNAME(BASETYPE, mul)(l.a[1], r.a[2]),       \
                               GM_OPNAME(BASETYPE, mul)(l.a[2], r.a[1])),      \
      GM_OPNAME(BASETYPE, sub)(GM_OPNAME(BASETYPE, mul)(l.a[2], r.a[0]),       \
                               GM_OPNAME(BASETYPE, mul)(l.a[0], r.a[2])),      \
      GM_OPNAME(BASETYPE, sub)(GM_OPNAME(BASETYPE, mul)(l.a[0], r.a[1]),       \
                               GM_OPNAME(BASETYPE, mul)(l.a[1], r.a[0]))};     \
  }

#define GM_SQLEN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)        \
  GM_CDECL BASETYPE GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    BASETYPE v = 0;                                                            \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      v += GM_OPNAME(BASETYPE, mul)(m.a[i], m.a[i]);                           \
    }                                                                          \
    return v;                                                                  \
  }

#define GM_LEN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)          \
  GM_CDECL BASETYPE GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    BASETYPE v = 0;                                                            \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      v += GM_OPNAME(BASETYPE, mul)(m.a[i], m.a[i]);                           \
    }                                                                          \
    return GM_OPNAME(BASETYPE, sqrt)(v);                                       \
  }

#define GM_DISTANCE_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)     \
  GM_CDECL BASETYPE GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,             \
                                                 const TYPENAME r)             \
  {                                                                            \
    return GM_OPERNAME(SHORTNAME, len)(GM_OPERNAME(SHORTNAME, sub)(r, l));     \
  }

#define GM_SQDISTANCE_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)   \
  GM_CDECL BASETYPE GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,             \
                                                 const TYPENAME r)             \
  {                                                                            \
    return GM_OPERNAME(SHORTNAME, sqlen)(GM_OPERNAME(SHORTNAME, sub)(r, l));   \
  }

#define GM_ANGLE_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)        \
  GM_CDECL angf GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,                 \
                                             const TYPENAME r)                 \
  {                                                                            \
    BASETYPE dotProduct = GM_OPERNAME(SHORTNAME, dot)(l, r);                   \
    BASETYPE lengths =                                                         \
      GM_OPERNAME(SHORTNAME, len)(l) * GM_OPERNAME(SHORTNAME, len)(r);         \
    if (lengths == 0)                                                          \
    {                                                                          \
      return afrads(0);                                                        \
    }                                                                          \
    else                                                                       \
    {                                                                          \
      BASETYPE cosTheta = dotProduct / lengths;                                \
      return afrads(fmax(fmin(cosTheta, (BASETYPE) 1.0), (BASETYPE) -1.0));    \
    }                                                                          \
  }

#define GM_SIGNED_ANGLE_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER) \
  GM_CDECL angf GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,                 \
                                             const TYPENAME r)                 \
  {                                                                            \
    BASETYPE unsignedAngle = GM_OPERNAME(SHORTNAME, angle)(l, r).a;            \
    TYPENAME crossProduct  = GM_OPERNAME(SHORTNAME, cross)(l, r);              \
    BASETYPE sign = GM_OPERNAME(SHORTNAME, dot)(l, crossProduct) < 0 ? -1 : 1; \
    return afrads(unsignedAngle * sign);                                       \
  }

#define GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)          \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,             \
                                                 const TYPENAME r)             \
  {                                                                            \
    (void) l;                                                                  \
    (void) r;                                                                  \
    TYPENAME v;                                                                \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      v.a[i] = GM_OPNAME(BASETYPE, OPER)(l.a[i], r.a[i]);                      \
    }                                                                          \
    return v;                                                                  \
  }

#define GM_LERP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)         \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME a,             \
                                                 const TYPENAME b, float t)    \
  {                                                                            \
    TYPENAME result;                                                           \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      result.a[i] = GM_CONCAT(OPER, TYPEPREFIX)(a.a[i], b.a[i], t);            \
    }                                                                          \
    return result;                                                             \
  }

#define GM_INVLERP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)      \
  GM_CDECL BASETYPE GM_OPERNAME(SHORTNAME, OPER)(                              \
    const TYPENAME a, const TYPENAME b, const TYPENAME x)                      \
  {                                                                            \
    BASETYPE result = 1.0f;                                                    \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      result *= GM_CONCAT(OPER, TYPEPREFIX)(a.a[i], b.a[i], x.a[i]);           \
    }                                                                          \
    return result;                                                             \
  }

#define GM_MIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)          \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME a,             \
                                                 const TYPENAME b)             \
  {                                                                            \
    TYPENAME result;                                                           \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      result.a[i] = GM_OPNAME(BASETYPE, lt)(a.a[i], b.a[i]) ? a.a[i] : b.a[i]; \
    }                                                                          \
    return result;                                                             \
  }

#define GM_MAX_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)          \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME a,             \
                                                 const TYPENAME b)             \
  {                                                                            \
    TYPENAME result;                                                           \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      result.a[i] = GM_OPNAME(BASETYPE, gt)(a.a[i], b.a[i]) ? a.a[i] : b.a[i]; \
    }                                                                          \
    return result;                                                             \
  }

#define GM_CLAMP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)        \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(                              \
    const TYPENAME v, const TYPENAME min, const TYPENAME max)                  \
  {                                                                            \
    TYPENAME result;                                                           \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      result.a[i] = GM_CONCAT(clamp, TYPEPREFIX)(v.a[i], min.a[i], max.a[i]);  \
    }                                                                          \
    return result;                                                             \
  }

#define GM_QUAT_MUL_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)     \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,             \
                                                 const TYPENAME r)             \
  {                                                                            \
    TYPENAME q;                                                                \
    q.a[0] =                                                                   \
      l.a[0] * r.a[0] - l.a[1] * r.a[1] - l.a[2] * r.a[2] - l.a[3] * r.a[3];   \
    q.a[1] =                                                                   \
      l.a[0] * r.a[1] + l.a[1] * r.a[0] + l.a[2] * r.a[3] - l.a[3] * r.a[2];   \
    q.a[2] =                                                                   \
      l.a[0] * r.a[2] - l.a[1] * r.a[3] + l.a[2] * r.a[0] + l.a[3] * r.a[1];   \
    q.a[3] =                                                                   \
      l.a[0] * r.a[3] + l.a[1] * r.a[2] - l.a[2] * r.a[1] + l.a[3] * r.a[0];   \
    return q;                                                                  \
  }

#include <math.h>

typedef struct Quaternion
{
  float a[4];
} Quaternion;

#define GM_QUAT_ROT_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)     \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME,                                     \
                                OPER)(GM_ANG_TYPENAME(BASETYPE, TYPEPREFIX) x, \
                                      GM_ANG_TYPENAME(BASETYPE, TYPEPREFIX) y, \
                                      GM_ANG_TYPENAME(BASETYPE, TYPEPREFIX) z) \
  {                                                                            \
    float half_radians_z = z.a * 0.5f;                                         \
    float half_radians_y = y.a * 0.5f;                                         \
    float half_radians_x = x.a * 0.5f;                                         \
    float cos_half_z     = cosf(half_radians_z);                               \
    float sin_half_z     = sinf(half_radians_z);                               \
    float cos_half_y     = cosf(half_radians_y);                               \
    float sin_half_y     = sinf(half_radians_y);                               \
    float cos_half_x     = cosf(half_radians_x);                               \
    float sin_half_x     = sinf(half_radians_x);                               \
    TYPENAME result      = {{1.0f, 0.0f, 0.0f, 0.0f}};                         \
    result.a[1]          = sin_half_x * cos_half_y * cos_half_z -              \
                  cos_half_x * sin_half_y * sin_half_z;                        \
    result.a[2] = cos_half_x * sin_half_y * cos_half_z +                       \
                  sin_half_x * cos_half_y * sin_half_z;                        \
    result.a[3] = cos_half_x * cos_half_y * sin_half_z -                       \
                  sin_half_x * sin_half_y * cos_half_z;                        \
    result.a[0] = cos_half_x * cos_half_y * cos_half_z +                       \
                  sin_half_x * sin_half_y * sin_half_z;                        \
    return result;                                                             \
  }

#define GM_QUAT_AANGLE_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)  \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(                              \
    GM_ANG_TYPENAME(BASETYPE, TYPEPREFIX) a,                                   \
    GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, 3) axis)                             \
  {                                                                            \
    BASETYPE half_angle = a.a * 0.5;                                           \
    BASETYPE sa2        = (BASETYPE) sinf((float) half_angle);                 \
    BASETYPE ca2        = (BASETYPE) cosf((float) half_angle);                 \
    return (TYPENAME){{ca2, axis.x * sa2, axis.y * sa2, axis.z * sa2}};        \
  }

#define GM_QUAT_CONJ_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)    \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME q)             \
  {                                                                            \
    BASETYPE norm = GM_CONCAT(sqrt, TYPEPREFIX)(                               \
      q.a[0] * q.a[0] + q.a[1] * q.a[1] + q.a[2] * q.a[2] + q.a[3] * q.a[3]);  \
    if (norm == 0)                                                             \
    {                                                                          \
      return q;                                                                \
    }                                                                          \
    BASETYPE invNorm = 1.0f / norm;                                            \
    return (TYPENAME){{q.a[0] * invNorm, -q.a[1] * invNorm, -q.a[2] * invNorm, \
                       -q.a[3] * invNorm}};                                    \
  }

#define GM_QUAT_SLERP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)   \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME q1,            \
                                                 const TYPENAME q2, float t)   \
  {                                                                            \
    float dot      = GM_OPERNAME(SHORTNAME, dot)(q1, q2);                      \
    float theta    = acosf(dot);                                               \
    float sinTheta = sinf(theta);                                              \
    float w1       = sinf((1.0f - t) * theta) / sinTheta;                      \
    float w2       = sinf(t * theta) / sinTheta;                               \
    return (TYPENAME){                                                         \
      {{w1 * q1.a[0] + w2 * q2.a[0], w1 * q1.a[1] + w2 * q2.a[1],              \
        w1 * q1.a[2] + w2 * q2.a[2], w1 * q1.a[3] + w2 * q2.a[3]}}};           \
  }

#define GM_QUAT_MUL_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER) \
  GM_CDECL TYPENAME *GM_OPERNAME(SHORTNAME, r##OPER)(TYPENAME * l,             \
                                                     const TYPENAME r)         \
  {                                                                            \
    *l = (TYPENAME){{l->a[0] * r.a[0] - l->a[1] * r.a[1] - l->a[2] * r.a[2] -  \
                       l->a[3] * r.a[3],                                       \
                     l->a[0] * r.a[1] + l->a[1] * r.a[0] + l->a[2] * r.a[3] -  \
                       l->a[3] * r.a[2],                                       \
                     l->a[0] * r.a[2] - l->a[1] * r.a[3] + l->a[2] * r.a[0] +  \
                       l->a[3] * r.a[1],                                       \
                     l->a[0] * r.a[3] + l->a[1] * r.a[2] - l->a[2] * r.a[1] +  \
                       l->a[3] * r.a[0]}};                                     \
    return l;                                                                  \
  }

#define GM_QUAT_ROTV(TYPENAME, SHORTNAME, VTYPENAME, BASETYPE, TYPEPREFIX, N,  \
                     OPER)                                                     \
  GM_CDECL VTYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME q,            \
                                                  const VTYPENAME v)           \
  {                                                                            \
    BASETYPE t = 2.0 * q.a[0] * q.a[0];                                        \
    BASETYPE x = t - 1.0;                                                      \
    BASETYPE y = 2.0 * q.a[0] * q.a[1];                                        \
    BASETYPE z = 2.0 * q.a[0] * q.a[2];                                        \
    t          = 2.0 * q.a[1] * q.a[1];                                        \
    x += 2.0 * t;                                                              \
    y += 2.0 * q.a[1] * q.a[2];                                                \
    z += 2.0 * q.a[1] * q.a[3];                                                \
    t = 2.0 * q.a[2] * q.a[2];                                                 \
    x += 2.0 * t;                                                              \
    y += 2.0 * t;                                                              \
    z += 2.0 * q.a[2] * q.a[3];                                                \
    x = v.a[0] * (x) + v.a[1] * (y) + v.a[2] * (z);                            \
    y = v.a[0] * (2.0 * q.a[1] * q.a[3] - 2.0 * q.a[0] * q.a[2]) +             \
        v.a[1] * (2.0 * q.a[0] * q.a[1] + 2.0 * q.a[2] * q.a[3]) +             \
        v.a[2] * (2.0 * q.a[0] * q.a[0] - 1.0 + 2.0 * q.a[3] * q.a[3]);        \
    z = v.a[0] * (2.0 * q.a[1] * q.a[2] + 2.0 * q.a[0] * q.a[3]) +             \
        v.a[1] * (2.0 * q.a[3] * q.a[3] - 1.0 + 2.0 * q.a[0] * q.a[0]) +       \
        v.a[2] * (2.0 * q.a[0] * q.a[1] - 2.0 * q.a[2] * q.a[3]);              \
    return (VTYPENAME){{{x, y, z}}};                                           \
  }

#define GM_QUAT_ROTM(TYPENAME, SHORTNAME, VTYPENAME, BASETYPE, TYPEPREFIX, N,  \
                     OPER)                                                     \
  GM_CDECL GM_MAT3X3_TYPENAME(BASETYPE, TYPEPREFIX)                            \
    GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME q)                             \
  {                                                                            \
    BASETYPE xx = q.x * q.x;                                                   \
    BASETYPE yy = q.y * q.y;                                                   \
    BASETYPE zz = q.z * q.z;                                                   \
    BASETYPE xy = q.x * q.y;                                                   \
    BASETYPE xz = q.x * q.z;                                                   \
    BASETYPE yz = q.y * q.z;                                                   \
    BASETYPE wx = q.w * q.x;                                                   \
    BASETYPE wy = q.w * q.y;                                                   \
    BASETYPE wz = q.w * q.z;                                                   \
    return (GM_MAT3X3_TYPENAME(BASETYPE, TYPEPREFIX)){                         \
      {{1 - 2 * (yy + zz), 2 * (xy - wz), 2 * (xz + wy), 2 * (xy + wz),        \
        1 - 2 * (xx + zz), 2 * (yz - wx), 2 * (xz - wy), 2 * (yz + wx),        \
        1 - 2 * (xx + yy)}}};                                                  \
  }

#define GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, OPER)          \
  GM_CDECL TYPENAME *GM_OPERNAME(SHORTNAME, r##OPER)(TYPENAME * l,             \
                                                     const TYPENAME r)         \
  {                                                                            \
    (void) l;                                                                  \
    (void) r;                                                                  \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      l->a[i] = GM_OPNAME(BASETYPE, OPER)(l->a[i], r.a[i]);                    \
    }                                                                          \
    return l;                                                                  \
  }

#define GM_MAT_MUL_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER)   \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME l,             \
                                                 const TYPENAME r)             \
  {                                                                            \
    TYPENAME v;                                                                \
    for (size_t i = 0; i < N; i++)                                             \
    {                                                                          \
      for (size_t j = 0; j < N; j++)                                           \
      {                                                                        \
        v.a[j * N + i] = 0;                                                    \
        for (size_t k = 0; k < N; k++)                                         \
        {                                                                      \
          v.a[j * N + i] += l.a[k * N + i] * r.a[j * N + k];                   \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    return v;                                                                  \
  }

#define GM_MAT_MUL_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N,     \
                          OPER)                                                \
  GM_CDECL TYPENAME *GM_OPERNAME(SHORTNAME, r##OPER)(TYPENAME * l,             \
                                                     const TYPENAME r)         \
  {                                                                            \
    TYPENAME v;                                                                \
    for (size_t i = 0; i < N; i++)                                             \
    {                                                                          \
      for (size_t j = 0; j < N; j++)                                           \
      {                                                                        \
        v.a[j * N + i] = 0;                                                    \
        for (size_t k = 0; k < N; k++)                                         \
        {                                                                      \
          v.a[j * N + i] += l->a[k * N + i] * r.a[j * N + k];                  \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    *l = (TYPENAME){{GM_MAT_VALS(N, v)}};                                      \
    return l;                                                                  \
  }

#define GM_TRANSPOSE_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER) \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    TYPENAME v;                                                                \
    for (size_t i = 0; i < N; i++)                                             \
    {                                                                          \
      for (size_t j = 0; j < N; j++)                                           \
      {                                                                        \
        v.a[j * N + i] = m.a[i * N + j];                                       \
      }                                                                        \
    }                                                                          \
    return v;                                                                  \
  }

#define GM_MAT2X2_DET(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER)   \
  GM_CDECL BASETYPE GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    return m.a[0] * m.a[3] - m.a[1] * m.a[2];                                  \
  }

#define GM_MAT2X2_INV(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER)   \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    BASETYPE d = m.a[0] * m.a[3] - m.a[1] * m.a[2];                            \
    if (d == 0)                                                                \
    {                                                                          \
      return m;                                                                \
    }                                                                          \
    return GM_OPERNAME(SHORTNAME, sdiv)(m, d);                                 \
  }

#define GM_MAT3X3_DET(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER)   \
  GM_CDECL BASETYPE GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    return m.a[0] * (m.a[4] * m.a[8] - m.a[5] * m.a[7]) -                      \
           m.a[3] * (m.a[1] * m.a[8] - m.a[2] * m.a[7]) +                      \
           m.a[6] * (m.a[1] * m.a[5] - m.a[2] * m.a[4]);                       \
  }

#define GM_MAT3X3_INV(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER)   \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    BASETYPE det = m.a[0] * (m.a[4] * m.a[8] - m.a[5] * m.a[7]) -              \
                   m.a[3] * (m.a[1] * m.a[8] - m.a[2] * m.a[7]) +              \
                   m.a[6] * (m.a[1] * m.a[5] - m.a[2] * m.a[4]);               \
    if (det == 0)                                                              \
    {                                                                          \
      return m;                                                                \
    }                                                                          \
    TYPENAME r;                                                                \
    r.a[0] = (m.a[4] * m.a[8] - m.a[7] * m.a[5]) / det;                        \
    r.a[1] = (m.a[2] * m.a[7] - m.a[1] * m.a[8]) / det;                        \
    r.a[2] = (m.a[1] * m.a[5] - m.a[2] * m.a[4]) / det;                        \
    r.a[3] = (m.a[5] * m.a[6] - m.a[3] * m.a[8]) / det;                        \
    r.a[4] = (m.a[0] * m.a[8] - m.a[2] * m.a[6]) / det;                        \
    r.a[5] = (m.a[2] * m.a[3] - m.a[0] * m.a[5]) / det;                        \
    r.a[6] = (m.a[3] * m.a[7] - m.a[4] * m.a[6]) / det;                        \
    r.a[7] = (m.a[1] * m.a[6] - m.a[0] * m.a[7]) / det;                        \
    r.a[8] = (m.a[0] * m.a[4] - m.a[1] * m.a[3]) / det;                        \
    return r;                                                                  \
  }

#define GM_MAT4X4_DET(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER)   \
  GM_CDECL BASETYPE GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    return (m.a[0] * (m.a[5] * (m.a[10] * m.a[15] - m.a[14] * m.a[11]) -       \
                      m.a[9] * (m.a[6] * m.a[15] - m.a[14] * m.a[7]) +         \
                      m.a[13] * (m.a[6] * m.a[11] - m.a[10] * m.a[7])) -       \
            m.a[4] * (m.a[1] * (m.a[10] * m.a[15] - m.a[14] * m.a[11]) -       \
                      m.a[9] * (m.a[2] * m.a[15] - m.a[14] * m.a[3]) +         \
                      m.a[13] * (m.a[2] * m.a[11] - m.a[10] * m.a[3])) +       \
            m.a[8] * (m.a[1] * (m.a[6] * m.a[15] - m.a[14] * m.a[7]) -         \
                      m.a[5] * (m.a[2] * m.a[15] - m.a[14] * m.a[3]) +         \
                      m.a[13] * (m.a[2] * m.a[7] - m.a[6] * m.a[3])) -         \
            m.a[12] * (m.a[1] * (m.a[6] * m.a[11] - m.a[10] * m.a[7]) -        \
                       m.a[5] * (m.a[2] * m.a[11] - m.a[10] * m.a[3]) +        \
                       m.a[9] * (m.a[2] * m.a[7] - m.a[6] * m.a[3])));         \
  }

#define GM_MAT4X4_FINV(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER)  \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    TYPENAME r;                                                                \
    for (int i = 0; i < 3; i++)                                                \
    {                                                                          \
      for (int j = 0; j < 3; j++)                                              \
      {                                                                        \
        r.a[i * 4 + j] = m.a[j * 4 + i];                                       \
      }                                                                        \
    }                                                                          \
    for (int i = 0; i < 3; i++)                                                \
    {                                                                          \
      r.a[i * 4 + 3] = -(r.a[i * 4 + 0] * m.a[3] + r.a[i * 4 + 1] * m.a[7] +   \
                         r.a[i * 4 + 2] * m.a[11]);                            \
    }                                                                          \
    r.a[12] = 0;                                                               \
    r.a[13] = 0;                                                               \
    r.a[14] = 0;                                                               \
    r.a[15] = 1;                                                               \
    return r;                                                                  \
  }

#define GM_MAT_LUDECOMP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER) \
  GM_CDECL void GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m, TYPENAME *l,    \
                                             TYPENAME *u)                      \
  {                                                                            \
    *l = GM_OPERNAME(SHORTNAME, zero);                                         \
    *u = GM_OPERNAME(SHORTNAME, zero);                                         \
    for (int i = 0; i < N; i++)                                                \
    {                                                                          \
      for (int k = i; k < N; k++)                                              \
      {                                                                        \
        BASETYPE sum = 0;                                                      \
        for (int j = 0; j < i; j++)                                            \
        {                                                                      \
          sum += (l->a[i * N + j] * u->a[j * N + k]);                          \
        }                                                                      \
        u->a[i * N + k] = m.a[i * N + k] - sum;                                \
      }                                                                        \
      for (int k = i; k < N; k++)                                              \
      {                                                                        \
        if (i == k)                                                            \
        {                                                                      \
          l->a[i * N + i] = 1;                                                 \
        }                                                                      \
        else                                                                   \
        {                                                                      \
          BASETYPE sum = 0;                                                    \
          for (int j = 0; j < i; j++)                                          \
          {                                                                    \
            sum += (l->a[k * N + j] * u->a[j * N + i]);                        \
          }                                                                    \
          l->a[k * N + i] = (m.a[k * N + i] - sum) / u->a[i * N + i];          \
        }                                                                      \
      }                                                                        \
    }                                                                          \
  }

#define GM_MAT4X4_TRS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER,   \
                      MTYPENAME, MSHORTNAME, VTYPENAME, VSHORTNAME)            \
  GM_CDECL TYPENAME GM_CONCAT(SHORTNAME, OPER)(const MTYPENAME r,              \
                                               const VTYPENAME p)              \
  {                                                                            \
    return (TYPENAME){                                                         \
      {{r.a[0], r.a[1], r.a[2], p.a[0], r.a[3], r.a[4], r.a[5], p.a[1],        \
        r.a[6], r.a[7], r.a[8], p.a[2], 0, 0, 0, 1}}};                         \
  }                                                                            \
  GM_CDECL VTYPENAME GM_OPERNAME(SHORTNAME, OPER##getpos)(const TYPENAME m)    \
  {                                                                            \
    return (VTYPENAME){{{m.a[3], m.a[7], m.a[11]}}};                           \
  }                                                                            \
  GM_CDECL MTYPENAME GM_OPERNAME(SHORTNAME, OPER##getrot)(const TYPENAME m)    \
  {                                                                            \
    return (MTYPENAME){{{m.a[0], m.a[1], m.a[2], m.a[4], m.a[5], m.a[6],       \
                         m.a[8], m.a[9], m.a[10]}}};                           \
  }                                                                            \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER##ortho)(                       \
    const BASETYPE left, const BASETYPE right, const BASETYPE bottom,          \
    const BASETYPE top, const BASETYPE near, const BASETYPE far)               \
  {                                                                            \
    BASETYPE rightMinusLeft = right - left;                                    \
    BASETYPE topMinusBottom = top - bottom;                                    \
    BASETYPE farMinusNear   = far - near;                                      \
    BASETYPE rightPlusLeft  = right + left;                                    \
    BASETYPE topPlusBottom  = top + bottom;                                    \
    BASETYPE farPlusNear    = far + near;                                      \
    return (TYPENAME){                                                         \
      {{2.0 / rightMinusLeft, 0, 0, -rightPlusLeft / rightMinusLeft, 0,        \
        2.0 / topMinusBottom, 0, -topPlusBottom / topMinusBottom, 0, 0,        \
        -2.0 / farMinusNear, -farPlusNear / farMinusNear, 0, 0, 0, 1}}};       \
  }                                                                            \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER##persp)(                       \
    const angf fovy, const BASETYPE aspect, const BASETYPE near,               \
    const BASETYPE far)                                                        \
  {                                                                            \
    BASETYPE tanHalfFovy              = tan(fovy.a / 2.0);                     \
    BASETYPE oneOverAspectTanHalfFovy = 1.0 / (aspect * tanHalfFovy);          \
    BASETYPE minusFarPlusNear         = -(far + near);                         \
    BASETYPE twoFarNear               = -2.0 * far * near;                     \
    return (TYPENAME){                                                         \
      {{oneOverAspectTanHalfFovy, 0, 0, 0, 0, 1.0 / tanHalfFovy, 0, 0, 0, 0,   \
        minusFarPlusNear / (far - near), twoFarNear / (far - near), 0, 0, -1,  \
        0}}};                                                                  \
  }                                                                            \
  GM_CDECL MTYPENAME GM_OPERNAME(SHORTNAME, OPER##rotx)(const angf x)          \
  {                                                                            \
    BASETYPE cosX = cosf(x.a);                                                 \
    BASETYPE sinX = sinf(x.a);                                                 \
    return (MTYPENAME){{1, 0, 0, 0, cosX, -sinX, 0, sinX, cosX}};              \
  }                                                                            \
  GM_CDECL MTYPENAME GM_OPERNAME(SHORTNAME, OPER##roty)(const angf y)          \
  {                                                                            \
    BASETYPE cosY = cosf(y.a);                                                 \
    BASETYPE sinY = sinf(y.a);                                                 \
    return (MTYPENAME){{cosY, 0, sinY, 0, 1, 0, -sinY, 0, cosY}};              \
  }                                                                            \
  GM_CDECL MTYPENAME GM_OPERNAME(SHORTNAME, OPER##rotz)(const angf z)          \
  {                                                                            \
    BASETYPE cosZ = cosf(z.a);                                                 \
    BASETYPE sinZ = sinf(z.a);                                                 \
    return (MTYPENAME){{cosZ, -sinZ, 0, sinZ, cosZ, 0, 0, 0, 1}};              \
  }                                                                            \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER##scl)(const VTYPENAME s)       \
  {                                                                            \
    return (TYPENAME){s.a[0], 0, 0,      0, 0, s.a[1], 0, 0,                   \
                      0,      0, s.a[2], 0, 0, 0,      0, 1};                  \
  }                                                                            \
  GM_CDECL MTYPENAME GM_OPERNAME(SHORTNAME, OPER##sclm)(const VTYPENAME s)     \
  {                                                                            \
    return (MTYPENAME){s.a[0], 0, 0, 0, s.a[1], 0, 0, 0, s.a[2]};              \
  }                                                                            \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER##pos)(const VTYPENAME p)       \
  {                                                                            \
    return (TYPENAME){1, 0, 0, p.a[0], 0, 1, 0, p.a[1],                        \
                      0, 0, 1, p.a[2], 0, 0, 0, 1};                            \
  }                                                                            \
  GM_CDECL MTYPENAME GM_OPERNAME(SHORTNAME, OPER##rotm)(                       \
    const angf x, const angf y, const angf z)                                  \
  {                                                                            \
    MTYPENAME rx = GM_OPERNAME(SHORTNAME, OPER##rotx)(x);                      \
    MTYPENAME ry = GM_OPERNAME(SHORTNAME, OPER##roty)(y);                      \
    MTYPENAME rz = GM_OPERNAME(SHORTNAME, OPER##rotz)(z);                      \
    MTYPENAME zx = GM_OPERNAME(MSHORTNAME, mul)(rz, rx);                       \
    return GM_OPERNAME(MSHORTNAME, mul)(zx, ry);                               \
  }                                                                            \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER##rot)(                         \
    const angf x, const angf y, const angf z)                                  \
  {                                                                            \
    MTYPENAME rx = GM_OPERNAME(SHORTNAME, OPER##rotx)(x);                      \
    MTYPENAME ry = GM_OPERNAME(SHORTNAME, OPER##roty)(y);                      \
    MTYPENAME rz = GM_OPERNAME(SHORTNAME, OPER##rotz)(z);                      \
    MTYPENAME zx = GM_OPERNAME(MSHORTNAME, mul)(rz, rx);                       \
    return GM_CONCAT(SHORTNAME, OPER)(GM_OPERNAME(MSHORTNAME, mul)(zx, ry),    \
                                      GM_OPERNAME(VSHORTNAME, zero));          \
  }                                                                            \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER##rs)(                          \
    const VTYPENAME p, const VTYPENAME r, const VTYPENAME s)                   \
  {                                                                            \
    MTYPENAME rm =                                                             \
      GM_OPERNAME(SHORTNAME, trotm)(afdegs(r.x), afdegs(r.y), afdegs(r.z));    \
    MTYPENAME sm = GM_OPERNAME(SHORTNAME, tsclm)(s);                           \
    MTYPENAME rs = GM_OPERNAME(MSHORTNAME, mul)(sm, rm);                       \
    return GM_CONCAT(SHORTNAME, OPER)(rs, p);                                  \
  }

#define GM_MAT4X4_INV(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, OPER)   \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    TYPENAME adj = GM_OPERNAME(SHORTNAME, adjoint)(m);                         \
    BASETYPE det = GM_OPERNAME(SHORTNAME, det)(m);                             \
    return GM_OPERNAME(SHORTNAME, sdiv)(adj, det);                             \
  }

#define GM_MAT_MINOR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, O, P,    \
                     OPER, MTYPENAME, MSHORTNAME, VTYPENAME, VSHORTNAME)       \
  GM_CDECL MTYPENAME GM_OPERNAME(SHORTNAME, OPER)(                             \
    const TYPENAME m, const size_t r, const size_t c)                          \
  {                                                                            \
    size_t mini = 0;                                                           \
    size_t minj = 0;                                                           \
    MTYPENAME v = GM_OPERNAME(MSHORTNAME, zero);                               \
    for (size_t i = 0; i < N; ++i)                                             \
    {                                                                          \
      if (i != r)                                                              \
      {                                                                        \
        minj = 0;                                                              \
        for (size_t j = 0; j < N; ++j)                                         \
        {                                                                      \
          if (j != c)                                                          \
          {                                                                    \
            v.a[minj * N + mini] = m.a[j * N + i];                             \
            minj++;                                                            \
          }                                                                    \
          mini++;                                                              \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    return v;                                                                  \
  }

#define GM_MAT_COFACTOR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, O, P, \
                        OPER, MTYPENAME, MSHORTNAME)                           \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    TYPENAME v = GM_OPERNAME(SHORTNAME, zero);                                 \
    for (size_t j = 0; j < N; ++j)                                             \
    {                                                                          \
      for (size_t i = 0; i < N; ++i)                                           \
      {                                                                        \
        MTYPENAME mm   = GM_OPERNAME(SHORTNAME, minor)(m, j, i);               \
        BASETYPE det   = GM_OPERNAME(MSHORTNAME, det)(mm);                     \
        v.a[j * N + i] = pow(-1, (i + 1) + (j + 1)) * det;                     \
      }                                                                        \
    }                                                                          \
    return v;                                                                  \
  }

#define GM_MAT_ADJOINT(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, O, P,  \
                       OPER, MTYPENAME, MSHORTNAME)                            \
  GM_CDECL TYPENAME GM_OPERNAME(SHORTNAME, OPER)(const TYPENAME m)             \
  {                                                                            \
    return GM_OPERNAME(SHORTNAME,                                              \
                       transpose)(GM_OPERNAME(SHORTNAME, cofactor)(m));        \
  }

//

#define GM_ANG_T(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX)                    \
  typedef struct                                                               \
  {                                                                            \
    BASETYPE a;                                                                \
  } TYPENAME;                                                                  \
  GM_CDECL TYPENAME GM_CONCAT(SHORTNAME, rads)(const BASETYPE a)               \
  {                                                                            \
    TYPENAME r = {a * ((BASETYPE) (1.0))};                                     \
    return r;                                                                  \
  }                                                                            \
  GM_CDECL TYPENAME GM_CONCAT(SHORTNAME, degs)(const BASETYPE a)               \
  {                                                                            \
    TYPENAME r = {a * ((BASETYPE) (GM_OPERNAME(gm, deg2rad_ld)))};             \
    return r;                                                                  \
  }

#define GM_VEC_T(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, ...)            \
  typedef struct                                                               \
  {                                                                            \
    union                                                                      \
    {                                                                          \
      struct                                                                   \
      {                                                                        \
        BASETYPE a[N];                                                         \
      };                                                                       \
      struct                                                                   \
      {                                                                        \
        BASETYPE __VA_ARGS__;                                                  \
      };                                                                       \
    };                                                                         \
  } TYPENAME;                                                                  \
  typedef GM_VEC_TYPENAME(GM_VEC_DEFAULT_BASETYPE, GM_VEC_DEFAULT_TYPEPREFIX,  \
                          N)                                                   \
    GM_VEC_DEFAULTNAME(GM_VEC_DEFAULT_BASETYPE, GM_VEC_DEFAULT_TYPEPREFIX, N); \
  GM_NEW_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, __VA_ARGS__);      \
  GM_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N);                     \
  GM_SQLEN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, sqlen);            \
  GM_LEN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, len);                \
  GM_SCL_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, dot);                \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, add);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, sub);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mul);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, div);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mod);              \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, add);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, sub);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mul);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, div);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mod);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, add);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, sub);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mul);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, div);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mod);                \
  GM_LERP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, lerp);              \
  GM_INVLERP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, invlerp);        \
  GM_MIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, min);                \
  GM_MAX_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, max);                \
  GM_CLAMP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, clamp);            \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, lt);              \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, le);              \
  GM_CMP_OP_AND(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, eq);             \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, ne);              \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, ge);              \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, gt);              \
  GM_DISTANCE_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, distance);      \
  GM_SQDISTANCE_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, sqdistance);

#define GM_MAT_T(TYPENAME, SHORTNAME, VECTYPE, BASETYPE, TYPEPREFIX, M, N)     \
  typedef struct                                                               \
  {                                                                            \
    union                                                                      \
    {                                                                          \
      struct                                                                   \
      {                                                                        \
        BASETYPE a[M];                                                         \
      };                                                                       \
      struct                                                                   \
      {                                                                        \
        VECTYPE v[N];                                                          \
      };                                                                       \
    };                                                                         \
  } TYPENAME;                                                                  \
  typedef GM_MAT_TYPENAME(GM_MAT_DEFAULT_BASETYPE, GM_MAT_DEFAULT_TYPEPREFIX,  \
                          M, N)                                                \
    GM_MAT_DEFAULTNAME(GM_MAT_DEFAULT_BASETYPE, GM_MAT_DEFAULT_TYPE, M, N);    \
  GM_NEW_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M);                     \
  GM_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M);                     \
  GM_IDENTITY(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N);                \
  GM_SQLEN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, sqlen);            \
  GM_LEN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, len);                \
  GM_SCL_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, dot);                \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, add);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, sub);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, mul);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, div);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, mod);              \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, add);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, sub);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, cmul);               \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, div);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, mod);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, add);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, sub);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, div);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, cmul);               \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, mod);                \
  GM_LERP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, lerp);              \
  GM_INVLERP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, invlerp);        \
  GM_MIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, min);                \
  GM_MAX_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, max);                \
  GM_CLAMP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, clamp);            \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, lt);              \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, le);              \
  GM_CMP_OP_AND(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, eq);             \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, ne);              \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, ge);              \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, gt);              \
  GM_TRANSPOSE_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, transpose); \
  GM_MAT_MUL_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, mul);         \
  GM_MAT_MUL_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, mul);

#define GM_QUAT_T(TYPENAME, SHORTNAME, VECTYPE, BASETYPE, TYPEPREFIX, N, ...)  \
  typedef struct                                                               \
  {                                                                            \
    union                                                                      \
    {                                                                          \
      struct                                                                   \
      {                                                                        \
        BASETYPE a[N];                                                         \
      };                                                                       \
      struct                                                                   \
      {                                                                        \
        BASETYPE __VA_ARGS__;                                                  \
      };                                                                       \
    };                                                                         \
  } TYPENAME;                                                                  \
  GM_CONST TYPENAME GM_OPERNAME(SHORTNAME, ident) = {                          \
    {(BASETYPE) (1), (BASETYPE) (0), (BASETYPE) (0), (BASETYPE) (0)}};         \
  typedef GM_QUAT_TYPENAME(GM_QUAT_DEFAULT_BASETYPE,                           \
                           GM_QUAT_DEFAULT_TYPEPREFIX)                         \
    GM_QUAT_DEFAULTNAME(GM_QUAT_DEFAULT_BASETYPE, GM_QUAT_DEFAULT_TYPE);       \
  GM_NEW_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, __VA_ARGS__);      \
  GM_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N);                     \
  GM_SQLEN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, sqlen);            \
  GM_LEN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, len);                \
  GM_SCL_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, dot);                \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, add);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, sub);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mul);              \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, cmul);               \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, div);              \
  GM_SCL_OP_1(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mod);              \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, add);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, sub);                \
  GM_QUAT_CONJ_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, conj);         \
  GM_QUAT_MUL_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mul);           \
  GM_QUAT_SLERP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, slerp);       \
  GM_QUAT_ROTV(TYPENAME, SHORTNAME, VECTYPE, BASETYPE, TYPEPREFIX, N, rotv);   \
  GM_QUAT_ROTM(TYPENAME, SHORTNAME, VECTYPE, BASETYPE, TYPEPREFIX, N, rotm);   \
  GM_QUAT_ROT_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, rot);           \
  GM_QUAT_AANGLE_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, aangle);     \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, div);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mod);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, add);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, sub);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mul);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, cmul);               \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, div);                \
  GM_LERP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, lerp);              \
  GM_INVLERP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, invlerp);        \
  GM_MIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, min);                \
  GM_MAX_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, max);                \
  GM_CLAMP_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, clamp);            \
  GM_QUAT_MUL_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, mod);       \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, lt);              \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, le);              \
  GM_CMP_OP_AND(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, eq);             \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, ne);              \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, ge);              \
  GM_CMP_OP_OR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, gt);

#define GM_MAT_2X2_T(TYPENAME, SHORTNAME, VECTYPE, BASETYPE, TYPEPREFIX, M, N) \
  GM_MAT2X2_DET(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, det);         \
  GM_MAT_LUDECOMP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, ludecomp);  \
  GM_MAT2X2_INV(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, inv);

#define GM_MAT_3X3_T(TYPENAME, SHORTNAME, VECTYPE, BASETYPE, TYPEPREFIX, M, N, \
                     O, P, MTYPENAME, MSHORTNAME, VTYPENAME, VSHORTNAME)       \
  GM_MAT3X3_DET(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, det);         \
  GM_MAT_LUDECOMP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, ludecomp);  \
  GM_MAT_MINOR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, O, P, minor,   \
               MTYPENAME, MSHORTNAME, VTYPENAME, VSHORTNAME);                  \
  GM_MAT_COFACTOR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, O, P,       \
                  cofactor, MTYPENAME, MSHORTNAME);                            \
  GM_MAT_ADJOINT(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, O, P,        \
                 adjoint, MTYPENAME, MSHORTNAME);                              \
  GM_MAT3X3_INV(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, inv);

#define GM_MAT_4X4_T(TYPENAME, SHORTNAME, VECTYPE, BASETYPE, TYPEPREFIX, M, N, \
                     O, P, MTYPENAME, MSHORTNAME, VTYPENAME, VSHORTNAME)       \
  GM_MAT4X4_TRS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, t, MTYPENAME, \
                MSHORTNAME, VTYPENAME, VSHORTNAME);                            \
  GM_MAT4X4_FINV(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, finv);       \
  GM_MAT4X4_DET(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, det);         \
  GM_MAT_LUDECOMP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, ludecomp);  \
  GM_MAT_MINOR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, O, P, minor,   \
               MTYPENAME, MSHORTNAME, VTYPENAME, VSHORTNAME);                  \
  GM_MAT_COFACTOR(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, O, P,       \
                  cofactor, MTYPENAME, MSHORTNAME);                            \
  GM_MAT_ADJOINT(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, O, P,        \
                 adjoint, MTYPENAME, MSHORTNAME);                              \
  GM_MAT4X4_INV(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, M, N, inv);

#define GM_BIN_OP_T(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N)              \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, lshift);             \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, rshift);             \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, band);               \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, bor);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, xor);                \
  GM_BIN_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, and);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, lshift);             \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, rshift);             \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, band);               \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, bor);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, xor);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, and);                \
  GM_REF_OP(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, or);

//

#define GM_VEC2_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N)           \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, right, {1, 0});  \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, up, {0, 1});     \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, down, {0, -1});  \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, left, {-1, 0});

#define GM_VEC3_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N)           \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, right,           \
                {1, 0, 0});                                                    \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, up, {0, 1, 0});  \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, forward,         \
                {0, 0, 1});                                                    \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, left,            \
                {-1, 0, 0});                                                   \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, down,            \
                {0, -1, 0});                                                   \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, back, {0, 0, -1});

#define GM_VEC4_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N)           \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, right,           \
                {1, 0, 0, 0});                                                 \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, up,              \
                {0, 1, 0, 0});                                                 \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, forward,         \
                {0, 0, 1, 0});                                                 \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, left,            \
                {-1, 0, 0, 0});                                                \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, down,            \
                {0, -1, 0, 0});                                                \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, back,            \
                {0, 0, -1, 0});

#define GM_MAT4X4_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N)         \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, catmull,         \
                {-0.5, 1.5, -1.5, 0.5, 1.0, -2.5, 2.0, -0.5, -0.5, 0.0, 0.5,   \
                 0.0, 0.0, 1.0, 0.0, 0.0});                                    \
  GM_DEF_CONSTS(TYPENAME, SHORTNAME, BASETYPE, TYPEPREFIX, N, bezier,          \
                {-1.0, 3.0, -3.0, 1.0, 3.0, -6.0, 3.0, 0.0, -3.0, 3.0, 0.0,    \
                 0.0, 1.0, 0.0, 0.0, 0.0});

#define X(BASETYPE, TYPEPREFIX)                                                \
  GM_ANG_T(GM_ANG_TYPENAME(BASETYPE, TYPEPREFIX),                              \
           GM_ANG_SHORTNAME(BASETYPE, TYPEPREFIX), BASETYPE, TYPEPREFIX);
GM_ANG_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, N, ...)                                        \
  GM_VEC_T(GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N),                           \
           GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, N), BASETYPE, TYPEPREFIX, N, \
           __VA_ARGS__);
GM_VEC2F_T_X_LIST;
GM_VEC3F_T_X_LIST;
GM_VEC4F_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, N, ...)                                        \
  GM_VEC_T(GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N),                           \
           GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, N), BASETYPE, TYPEPREFIX, N, \
           __VA_ARGS__);                                                       \
  GM_BIN_OP_T(GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N),                        \
              GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, N), BASETYPE, TYPEPREFIX, \
              N);
GM_VEC2I_T_X_LIST;
GM_VEC3I_T_X_LIST;
GM_VEC4I_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, N, ...)                                        \
  GM_VEC2_CONSTS(GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N),                     \
                 GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, N), BASETYPE,          \
                 TYPEPREFIX, N);
GM_VEC2F_T_X_LIST;
GM_VEC2I_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, N, ...)                                        \
  GM_VEC3_CONSTS(GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N),                     \
                 GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, N), BASETYPE,          \
                 TYPEPREFIX, N);
GM_VEC3F_T_X_LIST;
GM_VEC3I_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, N, ...)                                        \
  GM_VEC4_CONSTS(GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N),                     \
                 GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, N), BASETYPE,          \
                 TYPEPREFIX, N);
GM_VEC4F_T_X_LIST;
GM_VEC4I_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, N, ...)                                        \
  GM_VEC3_CROSS(GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N),                      \
                GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, N), BASETYPE,           \
                TYPEPREFIX, cross);                                            \
  GM_ANGLE_OP(GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N),                        \
              GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, N), BASETYPE, TYPEPREFIX, \
              N, angle);                                                       \
  GM_SIGNED_ANGLE_OP(GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N),                 \
                     GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, N), BASETYPE,      \
                     TYPEPREFIX, N, sangle);
GM_VEC3F_T_X_LIST;
GM_VEC3I_T_X_LIST;
GM_VEC4F_T_X_LIST;
GM_VEC4I_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, M, N, ...)                                     \
  GM_MAT_T(GM_MAT_TYPENAME(BASETYPE, TYPEPREFIX, M, N),                        \
           GM_MAT_SHORTNAME(BASETYPE, TYPEPREFIX, M, N),                       \
           GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N), BASETYPE, TYPEPREFIX, M,  \
           N);
GM_MAT2X2F_T_X_LIST;
GM_MAT3X3F_T_X_LIST;
GM_MAT4X4F_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, M, N, ...)                                     \
  GM_MAT_T(GM_MAT_TYPENAME(BASETYPE, TYPEPREFIX, M, N),                        \
           GM_MAT_SHORTNAME(BASETYPE, TYPEPREFIX, M, N),                       \
           GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N), BASETYPE, TYPEPREFIX, M,  \
           N);                                                                 \
  GM_BIN_OP_T(GM_MAT_TYPENAME(BASETYPE, TYPEPREFIX, M, N),                     \
              GM_MAT_SHORTNAME(BASETYPE, TYPEPREFIX, M, N), BASETYPE,          \
              TYPEPREFIX, M);
GM_MAT2X2I_T_X_LIST;
GM_MAT3X3I_T_X_LIST;
GM_MAT4X4I_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, M, N, O, P)                                    \
  GM_MAT_2X2_T(GM_MAT_TYPENAME(BASETYPE, TYPEPREFIX, M, N),                    \
               GM_MAT_SHORTNAME(BASETYPE, TYPEPREFIX, M, N),                   \
               GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N), BASETYPE, TYPEPREFIX, \
               M, N);
GM_MAT2X2I_T_X_LIST;
GM_MAT2X2F_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, M, N, O, P)                                    \
  GM_MAT_3X3_T(GM_MAT_TYPENAME(BASETYPE, TYPEPREFIX, M, N),                    \
               GM_MAT_SHORTNAME(BASETYPE, TYPEPREFIX, M, N),                   \
               GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N), BASETYPE, TYPEPREFIX, \
               M, N, O, P, GM_MAT_TYPENAME(BASETYPE, TYPEPREFIX, O, P),        \
               GM_MAT_SHORTNAME(BASETYPE, TYPEPREFIX, O, P),                   \
               GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, P),                       \
               GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, P));
GM_MAT3X3F_T_X_LIST;
GM_MAT3X3I_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, M, N, O, P)                                    \
  GM_MAT_4X4_T(GM_MAT_TYPENAME(BASETYPE, TYPEPREFIX, M, N),                    \
               GM_MAT_SHORTNAME(BASETYPE, TYPEPREFIX, M, N),                   \
               GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N), BASETYPE, TYPEPREFIX, \
               M, N, O, P, GM_MAT_TYPENAME(BASETYPE, TYPEPREFIX, O, P),        \
               GM_MAT_SHORTNAME(BASETYPE, TYPEPREFIX, O, P),                   \
               GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, P),                       \
               GM_VEC_SHORTNAME(BASETYPE, TYPEPREFIX, P));
GM_MAT4X4F_T_X_LIST;
GM_MAT4X4I_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, M, N, O, P)                                    \
  GM_MAT4X4_CONSTS(GM_MAT_TYPENAME(BASETYPE, TYPEPREFIX, M, N),                \
                   GM_MAT_SHORTNAME(BASETYPE, TYPEPREFIX, M, N), BASETYPE,     \
                   TYPEPREFIX, N);
GM_MAT4X4F_T_X_LIST;
#undef X

#define X(BASETYPE, TYPEPREFIX, N, ...)                                        \
  GM_QUAT_T(GM_QUAT_TYPENAME(BASETYPE, TYPEPREFIX),                            \
            GM_QUAT_SHORTNAME(BASETYPE, TYPEPREFIX),                           \
            GM_VEC_TYPENAME(BASETYPE, TYPEPREFIX, N), BASETYPE, TYPEPREFIX, N, \
            __VA_ARGS__);
GM_QUAT_T_X_LIST;
#undef X

#ifdef __cplusplus
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
dedicates any and all copyright interest in the Software to the public domain.
The author makes this dedication for the benefit of the public at large and to
the detriment of the author's heirs and successors. The author intends this
dedication to be an overt act of relinquishment in perpetuity of all present and
future rights to the Software under copyright law.

Any person obtaining a copy of the Software and associated documentation files
is free to to deal in the Software without restriction, including without
limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom
the Software is furnished to do so, subject to no conditions.
--------------------------------------------------------------------------------
*/
