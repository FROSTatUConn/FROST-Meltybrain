#ifndef IBusBM_h
#include <IBusBM.h>
#endif

#ifndef Communication_h
#define Communication_h

#define COMM_RECEIVER_VAL_CEILING 2000
#define COMM_DEADZONE_CONST 75

#define COMM_SAFETY POT_LEFT

typedef struct {
  IBusBM IBus;
  int channels[6];
  int* (*channel_data)(communication_data_t* comm_data);
} communication_data_t;

//  Potentiometer outputs go from CCW->CW

typedef enum {
  RS_X, RS_Y, LS_Y, LS_X, POT_LEFT, POT_RIGHT
} channel_t;

int* channel_data(communication_data_t* comm_data);  //  Returns a pointer to the first element of an array of size 6

#endif