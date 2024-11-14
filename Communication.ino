struct {
  int channelData[6];
} communicationData;

int getChannelData(int channelNumber) {
  communicationData.channelData[channelNumber] = IBus.readChannel(channelNumber);
  return communicationData.channelData[channelNumber];
}

int getAllChannels() {
  for (int i = 0; i < 6; i++) {
    communicationData.channelData[i] = IBus.readChannel(i);
  }
  return communicationData.channelData;
}

