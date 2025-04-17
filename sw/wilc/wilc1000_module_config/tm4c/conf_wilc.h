#define CONF_WILC_USE_SPI				

/** SPI pin and instance settings. */
#define CONF_WILC_SPI					SPI
#define CONF_WILC_SPI_ID				ID_SPI
#define CONF_WILC_SPI_MISO_GPIO			SPI_MISO_GPIO
#define CONF_WILC_SPI_MISO_FLAGS		SPI_MISO_FLAGS
#define CONF_WILC_SPI_MOSI_GPIO			SPI_MOSI_GPIO
#define CONF_WILC_SPI_MOSI_FLAGS		SPI_MOSI_FLAGS
#define CONF_WILC_SPI_CLK_GPIO			SPI_SPCK_GPIO
#define CONF_WILC_SPI_CLK_FLAGS			SPI_SPCK_FLAGS
#define CONF_WILC_SPI_CS_GPIO			SPI_NPCS0_GPIO
#define CONF_WILC_SPI_CS_FLAGS			PIO_OUTPUT_1
#define CONF_WILC_SPI_NPCS				(0)

/** SPI delay before SPCK and between consecutive transfer. */
#define CONF_WILC_SPI_DLYBS				(0)
#define CONF_WILC_SPI_DLYBCT 			(0)

/** SPI interrupt pin. */
#define CONF_WILC_SPI_INT_PIN			IOPORT_CREATE_PIN(PIOA, 1)
#define CONF_WILC_SPI_INT_PIO			PIOA
#define CONF_WILC_SPI_INT_PIO_ID		ID_PIOA
#define CONF_WILC_SPI_INT_MASK			(1 << 1)
/** Highest INT priority that can call FreeRTOS FromISR() functions. */
#define CONF_WILC_SPI_INT_PRIORITY		(configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)

/** Clock polarity & phase. */
#define CONF_WILC_SPI_POL				(0)
#define CONF_WILC_SPI_PHA				(1)

/** SPI clock. */
#define CONF_WILC_SPI_CLOCK				(48000000)

/*
   ---------------------------------
   ----------- OS options ----------
   ---------------------------------
*/

#define CONF_DRIVER_LOCK				1
#if CONF_DRIVER_LOCK
#  include <os/include/os_hook.h>
#  define DRIVER_SIGNAL_ISR				os_hook_isr
#else
#  define DRIVER_SIGNAL_ISR()
#endif

/*
   ---------------------------------
   --------- Debug options ---------
   ---------------------------------
*/

#define CONF_WILC_DEBUG					(0)
#define CONF_WILC_PRINTF				osprintf

#ifdef __cplusplus
}
#endif

#endif /* CONF_WILC_H_INCLUDED */
