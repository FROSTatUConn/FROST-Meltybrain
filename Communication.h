#ifndef _COMMUNICATION_
#define _COMMUNICATION_

#include <IBusBM.h>

#define RECEIVER_VAL_CEILING 2000
#define DEADZONE_CONST 75

typedef struct {
  int channelData[6];
} communication_data_t;

typedef enum {
  IDK, IDK1, LS_X, LS_Y, RS_X, RS_Y
} channel_t;

int getChannelData(IBusBM IBus, int channelNumber);
int* getAllChannels(IBusBM IBus);



#endif