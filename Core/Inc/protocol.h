#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdint.h>
#define PROTO_START0 0x55
#define PROTO_START1 0xAA

typedef struct {
    uint8_t cmd;
    uint8_t len;
    uint8_t data[255];
} Packet_t;

void Protocol_Init(void);
void Protocol_InputByte(uint8_t b);
uint8_t Protocol_PacketReceived(void);
void Protocol_GetPacket(Packet_t *pkt);
void Protocol_SendPacket(uint8_t cmd, uint8_t *data, uint8_t len);

#endif // PROTOCOL_H
