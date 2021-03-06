#include <stdlib.h>
#include <stdbool.h>

#include "gameStrategyContext.h"
#include "gameStrategyMotionHandler.h"
#include "teamColor.h"

#include "../../common/timer/cenTimer.h"
#include "../../common/timer/timerList.h"
#include "../../common/timer/timerConstants.h"

#include "../../common/log/logger.h"

#include "../../common/io/outputStream.h"
#include "../../common/io/printWriter.h"

#include "../../drivers/ioExpander/ioExpander.h"

#include "../../drivers/tof/tofList.h"

#include "../../motion/motionConstants.h"
#include "../../motion/position/trajectoryDebug.h"

void obstacleTimerCallbackFunc(Timer* timer) {
    GameStrategyContext* gameStrategyContext = (GameStrategyContext*) timer->object;
    Navigation* navigation = gameStrategyContext->navigation;
    PathList* pathList = navigation->paths;

    pathListDecreaseObstacleCost(pathList);
}

void initGameStrategyContext(GameStrategyContext* gameStrategyContext,
        RobotConfig* robotConfig,
        unsigned char strategyId,
        Navigation* navigation,
        EndMatch* endMatch,
        TofSensorList* tofSensorList,
        Point* robotPosition,
        Point* opponentRobotPosition,
        Point* lastObstaclePosition,
        ServoList* servoList) {
    gameStrategyContext->strategyId = strategyId;
    gameStrategyContext->navigation = navigation;
    gameStrategyContext->robotConfig = robotConfig;
    gameStrategyContext->endMatch = endMatch;
    gameStrategyContext->tofSensorList = tofSensorList;
    gameStrategyContext->servoList = servoList;
    gameStrategyContext->robotPosition = robotPosition;
    gameStrategyContext->opponentRobotPosition = opponentRobotPosition;
    gameStrategyContext->lastObstaclePosition = lastObstaclePosition;
    // Config
    gameStrategyContext->defaultAccelerationFactor = 1.0f;
    gameStrategyContext->defaultSpeedFactor = 1.0f;
    gameStrategyContext->loopTargetAndActions = !isConfigSet(robotConfig, CONFIG_STRATEGY_MANUAL_MODE);

    // Timer Init
    Timer* obstacleTimer = addTimer(ROBOT_OBSTACLE_TIMER_CODE, TIME_DIVIDER_1_HERTZ, &obstacleTimerCallbackFunc, "OBSTACLE TIMER", (int*) gameStrategyContext);
    gameStrategyContext->obstacleTimer = obstacleTimer;
    if (obstacleTimer != NULL) {
        obstacleTimer->callback = obstacleTimerCallbackFunc;
        obstacleTimer->enabled = true;
    }

    // Position Management
    initGameStrategyMotionHandler(gameStrategyContext);
}

void updateStrategyContextTrajectoryType(GameStrategyContext* gameStrategyContext, enum TrajectoryType trajectoryType) {
    if (trajectoryType == gameStrategyContext->trajectoryType) {
        return;
    }
    appendTrajectoryTypeAsString(getDebugOutputStreamLogger(), gameStrategyContext->trajectoryType);
    appendString(getDebugOutputStreamLogger(), " -> ");
    appendTrajectoryTypeAsString(getDebugOutputStreamLogger(), trajectoryType);
    appendCRLF(getDebugOutputStreamLogger());
    gameStrategyContext->trajectoryType = trajectoryType;
}

void showGameStrategyContextTeamColorAndStrategy(GameStrategyContext* gameStrategyContext) {
    OutputStream* outputStream = getAlwaysOutputStreamLogger();
    // COLOR
    appendTeamColorAsString(outputStream, gameStrategyContext->color);
    // STRATEGY
    appendString(outputStream, ",STG:");
    appendDec(outputStream, gameStrategyContext->strategyId);
    // SPEED
    appendString(outputStream, ",SPEED:");
    RobotConfig* robotConfig = gameStrategyContext->robotConfig;
    unsigned int speedIndex = (robotConfig->robotConfigReadInt(robotConfig) & CONFIG_SPEED_MASK) >> CONFIG_SPEED_SHIFT_BIT_VALUE;
    appendDec(outputStream, speedIndex);
    println(outputStream);
    // MANUAL MODE if activated
    if (!gameStrategyContext->loopTargetAndActions) {
        appendStringLN(outputStream, "!! MANUAL MODE !!");
    }
    // TOF
    unsigned int sonarIndex = (robotConfig->robotConfigReadInt(robotConfig) & CONFIG_SONAR_MASK) >> CONFIG_SONAR_SHIFT_BIT_VALUE;
    appendString(outputStream, "TOF:");
    appendDec(outputStream, sonarIndex);
    appendSpace(outputStream);
}