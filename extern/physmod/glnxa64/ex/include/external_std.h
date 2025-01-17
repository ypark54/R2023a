#ifndef __external_std_h__
#define __external_std_h__
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void erf_impl(real_T*ret,const real_T*x,const size_t*numels);void
erf_impl_custom_function_(void*out,const void*in);void erfc_impl(real_T*ret,
const real_T*x,const size_t*numels);void erfc_impl_custom_function_(void*out,
const void*in);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void tlu2_1d_akima_nearest_process(real_T*x,real_T*fx,const real_T*xs,const
real_T*fs,const size_t*n);void tlu2_1d_akima_nearest_process_custom_function_(
void*in,const void*out);void tlu2_1d_akima_nearest_value(real_T*fi,const real_T
*basis,const size_t*bin,const real_T*fx,const size_t*n,const real_T*work1,
const size_t*work2,const size_t*numels);void
tlu2_1d_akima_nearest_value_custom_function_(void*in,const void*out);void
tlu2_1d_akima_linear_process(real_T*x,real_T*fx,const real_T*xs,const real_T*
fs,const size_t*n);void tlu2_1d_akima_linear_process_custom_function_(void*out
,const void*in);void tlu2_1d_akima_linear_value(real_T*fi,const real_T*basis,
const size_t*bin,const real_T*fx,const size_t*n,const real_T*work1,const size_t
*work2,const size_t*numels);void tlu2_1d_akima_linear_value_custom_function_(
void*out,const void*in);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void tlu2_1d_linear_nearest_process(real_T*x,real_T*f,const real_T*xs,const
real_T*fs,const size_t*n);void tlu2_1d_linear_nearest_process_custom_function_
(void*in,const void*out);void tlu2_1d_linear_linear_process(real_T*x,real_T*f,
const real_T*xs,const real_T*fs,const size_t*n);void
tlu2_1d_linear_linear_process_custom_function_(void*out,const void*in);void
tlu2_1d_linear_nearest_value(real_T*fi,const real_T*H,const size_t*bin,const
real_T*f,const size_t*n,const size_t*numels);void
tlu2_1d_linear_nearest_value_custom_function_(void*in,const void*out);void
tlu2_1d_linear_linear_value(real_T*fi,const real_T*H,const size_t*bin,const
real_T*f,const size_t*n,const size_t*numels);void
tlu2_1d_linear_linear_value_custom_function_(void*out,const void*in);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void tlu2_2d_akima_nearest_process(real_T*x1,real_T*x2,real_T*fx1,const real_T
*x1s,const real_T*x2s,const real_T*fs,const size_t*n1,const size_t*n2);void
tlu2_2d_akima_nearest_process_custom_function_(void*in,const void*out);void
tlu2_2d_akima_linear_process(real_T*x1,real_T*x2,real_T*fx1,const real_T*x1s,
const real_T*x2s,const real_T*fs,const size_t*n1,const size_t*n2);void
tlu2_2d_akima_linear_process_custom_function_(void*in,const void*out);void
tlu2_2d_akima_nearest_value(real_T*fi,const real_T*basis1,const size_t*bin1,
const real_T*basis2,const size_t*bin2,const real_T*fx,const size_t*n1,const
size_t*n2,const real_T*work1,const size_t*work2,const size_t*numels);void
tlu2_2d_akima_nearest_value_custom_function_(void*in,const void*out);void
tlu2_2d_akima_linear_value(real_T*fi,const real_T*basis1,const size_t*bin1,
const real_T*basis2,const size_t*bin2,const real_T*fx,const size_t*n1,const
size_t*n2,const real_T*work1,const size_t*work2,const size_t*numels);void
tlu2_2d_akima_linear_value_custom_function_(void*in,const void*out);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void tlu2_2d_linear_nearest_process(real_T*x1,real_T*x2,real_T*f,const real_T*
x1s,const real_T*x2s,const real_T*fs,const size_t*n1,const size_t*n2);void
tlu2_2d_linear_nearest_process_custom_function_(void*in,const void*out);void
tlu2_2d_linear_linear_process(real_T*x1,real_T*x2,real_T*f,const real_T*x1s,
const real_T*x2s,const real_T*fs,const size_t*n1,const size_t*n2);void
tlu2_2d_linear_linear_process_custom_function_(void*in,const void*out);void
tlu2_2d_linear_nearest_value(real_T*fi,const real_T*H1,const size_t*bin1,const
real_T*H2,const size_t*bin2,const real_T*f,const size_t*n1,const size_t*n2,
const size_t*numels);void tlu2_2d_linear_nearest_value_custom_function_(void*
in,const void*out);void tlu2_2d_linear_linear_value(real_T*fi,const real_T*H1,
const size_t*bin1,const real_T*H2,const size_t*bin2,const real_T*f,const size_t
*n1,const size_t*n2,const size_t*numels);void
tlu2_2d_linear_linear_value_custom_function_(void*in,const void*out);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void tlu2_3d_akima_nearest_process(real_T*x1,real_T*x2,real_T*x3,real_T*fx1,
const real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*fs,const size_t
*n1,const size_t*n2,const size_t*n3);void
tlu2_3d_akima_nearest_process_custom_function_(void*in,const void*out);void
tlu2_3d_akima_linear_process(real_T*x1,real_T*x2,real_T*x3,real_T*fx1,const
real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*fs,const size_t*n1,
const size_t*n2,const size_t*n3);void
tlu2_3d_akima_linear_process_custom_function_(void*in,const void*out);void
tlu2_3d_akima_nearest_value(real_T*fi,const real_T*basis1,const size_t*bin1,
const real_T*basis2,const size_t*bin2,const real_T*basis3,const size_t*bin3,
const real_T*fx,const size_t*n1,const size_t*n2,const size_t*n3,const real_T*
work1,const size_t*work2,const size_t*numels);void
tlu2_3d_akima_nearest_value_custom_function_(void*in,const void*out);void
tlu2_3d_akima_linear_value(real_T*fi,const real_T*basis1,const size_t*bin1,
const real_T*basis2,const size_t*bin2,const real_T*basis3,const size_t*bin3,
const real_T*fx,const size_t*n1,const size_t*n2,const size_t*n3,const real_T*
work1,const size_t*work2,const size_t*numels);void
tlu2_3d_akima_linear_value_custom_function_(void*in,const void*out);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void tlu2_3d_linear_nearest_process(real_T*x1,real_T*x2,real_T*x3,real_T*f,
const real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*fs,const size_t
*n1,const size_t*n2,const size_t*n3);void
tlu2_3d_linear_nearest_process_custom_function_(void*in,const void*out);void
tlu2_3d_linear_linear_process(real_T*x1,real_T*x2,real_T*x3,real_T*f,const
real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*fs,const size_t*n1,
const size_t*n2,const size_t*n3);void
tlu2_3d_linear_linear_process_custom_function_(void*in,const void*out);void
tlu2_3d_linear_nearest_value(real_T*fi,const real_T*H1,const size_t*bin1,const
real_T*H2,const size_t*bin2,const real_T*H3,const size_t*bin3,const real_T*f,
const size_t*n1,const size_t*n2,const size_t*n3,const size_t*numels);void
tlu2_3d_linear_nearest_value_custom_function_(void*in,const void*out);void
tlu2_3d_linear_linear_value(real_T*fi,const real_T*H1,const size_t*bin1,const
real_T*H2,const size_t*bin2,const real_T*H3,const size_t*bin3,const real_T*f,
const size_t*n1,const size_t*n2,const size_t*n3,const size_t*numels);void
tlu2_3d_linear_linear_value_custom_function_(void*in,const void*out);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void tlu2_4d_akima_nearest_process(real_T*x1,real_T*x2,real_T*x3,real_T*x4,
real_T*fx1,const real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*x4s
,const real_T*fs,const size_t*n1,const size_t*n2,const size_t*n3,const size_t*
n4);void tlu2_4d_akima_nearest_process_custom_function_(void*in,const void*out
);void tlu2_4d_akima_linear_process(real_T*x1,real_T*x2,real_T*x3,real_T*x4,
real_T*fx1,const real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*x4s
,const real_T*fs,const size_t*n1,const size_t*n2,const size_t*n3,const size_t*
n4);void tlu2_4d_akima_linear_process_custom_function_(void*in,const void*out)
;void tlu2_4d_akima_nearest_value(real_T*fi,const real_T*basis1,const size_t*
bin1,const real_T*basis2,const size_t*bin2,const real_T*basis3,const size_t*
bin3,const real_T*basis4,const size_t*bin4,const real_T*fx,const size_t*n1,
const size_t*n2,const size_t*n3,const size_t*n4,const real_T*work1,const size_t
*work2,const size_t*numels);void tlu2_4d_akima_nearest_value_custom_function_(
void*in,const void*out);void tlu2_4d_akima_linear_value(real_T*fi,const real_T
*basis1,const size_t*bin1,const real_T*basis2,const size_t*bin2,const real_T*
basis3,const size_t*bin3,const real_T*basis4,const size_t*bin4,const real_T*fx
,const size_t*n1,const size_t*n2,const size_t*n3,const size_t*n4,const real_T*
work1,const size_t*work2,const size_t*numels);void
tlu2_4d_akima_linear_value_custom_function_(void*in,const void*out);void
tlu2_4d_linear_nearest_process(real_T*x1,real_T*x2,real_T*x3,real_T*x4,real_T*
f,const real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*x4s,const
real_T*fs,const size_t*n1,const size_t*n2,const size_t*n3,const size_t*n4);
void tlu2_4d_linear_nearest_process_custom_function_(void*in,const void*out);
void tlu2_4d_linear_linear_process(real_T*x1,real_T*x2,real_T*x3,real_T*x4,
real_T*f,const real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*x4s,
const real_T*fs,const size_t*n1,const size_t*n2,const size_t*n3,const size_t*
n4);void tlu2_4d_linear_linear_process_custom_function_(void*in,const void*out
);void tlu2_4d_linear_nearest_value(real_T*fi,const real_T*H1,const size_t*
bin1,const real_T*H2,const size_t*bin2,const real_T*H3,const size_t*bin3,const
real_T*H4,const size_t*bin4,const real_T*f,const size_t*n1,const size_t*n2,
const size_t*n3,const size_t*n4,const size_t*numels);void
tlu2_4d_linear_nearest_value_custom_function_(void*in,const void*out);void
tlu2_4d_linear_linear_value(real_T*fi,const real_T*H1,const size_t*bin1,const
real_T*H2,const size_t*bin2,const real_T*H3,const size_t*bin3,const real_T*H4,
const size_t*bin4,const real_T*f,const size_t*n1,const size_t*n2,const size_t*
n3,const size_t*n4,const size_t*numels);void
tlu2_4d_linear_linear_value_custom_function_(void*in,const void*out);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void tlu2_linear_linear_prelookup(real_T*H,real_T*G,size_t*bin,const real_T*x,
const real_T*t,const size_t*n,const size_t*numels);void
tlu2_linear_linear_prelookup_custom_function_(void*out,const void*in);void
tlu2_linear_nearest_prelookup(real_T*H,real_T*G,size_t*bin,const real_T*x,
const real_T*t,const size_t*n,const size_t*numels);void
tlu2_linear_nearest_prelookup_custom_function_(void*out,const void*in);void
tlu2_akima_linear_prelookup(real_T*basis,size_t*bin,const size_t*
derivativeOrder,const real_T*x,const real_T*t,const size_t*n,const size_t*
numels);void tlu2_akima_linear_prelookup_custom_function_(void*out,const void*
in);void tlu2_akima_nearest_prelookup(real_T*basis,size_t*bin,const size_t*
derivativeOrder,const real_T*x,const real_T*t,const size_t*n,const size_t*
numels);void tlu2_akima_nearest_prelookup_custom_function_(void*out,const void
*in);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void compute_gaussian_value(real_T*v,const real_T*mean,const real_T*sqrtvar,
const int32_T*seed);void compute_gaussian_value_custom_function_(PMValue y,
ConstPMValue u);void update_gaussian_seed(int32_T*out,const int32_T*x);void
update_gaussian_seed_custom_function_(PMValue y,ConstPMValue u);void
init_gaussian_seed(int32_T*out,const uint32_T*x);void
init_gaussian_seed_custom_function_(PMValue y,ConstPMValue u);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void compute_uniform_value(real_T*out,const real_T*minimum,const real_T*
maximum,const int32_T*seed);void compute_uniform_value_custom_function_(
PMValue y,ConstPMValue u);void update_uniform_seed(int32_T*out,const int32_T*x
);void update_uniform_seed_custom_function_(PMValue y,ConstPMValue u);void
init_uniform_seed(int32_T*out,const uint32_T*seed);void
init_uniform_seed_custom_function_(PMValue y,ConstPMValue u);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void slu2_2d_linear_nearest_process(real_T*inputPts,int*delaunayTri,size_t*
numSimplices,int*convHull,int*pointsOnConvHull,size_t*numPtsOnConvHull,const
real_T*x1s,const real_T*x2s,const real_T*fs,const size_t*numPts);void
slu2_2d_linear_nearest_process_custom_function_(void*out,const void*in);void
slu2_2d_linear_linear_process(real_T*inputPts,int*delaunayTri,size_t*
numSimplices,int*convHull,int*pointsOnConvHull,size_t*numPtsOnConvHull,const
real_T*x1s,const real_T*x2s,const real_T*fs,const size_t*numPts);void
slu2_2d_linear_linear_process_custom_function_(void*out,const void*in);void
slu2_2d_linear_nearest_value(real_T*fi,const real_T*inputPts,const real_T*
values,const real_T*x1query,const real_T*x2query,const int*delaunayTri,const
int*vertexIDsOnConvHull,const size_t*numSimplices,const size_t*
numPtsOnConvHull,const size_t*numPts,const size_t*numels,const real_T*work1,
const int*work2);void slu2_2d_linear_nearest_value_custom_function_(void*out,
const void*in);void slu2_2d_linear_linear_value(real_T*fi,const real_T*
inputPts,const real_T*values,const real_T*x1query,const real_T*x2query,const
int*delaunayTri,const int*vertexIDsOnConvHull,const size_t*numSimplices,const
size_t*numPtsOnConvHull,const size_t*numPts,const size_t*numels,const real_T*
work1,const int*work2);void slu2_2d_linear_linear_value_custom_function_(void*
out,const void*in);void slu2_2d_linear_nearest_derivatives(real_T*gi,const
real_T*inputPts,const real_T*values,const real_T*x1query,const real_T*x2query,
const int*delaunayTri,const int*vertexIDsOnConvHull,const size_t*numSimplices,
const size_t*numPtsOnConvHull,const size_t*numPts,const size_t*numels,const
real_T*work1,const int*work2);void
slu2_2d_linear_nearest_derivatives_custom_function_(void*out,const void*in);
void slu2_2d_linear_linear_derivatives(real_T*gi,const real_T*inputPts,const
real_T*values,const real_T*x1query,const real_T*x2query,const int*delaunayTri,
const int*vertexIDsOnConvHull,const size_t*numSimplices,const size_t*
numPtsOnConvHull,const size_t*numPts,const size_t*numels,const real_T*work1,
const int*work2);void slu2_2d_linear_linear_derivatives_custom_function_(void*
out,const void*in);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void slu2_2d_nearest_nearest_process(real_T*inputPts,int*delaunayTri,size_t*
numSimplices,int*convHull,int*pointsOnConvHull,size_t*numPtsOnConvHull,const
real_T*x1s,const real_T*x2s,const real_T*fs,const size_t*numPts);void
slu2_2d_nearest_nearest_process_custom_function_(void*out,const void*in);void
slu2_2d_nearest_linear_process(real_T*inputPts,int*delaunayTri,size_t*
numSimplices,int*convHull,int*pointsOnConvHull,size_t*numPtsOnConvHull,const
real_T*x1s,const real_T*x2s,const real_T*fs,const size_t*numPts);void
slu2_2d_nearest_linear_process_custom_function_(void*out,const void*in);void
slu2_2d_nearest_nearest_value(real_T*fi,const real_T*inputPts,const real_T*
values,const real_T*x1query,const real_T*x2query,const int*delaunayTri,const
int*vertexIDsOnConvHull,const size_t*numSimplices,const size_t*
numPtsOnConvHull,const size_t*numPts,const size_t*numels,const real_T*work1,
const int*work2);void slu2_2d_nearest_nearest_value_custom_function_(void*out,
const void*in);void slu2_2d_nearest_linear_value(real_T*fi,const real_T*
inputPts,const real_T*values,const real_T*x1query,const real_T*x2query,const
int*delaunayTri,const int*vertexIDsOnConvHull,const size_t*numSimplices,const
size_t*numPtsOnConvHull,const size_t*numPts,const size_t*numels,const real_T*
work1,const int*work2);void slu2_2d_nearest_linear_value_custom_function_(void
*out,const void*in);void slu2_2d_nearest_nearest_derivatives(real_T*gi,const
real_T*inputPts,const real_T*values,const real_T*x1query,const real_T*x2query,
const int*delaunayTri,const int*vertexIDsOnConvHull,const size_t*numSimplices,
const size_t*numPtsOnConvHull,const size_t*numPts,const size_t*numels,const
real_T*work1,const int*work2);void
slu2_2d_nearest_nearest_derivatives_custom_function_(void*out,const void*in);
void slu2_2d_nearest_linear_derivatives(real_T*gi,const real_T*inputPts,const
real_T*values,const real_T*x1query,const real_T*x2query,const int*delaunayTri,
const int*vertexIDsOnConvHull,const size_t*numSimplices,const size_t*
numPtsOnConvHull,const size_t*numPts,const size_t*numels,const real_T*work1,
const int*work2);void slu2_2d_nearest_linear_derivatives_custom_function_(void
*out,const void*in);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void slu2_3d_linear_nearest_process(real_T*inputPts,int*delaunayTri,size_t*
numSimplices,int*convHull,int*pointsOnConvHull,size_t*numPtsOnConvHull,const
real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*fs,const size_t*
numPts);void slu2_3d_linear_nearest_process_custom_function_(void*out,const
void*in);void slu2_3d_linear_linear_process(real_T*inputPts,int*delaunayTri,
size_t*numSimplices,int*convHull,int*pointsOnConvHull,size_t*numPtsOnConvHull,
const real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*fs,const size_t
*numPts);void slu2_3d_linear_linear_process_custom_function_(void*out,const
void*in);void slu2_3d_linear_nearest_value(real_T*fi,const real_T*inputPts,
const real_T*values,const real_T*x1query,const real_T*x2query,const real_T*
x3query,const int*delaunayTri,const int*vertexIDsOnConvHull,const size_t*
numSimplices,const size_t*numPtsOnConvHull,const size_t*numPts,const size_t*
numels,const real_T*work1,const int*work2);void
slu2_3d_linear_nearest_value_custom_function_(void*out,const void*in);void
slu2_3d_linear_linear_value(real_T*fi,const real_T*inputPts,const real_T*
values,const real_T*x1query,const real_T*x2query,const real_T*x3query,const int
*delaunayTri,const int*vertexIDsOnConvHull,const size_t*numSimplices,const
size_t*numPtsOnConvHull,const size_t*numPts,const size_t*numels,const real_T*
work1,const int*work2);void slu2_3d_linear_linear_value_custom_function_(void*
out,const void*in);void slu2_3d_linear_nearest_derivatives(real_T*gi,const
real_T*inputPts,const real_T*values,const real_T*x1query,const real_T*x2query,
const real_T*x3query,const int*delaunayTri,const int*vertexIDsOnConvHull,const
size_t*numSimplices,const size_t*numPtsOnConvHull,const size_t*numPts,const
size_t*numels,const real_T*work1,const int*work2);void
slu2_3d_linear_nearest_derivatives_custom_function_(void*out,const void*in);
void slu2_3d_linear_linear_derivatives(real_T*gi,const real_T*inputPts,const
real_T*values,const real_T*x1query,const real_T*x2query,const real_T*x3query,
const int*delaunayTri,const int*vertexIDsOnConvHull,const size_t*numSimplices,
const size_t*numPtsOnConvHull,const size_t*numPts,const size_t*numels,const
real_T*work1,const int*work2);void
slu2_3d_linear_linear_derivatives_custom_function_(void*out,const void*in);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void slu2_3d_nearest_nearest_process(real_T*inputPts,int*delaunayTri,size_t*
numSimplices,int*convHull,int*pointsOnConvHull,size_t*numPtsOnConvHull,const
real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*fs,const size_t*
numPts);void slu2_3d_nearest_nearest_process_custom_function_(void*out,const
void*in);void slu2_3d_nearest_linear_process(real_T*inputPts,int*delaunayTri,
size_t*numSimplices,int*convHull,int*pointsOnConvHull,size_t*numPtsOnConvHull,
const real_T*x1s,const real_T*x2s,const real_T*x3s,const real_T*fs,const size_t
*numPts);void slu2_3d_nearest_linear_process_custom_function_(void*out,const
void*in);void slu2_3d_nearest_nearest_value(real_T*fi,const real_T*inputPts,
const real_T*values,const real_T*x1query,const real_T*x2query,const real_T*
x3query,const int*delaunayTri,const int*vertexIDsOnConvHull,const size_t*
numSimplices,const size_t*numPtsOnConvHull,const size_t*numPts,const size_t*
numels,const real_T*work1,const int*work2);void
slu2_3d_nearest_nearest_value_custom_function_(void*out,const void*in);void
slu2_3d_nearest_linear_value(real_T*fi,const real_T*inputPts,const real_T*
values,const real_T*x1query,const real_T*x2query,const real_T*x3query,const int
*delaunayTri,const int*vertexIDsOnConvHull,const size_t*numSimplices,const
size_t*numPtsOnConvHull,const size_t*numPts,const size_t*numels,const real_T*
work1,const int*work2);void slu2_3d_nearest_linear_value_custom_function_(void
*out,const void*in);void slu2_3d_nearest_nearest_derivatives(real_T*gi,const
real_T*inputPts,const real_T*values,const real_T*x1query,const real_T*x2query,
const real_T*x3query,const int*delaunayTri,const int*vertexIDsOnConvHull,const
size_t*numSimplices,const size_t*numPtsOnConvHull,const size_t*numPts,const
size_t*numels,const real_T*work1,const int*work2);void
slu2_3d_nearest_nearest_derivatives_custom_function_(void*out,const void*in);
void slu2_3d_nearest_linear_derivatives(real_T*gi,const real_T*inputPts,const
real_T*values,const real_T*x1query,const real_T*x2query,const real_T*x3query,
const int*delaunayTri,const int*vertexIDsOnConvHull,const size_t*numSimplices,
const size_t*numPtsOnConvHull,const size_t*numPts,const size_t*numels,const
real_T*work1,const int*work2);void
slu2_3d_nearest_linear_derivatives_custom_function_(void*out,const void*in);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#include "lang_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
void tlu_simrf_F(real_T*out,const real_T*freqs,const real_T*ys,const real_T*
freq,const size_t*dims);void tlu_simrf_F_custom_function_(PMValue y,
ConstPMValue u);void tlu_simrf_DF(real_T*out,const real_T*freqs,const real_T*
ys,const real_T*freq,const size_t*dims);void tlu_simrf_DF_custom_function_(
PMValue y,ConstPMValue u);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __external_std_h__ */
