/*
 * SPI.c
 *
 *  Created on: 23 Jan 2020
 *      Author: SMurva
 */


/*******************************************************************************
 * Includes
 ******************************************************************************/

#include "WF200_host_common.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_gpio.h"
#include "fsl_iomuxc.h"
#include "fsl_lpspi.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Master related */
#define LPSPI_MASTER_BASEADDR (LPSPI3)

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER (7U)

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT (kLPSPI_Pcs0)
#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)
#define LPSPI_MASTER_CLK_FREQ (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER + 1U))

/*! Transfer dataSize */
#define TRANSFER_SIZE 8U

/*! Transfer baudrate - 5M */
#define TRANSFER_BAUDRATE 5000000U


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES
 ******************************************************************************/

static void SetupSPI(void);


/*******************************************************************************
 * FUNCTION DEFINITIONS
 ******************************************************************************/


/*******************************************************************************
 * @brief       sl_wfx_host_spi_cs_assert: assert the CS pin for the SPI comms
 *
 * @param[in]   None
 *
 * @return      Error Status
 *
 ******************************************************************************/
sl_status_t sl_wfx_host_spi_cs_assert(void)
{
	GPIO_PinWrite(GPIO1, GPIO1_PIN_03_WF200_CHIP_SELECT_PIN, 0U);
	return SL_STATUS_OK;
}


/*******************************************************************************
 * @brief       sl_wfx_host_spi_cs_deassert: deaaert the CS pin for SPI comms
 *
 * @param[in]   None
 *
 * @return      Error Status
 *
 ******************************************************************************/
sl_status_t sl_wfx_host_spi_cs_deassert(void)
{
	GPIO_PinWrite(GPIO1, GPIO1_PIN_03_WF200_CHIP_SELECT_PIN, 1U);
	return SL_STATUS_OK;
}


/*******************************************************************************
 * @brief       sl_wfx_host_init_bus: Initialise the SPI
 *
 * @param[in]   None
 *
 * @return      Error Status
 *
 ******************************************************************************/
sl_status_t sl_wfx_host_init_bus(void)
{
	SetupSPI();
	return SL_STATUS_OK;
}


/*******************************************************************************
 * @brief       sl_wfx_host_deinit_bus:
 *
 * @param[in]   None
 *
 * @return      Error Status
 *
 ******************************************************************************/
sl_status_t sl_wfx_host_deinit_bus( void )
{
	/* Deinit SPI interface */
    //HAL_SPI_MspDeInit(&hspi1);

	return SL_STATUS_OK;
}


/*******************************************************************************
 * @brief       sl_wfx_host_enable_platform_interrupt: Enables SPI Interrupt
 *
 * @param[in]   None
 *
 * @return      Err Status
 *
 ******************************************************************************/
sl_status_t sl_wfx_host_enable_platform_interrupt(void)
{
    /* Enable GPIO pin interrupt */
    GPIO_PortEnableInterrupts(GPIO1, 1U << GPIO1_PIN_28_WF200_SPI_DATA_READY_INT);

	return SL_STATUS_OK;
}


/*******************************************************************************
 * @brief       sl_wfx_host_disable_platform_interrupt: Disable SPI Interrupt
 *
 * @param[in]   None
 *
 * @return      Err Status
 *
 ******************************************************************************/
sl_status_t sl_wfx_host_disable_platform_interrupt(void)
{
	GPIO_PortDisableInterrupts(GPIO1, 1U << GPIO1_PIN_28_WF200_SPI_DATA_READY_INT);

	return SL_STATUS_OK;
}


/*******************************************************************************
 * @brief       SetupSPI: Sets up the SPI COmms
 *
 * @param[in]   None
 *
 * @return      None
 *
 ******************************************************************************/
static void SetupSPI(void)
{
	uint32_t whichPcs;
    uint32_t srcClock_Hz;
    // uint8_t g_masterFifoSize;
    lpspi_master_config_t masterConfig;

    /*Set clock source for LPSPI*/
    CLOCK_SetMux(kCLOCK_LpspiMux, EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT);
    CLOCK_SetDiv(kCLOCK_LpspiDiv, EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER);

    /*Master config*/
    masterConfig.baudRate     = TRANSFER_BAUDRATE;
    masterConfig.bitsPerFrame = 8;
    masterConfig.cpol         = kLPSPI_ClockPolarityActiveHigh;
    masterConfig.cpha         = kLPSPI_ClockPhaseFirstEdge;
    masterConfig.direction    = kLPSPI_MsbFirst;

    masterConfig.pcsToSckDelayInNanoSec        = 1000000000 / masterConfig.baudRate;
    masterConfig.lastSckToPcsDelayInNanoSec    = 1000000000 / masterConfig.baudRate;
    masterConfig.betweenTransferDelayInNanoSec = 1000000000 / masterConfig.baudRate;

    masterConfig.whichPcs           = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;
    masterConfig.pcsActiveHighOrLow = kLPSPI_PcsActiveLow;

    masterConfig.pinCfg        = kLPSPI_SdiInSdoOut;
    masterConfig.dataOutConfig = kLpspiDataOutRetained;

    srcClock_Hz = LPSPI_MASTER_CLK_FREQ;
    LPSPI_MasterInit(LPSPI_MASTER_BASEADDR, &masterConfig, srcClock_Hz);

    whichPcs                  = EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT;

    /*The TX and RX FIFO sizes are always the same*/
    // g_masterFifoSize = LPSPI_GetRxFifoSize(LPSPI_MASTER_BASEADDR);

    LPSPI_Enable(LPSPI_MASTER_BASEADDR, false);
    LPSPI_MASTER_BASEADDR->CFGR1 &= (~LPSPI_CFGR1_NOSTALL_MASK);
    LPSPI_Enable(LPSPI_MASTER_BASEADDR, true);

    /*Flush FIFO , clear status , disable all the interrupts.*/
    LPSPI_FlushFifo(LPSPI_MASTER_BASEADDR, true, true);
    LPSPI_ClearStatusFlags(LPSPI_MASTER_BASEADDR, kLPSPI_AllStatusFlag);
    LPSPI_DisableInterrupts(LPSPI_MASTER_BASEADDR, kLPSPI_AllInterruptEnable);

    LPSPI_MASTER_BASEADDR->TCR =
        (LPSPI_MASTER_BASEADDR->TCR &
         ~(LPSPI_TCR_CONT_MASK | LPSPI_TCR_CONTC_MASK | LPSPI_TCR_RXMSK_MASK | LPSPI_TCR_PCS_MASK)) |
        LPSPI_TCR_CONT(1) | LPSPI_TCR_CONTC(0) | LPSPI_TCR_RXMSK(0) | LPSPI_TCR_TXMSK(0) | LPSPI_TCR_PCS(whichPcs);


    /*TCR is also shared the FIFO , so wait for TCR written.*/
    while (LPSPI_GetTxFifoCount(LPSPI_MASTER_BASEADDR) != 0)
    {
    }
}


/*******************************************************************************
 * @brief      sl_wfx_host_spi_transfer_no_cs_assert: Carries out data transfers
 *             over the SPI
 *
 * @param[in]   type - Read or Write
 *              Pointer to header
 *              Header length
 *              Pointer to buffer
 *              Buffer length
 *
 * @return     Error Status
 *
 ******************************************************************************/
sl_status_t sl_wfx_host_spi_transfer_no_cs_assert(sl_wfx_host_bus_transfer_type_t type,
                                                  uint8_t *header,
                                                  uint16_t header_length,
                                                  uint8_t *buffer,
                                                  uint16_t buffer_length)
{
	sl_status_t    status  = SL_STATUS_FAIL;
	const bool     is_read = ( type == SL_WFX_BUS_READ );
    lpspi_transfer_t transfer;

    transfer.configFlags = (kLPSPI_MasterPcsContinuous | EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT);
    transfer.dataSize = header_length;
    transfer.rxData = NULL;
    transfer.txData = header;

    /* Check/wait for the LPSPI not to be busy.*/
    while (LPSPI_GetStatusFlags(LPSPI_MASTER_BASEADDR) & kLPSPI_ModuleBusyFlag)
    {

    }

    status = LPSPI_MasterTransferBlocking(LPSPI_MASTER_BASEADDR, &transfer);


    transfer.configFlags = (kLPSPI_MasterPcsContinuous | EXAMPLE_LPSPI_MASTER_PCS_FOR_INIT);

    if(is_read)
    {
    	transfer.dataSize = buffer_length;
        transfer.rxData = buffer;
        transfer.txData = NULL;
    }
    else
    {
    	transfer.dataSize = buffer_length;
    	transfer.txData = buffer;
    	transfer.rxData = NULL;
    }

    /* Check/wait for the LPSPI not to be busy.*/
    while (LPSPI_GetStatusFlags(LPSPI_MASTER_BASEADDR) & kLPSPI_ModuleBusyFlag)
    {

    }

    status = LPSPI_MasterTransferBlocking(LPSPI_MASTER_BASEADDR, &transfer);

	return status;
}
