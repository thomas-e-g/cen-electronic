#include "endMatchDetectorDevice.h"
#include "endMatchDetectorDeviceInterface.h"

#include "../../common/commons.h"
#include "../../common/timer/cenTimer.h"
#include "../../common/timer/timerList.h"

#include "../../common/io/inputStream.h"
#include "../../common/io/outputStream.h"
#include "../../common/io/printWriter.h"
#include "../../common/io/stream.h"

#include "../../common/log/logLevel.h"
#include "../../common/log/logger.h"

#include "../../device/device.h"

#include "../../robot/config/robotConfigDevice.h"

static int matchStarted = FALSE;

/** Current Time In Second. */
static int currentTimeInSecond = 0;

/** Flag to know if the robot must stop at the end of the match. */
static int doNotEnd = 0;

/** The timer struct to detect the end of the match. */
static Timer endMatchDetectorDeviceTimer;

// SPECIFIC PART

void endMatchDetectorCallbackFunc(void) {
    if (matchStarted) {
        currentTimeInSecond++;
    }
}

/**
* As there is a problem during init to load the config.
* We send it after
*/
void initEndMatchConfig() {
	doNotEnd = isConfigSet(CONFIG_DO_NOT_END);
}

void markStartMatch() {
	initEndMatchConfig();
    matchStarted = TRUE;
    currentTimeInSecond = 0;
}

void showEnd(OutputStream* outputStream) {
    appendString(outputStream, "End of match");
}

int getCurrentTimeInSecond(void) {
    return currentTimeInSecond;
}

BOOL isEnd() {
    if (doNotEnd) {
        return FALSE;
    }
    if (!matchStarted) {
        appendString(getOutputStreamLogger(ERROR), "You must call startMatch before");
    }
    return currentTimeInSecond >= MATCH_DURATION;
}

// COMMON PART

void initEndMatchDetector() {
    initTimer(&endMatchDetectorDeviceTimer,
            END_MATCH_DETECTOR_TIMER_CODE,
            TIME_DIVISER_1_HERTZ,
            endMatchDetectorCallbackFunc);
    addTimer(&endMatchDetectorDeviceTimer);
}


void stopEndMatchDetector() {

}

unsigned int isEndMatchDetectorDeviceOk() {
    return TRUE;
}

void deviceEndMatchDetectorHandleRawData(char header,
        InputStream* inputStream,
        OutputStream* outputStream) {
}

// allocate memory
static DeviceDescriptor endMatchDetectorDevice = {
    .deviceInit = &initEndMatchDetector,
    .deviceShutDown = &stopEndMatchDetector,
    .deviceIsOk = &isEndMatchDetectorDeviceOk,
    .deviceHandleRawData = &deviceEndMatchDetectorHandleRawData,
};

DeviceDescriptor* getEndMatchDetectorDeviceDescriptor() {
    return &endMatchDetectorDevice;
}