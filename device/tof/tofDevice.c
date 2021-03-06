#include "tofDevice.h"
#include "tofDeviceInterface.h"

#include <stdbool.h>
#include <stdlib.h>

#include "../../common/delay/cenDelay.h"

#include "../../common/error/error.h"

#include "../../common/io/inputStream.h"
#include "../../common/io/outputStream.h"
#include "../../common/io/printWriter.h"
#include "../../common/io/reader.h"

#include "../../common/timer/delayTimer.h"

#include "../../device/device.h"

#include "../../common/log/logger.h"
#include "../../common/log/logLevel.h"

#include "../../common/i2c/i2cBusConnectionList.h"

#include "../../drivers/tof/tof.h"
#include "../../drivers/tof/tofDetectionUtils.h"
#include "../../drivers/tof/tofList.h"
#include "../../drivers/tof/tofDebug.h"

// Forward declaration
TofSensorList* getTofDeviceTofSensorList(void);

// DEVICE INTERFACE

void deviceTofInit(void) {
    // MUST ALREADY HAVE BEEN INIT BY THE CONSTRUCTOR of tofList
}

void deviceTofShutDown(void) {

}

bool isTofDeviceOk(void) {
    // SVA : TODO
    return true;
}

void deviceTofHandleRawData(unsigned char commandHeader, InputStream* inputStream, OutputStream* outputStream, OutputStream* notificationOutputStream) {
    if (commandHeader == COMMAND_TOF_GET_DISTANCE) {
        ackCommand(outputStream, TOF_DEVICE_HEADER, COMMAND_TOF_GET_DISTANCE);
        unsigned char tofIndex = readHex2(inputStream);
        TofSensorList* tofSensorList = getTofDeviceTofSensorList();
        TofSensor* tofSensor = getTofSensorByIndex(tofSensorList, tofIndex);
        unsigned int distanceMM = tofSensor->tofGetDistanceMM(tofSensor);

        appendHex4(outputStream, distanceMM);
    } else if (commandHeader == COMMAND_TOF_LIST_CONFIG) {
        ackCommand(outputStream, TOF_DEVICE_HEADER, COMMAND_TOF_LIST_CONFIG);
        TofSensorList* tofSensorList = getTofDeviceTofSensorList();
        OutputStream* debugOutputStream = getInfoOutputStreamLogger();
        // We only print the config (not network neither call to detection is done)
        tofSensorList->tofSensorListConfigTableDebug(debugOutputStream, tofSensorList);
    } else if (commandHeader == COMMAND_TOF_LIST_NETWORK) {
        ackCommand(outputStream, TOF_DEVICE_HEADER, COMMAND_TOF_LIST_NETWORK);
        TofSensorList* tofSensorList = getTofDeviceTofSensorList();
        OutputStream* debugOutputStream = getInfoOutputStreamLogger();
        // We only print the network (not config neither call to detection is done)
        tofSensorList->tofSensorListNetworkTableDebug(debugOutputStream, tofSensorList);
    } else if (commandHeader == COMMAND_TOF_LIST_DETECTED) {
        ackCommand(outputStream, TOF_DEVICE_HEADER, COMMAND_TOF_LIST_DETECTED);
        TofSensorList* tofSensorList = getTofDeviceTofSensorList();
        OutputStream* debugOutputStream = getInfoOutputStreamLogger();
        // We only print the network (not config neither call to detection is done)
        tofSensorList->tofSensorListDetectionTableDebug(debugOutputStream, tofSensorList, NULL, 0.0f);
    } else if (commandHeader == COMMAND_TOF_BEEP_ON) {
        ackCommand(outputStream, TOF_DEVICE_HEADER, COMMAND_TOF_BEEP_ON);
        TofSensorList* tofSensorList = getTofDeviceTofSensorList();
        tofSensorList->beepLocked = true;
        tofSensorListBeepOverrideLock(tofSensorList, true);
    } else if (commandHeader == COMMAND_TOF_BEEP_OFF) {
        ackCommand(outputStream, TOF_DEVICE_HEADER, COMMAND_TOF_BEEP_OFF);
        TofSensorList* tofSensorList = getTofDeviceTofSensorList();
        tofSensorList->beepLocked = true;
        tofSensorListBeepOverrideLock(tofSensorList, false);
    } else if (commandHeader == COMMAND_TOF_SEARCH_IF_COLLIDING) {
        // TO : Place it in tof !
        ackCommand(outputStream, TOF_DEVICE_HEADER, COMMAND_TOF_SEARCH_IF_COLLIDING);
        TofSensorList* tofSensorList = getTofDeviceTofSensorList();

        tofSensorList->beepLocked = true;

        unsigned int tofIndex = readHex2(inputStream);
        checkIsSeparator(inputStream);
        unsigned int durationSeconds = readHex2(inputStream);
        unsigned int startTofIndex = 0;
        unsigned int endTofIndex = getTofSensorListSize(tofSensorList) - 1;
        if (tofIndex != 0xFF) {
            TofSensor* tofSensor = getTofSensorByIndex(tofSensorList, tofIndex);
            if (tofSensor == NULL) {
                return;
            }
            startTofIndex = tofIndex;
            endTofIndex = tofIndex;
        }
        Timer* timerDelay = timerDelayMark();
        while (!timerDelayTimeout(timerDelay, 1000 * durationSeconds)) {
            bool detected = false;
            for (tofIndex = startTofIndex; tofIndex <= endTofIndex; tofIndex++) {
                TofSensor* tofSensor = getTofSensorByIndex(tofSensorList, tofIndex);
                if (!tofSensor->enabled) {
                    continue;
                }
                tofSensor->tofGetDistanceMM(tofSensor);
                if (isTofDistanceInRange(tofSensor)) {
                    detected = true;
                    break;
                }
            }
            tofSensorListBeepOverrideLock(tofSensorList, detected);
        }
        // Off the beep at the end
        tofSensorListBeepOverrideLock(tofSensorList, false);

        tofSensorList->beepLocked = false;
    } else if (commandHeader == COMMAND_TOF_RESTART) {
        ackCommand(outputStream, TOF_DEVICE_HEADER, COMMAND_TOF_RESTART);
        unsigned char tofIndex = readHex2(inputStream);
        TofSensorList* tofSensorList = getTofDeviceTofSensorList();
        TofSensor* tofSensor = getTofSensorByIndex(tofSensorList, tofIndex);

        bool restartSuccess = tofRestart(tofSensor);
        appendBool(outputStream, restartSuccess);
    } else if (commandHeader == COMMAND_TOF_LED_SHOW) {
        ackCommand(outputStream, TOF_DEVICE_HEADER, COMMAND_TOF_LED_SHOW);
        TofSensorList* tofSensorList = getTofDeviceTofSensorList();

        LedArray* ledArray0 = tofSensorList->ledArray0;
        LedArray* ledArray1 = tofSensorList->ledArray1;
        if (ledArray0 != NULL) {
            int count = ledArray0->ledCount;
            int i;
            for (i = 0; i < count; i++) {
                ledArray0->ledArrayWriteValue(ledArray0, i, 0x00, 0xFF, 0x00);
            }
        }
        else {
            writeError(LED_ARRAY_NULL);
        }
        if (ledArray1 != NULL) {
            int count = ledArray1->ledCount;
            int i;
            for (i = 0; i < count; i++) {
                ledArray1->ledArrayWriteValue(ledArray1, i, 0x00, 0xFF, 0x00);
            }
        }
        else {
            writeError(LED_ARRAY_NULL);
        }
    } else if (commandHeader == COMMAND_TOF_LED_HIDE) {
        ackCommand(outputStream, TOF_DEVICE_HEADER, COMMAND_TOF_LED_HIDE);
        TofSensorList* tofSensorList = getTofDeviceTofSensorList();

        LedArray* ledArray0 = tofSensorList->ledArray0;
        LedArray* ledArray1 = tofSensorList->ledArray1;
        if (ledArray0 != NULL) {
            int count = ledArray0->ledCount;
            int i;
            for (i = 0; i < count; i++) {
                ledArray0->ledArrayWriteValue(ledArray0, i, 0x00, 0x00, 0x00);
            }
        }
        if (ledArray1 != NULL) {
            int count = ledArray1->ledCount;
            int i;
            for (i = 0; i < count; i++) {
                ledArray1->ledArrayWriteValue(ledArray1, i, 0x00, 0x00, 0x00);
            }
        }
    }
}

static DeviceDescriptor descriptor;

TofSensorList* getTofDeviceTofSensorList(void) {
    return (TofSensorList*) descriptor.object;
}

DeviceDescriptor* getTofDeviceDescriptor(TofSensorList* tofSensorListParam) {
    initDeviceDescriptor(&descriptor,
            &deviceTofInit,
            &deviceTofShutDown,
            &isTofDeviceOk,
            &deviceTofHandleRawData,
            (int*) tofSensorListParam);

    return &descriptor;
}

