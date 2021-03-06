#ifndef PID_H
#define PID_H

#include <stdlib.h>
#include <stdbool.h>

#include "pidConstants.h"

// NUMBER OF PID

#include "../../motion/pid/instructionType.h"
#include "../../motion/pid/pidType.h"


#include "../../common/eeprom/eeprom.h"
#include "../../common/io/outputStream.h"

#include "../../device/device.h"

#include "../../motion/extended/bspline.h"


#include "../../motion/pid/pidMotion.h"
#include "../../motion/pid/parameters/pidParameter.h"
#include "../../motion/pid/pidTimer.h"

#include "../../robot/kinematics/robotKinematics.h"

// We manage a precision of time to the millisecond
#define PID_TIME_SECOND_DIGIT_PRECISION 3

// We manage a precision for the PID of 1 digits
#define PID_VALUE_DIGIT_PRECISION       1

// We manage a precision for the U / NormalU of 1 digits
#define U_DIGIT_PRECISION               1

/**
 * Enable or disable a PID.
 * @param pidIndex the index of which pid we must change (between 0 and PID_INDEX_COUNT)
 * @param enabled false or true if we want to enabled or not the specified pid
 */
void setEnabledPid(PidMotion* pidMotion, unsigned int pidIndex, bool enabled);

/**
 * Sets the PID at the specified index.
 * @param pidIndex the index of the PID to set (between 0 and PID_COUNT)
 * @param p the P parameter
 * @param i the I parameter
 * @param d the D parameter
 * @param maxIntegral the bounds of the I term
 */
void setPidParameter(PidMotion* pidMotion, unsigned int pidIndex, float p, float i, float d, float maxIntegral);

/**
 * Returns the PID at the specified index.
 * @param index the index of the PID to set (between 0 and PID_TYPE_COUNT)
 * @return the PID at the given index
 */
PidParameter* getPidParameterByIndex(PidMotion* pidMotion, unsigned int pidTypeIndex);

/**
 * Returns the PID at the specified index.
 * @param index the index of the PID to set (between 0 and PID_TYPE_COUNT)
 * @return the PID at the given index
 */
PidParameter* getPidParameterByPidType(PidMotion* pidMotion, enum PidType pidType);

/**
 * Updates the motors values and set the type of control which is applied to the motors.
 * <ul>
 * <li>NO_POSITION_TO_REACH if we don't have any position to reach</li>
 * <li>POSITION_TO_MAINTAIN if we don't have any position, but we want to maintain the position</li>
 * <li>POSITION_IN_PROGRESS if we progress</li>
 * <li>POSITION_REACHED if we have reached the position</li>
 * <li>POSITION_FAILED if we have failed to reach the position (for example, if we are blocked)</li>
 * </li>
 */
void updateMotorsAndDetectedMotionType(PidMotion* pidMotion);

#endif
