#include <stdint.h>

extern char *tx_outfile;
extern char *rx_outfile;

void ethernetTX(uint8_t* payload, uint16_t size);

void ethernetRX (void);

void userRXData(uint8_t* payload, uint16_t size);
