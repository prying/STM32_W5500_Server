// MIT License
// 
// Copyright (c) 2021 Flynn Harrison
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#include "ethernet.h"
#include <cmsis_gcc.h>
#include "spi.h"
#include <wizchip_conf.h>

/**
 * @brief Enter the critical section by disableing all interrupts
 * @note Does not disable non maskable interrupts and hard faults 
 * 
 */
void ETH_CrisEnter(void)
{
	__set_PRIMASK(1);
}

/**
 * @brief Return from critical section by enabling all interrupts
 * @note Does not disable non maskable interrupts and hard faults 
 * 
 */
void ETH_CrisReturn(void)
{
	__set_PRIMASK(0);
}

/**
 * @brief Select the ethernet PHY ic
 * @note CS is active low
 * 
 */
void ETH_ChipSelect(void)
{
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
}

/**
 * @brief Deselect the ethernet PHY ic
 * @note CS is active low
 * 
 */
void ETH_ChipDeselect(void)
{
	HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

/**
 * @brief Recive one byte over SPI
 * 
 * @return uint8_t 
 */
uint8_t ETH_SpiReadByte(void)
{
	uint8_t rx;
	// Check if SPI is bussy sending <Could break here as hspi isnt set as volitile>
	while(hspi1.State != HAL_SPI_STATE_READY);
	if (HAL_SPI_Receive(&hspi1, &rx, sizeof(uint8_t), SPI_TIMEOUT) == HAL_TIMEOUT)
	{
		// Catch timeout case
	}

	return rx;
}

/**
 * @brief Transmits one byte of data
 * 
 * @param tx 
 */
void ETH_SpiWriteByte(uint8_t tx)
{
	// Check if SPI is bussy sending <Could break here as hspi isnt set as volitile>
	while(hspi1.State != HAL_SPI_STATE_READY);
	if (HAL_SPI_Transmit(&hspi1, &tx, sizeof(uint8_t), SPI_TIMEOUT) == HAL_TIMEOUT)
	{
		// Catch timeout case
	}
}

/**
 * @brief Recives len number of bytes over SPI
 * 
 * @param rxBuf 
 * @param len 
 */
void ETH_SpiRead(uint8_t* rxBuf, uint16_t len)
{
	// Check if SPI is bussy sending <Could break here as hspi isnt set as volitile>
	// Check if SPI is bussy sending <Could break here as hspi isnt set as volitile>
	while(hspi1.State != HAL_SPI_STATE_READY);
	if (HAL_SPI_Receive(&hspi1, rxBuf, len, SPI_TIMEOUT) == HAL_TIMEOUT)
	{
		// Catch timeout case
	}
}

/**
 * @brief Sends len number of bytes over SPI
 * 
 * @param txBuf 
 * @param len 
 */
void ETH_SpiWrite(uint8_t* txBuf, uint16_t len)
{
	// Check if SPI is bussy sending <Could break here as hspi isnt set as volitile>
	// Check if SPI is bussy sending <Could break here as hspi isnt set as volitile>
	while(hspi1.State != HAL_SPI_STATE_READY);
	if (HAL_SPI_Transmit(&hspi1, txBuf, len, SPI_TIMEOUT) == HAL_TIMEOUT)
	{
		// Catch timeout case
	}
}

void ETH_W5500_Attach(void)
{
	// Register critical selection callback functions
	reg_wizchip_cris_cbfunc(ETH_CrisEnter, ETH_CrisReturn);

	// Select variable length or fixed length
#if	_WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
	reg_wizchip_cs_cbfunc(ETH_ChipSelect, ETH_ChipDeselect);
#else
	#error "Expected to be operating in _WIZCHIP_IO_MODE_SPI_VDM_"
#endif

	// Register single byte SPI 
	reg_wizchip_spi_cbfunc(ETH_SpiReadByte, ETH_SpiWriteByte);

	// Register SPI
	reg_wizchip_spiburst_cbfunc(ETH_SpiRead, ETH_SpiWrite);
}
