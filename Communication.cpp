#include "Communication.h"

int get_channel_data(IBusBM IBus, int channel_number) {
  return IBus.readChannel(channel_number);
}

void get_all_channels(IBusBM IBus, int* channel_data) {
  for (int i = 0; i < 6; i++) channel_data[i] = IBus.readChannel(i);
}

