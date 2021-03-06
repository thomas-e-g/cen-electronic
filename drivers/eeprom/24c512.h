#ifndef ST24C512_H
#define ST24C512_H

#include "../../common/i2c/i2cCommon.h"

#include "../../common/eeprom/eeprom.h"

#define EEPROM_24C512_MAX_INDEX        0x07FFFF

/**
 * Initialize a type of eeprom : 24C512
 * @param eeprom a pointer of eeprom type.
 */
void init24C512Eeprom(Eeprom* eeprom_, I2cBusConnection* i2cBusConnection);

#endif

