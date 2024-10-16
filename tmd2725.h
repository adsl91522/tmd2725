#ifndef _tmd2725_h_
#define _tmd2725_h_

#include <zephyr/drivers/i2c.h>


/*  Register definitions  */
#define ENABLE                          0x80
#define ATIME                           0x81
#define PRATE                           0x82
#define WTIME                           0x83
#define AILTL                           0x84
#define AILTH                           0x85
#define AIHTL                           0x86
#define AIHTH                           0x87
#define PILT                            0x88
#define PIHT                            0x8A
#define PERS                            0x8C
#define CFG0                            0x8D
#define PCFG0                           0x8E
#define PCFG1                           0x8F
#define CFG1                            0x90
#define REVID                           0x91
#define ID                              0x92
#define STATUS                          0x93
#define PHOTOPICL                       0x94
#define PHOTOPICH                       0x95
#define ALS_IRL                         0x96
#define ALS_IRH                         0x97
#define PDATA                           0x9C
#define CFG2                            0x9F
#define CFG3                            0xAB
#define POFFSET_L                       0xC0
#define POFFSET_H                       0xC1
#define CALIB                           0xD7
#define CALIBCFG                        0xD9
#define CALIBSTAT                       0xDC
#define INTENAB                         0xDD
#define POFFSET_MAG                     0xE6
#define POFFSET_SIGN                    0xE7

/*  ENABLE_REG @0x80  */
#define PON                             0x01   
#define AEN                             0x02  
#define PEN                             0x04    
#define WEN                             0x08
#define ENABLE_MASK            0x00

/*  CFG0_REG @0x8D  */
#define WLONG                           0x04     

/*  PCFG0_REG @0x8E  */
#define PPLEN_4US                       0x00
#define PPLEN_8US                       0x40
#define PPLEN_16US                      0x80
#define PPLEN_32US                      0xC0

/*  PCFG1_REG @0x8F  */
#define PGAIN_1X                        0x00
#define PGAIN_2X                        0x40
#define PGAIN_4X                        0x80
#define PGAIN_8X                        0xC0

/*  CFG1_REG @0x90  */
#define AGAIN_1X                        0x00
#define AGAIN_4X                        0x01
#define AGAIN_16X                       0x02
#define AGAIN_64X                       0x03

/*  CFG2_REG @0x9F  */
#define GAINMAX                         0x00
#define AGAINL                          0x01

/*  CFG3_REG @0xAB  */
#define SAI                             0x10
#define INT_READ_CLEAR                  0x80 

/*  CALIB_REG @0xD7  */
#define START_OFFSET_CALIB              0x01      
#define ELECTRICAL_CALIBRATION          0x20        

/*  CALIB_CFG_REG @0xD9  */ 
#define PROX_DATA_AVG_DISABLE           0x00
#define PROX_DATA_AVG_2                 0x01
#define PROX_DATA_AVG_4                 0x02
#define PROX_DATA_AVG_8                 0x03    
#define PROX_DATA_AVG_16                0x04
#define PROX_DATA_AVG_32                0x05
#define PROX_DATA_AVG_64                0x06
#define PROX_DATA_AVG_128               0x07
#define AUTO_OFFSET_ADJ                 0x08 
#define BINSRCH_TARGET_0                0x00
#define BINSRCH_TARGET_1                0x20  
#define BINSRCH_TARGET_3                0x40    
#define BINSRCH_TARGET_7                0x60
#define BINSRCH_TARGET_15               0x80
#define BINSRCH_TARGET_31               0xA0
#define BINSRCH_TARGET_63               0xC0
#define BINSRCH_TARGET_127              0xF0  

/*  INTENAB_REG @0xDD  */  
#define CIEN                            0x08   
#define AIEN                            0x10   
#define PIEN                            0x20    
#define PSIEN                           0x40   
#define ASIEN                           0x80   

/**
 * @brief: Enables states and interrupts
 * @param: Follow the ENABLE_REG
 * @direction: R/W
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 write or read error
 *
 * @note1: Before activating AEN or PEN, preset each applicable operating mode registers and bits.
 * @note2: Set AEN=1 and PON=1 in the same command to ensure auto-zero function is run prior to 
 *         the first measurement.
 */
int tmd2725_enable(uint8_t en_flag);

/**
 * @brief: ADC integration time
 *
 *  The ATIME value specifies the ALS integration time in 2.81ms intervals
 *
 * @param: value | integration cycles | integreation time | max ALS
 *         0x00  |          1         |        2.8ms      |  1023
 *         0x01  |          2         |        5.6ms      |  2047
 *         0x11  |         18         |         50ms      |  18431
 *         0x23  |         36         |        108ms      |  36863
 *         0x3F  |         64         |        180ms      |  65535
 *         0xFF  |        256         |        719ms      |  65535
 *
 * @direction: R/W
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 write or read error
 */
int tmd2725_atime(uint8_t value);

/**
 * @brief: Proximity time
 * @param: This register defines the duration of 1 Prox Sample, which is (PRATE + 1) * 88ms
 * @direction: R/W
 * @Reset value: 0x1F
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 */
int tmd2725_prate(uint8_t value);

/**
 * @brief: This register sets the proximity threshold.
 *
 * If the value generated by the proximity channel is below the PILT threshold or
 * over the PIHTL/H threshold and the PPERS value is reached, the PINT bit is asserted.
 * If PIEN is set, then the INT pin will also assert.
 *
 * @param: This register sets the Proximity ADC channel high threshold.
 * @param: This register sets the Proximity ADC channel low threshold.
 * @direction: R/W
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 */
int tmd2725_prox_threshold(uint8_t piht, uint8_t pilt);

/**
 * @brief: ALS and proximity interrupt persistence filters.
 *
 * The frequency of consecutive proximity or ALS (CH0) channel results outside of threshold limits are counted;
 * This count value is compared against the PPERS or APERS value.
 * If the counter is equal to the PPERS or APERS value an interrupt is asserted.
 * Any time a proximity channel result is inside the threshold values the counter is cleared.
 *
 * @param: ppers register sets the proximity persistence filter. Parameter range 0 to 15.
 *         value | description
 *           0   | Every proximity cycle
 *           1   | Any value outside proximity thresholds
 *           2   | 2 consecutive proximity values out of range
 *           3   | 3 consecutive proximity values out of range
 *          ...  | ...
 *           15  | 15 consecutive proximity values out of range
 *
 * @param: apers register sets the ALS persistence filter. Parameter range 0 to 15.
 *         value | description
 *           0   | Every ALS cycle
 *           1   | Any value outside ALS thresholds
 *           2   | 2 consecutive ALS values out of range
 *           3   | 3 consecutive ALS values out of range
 *           4   | 5 consecutive ALS values out of range
 *           5   | 10 consecutive ALS values out of range
 *           6   | 15 consecutive ALS values out of range
 *          ...  | ...
 *           14  | 55 consecutive ALS values out of range
 *           15  | 60 consecutive ALS values out of range
 *
 * @direction: R/W
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 */
int tmd2725_pers(uint8_t ppers, uint8_t apers);

/**
 * @brief: Proximity configuration zero.
 *
 * @param: Proximity Pulse Length
 *         Follow the PCFG0_REG
 * @param: Maximum Number of Pulses in a single proximity cycle. Parameter range is 1 to 64.
 * @direction: R/W
 * @Reset value: 0x4F
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 */
int tmd2725_pcfg0(uint8_t ppulse_len, uint8_t ppulse);

/**
 * @brief: Proximity configuration one.
 *
 * @param: This field sets the gain of the proximity IR sensor.
 *         Follow the PCFG1_REG
 * @param: This field sets the drive strength of the IR LED current. Values are approximate; 
 *         actual current through LED is factory trimmed to normalize IR intensity.
 *         Parameter range 0 to 31, LED_Current = 6(PLDRIVE+1)mA
 *
 * @direction: R/W
 * @Reset value: 0x80
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 */
int tmd2725_pcfg1(uint8_t pgain, uint8_t pldrive);

/**
 * @brief: Configuration one
 *
 * @param: This field sets the gain of the ALS sensor. 
 *         Follow the CFG1_REG.
 * @direction: R/W
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 */
int tmd2725_cfg1(uint8_t again);

/**
 * @brief: Device status.
 *
 * bit |     bit name    | bit descript
 *  0  |   PSAT_AMBIENT  | The Ambient Proximity Saturation Interrupt flag signals that the AFE has saturated during the IR VCSEL inactive portion of proximity integration.
 *  1  | PSAT_REFLECTIVE | The Reflective Proximity Saturation Interrupt flag signals that the AFE has saturated during the IR VCSEL active portion of proximity integration.
 *  2  |     Reserved    | Reserved
 *  3  |       CINT      | Calibration interrupt flag indicates that calibration has completed.
 *  4  |       AINT      | ALS interrupt flag indicates that ALS results (CH0) have exceeded thresholds and persistence settings.
 *  5  |       PINT      | Proximity interrupt flag indicates that proximity results have exceeded thresholds and persistence settings.
 *  6  |       PSAT      | Proximity saturation flag indicates that an ambient-or reflective-saturation event occurred during a previous proximity cycle.
 *  7  |       ASAT      | Analog saturation flag signals that the ALS results may be unreliable due to saturation of the AFE.
 *
 * @param: Buffer pointer to storage for read data.
 *
 * @direction: R, SC
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 *
 * @note: All flags in this register can be cleared by setting the bit high.
          Alternatively, if the INT_READ_CLEAR in the CFG3 register bit is set,
          then simply reading this register automatically clears all eight flags.
 */
int tmd2725_status_reg(uint8_t *buf);

/**
 * @brief: This register contains the 16-bit photopic channel data.
 *
 * @param: Buffer pointer to storage for read data.
 *
 * @direction: R
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 */
int tmd2725_photopic(uint16_t *photopic_data);

/**
 * @brief: This register contains the 16-bit IR channel data.
 *
 * @param: Buffer pointer to storage for read data.
 *
 * @direction: R
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 */
int tmd2725_als_ir(uint16_t *ir_data);

/**
 * @brief: This register contains the 8-bit proximity channel data.
 *
 * @param: Buffer pointer to storage for read data.
 *
 * @direction: R
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 */
int tmd2725_pdata(uint8_t *pdata);

/**
 * @brief: Configuration three
 *
 * @param: Follow the CFG3_REG
 *
 * @direction: R/W
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 *
 * @note: The SAI bit sets the device operational mode following the completion of an ALS or proximity cycle.
 *        If AINT and AIEN are both set or if PINT and PIEN are both set, causing an interrupt on the INT pin,
 *        and the SAI bit is set, then the oscillator will deactivate. The device will appear as if PON = 0,
 *        however, PON will read as 1. The device can only be reactivated (oscillator enabled) by clearing
 *        the interrupts in the STATUS register.
 */
int tmd2725_cfg3(uint8_t cfg3_flag);

/**
 * @brief: Proximity offset data, This register contains the magnitude portion of proximity offset adjust value.
 *
 * @param: poffset The POFFSET controls shift PDATA to remove crosstalk from the proximity data.
 *         The values range from -255 to 255. This value can be set manually, or it can be set by the Proximity offset calibration.
 *
 * @direction: R/W
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 *
 * @note: Typically, optical and/or electrical crosstalk negatively influence proximity operation and results.
 *        The POFFSETL/POFFSETH registers provide a mechanism to remove system crosstalk from the proximity data.
 *        POFFSETL and POFFSETH contains the magnitude and sign of a value which adjusts PDATA is generated in the AFE.
 *        An offset value in the range of +-255 is possible.
 */
int tmd2725_poffset(int poffset);

/**
 * @brief: Interrupt enables.
 *
 * @param: Follow the INTENAB_REG.
 * @direction: R/W
 * @Reset value: 0x00
 *
 * @retval: 0 is successful
 * @retval: -1 is write or read error
 */
int tmd2725_intenab(uint8_t isr_flag);

/**
 * @brief Bind I2c to tmd2725
 *
 * @param i2c Pointer to the device structure for the driver instance.
 */
void tmd2725_i2c_init(const struct device *i2c);

#endif
