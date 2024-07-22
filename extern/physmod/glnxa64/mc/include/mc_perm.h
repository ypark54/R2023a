#ifndef __mc_perm_h__
#define __mc_perm_h__
#include "pm_std.h"
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
typedef struct _McNDTree{int32_T st,sz,id;struct _McNDTree*lt,*rt;}McNDTree;
typedef struct _McNDPermData{McNDTree*mNd;PmIntVector*mP;PmIntVector*mQ;}
McNDPermData;typedef struct _McNDPermDataFlat{size_t m,n;int32_T*st,*sz,*lt,*
rt;int32_T*p,*q;}McNDPermDataFlat;PmIntVector*mc_basker_column_permute(const
McNDTree*bt,const PmSparsityPattern*pat,PmAllocator*alloc);void
mc_basker_tree_free(McNDTree*nd);McNDTree*mc_basker_tree_copy(McNDTree*nd);
void mc_basker_tree_shift(McNDTree*nd,int32_T st);int32_T mc_basker_tree_level
(McNDTree*bt);void mc_free_perm_data(McNDPermData*pd);McNDPermDataFlat*
mc_perm_data_to_flat(const McNDPermData*pd);McNDPermData*
mc_perm_data_from_flat(const McNDPermDataFlat*pdf);void mc_free_perm_data_flat
(McNDPermDataFlat*pdf);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __mc_perm_h__ */
