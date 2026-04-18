#include "Communication.h"

int getChannelData(IBusBM IBus, int channelNumber) {
  return IBus.readChannel(channelNumber);
}

void getAllChannels(IBusBM IBus, int* channelData) {
  for (int i = 0; i < 6; i++) channelData[i] = IBus.readChannel(i);
}

