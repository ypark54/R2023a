/* Copyright 2017-2022 The MathWorks, Inc. */

/******************************************************************************
 *                                                                            *
 * File    : RTWCG_FMU1_target.c                                              *
 * Abstract:                                                                  *
 *      Wrapper functions to dynamic call libraries of FMU 1.0                *
 *      This file defines functions called by code generation                 *
 *      Also handle errors, and logger                                        *
 *                                                                            *
 ******************************************************************************/

#include "RTWCG_FMU1_target.h"
#define FMU1_MESSAGE_SIZE 1024

#if FMU_CG_TARGET == FMUCG_SLRT
#include "SLRTLoggerWrapper.hpp"
#endif
/*
  Whenever a default function is called, it means a functions is called without successful load,
  return a fmiError;
*/
static fmiStatus defaultfcn(fmiComponent c, ...){
    if(c != NULL){ return fmiFatal;}
    return fmiError;
}

static fmiString formatString(fmiString format, ...) {
	static char translatedMsg[FMU1_MESSAGE_SIZE];
	va_list args;
	va_start(args, format);
	vsnprintf(translatedMsg, FMU1_MESSAGE_SIZE - 1, format, args);
	va_end(args);
	return translatedMsg;
}

static const char* getLoadLibraryErrorMessage()
{
    static char message[FMU1_MESSAGE_SIZE];
#ifdef _WIN32
    unsigned long int errorCode = GetLastError();
    snprintf(message, FMU1_MESSAGE_SIZE, "Load Library Error: '%d' ", errorCode); 
#else
    const char* errorMessage = dlerror();
    snprintf(message, FMU1_MESSAGE_SIZE, "Load Library Error: '%s' ", errorMessage); 
#endif
    return message;
}

/* todo: component-instance mapping; translate value reference to names */
static void fmu1Logger(fmiComponent c,
                       fmiString instanceName,
                       fmiStatus status,
                       fmiString category,
                       fmiString message, ...) {
    (void) c; (void) instanceName; (void) status; (void) category; (void) message;
#if FMU_CG_TARGET == FMUCG_SLRT
    /* Skip printing info logs which are shown by fmiOK status*/
    if(status != fmiOK) {
        SLRTLog(kWarning,message);
    }    
#endif
    /*static const char* strStatus[] = {
        "fmiOK", "fmiWarning", "fmiDiscard", "fmiError", "fmiFatal", "fmiPending" };
    static char translatedMsg[FMU1_MESSAGE_SIZE];
    static char temp[FMU1_MESSAGE_SIZE];
    
    int prefixLength = snprintf(translatedMsg, FMU1_MESSAGE_SIZE, "Log from FMU: [category:%s, status:%s] ",
                            strStatus[status], category); 
    va_list args;
    va_start (args, message);
    vsnprintf(temp, FMU1_MESSAGE_SIZE, message, args);
    va_end(args);
    strncat(translatedMsg, temp, FMU1_MESSAGE_SIZE-prefixLength - 1);
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
    (void) instanceName; 
    void* diagnostic = CreateDiagnosticAsVoidPtr("SL_SERVICES:utils:PRINTFWRAPPER", 1,
                                           CODEGEN_SUPPORT_ARG_STRING_TYPE, translatedMsg);
    rt_ssReportDiagnosticAsInfo(NULL, diagnostic);
#else
    printf("%s\n", translatedMsg);
#endif*/
}


#ifdef _WIN32
static FMUHANDLE loadLibraryUTF8toUTF16(const char* library_loc)
{
    int convertResult = MultiByteToWideChar(CP_UTF8, 0, library_loc, (int)strlen(library_loc), NULL, 0);
    if (convertResult <= 0) {
        return NULL;
    }
    CHAR16_T* wLibrary_loc = (CHAR16_T*)calloc((convertResult + 1), sizeof(CHAR16_T));
    MultiByteToWideChar(CP_UTF8, 0, library_loc, (int)strlen(library_loc), wLibrary_loc, convertResult);
    FMUHANDLE handle = LoadLibraryExW(wLibrary_loc, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
    free(wLibrary_loc);
    return handle;
}
#endif

static _fmi_default_fcn_type LoadFMUFcn(struct FMU1_CS_RTWCG* fmustruct,
                                        const char* fcnName,
                                        int isOptional) {
    _fmi_default_fcn_type fcn = NULL;
    fmiString message = NULL;

    char fullFcnName[FULL_FCN_NAME_MAX_LEN];
    memset(fullFcnName, 0, FULL_FCN_NAME_MAX_LEN);
    strncpy(fullFcnName, fmustruct->modelIdentifier, FULL_FCN_NAME_MAX_LEN - 1);
    strncat(fullFcnName, "_", FULL_FCN_NAME_MAX_LEN - strlen(fullFcnName) - 1);
    strncat(fullFcnName, fcnName, FULL_FCN_NAME_MAX_LEN - strlen(fullFcnName) - 1);
        
#ifdef _WIN32
    fcn = (_fmi_default_fcn_type)LOAD_FUNCTION(fmustruct->Handle, fullFcnName);
#else
    *((void **)(&fcn)) = LOAD_FUNCTION(fmustruct->Handle, fullFcnName);
#endif

    if (!isOptional && fcn == NULL) {
        fmustruct->FMUErrorStatus = fmiWarning;
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
        void * diagnostic = CreateDiagnosticAsVoidPtr("FMUBlock:FMU:FMULoadLibFunctionError", 2,
                                                      CODEGEN_SUPPORT_ARG_STRING_TYPE, fcnName,
                                                      CODEGEN_SUPPORT_ARG_STRING_TYPE, fmustruct->fmuname);
        /*A loading failure will cause a warning, ANY CALL to default fcn will result in fmiError and stop*/
        rt_ssReportDiagnosticAsWarning(fmustruct->ssPtr, diagnostic);
#else
        message = formatString("Unable to load function '%s' for FMU '%s'.", fcnName, fmustruct->fmuname);
        fmustruct->callbacks.logger(fmustruct->mFMIComp, fmustruct->fmuname, fmustruct->FMUErrorStatus, "FMU1_fmuInitialize", message);  
#endif
        fcn = (_fmi_default_fcn_type) defaultfcn; 
    }
    return fcn;
}

/*Load FMU 1 CS Functions*/
static void LoadFMU1CSFunctions(struct FMU1_CS_RTWCG* fmustruct,
                                int loadGetRealOutputDerivativesFcn,
                                int loadSetRealInputDerivativesFcn) {
    fmustruct->getTypesPlatform   = (_fmiGetTypesPlatform)               LoadFMUFcn(fmustruct, "fmiGetTypesPlatform", 0); 
    fmustruct->instantiateSlave   = (_fmiInstantiateSlave)               LoadFMUFcn(fmustruct, "fmiInstantiateSlave", 0);
    fmustruct->initializeSlave    = (_fmiInitializeSlave)                LoadFMUFcn(fmustruct, "fmiInitializeSlave", 0);    
    fmustruct->terminateSlave     = (_fmiTerminateSlave)                 LoadFMUFcn(fmustruct, "fmiTerminateSlave", 0);
    fmustruct->resetSlave         = (_fmiResetSlave)                     LoadFMUFcn(fmustruct, "fmiResetSlave", 0);
    fmustruct->freeSlaveInstance  = (_fmiFreeSlaveInstance)              LoadFMUFcn(fmustruct, "fmiFreeSlaveInstance", 0);
    fmustruct->setRealInputDerivatives = (_fmiSetRealInputDerivatives)   LoadFMUFcn(fmustruct, "fmiSetRealInputDerivatives", !loadSetRealInputDerivativesFcn);
    fmustruct->getRealOutputDerivatives = (_fmiGetRealOutputDerivatives) LoadFMUFcn(fmustruct, "fmiGetRealOutputDerivatives", !loadGetRealOutputDerivativesFcn);
    fmustruct->cancelStep              = (_fmiCancelStep)                LoadFMUFcn(fmustruct, "fmiCancelStep", 0);
    fmustruct->doStep                  = (_fmiDoStep)                    LoadFMUFcn(fmustruct, "fmiDoStep", 0);
    fmustruct->getStatus               = (_fmiGetStatus)                 LoadFMUFcn(fmustruct, "fmiGetStatus", 0);
    fmustruct->getRealStatus           = (_fmiGetRealStatus)             LoadFMUFcn(fmustruct, "fmiGetRealStatus", 0);
    fmustruct->getIntegerStatus        = (_fmiGetIntegerStatus)          LoadFMUFcn(fmustruct, "fmiGetIntegerStatus", 0);
    fmustruct->getBooleanStatus        = (_fmiGetBooleanStatus)          LoadFMUFcn(fmustruct, "fmiGetBooleanStatus", 0);
    fmustruct->getStringStatus         = (_fmiGetStringStatus)           LoadFMUFcn(fmustruct, "fmiGetStringStatus", 0);

    fmustruct->getVersion              = (_fmiGetVersion)                LoadFMUFcn(fmustruct, "fmiGetVersion", 0);
    fmustruct->setDebugLogging         = (_fmiSetDebugLogging)           LoadFMUFcn(fmustruct, "fmiSetDebugLogging", 0);
    fmustruct->setReal                 = (_fmiSetReal)                   LoadFMUFcn(fmustruct, "fmiSetReal", 0);
    fmustruct->setInteger              = (_fmiSetInteger)                LoadFMUFcn(fmustruct, "fmiSetInteger", 0);
    fmustruct->setBoolean              = (_fmiSetBoolean)                LoadFMUFcn(fmustruct, "fmiSetBoolean", 0);
    fmustruct->setString               = (_fmiSetString)                 LoadFMUFcn(fmustruct, "fmiSetString", 0);
    fmustruct->getReal                 = (_fmiGetReal)                   LoadFMUFcn(fmustruct, "fmiGetReal", 0);
    fmustruct->getInteger              = (_fmiGetInteger)                LoadFMUFcn(fmustruct, "fmiGetInteger", 0);
    fmustruct->getBoolean              = (_fmiGetBoolean)                LoadFMUFcn(fmustruct, "fmiGetBoolean", 0);
    fmustruct->getString               = (_fmiGetString)                 LoadFMUFcn(fmustruct, "fmiGetString", 0);
}

static fmiBoolean CheckStatus(struct FMU1_CS_RTWCG* fmustruct,
                              fmiStatus status,
                              fmiString fcnName) {
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
    SimStruct* ss = fmustruct->ssPtr;
    
    if(status == fmiError || status == fmiFatal){
        void * diagnostic = CreateDiagnosticAsVoidPtr("FMUBlock:FMU:FMUSimFunctionErrorDebugToDisplayOn", 2,
                                                      CODEGEN_SUPPORT_ARG_STRING_TYPE, fcnName,
                                                      CODEGEN_SUPPORT_ARG_STRING_TYPE, fmustruct->fmuname);
        fmustruct->FMUErrorStatus = status;
        rt_ssSet_slErrMsg(ss, diagnostic);
        ssSetStopRequested(ss, 1);
    }else if (status == fmiPending) {
        void * diagnostic = CreateDiagnosticAsVoidPtr("FMUBlock:FMU:FMUSimPendingNotAllowed", 2,
                                                      CODEGEN_SUPPORT_ARG_STRING_TYPE, fcnName,
                                                      CODEGEN_SUPPORT_ARG_STRING_TYPE, fmustruct->fmuname);
        fmustruct->FMUErrorStatus = status;
        rt_ssSet_slErrMsg(ss, diagnostic);
        ssSetStopRequested(ss, 1);
    }
#else
    fmiString message = NULL;
    if(status == fmiError || status == fmiFatal){
        fmustruct->FMUErrorStatus = status;
        message = formatString("Error in supplied FMU: An error occurred in function '%s' for block '%s' during simulation.", fcnName, fmustruct->fmuname);
        fmustruct->callbacks.logger(fmustruct->mFMIComp, fmustruct->fmuname, fmustruct->FMUErrorStatus, "CheckStatus", message);
    }else if (status == fmiPending) {
        fmustruct->FMUErrorStatus = status;
        message = formatString("Function '%s' in block '%s' returned 'fmiPending' flag during simulation.", fcnName, fmustruct->fmuname);
        fmustruct->callbacks.logger(fmustruct->mFMIComp, fmustruct->fmuname, fmustruct->FMUErrorStatus, "CheckStatus", message);
    }
#endif
    
    if (status == fmiOK)
        return fmiTrue;
    else
        return fmiFalse;
}

/* fmuPrefix is modelIdentifier */
static void* FMU1_fmuInitialize(const char* lib,
                                fmiString instanceName,
                                fmiString modelIdentifier,
                                fmiString fmuGUID,
                                fmiString fmuLocation,
                                void* ssPtr,
                                int loadGetRealOutputDerivativesFcn,
                                int loadSetRealInputDerivativesFcn,
                                const void* fmuCallBacks,
                                fmiBoolean fmuVisible,
                                fmiBoolean fmuLoggingOn) {
    fmiReal timeout = 0;                  /* wait period in milli seconds, 0 for unlimited wait period"*/
    fmiString message = NULL;
    const char* mimeType = "application/x-fmu-sharedlibrary";
    
    fmiCallbackAllocateMemory allocateMemory = (fmuCallBacks == NULL) ? calloc : ((const fmiCallbackFunctions*)(fmuCallBacks))->allocateMemory;
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)allocateMemory(1, sizeof(struct FMU1_CS_RTWCG));
    
    fmustruct->callbacks.logger = (fmuCallBacks == NULL) ? fmu1Logger : ((const fmiCallbackFunctions*)(fmuCallBacks))->logger;
    fmustruct->callbacks.allocateMemory = allocateMemory;
    fmustruct->callbacks.freeMemory = (fmuCallBacks == NULL) ? free : ((const fmiCallbackFunctions*)(fmuCallBacks))->freeMemory;
    fmustruct->callbacks.stepFinished = (fmuCallBacks == NULL) ? NULL  : ((const fmiCallbackFunctions*)(fmuCallBacks))->stepFinished;
    
    fmiBoolean visible = (fmuCallBacks == NULL) ? fmiFalse : fmuVisible; /* no simulator user interface in rapid accel*/
    fmiBoolean isLoggingOn = (fmuCallBacks == NULL) ? fmiFalse : fmuLoggingOn; /*logging is currently disabled for rapid accel, on for debug*/ 
    fmiBoolean interactive = fmiFalse;    /* simulation run without user interaction*/
    
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
    (void) message;
    fmustruct->ssPtr = (SimStruct*) ssPtr;
    
#endif
    fmustruct->fmuname = (char *)instanceName;
    fmustruct->modelIdentifier = (char*)modelIdentifier;
    fmustruct->dllfile = (char *)lib;
    char* fmuResLocation = (char*) fmuLocation;
    fmustruct->FMUErrorStatus = fmiOK;
    fmustruct->modelInitialized = false;
   
    if (strlen(instanceName)+ FCN_NAME_MAX_LEN + 1 >= FULL_FCN_NAME_MAX_LEN) {
        /*FMU name is longer than 200+, rarely happens*/
        fmustruct->FMUErrorStatus = fmiFatal;
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
        void * diagnostic = CreateDiagnosticAsVoidPtr("SL_SERVICES:utils:PRINTFWRAPPER", 1,
                                                      CODEGEN_SUPPORT_ARG_STRING_TYPE, "FMU Name is too long.");
        rt_ssReportDiagnosticAsWarning(fmustruct->ssPtr, diagnostic);
        ssSetStopRequested(fmustruct->ssPtr, 1);
#else
        message = formatString("FMU Name '%s' is too long.", instanceName);
        fmustruct->callbacks.logger(fmustruct->mFMIComp, fmustruct->fmuname, fmustruct->FMUErrorStatus, "FMU1_fmuInitialize", message);
#endif
        return NULL;
    }

    fmustruct->Handle = LOAD_LIBRARY(fmustruct->dllfile);
    if (NULL == fmustruct->Handle) {
        fmustruct->FMUErrorStatus = fmiFatal;
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
        void * diagnostic = CreateDiagnosticAsVoidPtr("FMUBlock:FMU:FMULoadLibraryError", 2,
                                                      CODEGEN_SUPPORT_ARG_STRING_TYPE, fmustruct->dllfile,
                                                      CODEGEN_SUPPORT_ARG_STRING_TYPE, fmustruct->fmuname);
        /*loading lib failure will cause fmiFatal and stop*/
        rt_ssReportDiagnosticAsWarning(fmustruct->ssPtr, diagnostic);
        ssSetStopRequested(fmustruct->ssPtr, 1);
#else
        const char* errorCodeMessage = getLoadLibraryErrorMessage();
        message = formatString("Unable to load dynamic library: '%s'. %s", fmustruct->dllfile, errorCodeMessage);
        fmustruct->callbacks.logger(fmustruct->mFMIComp, fmustruct->fmuname, fmustruct->FMUErrorStatus, "FMU1_fmuInitialize", message);        
#endif
        return NULL;
    }

    /* load fmi functions */
    LoadFMU1CSFunctions(fmustruct, loadGetRealOutputDerivativesFcn, loadSetRealInputDerivativesFcn);

    /* instantiate fmu */
    fmustruct->mFMIComp = fmustruct->instantiateSlave(instanceName,
                                                      fmuGUID,
                                                      fmuResLocation,
                                                      mimeType,
                                                      timeout,
                                                      visible,
                                                      interactive,
                                                      fmustruct->callbacks,
                                                      isLoggingOn);
    if (NULL == fmustruct->mFMIComp){
        CheckStatus(fmustruct, fmiError, "fmiInstantiateSlave");
        return NULL;
    }
    return (void *) fmustruct;
}

void* FMU1_fmuInitializeCS(const char* lib,
                           fmiString instanceName,
                           fmiString modelIdentifier,
                           fmiString fmuGUID,
                           fmiString fmuLocation,
                           void* ssPtr,
                           int loadGetRealOutputDerivativesFcn,
                           int loadSetRealInputDerivativesFcn,
                           const void* fmuCallBacks,
                           fmiBoolean fmuVisible,
                           fmiBoolean fmuLoggingOn) {
    return FMU1_fmuInitialize(lib,
                              instanceName,
                              modelIdentifier,
                              fmuGUID,
                              fmuLocation,
                              ssPtr,
                              loadGetRealOutputDerivativesFcn,
                              loadSetRealInputDerivativesFcn,
                              fmuCallBacks,
                              fmuVisible,
                              fmuLoggingOn);
}

fmiBoolean FMU1_initializeSlave(void **fmuv,
                                fmiReal tStart) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiStatus fmiFlag = fmustruct->initializeSlave(fmustruct->mFMIComp, tStart, fmiFalse, 0);
    fmustruct->modelInitialized = true;
    return CheckStatus(fmustruct, fmiFlag, "fmiInitializeSlave");
}

fmiBoolean FMU1_terminateSlave(void **fmuv) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiBoolean returnStatus = fmiTrue;
    if (fmustruct == NULL) {
        return returnStatus;
    }
    if (fmustruct->FMUErrorStatus != fmiFatal) {
        if(fmustruct->FMUErrorStatus != fmiError &&
           fmustruct->modelInitialized == true) {
            fmiStatus fmiFlag = fmustruct->terminateSlave(fmustruct->mFMIComp);
            returnStatus = CheckStatus(fmustruct, fmiFlag, "fmiTerminateSlave");
        }
        fmustruct->freeSlaveInstance(fmustruct->mFMIComp);
    }
    CLOSE_LIBRARY(fmustruct->Handle);
    fmiCallbackFreeMemory freeMemory = fmustruct->callbacks.freeMemory;
    freeMemory(fmustruct);
    return returnStatus;
}

fmiBoolean FMU1_doStep(void **fmuv,
                      fmiReal currentCommunicationPoint,
                      fmiReal communicationStepSize) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiStatus fmiFlag = fmustruct->doStep(fmustruct->mFMIComp,
                                          currentCommunicationPoint,
                                          communicationStepSize,
                                          fmiTrue);
    return CheckStatus(fmustruct, fmiFlag, "fmiDoStep");
}

fmiBoolean FMU1_setRealVal(void **fmuv,
                          const fmiValueReference dvr,
                          size_t nvr,
                          const fmiReal dvalue) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiReal value = dvalue;
    fmiStatus fmiFlag = fmustruct->setReal(fmustruct->mFMIComp, &vr, nvr, &value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetReal");
}

fmiBoolean FMU1_setReal(void **fmuv,
                       const fmiValueReference dvr,
                       size_t nvr,
                       const fmiReal value[]) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->setReal(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetReal");
}

fmiBoolean FMU1_getReal(void **fmuv,
                       const fmiValueReference dvr,
                       size_t nvr,
                       fmiReal value[]) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->getReal(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiGetReal");
}

fmiBoolean FMU1_setIntegerVal(void **fmuv,
                             const fmiValueReference dvr,
                             size_t nvr,
                             const fmiInteger dvalue) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiInteger value = dvalue;
    fmiStatus fmiFlag = fmustruct->setInteger(fmustruct->mFMIComp, &vr, nvr, &value);
    return CheckStatus(fmustruct, fmiFlag, "fmiGetInteger");
}

fmiBoolean FMU1_setInteger(void **fmuv,
                          const fmiValueReference dvr,
                          size_t nvr,
                          const fmiInteger value[]) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->setInteger(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetInteger");

}

fmiBoolean FMU1_getInteger(void **fmuv,
                          const fmiValueReference dvr,
                          size_t nvr,
                          fmiInteger value[]) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->getInteger(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiGetInteger");
}

fmiBoolean FMU1_setBooleanVal(void **fmuv,
                             const fmiValueReference dvr,
                             size_t nvr,
                             const unsigned char dvalue) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiBoolean value = (fmiBoolean) dvalue;
    fmiStatus fmiFlag = fmustruct->setBoolean(fmustruct->mFMIComp, &vr, nvr, &value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetBoolean");
}

fmiBoolean FMU1_setBoolean(void **fmuv,
                          const fmiValueReference dvr,
                          size_t nvr,
                          const unsigned char value[]) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->setBoolean(fmustruct->mFMIComp, &vr, nvr, (fmiBoolean *) value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetBoolean");
}

fmiBoolean FMU1_getBoolean(void **fmuv,
                          const fmiValueReference dvr,
                          size_t nvr,
                          unsigned char value[]) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->getBoolean(fmustruct->mFMIComp, &vr, nvr, (fmiBoolean*) value);
    return CheckStatus(fmustruct, fmiFlag, "fmiGetBoolean");
}

fmiBoolean FMU1_setStringVal(void **fmuv,
                            const fmiValueReference dvr,
                            size_t nvr,
                            const fmiString dvalue) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiString value = dvalue;
    fmiStatus fmiFlag = fmustruct->setString(fmustruct->mFMIComp, &vr, nvr, &value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetString");
}

fmiBoolean FMU1_setString(void **fmuv,
                         const fmiValueReference dvr,
                         size_t nvr,
                         const fmiString value[]) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->setString(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetString");
}

fmiBoolean FMU1_getString(void **fmuv,
                         const fmiValueReference dvr,
                         size_t nvr,
                         fmiString value[]) {
    struct FMU1_CS_RTWCG * fmustruct = (struct FMU1_CS_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->getString(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiGetString");
}
