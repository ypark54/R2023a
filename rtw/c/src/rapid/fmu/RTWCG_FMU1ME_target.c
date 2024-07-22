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

#include "RTWCG_FMU1ME_target.h"
#define FMU1_MESSAGE_SIZE 1024

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
    
    /*
    va_list args;
    void * diagnostic;
    int prefixLength;

    static const char* strStatus[] = {
        "fmiOK", "fmiWarning", "fmiDiscard", "fmiError", "fmiFatal", "fmiPending" };
    static char translatedMsg[FMU1_MESSAGE_SIZE];
    static char temp[FMU1_MESSAGE_SIZE];
    
    prefixLength = snprintf(translatedMsg, FMU1_MESSAGE_SIZE, "Log from FMU: [category:%s, status:%s] ",
                            strStatus[status], category); 
    va_start (args, message);
    vsnprintf(temp, FMU1_MESSAGE_SIZE, message, args);
    va_end(args);
    
    strncat(translatedMsg, temp, FMU1_MESSAGE_SIZE-prefixLength - 1);
    diagnostic = CreateDiagnosticAsVoidPtr("SL_SERVICES:utils:PRINTFWRAPPER", 1,
                                           CODEGEN_SUPPORT_ARG_STRING_TYPE, translatedMsg);
                                           rt_ssReportDiagnosticAsWarning(NULL, diagnostic); 
    rt_ssReportDiagnosticAsWarning(NULL, diagnostic);*/
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

static _fmi_default_fcn_type LoadFMUFcn(struct FMU1_ME_RTWCG* fmustruct,
                                        const char * fcnName,
                                        int isOptional) {
    _fmi_default_fcn_type fcn = NULL;
    fmiString message = NULL;

    static char fullFcnName[FULL_FCN_NAME_MAX_LEN];
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
        /*A loading failure will cause a warning, ANY CALL to defualt Fcn will result in an Error and Stop*/
        rt_ssReportDiagnosticAsWarning(fmustruct->ssPtr, diagnostic);
#else
        message = formatString("Unable to load function '%s' for FMU '%s'.", fcnName, fmustruct->fmuname);
        fmustruct->callbacks.logger(fmustruct->mFMIComp, fmustruct->fmuname, fmustruct->FMUErrorStatus, "LoadFMUFcn", message);
#endif
        fcn = (_fmi_default_fcn_type) defaultfcn; 
    }
    return fcn;
}

/*Load FMU 1 ME Functions*/
static void LoadFMU1MEFunctions(struct FMU1_ME_RTWCG * fmustruct,
                                int loadGetRealOutputDerivativesFcn,
                                int loadSetRealInputDerivativesFcn) {
    (void)loadGetRealOutputDerivativesFcn;
    (void)loadSetRealInputDerivativesFcn;
    
    fmustruct->getModelTypesPlatform      = (_fmiGetModelTypesPlatform) LoadFMUFcn(fmustruct, "fmiGetModelTypesPlatform", 0);
    fmustruct->getVersion                 = (_fmiGetVersion)         LoadFMUFcn(fmustruct, "fmiGetVersion", 0);
    fmustruct->setDebugLogging            = (_fmiSetDebugLogging)    LoadFMUFcn(fmustruct, "fmiSetDebugLogging", 0);   
    fmustruct->instantiateModel           = (_fmiInstantiateModel)   LoadFMUFcn(fmustruct, "fmiInstantiateModel", 0);
    fmustruct->freeModelInstance          = (_fmiFreeModelInstance)  LoadFMUFcn(fmustruct, "fmiFreeModelInstance", 0);
    fmustruct->setTime                    = (_fmiSetTime)            LoadFMUFcn(fmustruct, "fmiSetTime", 0);    
    fmustruct->setContinuousStates        = (_fmiSetContinuousStates)      LoadFMUFcn(fmustruct, "fmiSetContinuousStates", 0);
    fmustruct->completedIntegratorStep    = (_fmiCompletedIntegratorStep)  LoadFMUFcn(fmustruct, "fmiCompletedIntegratorStep", 0);
    fmustruct->initialize                 = (_fmiInitialize)         LoadFMUFcn(fmustruct, "fmiInitialize", 0);
    fmustruct->getDerivatives             = (_fmiGetDerivatives)     LoadFMUFcn(fmustruct, "fmiGetDerivatives", 0);
    fmustruct->getEventIndicators         = (_fmiGetEventIndicators) LoadFMUFcn(fmustruct, "fmiGetEventIndicators", 0);
    fmustruct->eventUpdate                = (_fmiEventUpdate)        LoadFMUFcn(fmustruct, "fmiEventUpdate", 0);
    fmustruct->getContinuousStates        = (_fmiGetContinuousStates)         LoadFMUFcn(fmustruct, "fmiGetContinuousStates", 0);
    fmustruct->getNominalContinuousStates = (_fmiGetNominalContinuousStates)  LoadFMUFcn(fmustruct, "fmiGetNominalContinuousStates", 0);
    fmustruct->getStateValueReferences    = (_fmiGetStateValueReferences)     LoadFMUFcn(fmustruct, "fmiGetStateValueReferences", 0);
    fmustruct->terminate                  = (_fmiTerminate)                   LoadFMUFcn(fmustruct, "fmiTerminate", 0);
    fmustruct->setReal                    = (_fmiSetReal)                   LoadFMUFcn(fmustruct, "fmiSetReal", 0);
    fmustruct->setInteger                 = (_fmiSetInteger)                LoadFMUFcn(fmustruct, "fmiSetInteger", 0);
    fmustruct->setBoolean                 = (_fmiSetBoolean)                LoadFMUFcn(fmustruct, "fmiSetBoolean", 0);
    fmustruct->setString                  = (_fmiSetString)                 LoadFMUFcn(fmustruct, "fmiSetString", 0);
    fmustruct->getReal                    = (_fmiGetReal)                   LoadFMUFcn(fmustruct, "fmiGetReal", 0);
    fmustruct->getInteger                 = (_fmiGetInteger)                LoadFMUFcn(fmustruct, "fmiGetInteger", 0);
    fmustruct->getBoolean                 = (_fmiGetBoolean)                LoadFMUFcn(fmustruct, "fmiGetBoolean", 0);
    fmustruct->getString                  = (_fmiGetString)                 LoadFMUFcn(fmustruct, "fmiGetString", 0);
}

static fmiBoolean CheckStatus(struct FMU1_ME_RTWCG* fmustruct,
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
    }
#else
    fmiString message = NULL;
    if(status == fmiError || status == fmiFatal){
        fmustruct->FMUErrorStatus = status;
        message = formatString("Error in supplied FMU: An error occurred in function '%s' for block '%s' during simulation.", fcnName, fmustruct->fmuname);
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
    fmiReal timeout = 0;               /* wait period in milli seconds, 0 for unlimited wait period"*/
    fmiString message = NULL;

    fmiCallbackAllocateMemory allocateMemory = (fmuCallBacks == NULL) ? calloc : ((const fmiCallbackFunctions*)(fmuCallBacks))->allocateMemory;
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)allocateMemory(1, sizeof(struct FMU1_ME_RTWCG));
    fmustruct->callbacks.logger = (fmuCallBacks == NULL) ? fmu1Logger : ((const fmiCallbackFunctions*)(fmuCallBacks))->logger;
    fmustruct->callbacks.allocateMemory = allocateMemory;
    fmustruct->callbacks.freeMemory = (fmuCallBacks == NULL) ? free : ((const fmiCallbackFunctions*)(fmuCallBacks))->freeMemory;
    fmustruct->callbacks.stepFinished = (fmuCallBacks == NULL) ? NULL  : ((const fmiCallbackFunctions*)(fmuCallBacks))->stepFinished;
    
    fmiBoolean visible = (fmuCallBacks == NULL) ? fmiFalse : fmuVisible; /* no simulator user interface in rapid accel*/
    fmiBoolean isLoggingOn = (fmuCallBacks == NULL) ? fmiFalse : fmuLoggingOn; /*logging is currently disabled for rapid accel, on for debug*/ 
    fmiBoolean interactive = fmiFalse; /* simulation run without user interaction*/
       
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
    fmustruct->ssPtr = (SimStruct*) ssPtr;
    (void) message;
#endif
    fmustruct->fmuname = (char *)instanceName;
    fmustruct->modelIdentifier = (char*)modelIdentifier;
    fmustruct->dllfile = (char *)lib;
    fmustruct->FMUErrorStatus = fmiOK;
    fmustruct->modelInitialized = false;
    (void) fmuLocation;
    
    if (strlen(instanceName)+ FCN_NAME_MAX_LEN + 1 >= FULL_FCN_NAME_MAX_LEN){
        /*FMU name is longer than 200+, rarely happens*/
        fmustruct->FMUErrorStatus = fmiFatal;
#if FMU_CG_TARGET < 20
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
        /*loading lib failure will cause an Fatal and Stop*/
        rt_ssReportDiagnosticAsWarning(fmustruct->ssPtr, diagnostic);
        ssSetStopRequested(fmustruct->ssPtr, 1);
#else
        const char* errorCodeMessage = getLoadLibraryErrorMessage();
        message = formatString("Unable to load dynamic library: '%s'.", fmustruct->dllfile, errorCodeMessage);
        fmustruct->callbacks.logger(fmustruct->mFMIComp, fmustruct->fmuname, fmustruct->FMUErrorStatus, "FMU1_fmuInitialize", message);
#endif
        return NULL;
    }

    /* load fmi functions */
    LoadFMU1MEFunctions(fmustruct, loadGetRealOutputDerivativesFcn, loadSetRealInputDerivativesFcn);

    /* instantiate fmu */
    fmustruct->mFMIComp = fmustruct->instantiateModel(instanceName,
                                                      fmuGUID,
                                                      fmustruct->callbacks,
                                                      isLoggingOn);
    if (NULL == fmustruct->mFMIComp ){
        CheckStatus(fmustruct, fmiError, "fmiInstantiateModel");
        return NULL;
    }
    return (void *) fmustruct;
}

void* FMU1_fmuInitializeME(const char * lib,
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

fmiBoolean FMU1_terminateModel(void **fmuv) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    if (fmustruct == NULL) {
        return fmiTrue;
    }
    if(fmustruct->FMUErrorStatus != fmiFatal) {
       if(fmustruct->FMUErrorStatus != fmiError &&
          fmustruct->modelInitialized == true) {
            fmiStatus fmiFlag = fmustruct->terminate(fmustruct->mFMIComp);
            CheckStatus(fmustruct, fmiFlag, "fmiTerminate");
          }
        fmustruct->freeModelInstance(fmustruct->mFMIComp);
    }
    CLOSE_LIBRARY(fmustruct->Handle);
    fmiCallbackFreeMemory freeMemory = fmustruct->callbacks.freeMemory;
    freeMemory(fmustruct);
    return fmiTrue;
}

void FMU1_checkSimTerminate(void **fmuv,
                            const char* blkPath,
                            fmiReal time) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiString message = NULL;
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
    (void) message;
    if(fmustruct->eventInfo.terminateSimulation == fmiTrue){
        /* terminate the simulation (successfully) */
        void * diagnostic = CreateDiagnosticAsVoidPtr("FMUBlock:FMU:FMUSimEventUpdateTerminated", 2,
                                                      CODEGEN_SUPPORT_ARG_STRING_TYPE, blkPath,
                                                      CODEGEN_SUPPORT_ARG_REAL_TYPE, time);
        rt_ssReportDiagnosticAsInfo(fmustruct->ssPtr, diagnostic);
        ssSetStopRequested(fmustruct->ssPtr, 1);
    }
#else
    message = formatString("'TerminateSimulation' flag was set to true by FMU '%s' during 'fmiEventUpdate'."
                                     " Simulation will stop after the current time step at %f.", blkPath, time);
    fmustruct->callbacks.logger(fmustruct->mFMIComp, fmustruct->fmuname, fmustruct->FMUErrorStatus, "FMU1_checkSimTerminate", message);
#endif
}

fmiBoolean FMU1_initialize(void **fmuv,
                          unsigned char isToleranceUsed,
                          fmiReal toleranceValue) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiStatus fmiFlag = fmustruct->initialize(fmustruct->mFMIComp,
                                              (fmiBoolean) isToleranceUsed,
                                              toleranceValue, &(fmustruct->eventInfo));
    fmustruct->modelInitialized = true;
    return CheckStatus(fmustruct, fmiFlag, "fmiInitialize");
}

fmiBoolean FMU1_setTime(void **fmuv,
                       fmiReal time) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiStatus fmiFlag = fmustruct->setTime(fmustruct->mFMIComp, time);
    return CheckStatus(fmustruct, fmiFlag, "fmisetTime");
}

fmiBoolean FMU1_completedIntegratorStep(void **fmuv,
                                       int8_T* hasStepEvent) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiStatus fmiFlag = fmustruct->completedIntegratorStep(fmustruct->mFMIComp, (fmiBoolean *)hasStepEvent);
    return CheckStatus(fmustruct, fmiFlag, "completedIntegratorStep");
}

fmiBoolean FMU1_setContinuousStates(void **fmuv,
                                   fmiReal states[],
                                   size_t nx) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiStatus fmiFlag = fmustruct->setContinuousStates(fmustruct->mFMIComp, states, nx);
    return CheckStatus(fmustruct, fmiFlag, "setContinuousStates");
}
    
fmiBoolean FMU1_getContinuousStates(void **fmuv,
                                   fmiReal states[],
                                   size_t nx) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiStatus fmiFlag = fmustruct->getContinuousStates(fmustruct->mFMIComp, states, nx);
    return CheckStatus(fmustruct, fmiFlag, "getContinuousStates");
}

fmiBoolean FMU1_getDerivatives(void **fmuv,
                              fmiReal derivatives[],
                              size_t nx) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiStatus fmiFlag = fmustruct->getDerivatives(fmustruct->mFMIComp, derivatives, nx);
    return CheckStatus(fmustruct, fmiFlag, "getDerivatives");
}

fmiBoolean FMU1_getEventIndicators(void **fmuv,
                                  fmiReal eventIndicators[],
                                  size_t nx) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiStatus fmiFlag = fmustruct->getEventIndicators(fmustruct->mFMIComp, eventIndicators, nx);
    return CheckStatus(fmustruct, fmiFlag, "getEventIndicators");
}

/* event helper functions*/
fmiBoolean FMU1_eventIteration(void **fmuv,
                              const char* blkPath,
                              fmiReal time) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiStatus fmiFlag = fmiOK;
    fmiBoolean returnStatus = fmiTrue;
    int iterationNumber = 0;
    fmiString message = NULL;
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
    (void) message;
#endif
    while(fmustruct->eventInfo.iterationConverged == fmiFalse){
        /*safe call to eventUpdate*/
        fmiFlag = fmustruct->eventUpdate(fmustruct->mFMIComp, fmiFalse, &(fmustruct->eventInfo));
        returnStatus = CheckStatus(fmustruct, fmiFlag, "eventUpdate");

        if(returnStatus == fmiTrue && fmustruct->eventInfo.terminateSimulation == fmiTrue){
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
            /* terminate the simulation (successfully) */
            void * diagnostic = CreateDiagnosticAsVoidPtr("FMUBlock:FMU:FMUSimEventUpdateTerminated", 2,
                                                          CODEGEN_SUPPORT_ARG_STRING_TYPE, blkPath,
                                                          CODEGEN_SUPPORT_ARG_REAL_TYPE, time);
            rt_ssReportDiagnosticAsInfo(fmustruct->ssPtr, diagnostic);
            ssSetStopRequested(fmustruct->ssPtr, 1);
#else
            message = formatString("'TerminateSimulation' flag was set to true by FMU %s during 'fmiEventUpdate'."
                                             " Simulation will stop after the current time step at %f.", blkPath, time);
            fmustruct->callbacks.logger(fmustruct->mFMIComp, fmustruct->fmuname, fmustruct->FMUErrorStatus, "FMU1_checkSimTerminate", message);
#endif
        }


        if(iterationNumber >= 10000){
#if FMU_CG_TARGET < FMUCG_STANDALONE_TARGETS
            void * diagnostic = CreateDiagnosticAsVoidPtr("FMUBlock:FMU:FMUSimEventUpdateNotConverge", 3,
                                                          CODEGEN_SUPPORT_ARG_REAL_TYPE, time,
                                                          CODEGEN_SUPPORT_ARG_INTEGER_TYPE, iterationNumber,
                                                          CODEGEN_SUPPORT_ARG_STRING_TYPE, blkPath);
            rt_ssReportDiagnosticAsWarning(fmustruct->ssPtr, diagnostic);
#else
            message = formatString("Error in supplied FMU: 'fmiEventUpdate' at time %f iterated over %d times in block %s.", time, iterationNumber, blkPath);
            
            fmustruct->callbacks.logger(fmustruct->mFMIComp, fmustruct->fmuname, fmustruct->FMUErrorStatus, "FMU1_eventIteration", "FMUSimEventUpdateNotConverge.");
#endif
            returnStatus = fmiFalse;
            break;
            
        } else
            iterationNumber ++;
    }
    return returnStatus;
}

void FMU1_ifStateVRchanged(void **fmuv,
                           int8_T* fmustateValueOrStateValueRefChanged) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    *fmustateValueOrStateValueRefChanged = (int8_T) (fmustruct->eventInfo.stateValuesChanged || fmustruct->eventInfo.stateValueReferencesChanged);
}

void FMU1_setIterationConverged(void **fmuv,
                                int fmuIsInitialized,
                                fmiReal time) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    bool callEventUpdate;
    if(fmuIsInitialized != 0){
        fmustruct->eventInfo.iterationConverged = fmiFalse;
    } else {
        callEventUpdate = (fmustruct->eventInfo.upcomingTimeEvent == fmiTrue &&
                           fmustruct->eventInfo.nextEventTime <= time);
        fmustruct->eventInfo.iterationConverged = (callEventUpdate) ? fmiFalse : fmiTrue;
    }
}

void FMU1_getNextEventTime(void **fmuv,
                           fmiReal* nextEventTime,
                           int8_T* upcomingTimeEvent) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    *nextEventTime = fmustruct->eventInfo.nextEventTime;
    *upcomingTimeEvent = (int8_T) fmustruct->eventInfo.upcomingTimeEvent;
}

/*get set function*/
fmiBoolean FMU1ME_setRealVal(void **fmuv,
                            const fmiValueReference dvr,
                            size_t nvr,
                            const fmiReal dvalue) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiReal value = dvalue;
    fmiStatus fmiFlag = fmustruct->setReal(fmustruct->mFMIComp, &vr, nvr, &value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetReal");
}

fmiBoolean FMU1ME_setReal(void **fmuv,
                         const fmiValueReference dvr,
                         size_t nvr,
                         const fmiReal value[]) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->setReal(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetReal");
}

fmiBoolean FMU1ME_getReal(void **fmuv,
                         const fmiValueReference dvr,
                         size_t nvr,
                         fmiReal value[]) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->getReal(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiGetReal");
}

fmiBoolean FMU1ME_setIntegerVal(void **fmuv,
                               const fmiValueReference dvr,
                               size_t nvr,
                               const fmiInteger dvalue) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiInteger value = dvalue;
    fmiStatus fmiFlag = fmustruct->setInteger(fmustruct->mFMIComp, &vr, nvr, &value);
    return CheckStatus(fmustruct, fmiFlag, "fmiGetInteger");
}

fmiBoolean FMU1ME_setInteger(void **fmuv,
                            const fmiValueReference dvr,
                            size_t nvr,
                            const fmiInteger value[]) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->setInteger(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetInteger");
}

fmiBoolean FMU1ME_getInteger(void **fmuv,
                            const fmiValueReference dvr,
                            size_t nvr,
                            fmiInteger value[]) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->getInteger(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiGetInteger");
}

fmiBoolean FMU1ME_setBooleanVal(void **fmuv,
                               const fmiValueReference dvr,
                               size_t nvr,
                               const unsigned char dvalue) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiBoolean value = (fmiBoolean) dvalue;
    fmiStatus fmiFlag = fmustruct->setBoolean(fmustruct->mFMIComp, &vr, nvr, &value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetBoolean");
}

fmiBoolean FMU1ME_setBoolean(void **fmuv,
                            const fmiValueReference dvr,
                            size_t nvr,
                            const unsigned char value[]) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->setBoolean(fmustruct->mFMIComp, &vr, nvr, (fmiBoolean *) value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetBoolean");
}

fmiBoolean FMU1ME_getBoolean(void **fmuv,
                            const fmiValueReference dvr,
                            size_t nvr,
                            unsigned char value[]) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->getBoolean(fmustruct->mFMIComp, &vr, nvr, (fmiBoolean *) value);
    return CheckStatus(fmustruct, fmiFlag, "fmiGetBoolean");
}

fmiBoolean FMU1ME_setStringVal(void **fmuv,
                              const fmiValueReference dvr,
                              size_t nvr,
                              const fmiString dvalue) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiString value = dvalue;
    fmiStatus fmiFlag = fmustruct->setString(fmustruct->mFMIComp, &vr, nvr, &value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetString");
}

fmiBoolean FMU1ME_setString(void **fmuv,
                           const fmiValueReference dvr,
                           size_t nvr,
                           const fmiString value[]) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->setString(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiSetString");
}

fmiBoolean FMU1ME_getString(void **fmuv,
                           const fmiValueReference dvr,
                           size_t nvr, fmiString value[]) {
    struct FMU1_ME_RTWCG * fmustruct = (struct FMU1_ME_RTWCG *)(*fmuv);
    fmiValueReference vr =dvr;
    fmiStatus fmiFlag = fmustruct->getString(fmustruct->mFMIComp, &vr, nvr, value);
    return CheckStatus(fmustruct, fmiFlag, "fmiGetString");
}
