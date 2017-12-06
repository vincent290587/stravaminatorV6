#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/* PORTC2 (number 72), SHARP_CS */
#define BOARD_INITPINS_SHARP_CS_PERIPHERAL                                  SPI0   /*!< Device name: SPI0 */
#define BOARD_INITPINS_SHARP_CS_SIGNAL                                      PCS2   /*!< SPI0 signal: PCS2 */
#define BOARD_INITPINS_SHARP_CS_PIN_NAME                               SPI0_PCS2   /*!< Pin name */
#define BOARD_INITPINS_SHARP_CS_LABEL                                 "SHARP_CS"   /*!< Label */
#define BOARD_INITPINS_SHARP_CS_NAME                                  "SHARP_CS"   /*!< Identifier name */

/* PORTC0 (number 70), SPI_CS0 */
#define BOARD_INITPINS_SPI_CS0_PERIPHERAL                                   SPI0   /*!< Device name: SPI0 */
#define BOARD_INITPINS_SPI_CS0_SIGNAL                                       PCS4   /*!< SPI0 signal: PCS4 */
#define BOARD_INITPINS_SPI_CS0_PIN_NAME                                SPI0_PCS4   /*!< Pin name */
#define BOARD_INITPINS_SPI_CS0_LABEL                                   "SPI_CS0"   /*!< Label */
#define BOARD_INITPINS_SPI_CS0_NAME                                    "SPI_CS0"   /*!< Identifier name */

/* PORTB21 (number 67), INT1 */
#define BOARD_INITPINS_INT1_GPIO                                           GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITPINS_INT1_PORT                                           PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITPINS_INT1_GPIO_PIN                                         21U   /*!< PORTB pin index: 21 */
#define BOARD_INITPINS_INT1_PIN_NAME                                       PTB21   /*!< Pin name */
#define BOARD_INITPINS_INT1_LABEL                                         "INT1"   /*!< Label */
#define BOARD_INITPINS_INT1_NAME                                          "INT1"   /*!< Identifier name */
#define BOARD_INITPINS_INT1_DIRECTION                    kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTB22 (number 68), PPS */
#define BOARD_INITPINS_PPS_GPIO                                            GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITPINS_PPS_PORT                                            PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITPINS_PPS_GPIO_PIN                                          22U   /*!< PORTB pin index: 22 */
#define BOARD_INITPINS_PPS_PIN_NAME                                        PTB22   /*!< Pin name */
#define BOARD_INITPINS_PPS_LABEL                                           "PPS"   /*!< Label */
#define BOARD_INITPINS_PPS_NAME                                            "PPS"   /*!< Identifier name */
#define BOARD_INITPINS_PPS_DIRECTION                     kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTC3 (number 73), CS_NRF */
#define BOARD_INITPINS_CS_NRF_PERIPHERAL                                    SPI0   /*!< Device name: SPI0 */
#define BOARD_INITPINS_CS_NRF_SIGNAL                                        PCS1   /*!< SPI0 signal: PCS1 */
#define BOARD_INITPINS_CS_NRF_PIN_NAME                                 SPI0_PCS1   /*!< Pin name */
#define BOARD_INITPINS_CS_NRF_LABEL                                     "CS_NRF"   /*!< Label */
#define BOARD_INITPINS_CS_NRF_NAME                                      "CS_NRF"   /*!< Identifier name */

/* PORTC1 (number 71), SST_CS */
#define BOARD_INITPINS_SST_CS_PERIPHERAL                                    SPI0   /*!< Device name: SPI0 */
#define BOARD_INITPINS_SST_CS_SIGNAL                                        PCS3   /*!< SPI0 signal: PCS3 */
#define BOARD_INITPINS_SST_CS_PIN_NAME                                 SPI0_PCS3   /*!< Pin name */
#define BOARD_INITPINS_SST_CS_LABEL                                     "SST_CS"   /*!< Label */
#define BOARD_INITPINS_SST_CS_NAME                                      "SST_CS"   /*!< Identifier name */

/* PORTB23 (number 69), 3D */
#define BOARD_INITPINS_PIN_3D_GPIO                                         GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITPINS_PIN_3D_PORT                                         PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITPINS_PIN_3D_GPIO_PIN                                       23U   /*!< PORTB pin index: 23 */
#define BOARD_INITPINS_PIN_3D_PIN_NAME                                     PTB23   /*!< Pin name */
#define BOARD_INITPINS_PIN_3D_LABEL                                         "3D"   /*!< Label */
#define BOARD_INITPINS_PIN_3D_NAME                                      "PIN_3D"   /*!< Identifier name */
#define BOARD_INITPINS_PIN_3D_DIRECTION                  kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTB20 (number 66), INT2 */
#define BOARD_INITPINS_INT2_GPIO                                           GPIOB   /*!< GPIO device name: GPIOB */
#define BOARD_INITPINS_INT2_PORT                                           PORTB   /*!< PORT device name: PORTB */
#define BOARD_INITPINS_INT2_GPIO_PIN                                         20U   /*!< PORTB pin index: 20 */
#define BOARD_INITPINS_INT2_PIN_NAME                                       PTB20   /*!< Pin name */
#define BOARD_INITPINS_INT2_LABEL                                         "INT2"   /*!< Label */
#define BOARD_INITPINS_INT2_NAME                                          "INT2"   /*!< Identifier name */
#define BOARD_INITPINS_INT2_DIRECTION                    kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTD5 (number 98), USB_PRES */
#define BOARD_INITPINS_USB_PRES_GPIO                                       GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITPINS_USB_PRES_PORT                                       PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITPINS_USB_PRES_GPIO_PIN                                      5U   /*!< PORTD pin index: 5 */
#define BOARD_INITPINS_USB_PRES_PIN_NAME                                    PTD5   /*!< Pin name */
#define BOARD_INITPINS_USB_PRES_LABEL                                 "USB_PRES"   /*!< Label */
#define BOARD_INITPINS_USB_PRES_NAME                                  "USB_PRES"   /*!< Identifier name */
#define BOARD_INITPINS_USB_PRES_DIRECTION                kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTD4 (number 97), FXOS_RST */
#define BOARD_INITPINS_FXOS_RST_GPIO                                       GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITPINS_FXOS_RST_PORT                                       PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITPINS_FXOS_RST_GPIO_PIN                                      4U   /*!< PORTD pin index: 4 */
#define BOARD_INITPINS_FXOS_RST_PIN_NAME                                    PTD4   /*!< Pin name */
#define BOARD_INITPINS_FXOS_RST_LABEL                                 "FXOS_RST"   /*!< Label */
#define BOARD_INITPINS_FXOS_RST_NAME                                  "FXOS_RST"   /*!< Identifier name */
#define BOARD_INITPINS_FXOS_RST_DIRECTION                kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTD3 (number 96), BTN_1 */
#define BOARD_INITPINS_BTN_1_GPIO                                          GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITPINS_BTN_1_PORT                                          PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITPINS_BTN_1_GPIO_PIN                                         3U   /*!< PORTD pin index: 3 */
#define BOARD_INITPINS_BTN_1_PIN_NAME                                       PTD3   /*!< Pin name */
#define BOARD_INITPINS_BTN_1_LABEL                                       "BTN_1"   /*!< Label */
#define BOARD_INITPINS_BTN_1_NAME                                        "BTN_1"   /*!< Identifier name */
#define BOARD_INITPINS_BTN_1_DIRECTION                   kPIN_MUX_DirectionInput   /*!< Direction */

/* PORTD2 (number 95), BTN_2 */
#define BOARD_INITPINS_BTN_2_GPIO                                          GPIOD   /*!< GPIO device name: GPIOD */
#define BOARD_INITPINS_BTN_2_PORT                                          PORTD   /*!< PORT device name: PORTD */
#define BOARD_INITPINS_BTN_2_GPIO_PIN                                         2U   /*!< PORTD pin index: 2 */
#define BOARD_INITPINS_BTN_2_PIN_NAME                                       PTD2   /*!< Pin name */
#define BOARD_INITPINS_BTN_2_LABEL                                       "BTN_2"   /*!< Label */
#define BOARD_INITPINS_BTN_2_NAME                                        "BTN_2"   /*!< Identifier name */
#define BOARD_INITPINS_BTN_2_DIRECTION                   kPIN_MUX_DirectionInput   /*!< Direction */


/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/*******************************************************************************
 * EOF
 ******************************************************************************/
