#include "Communication.h"

int* channel_data(communication_data_t* comm_data) {
  if (comm_data == nullptr) return nullptr;
  for (int i = 0; i < 6; i++) comm_data->channels[i] = comm_data->IBus.readChannel(i);
  return comm_data->channels;
}