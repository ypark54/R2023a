/* Copyright 2015-2021 The MathWorks, Inc. */
/*!
 * @file
 * simscape LTI header.
 */

#ifndef nesl_rtw_lti_h
#define nesl_rtw_lti_h

#include <nesl_rtw.h>

PMF_DEPLOY_STATIC boolean_T cgIcSolve(PmRealVector*         x,
                                      const PmRealVector*   u,
                                      const LtiIcParams*    icParams,
                                      NeuDiagnosticManager* mgr) {
    return ic_solve_impl(x, u, icParams, NULL, mgr);
}

PMF_DEPLOY_STATIC SscRTWLogFcnManager* lti_rtw_logging_manager_create(
    const LtiRtwLogParams* logParams,
    real_T                 startTime,
    unsigned int           numInputs,
    unsigned int           numStates,
    unsigned int           inputPortWidth,
    unsigned int           bufferWidth,
    NeuDiagnosticManager*  diagMgr) {

    return ssc_lti_create_rtw_log_fcn_manager(logParams, startTime, numInputs, numStates,
                                              inputPortWidth, bufferWidth, diagMgr);
}

#endif /* include guard */
