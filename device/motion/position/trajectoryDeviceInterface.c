#include <stdbool.h>

#include "trajectoryDeviceInterface.h"

#include "../../../device/device.h"
#include "../../../device/deviceInterface.h"
#include "../../../device/deviceConstants.h"

#define GET_ABS_POS_STRING             "get Absolute Position"
#define GET_DEBUG_ABS_POS_STRING       "get Debug Absolute Position"
#define SET_ABS_POS_STRING             "set Absolute Position"
#define ANGLE_1_10_DEG                 "angle (deci degree)"
#define X_MM                           "x (microMM)"
#define Y_MM                           "y (microMM)"

const char* getTrajectoryDeviceName(void) {
    return "Trajectory";
}

int trajectoryGetInterface(unsigned char commandHeader, DeviceInterfaceMode mode, bool fillDeviceArgumentList) {
    if (commandHeader == COMMAND_TRAJECTORY_GET_ABSOLUTE_POSITION) {
        if (fillDeviceArgumentList) {
            setFunction(GET_ABS_POS_STRING, 0, 5);
            setResultFloatHex4(0, X_MM);
            setResultSeparator(1);
            setResultFloatHex4(2, Y_MM);
            setResultSeparator(3);
            setResultUnsignedHex4(4, ANGLE_1_10_DEG);
        }
        return commandLengthValueForMode(mode, 0, 14);
    } else if (commandHeader == COMMAND_TRAJECTORY_DEBUG_GET_ABSOLUTE_POSITION) {
        // Same return in case of input / output
        if (fillDeviceArgumentList) {
            setFunctionNoArgumentAndNoResult(GET_DEBUG_ABS_POS_STRING);
        }
        return commandLengthValueForMode(mode, 0, 0);
    } else if (commandHeader == COMMAND_TRAJECTORY_DEBUG_CODERS) {
        // Same return in case of input / output
        if (fillDeviceArgumentList) {
            setFunctionNoArgumentAndNoResult("Debug Coders History");
        }
        return commandLengthValueForMode(mode, 0, 0);
    } else if (commandHeader == COMMAND_TRAJECTORY_SET_ABSOLUTE_POSITION) {
        if (fillDeviceArgumentList) {
            setFunction(SET_ABS_POS_STRING, 5, 0);
            setArgumentFloatHex6(0, X_MM);
            setArgumentSeparator(1);
            setArgumentFloatHex6(2, Y_MM);
            setArgumentSeparator(3);
            setArgumentUnsignedHex4(4, ANGLE_1_10_DEG);
        }
        return commandLengthValueForMode(mode, 18, 0);
    } else if (commandHeader == COMMAND_TRAJECTORY_CLEAR_ABSOLUTE_POSITION) {
        if (fillDeviceArgumentList) {
            setFunctionNoArgumentAndNoResult("Pos(x=0, y=0, angle=0)");
        }
        return commandLengthValueForMode(mode, 0, 0);
    } else if (commandHeader == COMMAND_TRAJECTORY_ADJUST_X) {
        if (fillDeviceArgumentList) {
            setFunction("Adjust X", 1, 1);
            setArgumentFloatHex6(0, X_MM);
            setResultUnsignedChar1(0, "Done or not");
        }
        return commandLengthValueForMode(mode, 6, 1);
    } else if (commandHeader == COMMAND_TRAJECTORY_ADJUST_Y) {
        if (fillDeviceArgumentList) {
            setFunction("Adjust Y", 1, 1);
            setArgumentFloatHex6(0, X_MM);
            setResultUnsignedChar1(0, "Done or not");
        }
        return commandLengthValueForMode(mode, 6, 1);
    }// NOTIFY PARAMETERS
    else if (commandHeader == COMMAND_TRAJECTORY_NOTIFY_OFF) {
        if (fillDeviceArgumentList) {
            setFunctionNoArgumentAndNoResult("Notify Off");
        }
        return commandLengthValueForMode(mode, 0, 0);
    } else if (commandHeader == COMMAND_TRAJECTORY_NOTIFY_ON) {
        if (fillDeviceArgumentList) {
            setFunctionNoArgumentAndNoResult("Notify On");
        }
        return commandLengthValueForMode(mode, 0, 0);
    } else if (commandHeader == COMMAND_TRAJECTORY_NOTIFY_SET_PARAMETERS) {
        if (fillDeviceArgumentList) {
            setFunction("Notify Set Param", 3, 0);
            setArgumentFloatHex4(0, "Dist (mm)");
            setArgumentSeparator(1);
            setArgumentFloatHex4(2, "Angle (deciDegree)");
        }
        return commandLengthValueForMode(mode, 9, 0);
    }
    // NOTIFICATION
    if (DEVICE_MODE_NOTIFY == mode) {
        if (commandHeader == NOTIFY_TRAJECTORY_CHANGED) {
            if (fillDeviceArgumentList) {
                setNotification("Trajectory Changed", 7);
                setArgumentUnsignedHex4(0, "x(mm)");
                setArgumentSeparator(1);
                setArgumentUnsignedHex4(2, "y(mm)");
                setArgumentSeparator(3);
                setArgumentUnsignedHex4(4, "ang(1/10)deg");
                setArgumentSeparator(5);
                setArgumentUnsignedChar1(6, "Trajectory Type");
            }
            return 16;
        }
    }
    return DEVICE_HEADER_NOT_HANDLED;
}

static DeviceInterface deviceInterface = {
    .deviceHeader = TRAJECTORY_DEVICE_HEADER,
    .deviceGetName = &getTrajectoryDeviceName,
    .deviceGetInterface = &trajectoryGetInterface
};

DeviceInterface* getTrajectoryDeviceInterface(void) {
    return &deviceInterface;
}
