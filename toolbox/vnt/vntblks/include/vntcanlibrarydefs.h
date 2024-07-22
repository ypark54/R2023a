/**
* @file: vntcanlibrarydefs.h
* Purpose: Exports C functions used to to access vendor APIs.
* Copyright: 2010-2017 The MathWorks, Inc.
*/

#ifndef vntcanlibrarydefs_header
#define vntcanlibrarydefs_header

namespace vntcansl{

    namespace vendor {

        /**
        * Open driver and device connection and set required configurations for CAN.
        */
        const char* const VENDOR_OPEN_FCN = "exSLVendorOpen";
        typedef void (*VENDOR_OPEN_FCN_PTR)(const char* const deviceName, 
                                       int& serialNumber,
                                       int deviceCode, 
                                       int& deviceIndex, 
                                       int deviceChannelIndex, 
                                       double& accessMask, 
                                       double& portHandle, 
                                       double& permissionMask, 
                                       double busSpeed, int sjw, int tseg1, int tseg2, int numSamples, 
                                       int silentMode,
                                       double blockType);

        /** 
        * Open driver and device connection and set required configurations for CAN FD.
        */
        const char* const VENDOR_OPEN_FD_FCN = "exSLVendorOpenFD";
        typedef void (*VENDOR_OPEN_FD_FCN_PTR)(const char* const deviceName, 
                                       int& serialNumber,
                                       int deviceCode, 
                                       int& deviceIndex, 
                                       int deviceChannelIndex, 
                                       double& accessMask, 
                                       double& portHandle, 
                                       double& permissionMask, 
                                       double arbitrationBusSpeed, int arbitrationSJW, int arbitrationTSEG1, int arbitrationTSEG2,
                                       double dataBusSpeed, int dataSJW, int dataTSEG1, int dataTSEG2,
                                       int busFrequency, int arbitrationPrescalar, int dataPrescaler,
                                       int silentMode,
                                       double blockType);

        /** 
        * Close driver and device connection.
        */
        const char* const VENDOR_CLOSE_FCN = "exSLVendorClose";
        typedef void (*VENDOR_CLOSE_FCN_PTR)(double portHandle, double accessMask);

        /** 
        * Set the driver-level message filtering configuration.
        */
        const char* const VENDOR_SETIDS_FCN = "exSLVendorSetIDsFilter";
        typedef void (*VENDOR_SETIDS_FCN_PTR)(double portHandle, double accessMask, 
                                              int enableStdIDFilter, int enableExtIDFilter, 
                                              double* stdIDsStart, double* stdIDsEnd, 
                                              int lengthStdIDsRange, int* idMaskParams);
    }
}

#endif
