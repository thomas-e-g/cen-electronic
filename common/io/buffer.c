#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "buffer.h"
#include "bufferDebug.h"

#include "inputStream.h"
#include "outputStream.h"

#include "../../common/error/error.h"
#include "../../common/io/ioUtils.h"

#include "../../common/log/logHandler.h"
#include "../../common/log/logger.h"

// BUFFER

bool checkBufferNotNull(const Buffer* buffer) {
    if (buffer == NULL) {
        writeError(IO_BUFFER_NULL);
        return false;
    }
    return true;
}

// -> OUTPUT STREAM INTERFACE

/**
 * @private
 */
void _openBufferOutputStream(OutputStream* outputStream, int param1) {
    Buffer* buffer = (Buffer*) outputStream->object;

    deepClearBuffer(buffer);
}

/**
 * @private
 */
void _closeBufferOutputStream(OutputStream* outputStream) {
    Buffer* buffer = (Buffer*) outputStream->object;

    deepClearBuffer(buffer);
}

/**
 * @private
 * Definition of a function which is able to write a character.
 */
void _bufferWriteChar(OutputStream* outputStream, unsigned char c) {
    Buffer* buffer = (Buffer*) outputStream->object;

    bufferWriteChar(buffer, c);
}

/**
 * @private
 * Flush the stream.
 */
void _bufferFlush(OutputStream* outputStream) {
    // DO NOTHING
}

// -> INPUT STREAM INTERFACE

/**
 * @private
 */
void _openBufferInputStream(InputStream* inputStream, int param1) {
    Buffer* buffer = (Buffer*) inputStream->object;

    deepClearBuffer(buffer);
}

/**
 * @private
 */
void _closeBufferInputStream(InputStream* inputStream) {
    Buffer* buffer = (Buffer*) inputStream->object;

    deepClearBuffer(buffer);
}

/**
 * @private
 */
unsigned char _bufferReadChar(InputStream* inputStream) {
    Buffer* buffer = (Buffer*) inputStream->object;
    unsigned char result = bufferReadChar(buffer);
    return result;
}

/**
 * @private
 * Definition of a function which is able to return if there is character to read.
 */
bool _bufferAvailableData(InputStream* inputStream) {
    Buffer* buffer = (Buffer*) inputStream->object;
    bool result = !isBufferEmpty(buffer);

    return result;
}


// BUFFER INTERFACE

void initBuffer(Buffer* buffer, unsigned char (*s)[], unsigned int length, const char* name, const char* type) {
    if (!checkBufferNotNull(buffer)) {
        return;
    }
    buffer->s = s;
    buffer->length = length;
    deepClearBuffer(buffer);

    // inputStream
    initInputStream(
            &(buffer->inputStream),
            &_openBufferInputStream,
            &_closeBufferInputStream,
            &_bufferReadChar,
            &_bufferAvailableData,
            (int*) buffer);

    // outputStream
    initOutputStream(
            &(buffer->outputStream),
            &_openBufferOutputStream,
            &_closeBufferOutputStream,
            &_bufferWriteChar,
            &_bufferFlush,
            (int*) buffer
            );
    buffer->name = name;
    buffer->type = type;
}

bool isBufferInitialized(Buffer* buffer) {
    if (!checkBufferNotNull(buffer)) {
        return false;
    }
    return buffer->length > 0;
}

bool isBufferFull(const Buffer* buffer) {
    if (!checkBufferNotNull(buffer)) {
        return false;
    }
    return ((buffer->writeIndex + 1) % buffer->length) == buffer->readIndex;
}

bool isBufferEmpty(const Buffer* buffer) {
    return buffer->readIndex == buffer->writeIndex;
}

bool isBufferEqualsToString(const Buffer* buffer, char* s) {
    int i = 0;
    unsigned char c = *s;
    while (c != '\0') {
        if (getBufferElementsCount(buffer) <= i) {
            return false;
        }
        unsigned char cBuffer = bufferGetCharAtIndex(buffer, i);
        if (cBuffer != c) {
            return false;
        }
        s++;
        c = *s;
        i++;
    }
    if (getBufferElementsCount(buffer) != i) {
        return false;
    }
    return true;
}

int getBufferElementsCount(const Buffer* buffer) {
    int result = buffer->writeIndex - buffer->readIndex;
    if (result < 0) {
        result += buffer->length;
    }
    return result;
}

int getBufferCapacity(const Buffer* buffer) {
    return buffer->length - 1;
}

void bufferWriteChar(Buffer* buffer, unsigned char c) {
    bool isFull = isBufferFull(buffer);
    if (!isFull) {
        char* sPointer = (char*) buffer->s;
        // Shift to the right cell index
        sPointer += buffer->writeIndex;
        *sPointer = c;
        buffer->writeIndex++;
        buffer->writeIndex %= buffer->length;
    } else {
        // We must log the problem
        writeError(IO_BUFFER_FULL);
        // Print Buffer
        printDebugBuffer(getErrorOutputStreamLogger(), buffer);
    }
}

unsigned char bufferReadChar(Buffer* buffer) {
    bool isEmpty = isBufferEmpty(buffer);
    if (!isEmpty) {
        // unsigned char result = buffer->s[buffer->readIndex];
        unsigned char* sPointer = (unsigned char*) buffer->s;
        // Shift to the right cell index
        sPointer += buffer->readIndex;
        unsigned char result = *sPointer;

        buffer->readIndex++;
        buffer->readIndex %= buffer->length;
        return result;
    } else {
        // We must log the problem
        writeError(IO_BUFFER_EMPTY);
        return 0;
    }
}

unsigned char bufferGetCharAtIndex(const Buffer* buffer, int charIndex) {
    int size = getBufferElementsCount(buffer);
    if (charIndex < size) {
        unsigned char* sPointer = (unsigned char*) buffer->s;
        // Shift to the right cell index
        sPointer += ((buffer->readIndex + charIndex) % buffer->length);
        unsigned char result = *sPointer;

        return result;
    } else {
        // We must log the problem
        writeError(IO_BUFFER_NOT_ENOUGH_DATA);
    }
    return 0;
}

bool bufferWriteCharAtIndex(const Buffer* buffer, int charIndex, unsigned char c) {
    int size = getBufferElementsCount(buffer);
    if (charIndex < size) {
        char* sPointer = (char*) buffer->s;
        // Shift to the right cell index
        sPointer += ((buffer->readIndex + charIndex) % buffer->length);
        *sPointer = c;

        return true;
    } else {
        // We must log the problem
        writeError(IO_BUFFER_ILLEGAL_INDEX_ACCESS);
        return false;
    }
}

void bufferClearLastChars(Buffer* buffer, unsigned char charToRemoveCount) {
    if (!checkBufferNotNull(buffer)) {
        return;
    }
    int size = getBufferElementsCount(buffer);
    if (charToRemoveCount <= size) {
        buffer->readIndex += charToRemoveCount;
        buffer->readIndex %= buffer->length;
    } else {
        writeError(IO_BUFFER_NOT_ENOUGH_DATA);
    }
}

void clearBuffer(Buffer* buffer) {
    if (!checkBufferNotNull(buffer)) {
        return;
    }
    buffer->writeIndex = 0;
    buffer->readIndex = 0;
}

void deepClearBuffer(Buffer* buffer) {
    if (!checkBufferNotNull(buffer)) {
        return;
    }
    unsigned int i;
    char* sPointer = (char*) buffer->s;
    for (i = 0; i < buffer->length; i++) {
        // Clear the value
        *sPointer = 0;
        // Shift to the right cell index
        sPointer++;
    }
    clearBuffer(buffer);
}


void clearAfterWriteIndexBuffer(Buffer* buffer) {
    if (!checkBufferNotNull(buffer)) {
        return;
    }
    unsigned int i;
    char* sPointer = (char*)buffer->s;
    // Start at writeIndex
    sPointer += buffer->writeIndex;
    for (i = buffer->writeIndex; i < buffer->length; i++) {
        // Clear the value
        *sPointer = 0;
        // Shift to the right cell index
        sPointer++;
    }
}

InputStream* getInputStream(Buffer* buffer) {
    if (!checkBufferNotNull(buffer)) {
        return NULL;
    }
    return &(buffer->inputStream);
}

OutputStream* getOutputStream(Buffer* buffer) {
    if (!checkBufferNotNull(buffer)) {
        return NULL;
    }
    return &(buffer->outputStream);
}

