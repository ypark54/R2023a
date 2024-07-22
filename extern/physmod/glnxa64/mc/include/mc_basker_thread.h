#ifndef __mc_basker_thread_h__
#define __mc_basker_thread_h__
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
typedef void*(*ThreadFunc)(void*);typedef struct ThreadFuncArgTag{void*mArg;
size_t mId;}ThreadFuncArg;typedef struct McBaskerThreadTag McBaskerThread;
typedef struct McBaskerThreadMethodsTag{McBaskerThread*(*mCreateThreadMethod)(
ThreadFunc fcn,size_t n);int(*mRunThreadMethod)(McBaskerThread*tm,void*arg);
void(*mDestroyThreadMethod)(McBaskerThread*tm);}McBaskerThreadMethods;typedef
struct McThreadBarrierTag McThreadBarrier;void mc_register_thread_support(
McBaskerThreadMethods*methods);McBaskerThreadMethods*mc_get_thread_support(
void);McThreadBarrier*mc_create_thread_barrier(size_t numThread);void
mc_destroy_thread_barrier(McThreadBarrier*bb);void mc_thread_barrier(
McThreadBarrier*bb,size_t id);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __mc_basker_thread_h__ */
