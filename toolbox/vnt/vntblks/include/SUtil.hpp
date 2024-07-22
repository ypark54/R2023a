/**
 * @file: SUtil.hpp
 * Purpose: Several C++ S-Function utilities.
 * Copyright 2017-2022 The MathWorks, Inc.
 *
 * Note that this file a copy from DSP. It will be refactored into a shared testmeas 
 * Simulink component at a later time.
 *
 */

#ifndef SUTIL_HPP
#define SUTIL_HPP

#include <string.h>
#include "cg_ir/builder/FcnBuilder.hpp"

// Move the following somewhere else
static const int MAX_STRING_PARAMETER_LENGTH=1024;

#if !defined(MAX_ERR_MSG_LEN)
#define MAX_ERR_MSG_LEN 1024
#endif

// Include the following for header for code generation
#include "dspcg/DSPCGCoreCtx.hpp"

// Everything should live in the SUtil namespace
namespace SUtil {

	/** If we are being called for external simulation, we should not do anything 
	 * in the start routine.  If we don't do this, there will be device contention 
	 * between the processes, since Simulink mdlStart calls are not always paired 
	 * with mdlTerminate.
	 */
	inline bool executeStartRoutine(SimStruct *S)
	{
#ifndef COMPILE_FOR_SEA
        // It would be more efficient to use the same code that we are using for System  
        // objects, but it is too late in the R2011a release to do so -arogers 9/15/2010
        return !ssIsExternalSim(S);
#else
        return ssGetSimMode(S)==SS_SIMMODE_NORMAL;
#endif
	}
	
	// A param getter that must be explicitly specialized
    template<typename T>
    T getParam(SimStruct *S, int param);
    
    template<>
    double getParam<double>(SimStruct *S, int param) {
        const mxArray *tmp = ssGetSFcnParam(S, param);
        return mxGetScalar(tmp);
    }
    template<>
    int_T getParam<int_T>(SimStruct *S, int param) {
        const mxArray *tmp = ssGetSFcnParam(S, param);
        return static_cast<int_T>(mxGetScalar(tmp));
    }
    template<>
    std::string getParam<std::string>(SimStruct *S, int param) {
        char str[MAX_STRING_PARAMETER_LENGTH];
        const mxArray *tmp = ssGetSFcnParam(S, param);
        mxGetString(tmp, str, MAX_STRING_PARAMETER_LENGTH);
        return std::string(str);
    }
    template<>
    bool getParam<bool>(SimStruct *S, int param) {
        const mxArray *tmp = ssGetSFcnParam(S, param);
        return static_cast<bool>(mxGetScalar(tmp));
    }
    
    // A utility for creating DWORKS based on DWORK traits.
#define CREATE_DWORK(S, arg)                                        \
    ssSetDWorkName(S,     (int)arg, SUtil::DWork<arg>::DWORK_NAME); \
    ssSetDWorkDataType(S, (int)arg, SUtil::DWork<arg>::DWORK_TYPE); \
    ssSetDWorkWidth(S,    (int)arg, SUtil::DWork<arg>::DWORK_SIZE); 
  
    // typesafe min and max.
    template<class T>
    T Max(T a, T b) {
        return a>b?a:b;
    }
    template<class T>
    T Min(T a, T b) {
        return a<b?a:b;
    }
    
    class RTWCG_Base : public DSPCG::CoreCtx
    {
      protected:
        RTWCG::CGIR_Block_IF *mRtwBlock;
        SimStruct* mS;
        
        RTWCG_Base(SimStruct* S,
                   void *ptr,
                   fxpModeRounding round = FXP_ROUND_FLOOR,
                   fxpModeOverflow overflow = FXP_OVERFLOW_WRAP)
            : DSPCG::CoreCtx(ptr, round, overflow), mRtwBlock((RTWCG::CGIR_Block_IF *)ptr), mS(S)
        {}
        ~RTWCG_Base()
        {}
        
        CG::Fcn* externFcn(const std::string &name,
                           unsigned int numInputs = 0, unsigned int numOutputs = 0,
                           CG::Type *inputTypes[] = 0, CG::Type *outputTypes[] = 0,
                           const std::string *inputNames = 0, const std::string *outputNames = 0) {
            CG::Array<CG::Type*> inputTypesArr(pool(), numInputs);
            CG::Array<const char*> inputNamesArr(pool(), numInputs);
            for(unsigned int i=0; i<numInputs; i++) {
                inputTypesArr.set(i, inputTypes[i]);
                inputNamesArr.set(i, context()->cgString(inputNames[i].c_str()) );
            }
            CG::Array<CG::Type*> outputTypesArr(pool(), numOutputs);
            CG::Array<const char*> outputNamesArr(pool(), numOutputs);
            for(unsigned int i=0; i<numOutputs; i++) {
                outputTypesArr.set(i, outputTypes[i]);
                outputNamesArr.set(i, context()->cgString(outputNames[i].c_str()) );
            }
            CG::Fcn *tempFcn = DSPCG::CoreCtx::externFcn(name.c_str(), &inputTypesArr, &inputNamesArr, &outputTypesArr, &outputNamesArr);
            return tempFcn;
        }

        CG::VoidType* voidT() const {return typeFactory()->voidType();}
        CG::StringType* stringT() const {return typeFactory()->stringType();}
        CG::IntegerType* sint8T() const {return typeFactory()->signedInt8Type();}
        CG::IntegerType* uint8T() const {return typeFactory()->unsignedInt8Type();}
        CG::CharType* charT() const {return typeFactory()->charType();}
        
        CG::Fcn* createTlcFcn(const std::string& name, CG::Type* outType=0, CG::Type* inType1=0, CG::Type* inType2=0) {
            CG::FcnBuilder fcnPtr(context()->globalScope(), name);
            if(outType) {
                cg_fcn_create_add_output(&fcnPtr, 0, pointerT(outType));
            }
            if(inType1) {
                cg_fcn_create_add_input(&fcnPtr, 0, inType1);
            }
            if(inType2) {
                cg_fcn_create_add_input(&fcnPtr, 0, inType2);
            }
            auto newFcn = fcnPtr.createClassic();
            newFcn->setIsTlc(true);
            return newFcn;
        }
        
        /// Emit a warning if the message parameter is non-null
        void checkWarning(const DSPCG::ConstructionFacade::IdxRef &sErr) {
            // This is similar to checkError, but we don't call RTMSetStopRequested
            IdxVal zero = idxConst(0, sint32T());
            IdxVal sErrVal = deref(derefToRef(sErr));
            beginIf(sErrVal != zero, FL); {
#ifdef COMPILE_FOR_SEA
                //void PrintError(char * error);
                const int numInputs  = 1;
                const int numOutputs = 0;
                CG::Type* inputValues[numInputs];
                std::string inputNames[numInputs];
                inputValues[0] = pointerT(charT());
                CG::Fcn *cFcn = externFcn("PrintWarning", numInputs, numOutputs, inputValues, 0, inputNames, 0);
                append(callNumVal(cFcn, sErr), FL);
#else
                // %<RTMSetErrStat("sErr")>;
                CG::Fcn* tlcFcn =
                    targetFcn<RTWCG::target::FcnId::SetErrorStatus>();
                append(callIdxVal(tlcFcn, sErr), FL);
#endif
                append(assign(sErr, zero), FL);
            } endIf(FL);
        }
        /// Emit an error if the message parameter is non-null.
        void checkError(const DSPCG::ConstructionFacade::IdxRef &sErr) {
            IdxVal zero = idxConst(0, sint32T());
            IdxVal sErrVal = deref(derefToRef(sErr));
            beginIf(sErrVal != zero, FL); {
#ifdef COMPILE_FOR_SEA
                //void PrintError(char * error);
                const int numInputs  = 1;
                const int numOutputs = 0;
                CG::Type* inputValues[numInputs];
                std::string inputNames[numInputs];
                inputValues[0] = pointerT(charT());
                CG::Fcn *cFcn = externFcn("PrintError", numInputs, numOutputs, inputValues, 0, inputNames, 0);
                append(callNumVal(cFcn, sErr), FL);
#else
                // %<RTMSetErrStat("sErr")>;
                CG::Fcn* tlcFcn =
                    targetFcn<RTWCG::target::FcnId::SetErrorStatus>();
                append(callIdxVal(tlcFcn, sErr), FL);
                
                // %<RTMSetStopRequested("1")>;
                tlcFcn = targetFcn<RTWCG::target::FcnId::SetStopRequested>();
                IdxVal arg = idxConst(1,tlcIntegerT());
                append(callIdxVal(tlcFcn, arg), FL);
#endif
            } endIf(FL);
        }
        /// Create an error buffer, and initialize the pointer with it.
        IdxRef getErrorBuffer(const DSPCG::ConstructionFacade::IdxVal &hostLib) {
            IdxRef sErr = localIdxVar(pointerT(charT()), "sErr");

            // We make the following call to get a static character array:
            //char * GetErrorBuffer();
            
            const int numInputs  = 1;
            const int numOutputs = 1;
            CG::Type* inputValues[numInputs];
            std::string inputNames[numInputs];
            inputValues[0] = pointerT(voidT());
            CG::Type* outputValues[numOutputs];
            std::string outputNames[numOutputs];
            outputValues[0] = pointerT(charT());
            CG::Fcn *rslt = externFcn("GetErrorBuffer", numInputs, numOutputs, inputValues, outputValues, inputNames, outputNames);
            // Mark the function as global independent -- The function does not read/write any globals
            // represented in CGIR
            rslt->setReferencesOnlyUnmodeledGlobals(true);
            append(assign(sErr, callIdxVal(rslt, hostLib)), FL);
            return sErr;
        }

        NumRef getNullPtr(const char *name) {
            NumRef nil = localNumVar(pointerT(voidT()), name);
#if 0
            // The following code is not currently working.
            //append(nil = nodeFactory()->nilExpr(), FL);
            //append(nil = IdxVal(*nodeFactory()->nilExpr(),*this), FL);
#else
            // We make the following call to get a null pointer:
            //void * GetNullPtr();
            const int numInputs  = 0;
            const int numOutputs = 1;
            CG::Type* outputValues[numOutputs];
            std::string outputNames[numOutputs];
            outputValues[0] = pointerT(voidT());
            CG::Fcn *rslt = externFcn("GetNullPointer", numInputs, numOutputs, NULL, outputValues, NULL, outputNames);
            // dserr: this API is temporary, do not copy. Contact me if you have any questions
            CG::EffectTools::markPure(rslt);
            append(assign(nil, callNumVal(rslt)), FL);
#endif
            return nil;
        }
        
        IdxRef createString(const char *str, const char *varName) {
            IdxRef localVar = localIdxVar(stringT(), varName);
            CG::Const *temp = CG::Const::cast(getStringConst(str));
            IdxVal var(*constE(temp), *this);
            append(assign(localVar, var), FL);
            return localVar;
        }
        /// Add to the Model Sources
        void addToModelSources(const char *filename) {
            std::string fcnName("LibAddToModelSources");
            CG::Fcn* tlcFcn = createTlcFcn(fcnName, voidT(), stringT());
            IdxRef arg = createString(filename, "filename");
            append(callNumRef(tlcFcn, arg), FL);
        }
        
        // create the CGIR representation of a function
        virtual CG::Fcn * getHostFcn(const char * name) {
            CG::Fcn *rslt = NULL;
            // Common functions
            if(!strcmp(name, "CreateHostLibrary")) {
                //void CreateHostLibrary(HostLibrary *adl, char *err);
                const int numInputs  = 2;
                const int numOutputs = 0;
                CG::Type* inputValues[numInputs];
                std::string inputNames[numInputs];
                inputValues[0] = stringT();
                inputValues[1] = pointerT(voidT());
                rslt = externFcn(name, numInputs, numOutputs, inputValues, 0, inputNames, 0);
            }
            else if(!strcmp(name, "LibStart")) {
                //void LibStart(HostLibrary * hostLib);
                const int numInputs  = 1;
                const int numOutputs = 0;
                CG::Type* inputValues[numInputs];
                std::string inputNames[numInputs];
                inputValues[0] = pointerT(voidT());
                rslt = externFcn(name, numInputs, numOutputs, inputValues, 0, inputNames, 0);
            }
            else if(!strcmp(name, "LibReset")) {
                //void LibReset(HostLibrary * hostLib);
                const int numInputs  = 1;
                const int numOutputs = 0;
                CG::Type* inputValues[numInputs];
                std::string inputNames[numInputs];
                inputValues[0] = pointerT(voidT());
                rslt = externFcn(name, numInputs, numOutputs, inputValues, 0, inputNames, 0);
            }
            else if(!strcmp(name, "LibTerminate")) {
                //void LibTerminate(HostLibrary * hostLib);
                const int numInputs  = 1;
                const int numOutputs = 0;
                CG::Type* inputValues[numInputs];
                std::string inputNames[numInputs];
                inputValues[0] = pointerT(voidT());
                rslt = externFcn(name, numInputs, numOutputs, inputValues, 0, inputNames, 0);
            }
            else if(!strcmp(name, "LibDestroy")) {
                //void LibDestroy(HostLibrary * hostLib, int type);
                const int numInputs  = 2;
                const int numOutputs = 0;
                CG::Type* inputValues[numInputs];
                std::string inputNames[numInputs];
                inputValues[0] = pointerT(voidT());
                inputValues[1] = sint32T();
                rslt = externFcn(name, numInputs, numOutputs, inputValues, 0, inputNames, 0);
            }
            else if(!strcmp(name, "DestroyHostLibrary")) {
                //void DestroyHostLibrary(HostLibrary *adl);
                const int numInputs  = 1;
                const int numOutputs = 0;
                CG::Type* inputValues[numInputs];
                std::string inputNames[numInputs];
                inputValues[0] = pointerT(voidT());
                rslt = externFcn(name, numInputs, numOutputs, inputValues, 0, inputNames, 0);
            }
            
            if (rslt) {
                // Mark the function as global independent -- The function does not read/write any globals
                // represented in CGIR
                rslt->setReferencesOnlyUnmodeledGlobals(true);
            }
            return rslt;
        }
    };
    
} //namespace SUtil

#endif // SUTIL_HPP
