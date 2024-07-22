/* Copyright 2021 The MathWorks, Inc. */

#ifndef ssc_rtw_logging_h
#define ssc_rtw_logging_h

#include <nesl_rtw.h>

PMF_DEPLOY_STATIC
SscRTWLogFcnManager* ssc_lease_rtw_log_fcn_manager(const char*        key,
                                                   int                index,
                                                   unsigned int const numInputPorts,
                                                   boolean_T const*   directFeedThrough,
                                                   unsigned int const inputPortWidth,
                                                   unsigned int const bufferWidth) {
    const NeslSimulatorGroupRegistry* registry = nesl_get_registry();

    /* Const cast required to pass through RTWCG-generated pointer type*/
    union {
        const SscRTWLogFcnManager* constManager;
        SscRTWLogFcnManager*       manager;
    } u;

    u.constManager = registry->mLeaseRtwLogFcnManager(registry, key, (size_t)index);

    if (u.constManager != NULL) {
        u.manager->mRtwObject->mNumInputPorts = numInputPorts;

        if (numInputPorts > 0) {
            u.manager->mRtwObject->mDirectFeedThrough =
                (boolean_T*)malloc(numInputPorts * sizeof(boolean_T));
            memcpy(u.manager->mRtwObject->mDirectFeedThrough, directFeedThrough,
                   numInputPorts * sizeof(boolean_T));
        }

        u.manager->mRtwObject->mInputPortWidth    = inputPortWidth;
        u.manager->mRtwObject->mBufferWidth       = bufferWidth;
        u.manager->mRtwObject->mDiagnosticManager = rtw_create_diagnostics();
    }
    return u.manager;
}

PMF_DEPLOY_STATIC
void ssc_rtw_log_fcn_destroy(SscRTWLogFcnManager* manager) {
    if (manager != NULL) {
        manager->mRtwDestroy(manager);
    }
}


#endif /* include guard */
