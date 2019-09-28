#ifndef NDEBUG

#define unitStr(u)          u==METRIC?"kg":"lb"             /**< Units stringizer.                */

/***********************************************************************************************//**
 * \name Enumerations
 **************************************************************************************************/
/**@{*/
enum msg_t   {NOMSG,                            /**< Message type - no message type               */
              HELP,                             /**< Message type - help information              */
              INFO,                             /**< Message type - general information           */
              STATUS,                           /**< Message type - instrument status             */
              PROMPT,                           /**< Message type - user input prompt             */
              ERROR                             /**< Message type - error notification            */
};
/**@}*/

/***********************************************************************************************//**
 * \brief      Write a message to the serial console.
 * \remarks    This is useful for debugging over the serial link during development. In some cases 
 *             it can remain in the release version as part of the application functionality.
 *
 * \author     JFitter
 * \date       14 Sep 2019
 *
 * \param [in] msgType Type of message to print.
 * \param [in] format  Output format (per c-style printf documentation).
 * \param [in] ...     Variable argument list.
 * \warning    Ensure the output string does not exceed 100 chars.
 **************************************************************************************************/

void ConsoleMsg(msg_t msgType, PGM_P format, ...) {
    char msg[101];
    va_list arglist;

    if(msgType == PROMPT) Serial.print("> ");
    else {
        va_start(arglist, format);
        switch (msgType) {
            case HELP   : Serial.print(F("HELP: "));   break;
            case INFO   : Serial.print(F("INFO: "));   break;
            case STATUS : Serial.print(F("STATUS: ")); break;
            case ERROR  : Serial.print(F("ERROR: "));
        }
        vsnprintf_P(msg, sizeof(msg), format, arglist);
        Serial.println(msg);
        va_end(arglist);
    }
}

/***********************************************************************************************//**
 * \brief      Example 1. How to use ConsoleMsg.
 * \remarks    Writes a program header to the serial output device (console).
 *
 * \author     JFitter
 * \date       24 Sep 2019
 **************************************************************************************************/

void Splash() {

    ConsoleMsg(NOMSG, PSTR("\r\n"));
    ConsoleMsg(INFO,  PSTR("Program:   " _PROGNAME_ " v%u.%u%u"), _PRODUCT_VER, _PRODUCT_MAJ_REV, 
                                                                  _PRODUCT_MIN_REV);
    ConsoleMsg(INFO,  PSTR("Compiled:  " __DATE__ " at " __TIME__ ", AVR-LIBC " \
                                                                __AVR_LIBC_VERSION_STRING__));
    ConsoleMsg(INFO,  PSTR("Product:   " _PRODUCT_ID_ " " _DATE_));
    ConsoleMsg(INFO,  PSTR("Author:    " _AUTHOR_));
    ConsoleMsg(INFO,  PSTR("Copyright: " _COPYRIGHT_ " " _COMPANY_));
    ConsoleMsg(INFO,  PSTR("           All rights reserved.\r\n"));
    ConsoleMsg(PROMPT, 0);
}

/***********************************************************************************************//**
 * \brief      Example 2. How to use ConsoleMsg.
 * \remarks    Process completed incoming messages for monitor operation. 
 *             Messages arrive on the serial port.\n 
 *             With the 8MHz clock the maximum achievable baud rate for error free reception is 38400.
 *
 * \author     JFitter
 * \date       23 Sep 2019
 **************************************************************************************************/

void messageCompleted() {
    
    if (Message.checkString("s")) {
        ConsoleMsg(STATUS, PSTR("Sample %u Weight = %u %s, Vcc = %u mV"), sampleNum,
            convertWeight(SgbAmp.iweight, getUnits()), unitStr(getUnits()), BatteryMV());

    } else if (Message.checkString("t")) {
        SgbAmp.Tare();
        ConsoleMsg(NOMSG, PSTR("Tare set"));

    } else if (Message.checkString("c")) {
        uint16_t i = Message.readInt() * 20;
        SetSpan((i == 0) || (i > MAX_SPAN) ? DEF_SPAN : i);
        ConsoleMsg(NOMSG, PSTR("Span = %u %s"),
            ConvertWeight(GetSpan(), GetUnits()), unitStr(GetUnits()));

    } else if (Message.checkString("u")) {
        if(GetUnits() == METRIC) SetUnits(IMPERIAL);
        else SetUnits(METRIC);
        ConsoleMsg(NOMSG, PSTR("Units = %s"), unitStr(GetUnits()));

    } else if (Message.checkString("g")) {
        ConsoleMsg(NOMSG, PSTR("Units = %s"), unitStr(GetUnits()));
        ConsoleMsg(NOMSG, PSTR("Span  = %u"), GetSpan());

    } else if (Message.checkString("r")) {
        SetUnits(DEF_UNITS);
        SetSpan(DEF_SPAN);
        ConsoleMsg(NOMSG, PSTR("Factory reset done"));

    } else if (Message.checkString("z")) {
        for(int n = EEPROMmemBase(); n < EEPROMmemSize(); n++) EEPROMwriteByte(n, 0);
        ConsoleMsg(NOMSG, PSTR("EEPROM cleared"));

    } else if (Message.checkString("d")) {
        ConsoleMsg(NOMSG, PSTR("EEPROM Hex dump"));
        ConsolePrintHex(EEPROMmemBase(), EEPROMmemSize());

    } else if (Message.checkString("?")) {
        ConsoleMsg(HELP,  PSTR("Monitor Commands (+enter)"));
        ConsoleMsg(HELP,  PSTR("============================"));
        ConsoleMsg(HELP,  PSTR("s       Show status"));
        ConsoleMsg(HELP,  PSTR("t       Set Tare"));
        ConsoleMsg(HELP,  PSTR("c <w>   Calibrate to w kg/lb"));
        ConsoleMsg(HELP,  PSTR("u       Toggle units (kg <> lb)"));
        ConsoleMsg(HELP,  PSTR("g       Get settings"));
        ConsoleMsg(HELP,  PSTR("r       Reset to factory defaults"));
        ConsoleMsg(HELP,  PSTR("z       Clear EEPROM to 0"));
        ConsoleMsg(HELP,  PSTR("d       Dump EEPROM contents"));
        ConsoleMsg(HELP,  PSTR("?       Show this help"));
    }
    ConsoleMsg(PROMPT, 0);
}

#endif // NDEBUG
