#ifndef MAIN_BOARD_2019_H
#define MAIN_BOARD_2019_H

#include "../../main/meca1/mechanicalMainBoard1Common.h"

#include "../../common/pwm/servo/servoList.h"

#include "../../navigation/navigation.h"

#include "../../drivers/colorSensor/colorSensor.h"
#include "../../drivers/tof/tofList.h"

#include "../../robot/strategy/gameStrategyContext.h"
#include "../../robot/strategy/teamColor.h"

#include "../../robot/gameboard/gameboard.h"

#include "../../robot/match/startMatch.h"
#include "../../robot/match/endMatch.h"

#include "../../robot/config/robotConfig.h"

#include "../../robot/2019/distributor/distributor2019.h"

/**
 * This unit is done to isolate specific Functions to 2019 Editions & common
 * to PC and PIC Implementation
 */

// 2019
#define MAIN_BOARD_2019_GAME_BOARD_PRINT_ELEMENT_ARRAY_LENGTH    30

/**
 * Startup CheckList
 * @param startMatch
 * @return 
 */
bool startupCheckList2019(StartMatch* startMatch);

GameStrategyContext* initGameStrategyContext2019(RobotConfig* robotConfig,
        EndMatch* endMatch,
        TofSensorList* tofSensorList,
        ServoList* servoList);

Navigation* initNavigation2019(void);

GameBoard* initGameBoard2019(GameStrategyContext* gameStrategyContext);

void mainBoardCommonStrategyMainEndInit2019(GameStrategyContext* gameStrategyContext);

#endif
