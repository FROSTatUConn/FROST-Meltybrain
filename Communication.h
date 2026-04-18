#ifndef IBusBM_h
#include <IBusBM.h>
#endif

#ifndef _COMMUNICATION_
#define _COMMUNICATION_

#define RECEIVER_VAL_CEILING 2000
#define DEADZONE_CONST 75

typedef struct {
  int channelData[6];
} communication_data_t;

//  Potentiometer outputs go from CCW->CW

typedef enum {
  RS_X, RS_Y, LS_Y, LS_X, POT_LEFT, POT_RIGHT
} channel_t;

int getChannelData(IBusBM IBus, int channelNumber);
void getAllChannels(IBusBM IBus);


#endif