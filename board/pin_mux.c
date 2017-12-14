/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v3.0
processor: MK64FN1M0xxx12
package_id: MK64FN1M0VLL12
mcu_data: ksdk2_0
processor_version: 2.0.0
pin_labels:
- {pin_num: '69', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28, label: 3D, identifier: PIN_3D}
- {pin_num: '66', pin_signal: PTB20/SPI2_PCS0/FB_AD31/CMP0_OUT, label: INT2, identifier: INT2}
- {pin_num: '98', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/EWM_OUT_b/SPI1_SCK, label: USB_PRES, identifier: USB_PRES}
- {pin_num: '97', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/EWM_IN/SPI1_PCS0, label: FXOS_RST, identifier: FXOS_RST}
- {pin_num: '67', pin_signal: PTB21/SPI2_SCK/FB_AD30/CMP1_OUT, label: INT1, identifier: INT1}
- {pin_num: '68', pin_signal: PTB22/SPI2_SOUT/FB_AD29/CMP2_OUT, label: PPS, identifier: PPS}
- {pin_num: '72', pin_signal: ADC0_SE4b/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/I2S0_TX_FS, label: SHARP_CS, identifier: SHARP_CS}
- {pin_num: '70', pin_signal: ADC0_SE14/PTC0/SPI0_PCS4/PDB0_EXTRG/USB_SOF_OUT/FB_AD14/I2S0_TXD1, label: SPI_CS0, identifier: SPI_CS0}
- {pin_num: '71', pin_signal: ADC0_SE15/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/I2S0_TXD0, label: SST_CS, identifier: SST_CS}
- {pin_num: '73', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, label: CS_NRF, identifier: CS_NRF}
- {pin_num: '96', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/I2C0_SDA, label: BTN_1, identifier: BTN_1}
- {pin_num: '95', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/I2C0_SCL, label: BTN_2, identifier: BTN_2}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/*FUNCTION**********************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 *END**************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
}

#define PCR_DSE_HIGH                  0x01u   /*!< Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
#define PCR_ODE_ENABLED               0x01u   /*!< Open Drain Enable: Open drain output is enabled on the corresponding pin, if the pin is configured as a digital output. */
#define PCR_PE_ENABLED                0x01u   /*!< Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */
#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */
#define PIN26_IDX                       26u   /*!< Pin number for pin 26 in a port */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART 0 transmit data source select: UART0_TX pin */

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '11', peripheral: USB0, signal: DM, pin_signal: USB0_DM}
  - {pin_num: '10', peripheral: USB0, signal: DP, pin_signal: USB0_DP}
  - {pin_num: '34', peripheral: JTAG, signal: JTAG_TCLK_SWD_CLK, pin_signal: PTA0/UART0_CTS_b/UART0_COL_b/FTM0_CH5/JTAG_TCLK/SWD_CLK/EZP_CLK}
  - {pin_num: '37', peripheral: JTAG, signal: JTAG_TMS_SWD_DIO, pin_signal: PTA3/UART0_RTS_b/FTM0_CH0/JTAG_TMS/SWD_DIO}
  - {pin_num: '35', peripheral: JTAG, signal: TDI, pin_signal: PTA1/UART0_RX/FTM0_CH6/JTAG_TDI/EZP_DI}
  - {pin_num: '36', peripheral: JTAG, signal: TDO, pin_signal: PTA2/UART0_TX/FTM0_CH7/JTAG_TDO/TRACE_SWO/EZP_DO}
  - {pin_num: '39', peripheral: JTAG, signal: TRST, pin_signal: PTA5/USB_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b}
  - {pin_num: '72', peripheral: SPI0, signal: PCS2, pin_signal: ADC0_SE4b/CMP1_IN0/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/I2S0_TX_FS}
  - {pin_num: '70', peripheral: SPI0, signal: PCS4, pin_signal: ADC0_SE14/PTC0/SPI0_PCS4/PDB0_EXTRG/USB_SOF_OUT/FB_AD14/I2S0_TXD1}
  - {pin_num: '78', peripheral: SPI0, signal: SOUT, pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/I2S0_MCLK}
  - {pin_num: '55', peripheral: I2C0, signal: SCL, pin_signal: ADC0_SE12/PTB2/I2C0_SCL/UART0_RTS_b/ENET0_1588_TMR0/FTM0_FLT3, pull_select: up, pull_enable: enable}
  - {pin_num: '56', peripheral: I2C0, signal: SDA, pin_signal: ADC0_SE13/PTB3/I2C0_SDA/UART0_CTS_b/UART0_COL_b/ENET0_1588_TMR1/FTM0_FLT0, pull_select: up, pull_enable: enable}
  - {pin_num: '4', peripheral: SDHC, signal: CMD, pin_signal: ADC0_DM2/ADC1_SE7a/PTE3/SPI1_SIN/UART1_RTS_b/SDHC0_CMD/TRACE_D1/SPI1_SOUT, drive_strength: high, pull_select: up,
    pull_enable: enable}
  - {pin_num: '2', peripheral: SDHC, signal: 'DATA, 0', pin_signal: ADC1_SE5a/PTE1/LLWU_P0/SPI1_SOUT/UART1_RX/SDHC0_D0/TRACE_D3/I2C1_SCL/SPI1_SIN, drive_strength: high,
    pull_select: up, pull_enable: enable}
  - {pin_num: '1', peripheral: SDHC, signal: 'DATA, 1', pin_signal: ADC1_SE4a/PTE0/SPI1_PCS1/UART1_TX/SDHC0_D1/TRACE_CLKOUT/I2C1_SDA/RTC_CLKOUT, drive_strength: high,
    pull_select: up, pull_enable: enable}
  - {pin_num: '6', peripheral: SDHC, signal: 'DATA, 2', pin_signal: PTE5/SPI1_PCS2/UART3_RX/SDHC0_D2/FTM3_CH0, drive_strength: high, pull_select: up, pull_enable: enable}
  - {pin_num: '5', peripheral: SDHC, signal: 'DATA, 3', pin_signal: PTE4/LLWU_P2/SPI1_PCS0/UART3_TX/SDHC0_D3/TRACE_D0, drive_strength: high, pull_select: up, pull_enable: enable}
  - {pin_num: '3', peripheral: SDHC, signal: DCLK, pin_signal: ADC0_DP2/ADC1_SE6a/PTE2/LLWU_P1/SPI1_SCK/UART1_CTS_b/SDHC0_DCLK/TRACE_D2, drive_strength: high, pull_select: up,
    pull_enable: enable}
  - {pin_num: '8', peripheral: SUPPLY, signal: 'VDD, 1', pin_signal: VDD16}
  - {pin_num: '40', peripheral: SUPPLY, signal: 'VDD, 3', pin_signal: VDD63}
  - {pin_num: '48', peripheral: SUPPLY, signal: 'VDD, 4', pin_signal: VDD80}
  - {pin_num: '61', peripheral: SUPPLY, signal: 'VDD, 5', pin_signal: VDD110}
  - {pin_num: '75', peripheral: SUPPLY, signal: 'VDD, 6', pin_signal: VDD124}
  - {pin_num: '89', peripheral: SUPPLY, signal: 'VDD, 7', pin_signal: VDD140}
  - {pin_num: '22', peripheral: SUPPLY, signal: 'VDDA, 0', pin_signal: VDDA}
  - {pin_num: '9', peripheral: SUPPLY, signal: 'VSS, 1', pin_signal: VSS17}
  - {pin_num: '41', peripheral: SUPPLY, signal: 'VSS, 4', pin_signal: VSS64}
  - {pin_num: '49', peripheral: SUPPLY, signal: 'VSS, 5', pin_signal: VSS81}
  - {pin_num: '60', peripheral: SUPPLY, signal: 'VSS, 6', pin_signal: VSS109}
  - {pin_num: '74', peripheral: SUPPLY, signal: 'VSS, 7', pin_signal: VSS123}
  - {pin_num: '88', peripheral: SUPPLY, signal: 'VSS, 8', pin_signal: VSS139}
  - {pin_num: '25', peripheral: SUPPLY, signal: 'VSSA, 0', pin_signal: VSSA}
  - {pin_num: '12', peripheral: USB0, signal: VOUT33, pin_signal: VOUT33}
  - {pin_num: '13', peripheral: USB0, signal: VREGIN, pin_signal: VREGIN}
  - {pin_num: '77', peripheral: SPI0, signal: SCK, pin_signal: PTC5/LLWU_P9/SPI0_SCK/LPTMR0_ALT2/I2S0_RXD0/FB_AD10/CMP0_OUT/FTM0_CH2}
  - {pin_num: '79', peripheral: SPI0, signal: SIN, pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB_SOF_OUT/I2S0_RX_FS/FB_AD8}
  - {pin_num: '50', peripheral: OSC, signal: EXTAL0, pin_signal: EXTAL0/PTA18/FTM0_FLT2/FTM_CLKIN0}
  - {pin_num: '51', peripheral: OSC, signal: XTAL0, pin_signal: XTAL0/PTA19/FTM1_FLT0/FTM_CLKIN1/LPTMR0_ALT1}
  - {pin_num: '62', peripheral: UART0, signal: RX, pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN, pull_select: up, pull_enable: enable}
  - {pin_num: '63', peripheral: UART0, signal: TX, pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b}
  - {pin_num: '67', peripheral: GPIOB, signal: 'GPIO, 21', pin_signal: PTB21/SPI2_SCK/FB_AD30/CMP1_OUT, direction: INPUT}
  - {pin_num: '68', peripheral: GPIOB, signal: 'GPIO, 22', pin_signal: PTB22/SPI2_SOUT/FB_AD29/CMP2_OUT, direction: INPUT}
  - {pin_num: '73', peripheral: SPI0, signal: PCS1, pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK}
  - {pin_num: '71', peripheral: SPI0, signal: PCS3, pin_signal: ADC0_SE15/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/I2S0_TXD0}
  - {pin_num: '69', peripheral: GPIOB, signal: 'GPIO, 23', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28, direction: INPUT}
  - {pin_num: '66', peripheral: GPIOB, signal: 'GPIO, 20', pin_signal: PTB20/SPI2_PCS0/FB_AD31/CMP0_OUT, direction: INPUT}
  - {pin_num: '98', peripheral: GPIOD, signal: 'GPIO, 5', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/EWM_OUT_b/SPI1_SCK, direction: INPUT}
  - {pin_num: '97', peripheral: GPIOD, signal: 'GPIO, 4', pin_signal: PTD4/LLWU_P14/SPI0_PCS1/UART0_RTS_b/FTM0_CH4/FB_AD2/EWM_IN/SPI1_PCS0, direction: INPUT}
  - {pin_num: '96', peripheral: GPIOD, signal: 'GPIO, 3', pin_signal: PTD3/SPI0_SIN/UART2_TX/FTM3_CH3/FB_AD3/I2C0_SDA, direction: INPUT, pull_enable: enable}
  - {pin_num: '95', peripheral: GPIOD, signal: 'GPIO, 2', pin_signal: PTD2/LLWU_P13/SPI0_SOUT/UART2_RX/FTM3_CH2/FB_AD4/I2C0_SCL, direction: INPUT, pull_enable: enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
	CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

	PORT_SetPinMux(PORTA, PIN0_IDX, kPORT_MuxAlt7);            /* PORTA0 (pin 34) is configured as JTAG_TCLK */
	PORT_SetPinMux(PORTA, PIN1_IDX, kPORT_MuxAlt7);            /* PORTA1 (pin 35) is configured as JTAG_TDI */
	PORT_SetPinMux(PORTA, PIN18_IDX, kPORT_PinDisabledOrAnalog); /* PORTA18 (pin 50) is configured as EXTAL0 */
	PORT_SetPinMux(PORTA, PIN19_IDX, kPORT_PinDisabledOrAnalog); /* PORTA19 (pin 51) is configured as XTAL0 */
	PORT_SetPinMux(PORTA, PIN2_IDX, kPORT_MuxAlt7);            /* PORTA2 (pin 36) is configured as JTAG_TDO */
	PORT_SetPinMux(PORTA, PIN3_IDX, kPORT_MuxAlt7);            /* PORTA3 (pin 37) is configured as JTAG_TMS */
	PORT_SetPinMux(PORTA, PIN5_IDX, kPORT_MuxAlt7);            /* PORTA5 (pin 39) is configured as JTAG_TRST_b */
	PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 62) is configured as UART0_RX */
	PORTB->PCR[16] = ((PORTB->PCR[16] &
			(~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
			| PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding PE field is set. */
			| PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
	);
	PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 63) is configured as UART0_TX */

#ifndef DEBUG_CONFIG

	const port_pin_config_t porte2_pin55_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
			kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
			kPORT_MuxAlt2,                                           /* Pin is configured as I2C0_SCL */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTB, PIN2_IDX, &porte2_pin55_config); /* PORTE24 (pin 31) is configured as I2C0_SCL */

	const port_pin_config_t porte2_pin56_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
			kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
			kPORT_MuxAlt2,                                           /* Pin is configured as I2C0_SDA */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTB, PIN3_IDX, &porte2_pin56_config); /* PORTE25 (pin 32) is configured as I2C0_SDA */
#else

	const port_pin_config_t porte24_pin31_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
			kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
			kPORT_MuxAlt5,                                           /* Pin is configured as I2C0_SCL */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTE, PIN24_IDX, &porte24_pin31_config); /* PORTE24 (pin 31) is configured as I2C0_SCL */
	const port_pin_config_t porte25_pin32_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainEnable,                                   /* Open drain is enabled */
			kPORT_LowDriveStrength,                                  /* Low drive strength is configured */
			kPORT_MuxAlt5,                                           /* Pin is configured as I2C0_SDA */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTE, PIN25_IDX, &porte25_pin32_config); /* PORTE25 (pin 32) is configured as I2C0_SDA */

#endif

	PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_MuxAsGpio);         /* PORTB20 (pin 66) is configured as PTB20 */
        PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_MuxAsGpio);         /* PORTB21 (pin 67) is configured as PTB21 */
	PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_MuxAsGpio);         /* PORTB22 (pin 68) is configured as PTB22 */
	PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_MuxAsGpio);         /* PORTB23 (pin 69) is configured as PTB23 */
	PORT_SetPinMux(PORTC, PIN0_IDX, kPORT_MuxAlt2);            /* PORTC0 (pin 70) is configured as SPI0_PCS4 */
	PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAlt2);            /* PORTC1 (pin 71) is configured as SPI0_PCS3 */
	PORT_SetPinMux(PORTC, PIN2_IDX, kPORT_MuxAlt2);            /* PORTC2 (pin 72) is configured as SPI0_PCS2 */

//	PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt2);            /* PORTC3 (pin 73) is configured as SPI0_PCS1 */
    PORT_SetPinMux(BOARD_INITPINS_CS_NRF_PORT, BOARD_INITPINS_CS_NRF_PIN, kPORT_MuxAsGpio); /* PORTC3 (pin 73) is configured as PTC3 */

	PORT_SetPinMux(PORTC, PIN5_IDX, kPORT_MuxAlt2);            /* PORTC5 (pin 77) is configured as SPI0_SCK */
	PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAlt2);            /* PORTC6 (pin 78) is configured as SPI0_SOUT */
	PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAlt2);            /* PORTC7 (pin 79) is configured as SPI0_SIN */
  PORT_SetPinMux(PORTD, PIN2_IDX, kPORT_MuxAsGpio);          /* PORTD2 (pin 95) is configured as PTD2 */
	PORTD->PCR[2] = ((PORTD->PCR[2] &
    (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))               /* Mask bits to zero which are setting */
			| PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
	);
  PORT_SetPinMux(PORTD, PIN3_IDX, kPORT_MuxAsGpio);          /* PORTD3 (pin 96) is configured as PTD3 */
  PORTD->PCR[3] = ((PORTD->PCR[3] &
    (~(PORT_PCR_PE_MASK | PORT_PCR_ISF_MASK)))               /* Mask bits to zero which are setting */
      | PORT_PCR_PE(PCR_PE_ENABLED)                          /* Pull Enable: Internal pullup or pulldown resistor is enabled on the corresponding pin, if the pin is configured as a digital input. */
    );
	PORT_SetPinMux(PORTD, PIN4_IDX, kPORT_MuxAsGpio);          /* PORTD4 (pin 97) is configured as PTD4 */
	PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAsGpio);          /* PORTD5 (pin 98) is configured as PTD5 */
	PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAlt4);            /* PORTE0 (pin 1) is configured as SDHC0_D1 */
	const port_pin_config_t porte0_pin1_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
			kPORT_HighDriveStrength,                                 /* High drive strength is configured */
			kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_D1 */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTE, PIN0_IDX, &porte0_pin1_config);   /* PORTE0 (pin 1) is configured as SDHC0_D1 */
	const port_pin_config_t porte1_pin2_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
			kPORT_HighDriveStrength,                                 /* High drive strength is configured */
			kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_D0 */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTE, PIN1_IDX, &porte1_pin2_config);   /* PORTE1 (pin 2) is configured as SDHC0_D0 */
	const port_pin_config_t porte2_pin3_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
			kPORT_HighDriveStrength,                                 /* High drive strength is configured */
			kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_DCLK */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTE, PIN2_IDX, &porte2_pin3_config);   /* PORTE2 (pin 3) is configured as SDHC0_DCLK */
	const port_pin_config_t porte3_pin4_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
			kPORT_HighDriveStrength,                                 /* High drive strength is configured */
			kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_CMD */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTE, PIN3_IDX, &porte3_pin4_config);   /* PORTE3 (pin 4) is configured as SDHC0_CMD */
	const port_pin_config_t porte4_pin5_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
			kPORT_HighDriveStrength,                                 /* High drive strength is configured */
			kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_D3 */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTE, PIN4_IDX, &porte4_pin5_config);   /* PORTE4 (pin 5) is configured as SDHC0_D3 */
	const port_pin_config_t porte5_pin6_config = {
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
			kPORT_HighDriveStrength,                                 /* High drive strength is configured */
			kPORT_MuxAlt4,                                           /* Pin is configured as SDHC0_D2 */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
	PORT_SetPinConfig(PORTE, PIN5_IDX, &porte5_pin6_config);   /* PORTE5 (pin 6) is configured as SDHC0_D2 */
	PORT_SetPinMux(PORTE, PIN6_IDX, kPORT_MuxAsGpio);          /* PORTE6 (pin 7) is configured as PTE6 */
	SIM->SOPT5 = ((SIM->SOPT5 &
			(~(SIM_SOPT5_UART0TXSRC_MASK)))                          /* Mask bits to zero which are setting */
			| SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART 0 transmit data source select: UART0_TX pin */
	);
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
