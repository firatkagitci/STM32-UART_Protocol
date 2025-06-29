#include "protocol.h"
#include "usart.h"

// CRC-16-IBM (poly 0x8005)
static uint16_t crc16(const uint8_t *data, uint16_t len) {
    uint16_t crc = 0x0000;
    for (uint16_t i=0; i<len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j=0; j<8; j++)
            crc = (crc & 0x8000) ? (crc<<1) ^ 0x8005 : crc<<1;
    }
    return crc;
}

static enum { S0, S1, S_CMD, S_LEN, S_DATA, S_CRC1, S_CRC0 } state;
static Packet_t rx_pkt;
static uint8_t rx_idx;
static uint8_t rx_crc_hi;
static uint8_t packet_ready;

void Protocol_Init(void) {
    state = S0;
    rx_idx = 0;
    packet_ready = 0;
}

void Protocol_InputByte(uint8_t b) {
    switch (state) {
    case S0: if (b==PROTO_START0) state=S1; break;
    case S1: if (b==PROTO_START1) state=S_CMD; else state=S0; break;
    case S_CMD: rx_pkt.cmd = b; state=S_LEN; break;
    case S_LEN: rx_pkt.len = b; rx_idx=0; 
               if (b==0) state=S_CRC1; else state=S_DATA; break;
    case S_DATA:
        rx_pkt.data[rx_idx++] = b;
        if (rx_idx >= rx_pkt.len) state=S_CRC1;
        break;
    case S_CRC1: rx_crc_hi = b; state=S_CRC0; break;
    case S_CRC0: {
        uint16_t rx_crc = ((uint16_t)rx_crc_hi<<8) | b;
        uint8_t buf[2 + 2 + rx_pkt.len];
        buf[0]=PROTO_START0; buf[1]=PROTO_START1;
        buf[2]=rx_pkt.cmd; buf[3]=rx_pkt.len;
        for (uint8_t i=0;i<rx_pkt.len;i++) buf[4+i]=rx_pkt.data[i];
        if (crc16(buf+2, 2+rx_pkt.len)==rx_crc)
            packet_ready = 1;
        state=S0;
    } break;
    }
}

uint8_t Protocol_PacketReceived(void) {
    return packet_ready;
}

void Protocol_GetPacket(Packet_t *pkt) {
    *pkt = rx_pkt;
    packet_ready = 0;
}

void Protocol_SendPacket(uint8_t cmd, uint8_t *data, uint8_t len) {
    uint8_t header[4] = { PROTO_START0, PROTO_START1, cmd, len };
    uint16_t crc = crc16(header+2, 2 + len);
    UART_SendBuffer(header, 4);
    if (len) UART_SendBuffer(data, len);
    UART_SendByte((crc>>8)&0xFF);
    UART_SendByte(crc & 0xFF);
}
