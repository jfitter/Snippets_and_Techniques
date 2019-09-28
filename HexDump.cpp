/***********************************************************************************************//**
 * \brief   Dump RAM buffer contents in hex to the console.
 *
 * \remarks Writes crlf terminated strings to the console. A string is a hex representation of 16
 *          bytes of buffer data each displayed as a two digit ascii hex value followed by a
 *          space. Each string is followed by 16 character ascii, printable only representation
 *          of the buffer data.<BR><BR>
 *          A (sample) hex data dump is shown here;
 *
 * \verbatim
    0000 0A FF 0B FF 10 FF 0F FF 11 FF 0D FF 11 FF 0E FF  ................
    0010 0C FF 0B FF 12 FF 0F FF 12 FF 0F FF 22 FF 1C FF  ............"...
    0020 1F FF 1B FF 1F FF 1E FF 21 FF 1D FF 1E FF 20 FF  ........!..... .
    0030 21 FF 1D FF 21 FF 33 FF 33 FF 1F FF 22 FF 1E FF  !...!.3.3..."...
    0040 10 FF 0D FF FE FE E1 FE CD FE BD FE 9E FE 7A FE  ..............z.
    0050 6C FE 4D FE 3E FE 2B FE 20 FE 1C FE 30 FE 3A FE  l.M.>.+. ...0.:.
    0060 3D FE 9F FE DD FE 2A FF 8F FF 00 00 6F 00 FE 00  =.....*.....o...
    0070 83 01 20 02 B5 02 47 03 DA 03 68 04 D6 04 4B 05  .. ...G...h...K.
    0080 A7 05 E7 05 1C 06 3E 06 39 06 1C 06 EB 05 AC 05  ......>.9.......
    0090 4D 05 DC 04 67 04 D3 03 44 03 B1 02 24 02 7F 01  M...g...D...$...
    00A0 FE 00 6D 00 03 00 92 FF 2D FF E0 FE 9A FE 6C FE  ..m.....-.....l.
    00B0 39 FE 2E FE 1D FE 1A FE 31 FE 3A FE 49 FE 6F FE  9.......1.:.I.o.
    00C0 7B FE 9A FE BC FE CB FE DC FE 00 FF 12 FF 10 FF  {...............
    00D0 1E FF 21 FF 1E FF 31 FF 2B FF 1F FF 22 FF 1E FF  ..!...1.+..."...
    00E0 20 FF 1B FF 1F FF 1B FF 1F FF 20 FF 21 FF 1D FF   ......... .!...
    00F0 1A FF 1D FF 0D FF 0C FF 11 FF 10 FF 0C FF 0D FF  ................
    0100 11 FF 0D FF 0D FF 0F FF 12 FF 0F FF 0D FF 0B FF  ................
    0110 0C FF 0F FF 0B FF 0D FF 10 FF 12 FF 10 FF 0B FF  ................
    0120 0D FF 0C FF 0E FF 0C FF 0E FF 0F FF 0E FF 0E FF  ................
    0130 10 FF 0B FF 0E FF 0D FF 11 FF 0D FF 0C FF 0D FF  ................
    0140 0F FF 0B FF 10 FF 0D FF 0D FF 0B FF 0D FF 0D FF  ................
    0150 10 FF 11 FF 0C FF 12 FF 10 FF 0C FF 0D FF 11 FF  ................
    0160 11 FF 0B FF 0E FF 0F FF                          ........
   \endverbatim
 *
 * \author  Jfitter
 * \date    11 Nov 2018
 *
 * \param [in]  inbuf   Pointer to input binary buffer.
 * \param [in]  count   Count of bytes to write.
 **************************************************************************************************/

void HexRamDump(uint8_t *inbuf, uint16_t count) {
    char hex[4], buf[17], *pbuf;
    uint8_t byt;
    uint16_t cnt;

    while(count) {
        snprintf(buf, 6, "%04X ", (uint16_t)(inbuf & 0xffff)); 
        Serial.write(buf);

        pbuf = buf;
        cnt = 1 << 15;
        while(cnt) {
            cnt >>= 1;
            if(count) {
                byt = *(inbuf++);
                count--;
                snprintf(hex, sizeof(hex), "%02X ", byt);
                Serial.write(hex);
            } else {
                byt = ' ';
                Serial.write("   ");
            }
            *(pbuf++) = byt < 0x20 || byt > 0x7E ? '.' : byt;
            // Note: 7F is ignored because the html renderer will not print it and for Latex output
            // the Tex compiler chokes on it and raises an error.
        }
        *pbuf = 0;
        Serial.write(' ');
        Serial.write(buf);
        CRLF();
    }
}

/***********************************************************************************************//**
 * \brief   Dump EEPROM memory contents in hex to the console.
 *
 * \author  Jfitter
 * \date    11 Nov 2018
 *
 * \param [in]  saddr   Start address in EEPROM of data to list.
 * \param [in]  count   Count of bytes to list.
 **************************************************************************************************/

void HexEEPROMDump(uint16_t saddr, uint16_t count) {

    char     hex[4];                                            // Byte string (per byte)
    char     buf[17];                                           // ASCII string (per line)
    char*    pbuf;                                              // Pointer to ASCII string
    uint8_t  byt;                                               // Byte to print
    uint16_t cnt;                                               // Byte counter (per line)

    while(count) {                                              // Loop for all bytes
        snprintf(buf, 6, "%04X ", (uint16_t)(saddr & 0xffff));  // Print the address in hex
        Serial.write(buf);

        pbuf = buf;                                             // Get a pointer to ASCII string
        cnt = 1 << 15;                                          // Load the bit-shift byte counter
        while(cnt) {                                            // Loop for all bytes in a line
            cnt >>= 1;                                          // Update the bit-shift counter
            if(count) {                                         // Are there any bytes left to print
                byt =  EEPROM.readByte(saddr++);                // Yes, then get the byte
                count--;                                        // Update the total byte counter
                snprintf(hex, 4, "%02X ", byt);                 // Print the byte in hex
                Serial.write(hex);
            } else {                                            // No, there are no more bytes
                byt = ' ';                                      // then create a blank byte
                Serial.write("   ");                            // and write a space
            }
            *(pbuf++) = byt < 0x20 || byt > 0x7E ? '.' : byt;   // Assemble the ASCII byte string
            // Note: 7F is ignored because the html renderer
            // will not print it and for Latex output the Tex
            // compiler chokes on it and raises an error.
        }
        *pbuf = 0;                                              // Add a NULL terminator then
        Serial.write(' ');                                      // write a space then
        Serial.write(buf);                                      // write the ASCII byte string
        Serial.write("\r\n");                                   // followed by a newline
    }                                                           // and go back for another 16 bytes
}
