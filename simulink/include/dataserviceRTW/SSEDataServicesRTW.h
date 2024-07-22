/* Copyright 2020-2022 The MathWorks, Inc. */
#ifdef SUPPORTS_PRAGMA_ONCE
#pragma once
#endif

#ifndef SSEDATASERVICESRTW_H
#define SSEDATASERVICESRTW_H

#include "tmwtypes.h"
#include "SSEDataServicesRTW_spec.h"

/*
 * These functions act as a bridge to call the c-api of liveio from rapid accel target.
 * The call from c to c++ takes place here, 'Codegen' postfixed files in live/core
 * convert the c parameters to c++ objects and call the library functions.
 */

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C int sse_dataservice_RegisterDataReader(const char* dtName,
                                                                            const char* typeName);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C void sse_dataservice_UnregisterDataReader(int uniqueID);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C int sse_dataservice_GetAgentID(int localID);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C int sse_dataservice_RegisterDataWriter(const char* dtName,
                                                                            const char* typeName,
                                                                            const char* icValues);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C void sse_dataservice_UnregisterDataWriter(int uniqueID);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C void sse_dataservice_SendWriteQuery(int uniqueID, char* query);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C char* sse_dataservice_SendReadQuery(int uniqueID, char* query);

/*---------------------------------------------------------------------------*/

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
uint64_T sseReadActorId(const char* modelName);

/*------------------------- SysObj Actor Functions --------------------------*/

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
void sseInitSysObjActorDataSvc(uint64_T actorId);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
void sseGetAttributePose(double* outX, double* outY, double* outZ);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
void sseSetAttributePose(const double x, const double y, const double z);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
void sseGetAttributeVelocity(double* outX, double* outY, double* outZ);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
void sseSetAttributeVelocity(const double x, const double y, const double z);

/*---------------------- RRScenario Block Functions -------------------------*/

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
void sseLoadActorID(char* modelName);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
void sseRegisterReaderBusType(char* busType);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
void sseRegisterWriterBusType(char* busType);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
bool sseRegisterModel();

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
void sseResetStep();

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
uint64_T sseGetActorID();

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
bool sseSendMessage(void* msgData, char* msgType);

SSE_DATASERVICES_RTW_EXPORT_EXTERN_C
void sseReadMessage(char* msgType, void* msgData);

#endif
