unsigned char ethProcessFrame (unsigned char *recibireth, unsigned int rxPacketSize);
void ethDoTask (void);
void ethSendFrame(unsigned char socket, unsigned char *buffer, unsigned int frameLenght, unsigned char *address, unsigned int port);
