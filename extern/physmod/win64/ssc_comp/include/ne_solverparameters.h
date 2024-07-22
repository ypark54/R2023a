#ifndef __ne_solverparameters_h__
#define __ne_solverparameters_h__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
typedef struct NeSolverParametersTag NeSolverParameters;typedef enum
NeLocalSolverChoiceTag{NE_INVALID_ADVANCER_CHOICE= -1,
NE_BACKWARD_EULER_ADVANCER,NE_TRAPEZOIDAL_ADVANCER,NE_NDF2_ADVANCER,
NE_VARIABLE_STEP_ADVANCER,NE_PARTITIONING_ADVANCER,NE_NUM_ADVANCER_CHOICES}
NeLocalSolverChoice;typedef enum NeIndexReductionMethodTag{
NE_INVALID_IR_METHOD= -1,NE_NONE_IR,NE_DERIVATIVE_REPLACEMENT_IR,
NE_PROJECTION_IR,NE_NUM_IR_METHOD}NeIndexReductionMethod;typedef enum
NeLinearAlgebraChoiceTag{NE_INVALID_LA_CHOICE= -1,NE_AUTO_LA,NE_SPARSE_LA,
NE_FULL_LA,NE_NUM_LA_CHOICES}NeLinearAlgebraChoice;typedef enum
NeEquationFormulationChoiceTag{NE_INVALID_EF_CHOICE= -1,NE_TIME_EF,
NE_FREQUENCY_TIME_EF,NE_NUM_EF_CHOICES}NeEquationFormulationChoice;typedef enum
NePartitionStorageMethodTag{NE_PARTITIONING_INVALID= -1,
NE_PARTITIONING_AS_NEEDED,NE_PARTITIONING_EXHAUSTIVE,NE_PARTITIONING_NUM}
NePartitionStorageMethod;typedef enum NePartitionMethodTag{
NE_PARTITIONING_METHOD_INVALID= -1,NE_PARTITIONING_METHOD_ROBUST,
NE_PARTITIONING_METHOD_FAST,NE_PARTITIONING_METHOD_ALG_EQS,
NE_PARTITIONING_METHOD_ALL_EQS,NE_PARTITIONING_METHOD_MERGE,
NE_PARTITIONING_METHOD_MULTICORE,NE_PARTITIONING_METHOD_NUM}NePartitionMethod;
typedef enum NeConsistencySolverTag{NE_CONSISTENCY_SOLVER_INVALID= -1,
NE_CS_NEWTON_FTOL,NE_CS_NEWTON_XTOL,NE_CS_NEWTON_XTOL_AFTER_FTOL,
NE_CONSISTENCY_SOLVER_NUM}NeConsistencySolver;typedef enum NeToleranceSourceTag
{NE_TOLERANCE_SOURCE_INVALID= -1,NE_TOLERANCE_SOURCE_LOCAL,
NE_TOLERANCE_SOURCE_GLOBAL,NE_TOLERANCE_SOURCE_NUM}NeToleranceSource;typedef
enum NeMBLocalSolverChoiceTag{NE_MB_LOCAL_SOLVER_INVALID= -1,
NE_MB_LOCAL_SOLVER_ODE1,NE_MB_LOCAL_SOLVER_ODE2,NE_MB_LOCAL_SOLVER_ODE3,
NE_MB_LOCAL_SOLVER_ODE4,NE_MB_LOCAL_SOLVER_ODE5,NE_MB_LOCAL_SOLVER_ODE8,
NE_MB_LOCAL_SOLVER_NUM}NeMBLocalSolverChoice;struct NeSolverParametersTag{
boolean_T mMBUseLocalSolver;NeMBLocalSolverChoice mMBLocalSolverChoice;real_T
mMBLocalSolverSampleTime;boolean_T mProfile;boolean_T mUseLocalSampling;
boolean_T mEnableSwitchedLinearOptims;boolean_T mFrequencyDomain;boolean_T
mUseCCode;real_T mRelTol;real_T mAbsTol;real_T mMinStep;boolean_T mToWorkspace
;boolean_T mRevertToSquareIcSolve;size_t mNumHomotopyIterations;boolean_T
mPhasorMode;size_t mPhasorModeNumHarmonics;NeConsistencySolver
mConsistencySolver;NeIndexReductionMethod mIndexReductionMethod;boolean_T mDoDC
;real_T mResidualTolerance;NeToleranceSource mConsistencyTolSource;real_T
mConsistencyAbsTol;real_T mConsistencyRelTol;real_T mConsistencyTolFactor;
boolean_T mUseLocalSolver;NeLocalSolverChoice mLocalSolverChoice;boolean_T
mResolveIndetEquations;size_t mFunctionEvalNumThread;real_T
mLocalSolverSampleTime;boolean_T mDoFixedCost;size_t mMaxNonlinIter;size_t
mMaxModeIter;boolean_T mComputeImpulses;size_t mImpulseIterations;
NeLinearAlgebraChoice mLinearAlgebra;size_t mLinearAlgebraNumThread;
NeEquationFormulationChoice mEquationFormulation;size_t mDelaysMemoryBudget;
boolean_T mAutomaticFiltering;real_T mFilteringTimeConstant;
NePartitionStorageMethod mPartitionStorageMethod;size_t mPartitionMemoryBudget
;NePartitionMethod mPartitionMethod;};
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __ne_solverparameters_h__ */
