#ifndef __ne_initer_dae_h__
#define __ne_initer_dae_h__
#include "ne_initer_fwd.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
SscIniter*sec_create_initer(NeDae*dae,boolean_T isFreqTime);SscComputeState*
sec_initer_create_converter(PmBoolVector*obsLin,McRealFunction*obsSimFunc,
SscIniter*initer,const McLinearAlgebraFactory*fac);NeStiffness*
sec_initer_create_stiffness(SscIniter*initer,SscComputeState*cs,
NeSystemInputSizes*sizes);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ne_initer_dae_h__ */
