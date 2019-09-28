/***********************************************************************************************//**
 * \brief      Read the battery voltage.
 * \remarks    Compares the battery voltage (\f$AV_{cc}\f$) against the internal reference and
 *             returns the result in mV. The result is the average of NSAMP_BATT_MV samples.
 *             To minimize the power consumption the A/D converter is powered up to take the
 *             measurements then shutdown immediately after.
 * \par
 *             \f{eqnarray*}{
 *                         ADC  &=& V_{ref} \times \frac{1024}{AV_{cc}} \\
 *                where \; ADC  &=& A/D \; converter \; 10 \; bit \; unsigned \; result \\
 *                      V_{ref} &=& Internal \; reference \; voltage \; (1.1V) \\
 *                      AV_{cc} &=& Voltage \; to \; be \; measured \; (battery \; voltage) \\ \\
 *                      AV_{cc} &=& V_{ref} \times \frac{1024}{ADC} \\
 *                              &=& \frac{1,126,400}{ADC} \; mV
 *             \f}
 * \author     JFitter
 * \date       18 Sep 2019
 *
 * \return     The battery voltage in mV.
 **************************************************************************************************/

//  ADC        = Vref * 1024 / AVcc
//  where ADC  = A/D converter 10 bit unsigned result
//        Vref = Internal reference voltage (1.1V)
//        AVcc = Voltage to be measured (battery voltage)

//  AVcc [V]   = Vref*1024 / ADC
//  AVcc [mV]  = 1,126,400 / ADC

#define NSAMP_BATT_MV       100                     /**< Number of samples to get for average.    */

uint16_t BatteryMV(void) {
    uint32_t adcResult = 0;

    power_adc_enable();                             // Power-up ADC just for this conversion.
    ADMUX  = (1 << REFS0) | (1 << MUX4) |           // ADC multiplexer selection register.
             (1 << MUX3)  | (1 << MUX2) |           // AVCC is the reference and VREF is the
             (1 << MUX1);                           // measured value.
    ADCSRA = (1 << ADEN)  | (1 << ADPS2) |          // Enable the ADC and set the clock to
             (1 << ADPS1);                          // clk/64 = 125kHz for best resolution.
    delayMicroseconds(100);                         // Bandgap reference max. startup time 70us.

    for(uint8_t n = 0; n < NSAMP_BATT_MV; n++) {
        ADCSRA |= (1 << ADSC);                      // Set ADSC bit to trigger a conversion.
        while (ADCSRA & (1 << ADSC));               // Wait for the conversion to finish.
        adcResult += (ADCL | (ADCH << 8));          // Accumulate the result.
    }
    ADCSRA = 0;                                     // Disable the ADC for lowest power consumption.
    power_adc_disable();                            // Power-down ADC ready for sleep mode.
    return (1126400L * NSAMP_BATT_MV) / adcResult;  // Return the average converted to mV.
}
