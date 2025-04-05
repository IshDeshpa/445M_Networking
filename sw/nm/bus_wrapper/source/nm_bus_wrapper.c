#include "nm_bsp.h"
#include "nm_bus_wrapper.h"

#include "../../../driverlib/ssi.h"
#include "../../../driverlib/udma.h"
#include "../../../driverlib/sysctl.h"
#include "../../../inc/hw_sysctl.h"

#define SSI1_BASER (0x40009000)
#define SSI1_O_DR (0x8)

// To not wasted resources on smaller spi transfers
#define MIN_DMA_SPI_SIZE 16

#pragma DATA_ALIGN(udmaControlTable, 1024)
uint8_t udmaControlTable[1024];


static uint8_t spi_rw_dma(uint8_t *pu8Mosi, uint8_t *pu8Miso, uint16_t u16Sz) {
    uint32_t i;
    uint32_t in = 0;

    // Assert chip select
    // Don't think we need to assert chip select here; Fss should be doing that for us?
    // WILC_CS_LO;

    // Set up the DMA transfer
    uDMAChannelTransferSet(UDMA_CHANNEL_SSI1RX | UDMA_PRI_SELECT,
                           UDMA_MODE_BASIC,
                           (void *)(SSI1_BASER + SSI1_O_DR),
                           pu8Miso,
                           u16Sz);

    uDMAChannelTransferSet(UDMA_CHANNEL_SSI1TX | UDMA_PRI_SELECT,
                           UDMA_MODE_BASIC,
                           pu8Mosi,
                           (void *)(SSI1_BASER + SSI1_O_DR),
                           u16Sz);

    // Start the DMA transfer
    uDMAChannelEnable(UDMA_CHANNEL_SSI1RX);
    uDMAChannelEnable(UDMA_CHANNEL_SSI1TX);

    // Wait for the transfer to complete
    // TODO: make this some sort of OS_Sleep?
    while (uDMAChannelIsEnabled(UDMA_CHANNEL_SSI1RX) || uDMAChannelIsEnabled(UDMA_CHANNEL_SSI1TX));

    // De-assert chip select
    // WILC_CS_HI;

    return 0;
}

static int8_t spi_rw(uint8_t *pu8Mosi, uint8_t *pu8Miso, uint16_t u16Sz) {
    uint32_t i;
    uint32_t in = 0;

    // Assert chip select
    // Don't think we need to assert chip select here; Fss should be doing that for us?
    // WILC_CS_LO;

    for (i = 0; i < u16Sz; i++) {
        // Transmit byte to SPI
        // while (SSIDataGetNonBlocking(SSI1_BASER, 0) == 0);
        SSIDataPut(SSI1_BASER, pu8Mosi ? pu8Mosi[i] : 0);

        // Receive byte from SPI if available
        while (SSIDataGetNonBlocking(SSI1_BASER, (uint32_t*)&pu8Miso[in]) == 0);
        in++;
    }

    // De-assert chip select
    // WILC_CS_HI;

    return 0;
}

int8_t nm_bus_init(){
    // TODO: add mutex protections here
    WILC_WAKE_HI;

    // Configure SPI
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);  // activate SSI1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD); // activate port D
    
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI1)); // allow time for SSI1 clock to start
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD)); // allow time for GPIO D clock to start

    GPIO_PORTD_DIR_R |= 0x0B;        // make PD3,1,0 output
    GPIO_PORTD_AFSEL_R |= 0x0F;     // enable alt funct on PD3-0
    GPIO_PORTD_DEN_R |= 0x0F;        // enable digital I/O on PD3-0
    GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & ~0x0000FFFF) | 0x00002222; // set to SSI0
    GPIO_PORTD_AMSEL_R &= ~0x0F;    // disable analog functionality on PD
    
    SSIConfigSetExpClk(SSI1_BASER, 8000000, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000000, 8); // 8MHz clock, 1Mbps baud rate, 8-bit data
    SSIEnable(SSI1_BASER); // enable SSI

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    while (!SysCtlPeripheralReady(SYSCTL_PERIPH_UDMA));

    uDMAEnable();
    uDMAControlBaseSet(udmaControlTable);

    // Enable DMA for SSI0 TX and RXl
    SSIDMAEnable(SSI1_BASER, SSI_DMA_TX | SSI_DMA_RX);

    // Configure the TX DMA channel (uDMA Channel 24 - SSI0 TX)
    uDMAChannelAttributeDisable(UDMA_CHANNEL_SSI0TX, UDMA_ATTR_ALL);
    uDMAChannelControlSet(UDMA_CHANNEL_SSI0TX | UDMA_PRI_SELECT,
                          UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_4);

    // Configure the RX DMA channel (uDMA Channel 25 - SSI0 RX)
    uDMAChannelAttributeDisable(UDMA_CHANNEL_SSI0RX, UDMA_ATTR_ALL);
    uDMAChannelControlSet(UDMA_CHANNEL_SSI0RX | UDMA_PRI_SELECT,
                          UDMA_SIZE_8 | UDMA_SRC_INC_NONE | UDMA_DST_INC_8 | UDMA_ARB_4);
    
}

// Send/receive data using DMA
// u8Cmd is ignored but kept for compatibility with other drivers
int8_t nm_bus_ioctl(uint8_t u8Cmd, void* pvParameter){
    int8_t s8Ret = 0;

    tstrNmSpiRw *pstrParam = (tstrNmSpiRw *) pvParameter;
    if (pstrParam->u16Sz > MIN_DMA_SPI_SIZE) {
        s8Ret = spi_rw_dma(pstrParam->pu8InBuf, pstrParam->pu8OutBuf, pstrParam->u16Sz);
    } else {
        s8Ret = spi_rw(pstrParam->pu8InBuf, pstrParam->pu8OutBuf, pstrParam->u16Sz);
    }

    return s8Ret;
}

int8_t nm_bus_deinit(void){
    WILC_CHIPEN_LO;
    WILC_RESET_LO;
    return 0;
}