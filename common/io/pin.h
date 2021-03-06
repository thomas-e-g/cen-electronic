#ifndef PIN_H
#define PIN_H

#include <stdbool.h>
#include <stdlib.h>

#include "../../common/error/error.h"
#include "outputStream.h"

// forward declaration
struct PinList;
typedef struct PinList PinList;

/**
* As we do not use always the same hardware, some pin could be 
*/
typedef bool IsPinValidFunction(PinList* pinList, int pinIndex);

/**
 * Returns if the pin is input
 * @return true if the pin is input, false else
 */
typedef bool IsPinInputFunction(PinList* pinList, int pinIndex);

/**
* Set the input / output for the pinList.
 * @param pinList the encapsulation of the list of pin
 * @param pintIndex the index of the pin
 * @param input true if we would like to set the pin as input, false if we want an output
*/
typedef void SetPinInputFunction(PinList* pinList, int pinIndex, bool input);

/**
 * Get the pin value for the index.
 * @param pinList the encapsulation of the list of pin
 * @param pinIndex the index of the pin
 * @return true if the pin is on, false if the pin is off
 */
typedef bool GetPinValueFunction(PinList* pinList, int pinIndex);

/**
 * Set the value for the pin Index (see pin.h)
 * @param pinList the encapsulation of the list of pin
 * @param pinIndex the index of the pin
 * @param pinValue the new value of the pin
 */
typedef void SetPinValueFunction(PinList* pinList, int pinIndex, bool pinValue);

/**
 * Initialise a pin List for Pc.
 * @param pinList pointer on pinList object (POO Paradigm)
 */
void initPinList(PinList* pinList, 
        IsPinValidFunction* isPinValidFunction,
        IsPinInputFunction* isPinInputFunction,
        SetPinInputFunction* setPinInputFunction,
        GetPinValueFunction* getPinValueFunction,
        SetPinValueFunction* setPinValueFunction,
        int* object);

struct PinList {
    /** Returns if the pin is valid or not (because of the use of PIC with 64 pin, 100 pin, or 144 pin. */
    IsPinValidFunction* isPinValidFunction;
    /** Returns if the pin is in input (1) or output (0)*/
    IsPinInputFunction* isPinInputFunction;
    /** Set the mode input/output of the pin. */
    SetPinInputFunction* setPinInputFunction;
    /** The function which must be called to get the status of a pin. */
    GetPinValueFunction* getPinValueFunction;
    /** The function which must be called to set a pin. */
    SetPinValueFunction* setPinValueFunction;
    /** pointer on other object (useful PC Implementation for example).*/
    int* object;
};

// PIC32 Definition : http://ww1.microchip.com/downloads/en/devicedoc/60001156j.pdf

// PORT A : only for PIC with 100 pin
#define PIN_INDEX_RA0        0x00
#define PIN_INDEX_RA1        0x01
#define PIN_INDEX_RA2        0x02
#define PIN_INDEX_RA3        0x03
#define PIN_INDEX_RA4        0x04
#define PIN_INDEX_RA5        0x05
#define PIN_INDEX_RA6        0x06
#define PIN_INDEX_RA7        0x07
#define PIN_INDEX_RA8        0x08
#define PIN_INDEX_RA9        0x09
#define PIN_INDEX_RA10       0x0A
#define PIN_INDEX_RA11       0x0B
#define PIN_INDEX_RA12       0x0C
#define PIN_INDEX_RA13       0x0D
#define PIN_INDEX_RA14       0x0E
#define PIN_INDEX_RA15       0x0F

// Port B : all PIC 32

#define PIN_INDEX_RB0        0x10
#define PIN_INDEX_RB1        0x11
#define PIN_INDEX_RB2        0x12
#define PIN_INDEX_RB3        0x13
#define PIN_INDEX_RB4        0x14
#define PIN_INDEX_RB5        0x15
#define PIN_INDEX_RB6        0x16
#define PIN_INDEX_RB7        0x17
#define PIN_INDEX_RB8        0x18
#define PIN_INDEX_RB9        0x19
#define PIN_INDEX_RB10       0x1A
#define PIN_INDEX_RB11       0x1B
#define PIN_INDEX_RB12       0x1C
#define PIN_INDEX_RB13       0x1D
#define PIN_INDEX_RB14       0x1E
#define PIN_INDEX_RB15       0x1F

// Port C
#define PIN_INDEX_RC0        0x20
#define PIN_INDEX_RC1        0x21
#define PIN_INDEX_RC2        0x22
#define PIN_INDEX_RC3        0x23
#define PIN_INDEX_RC4        0x24
#define PIN_INDEX_RC5        0x25
#define PIN_INDEX_RC6        0x26
#define PIN_INDEX_RC7        0x27
#define PIN_INDEX_RC8        0x28
#define PIN_INDEX_RC9        0x29
#define PIN_INDEX_RC10       0x2A
#define PIN_INDEX_RC11       0x2B
#define PIN_INDEX_RC12       0x2C
#define PIN_INDEX_RC13       0x2D
#define PIN_INDEX_RC14       0x2E
#define PIN_INDEX_RC15       0x2F

// Port D
#define PIN_INDEX_RD0        0x30
#define PIN_INDEX_RD1        0x31
#define PIN_INDEX_RD2        0x32
#define PIN_INDEX_RD3        0x33
#define PIN_INDEX_RD4        0x34
#define PIN_INDEX_RD5        0x35
#define PIN_INDEX_RD6        0x36
#define PIN_INDEX_RD7        0x37
#define PIN_INDEX_RD8        0x38
#define PIN_INDEX_RD9        0x39
#define PIN_INDEX_RD10       0x3A
#define PIN_INDEX_RD11       0x3B
#define PIN_INDEX_RD12       0x3C
#define PIN_INDEX_RD13       0x3D
#define PIN_INDEX_RD14       0x3E
#define PIN_INDEX_RD15       0x3F

// Port E
#define PIN_INDEX_RE0        0x40
#define PIN_INDEX_RE1        0x41
#define PIN_INDEX_RE2        0x42
#define PIN_INDEX_RE3        0x43
#define PIN_INDEX_RE4        0x44
#define PIN_INDEX_RE5        0x45
#define PIN_INDEX_RE6        0x46
#define PIN_INDEX_RE7        0x47
#define PIN_INDEX_RE8        0x48
#define PIN_INDEX_RE9        0x49
#define PIN_INDEX_RE10       0x4A

// PORT F
#define PIN_INDEX_RF0        0x4B
#define PIN_INDEX_RF1        0x4C
#define PIN_INDEX_RF2        0x4D
#define PIN_INDEX_RF3        0x4E
#define PIN_INDEX_RF4        0x4F
#define PIN_INDEX_RF5        0x50
#define PIN_INDEX_RF6        0x51
#define PIN_INDEX_RF7        0x52
#define PIN_INDEX_RF8        0x53

#define PIN_MIN_INDEX        PIN_INDEX_RA0
#define PIN_MAX_INDEX        PIN_INDEX_RF8

/**
* Returns the name of the pin.
 * @param pinList the encapsulation of the list of pin
 * @param the index of the pin
 * @return the name of the pin
*/
char* getPinName(PinList* pinList, int pinIndex);

/**
 * Returns the value of the pin Index (see pin.h)
 * @param pinList the encapsulation of the list of pin
 * @param the index of the pin
 * @return 
 */
bool getPinValue(PinList* pinList, int pinIndex);

/**
 * Set the value for the pin Index (see pin.h)
 * @param pinList the encapsulation of the list of pin
 * @param pinIndex the index of the pin
 * @param pinValue the new value of the pin
 */
void setPinValue(PinList* pinList, int pinIndex, bool pinValue);

/**
 * Returns if the pin is configured as input or output.
 * @param pinList the encapsulation of the list of pin
 * @param pinIndex the index of the pin
 * @return true if the pin is in input (true) or output (false)
 */
bool isPinInput(PinList* pinList, int pinIndex);

/**
* Set as input or output the pin defined by the index
* @param pinList the encapsulation of the list of pin
* @param pinIndex the index of the pin
* @param input true if we want to set the pin as input, false if we want to set pin as output
*/
void setPinInput(PinList* pinList, int pinIndex, bool input);

/**
 * Returns if the pin is a really valid or invalid pin.
 * @param pinList the encapsulation of the list of pin
 * @param pinIndex the index of the pin
 * @return true if the pin is valid or invalid
 */
bool isPinValid(PinList* pinList, int pinIndex);

#endif

