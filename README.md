## Snippets and Techniques

This archive is written for the purpose of demonstrating coding techniques for certain classes of problems in addition to offering code snippets demonstrating these techniques.

Most of the code was originally developed in its native environment, being various AVR and ARM processors. These snippets have been ported to the Arduino coding environment to make them useful to a wider audience, however in most cases porting it away from Arduino is quite trivial.

All of the code examples are tried and tested in the author's projects, some of which are commercial products. All work on AVR architectures and most work on ARMs. The list could never be exhaustive and these examples are provided here for any purpose and without warranty. At the very least they are an opportunity to study another author's code and engage in creative thought attempting to improve the code (or prove the author wrong!!).

> A note on coding style. *The author's coding style is very simple. There are many complexities in C++ that lend themselves to exploitation by clever programmers, and they may or may not produce better code. They are certainly able to produce code that is almost impossible to read and figure out what it does or how it works. Hard to find bugs can be the result. If it's PC code then the application goes "belly up" accompanied by much profanity and restarting of the app. If it's embedded code the whole factory explodes destroying the nearby town and killing thousands. The KISS principle reigns.*

### Descriptions of Snippets

1. ***[HexDump.cpp](./HexDump.cpp) A dump of memory in HEX format.***

   Writes CRLF terminated strings to the console. A string is a hex representation of 16 bytes of buffer data each displayed as a two digit ascii hex value followed by a space. Each string is preceeded by the LS 16 bits of the address of the first byte of the string. The string is followed by a 16 character ascii, printable only representation of the buffer data.

   The code is easily modified to enable access to any block of sequential memory. The examples provided show two versions, one accessing a ram memory buffer, and the other accessing eeprom memory.
   
   A (sample) hex data dump is shown here;
```
   0000 0A FF 0B FF 10 FF 0F FF 11 FF 0D FF 11 FF 0E FF  ................
   0010 0C FF 0B FF 12 FF 0F FF 12 FF 0F FF 22 FF 1C FF  ............"...
   0020 1F FF 1B FF 1F FF 1E FF 21 FF 1D FF 1E FF 20 FF  ........!..... .
   0030 21 FF 1D FF 21 FF 33 FF 33 FF 1F FF 22 FF 1E FF  !...!.3.3..."...
   0040 10 FF 0D FF FE FE E1 FE CD FE BD FE 9E FE 7A FE  ..............z.
   0050 6C FE 4D FE 3E FE 2B FE 20 FE 1C FE 30 FE 3A FE  l.M.>.+. ...0.:.
   0060 3D FE 9F FE DD FE 2A FF 8F FF 00 00 6F 00 FE 00  =.....*.....o...
   0070 83 01 20 02 B5 02 47 03 DA 03 68 04 D6 04 4B 05  .. ...G...h...K.
   0080 A7 05 E7 05 1C 06 3E 06 39 06 1C 06 EB 05 AC 05  ......>.9.......
   0090 4D 05 DC 04 67 04 D3 03 44 03 B1 02 24 02 7F 01  M...g...D...$...
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
```

2. ***[ConsoleMsg.cpp](./ConsoleMsg.cpp) Write a message with a variable argument list to the serial console.***
   
   Useful for debugging over the serial link during development. In some cases the code can remain in the release version as part of the application functionality.
   
   Two examples are provided. In the first example, *Splash()*, a simple header is displayed, typically at program start-up, showing program build parameters. The second example, *messageCompleted()*, is more complex and is a complete command handler processing a simple monitor command protocol. 
   
   A sample of the output of *Splash()* is shown here;
```
   INFO: Program:   John's Extraordinary Widget Firmware v1.00
   INFO: Compiled:  Sep 26 2019 at 20:16:33, AVR-LIBC 2.0.0
   INFO: Product:   JEW001 14sep2019
   INFO: Author:    J.W.Maker
   INFO: Copyright: 2019 Thingos Australia Pty. Ltd.
   INFO:            All rights reserved.
   >
```
   This is a sample of the output of *messageCompleted()*;
```
   > ?
   HELP: Monitor Commands (+enter)
   HELP: ============================
   HELP: s       Show status
   HELP: t       Set Tare
   HELP: c <w>   Calibrate to w kg/lb
   HELP: u       Toggle units (kg <> lb)
   HELP: g       Get settings
   HELP: r       Reset to factory defaults
   HELP: z       Clear EEPROM to 0
   HELP: d       Dump EEPROM contents
   HELP: ?       Show this help
   > t
   Tare set
   > u
   Units = lb
   > u
   Units = kg
   > g
   Units = kg
   Span  = 2000
   > r
   Factory reset done
   > s
   STATUS: Sample 97 Weight = 50 kg, Vcc = 3445 mV
   > s
   STATUS: Sample 100 Weight = 50 kg, Vcc = 3447 mV
   > s
   STATUS: Sample 108 Weight = 50 kg, Vcc = 3449 mV
   >
```
1. ***[BattVolts.cpp](./BattVolts.cpp) Return Vcc on an AVR processor.***

   Compares the processor power supply voltage (Vcc) against the internal reference and returns the result in mV. The result is the average of a predefined number of samples. To minimize the power consumption the A/D converter is powered up to take the measurements then shutdown immediately after.
   
   The method used is as follows;
```
   ADC        = Vref * 1024 / AVcc
   where ADC  = A/D converter 10 bit unsigned result
         Vref = Internal reference voltage (1.1V)
         AVcc = Voltage to be measured (battery voltage)

    AVcc [V]  = Vref*1024 / ADC
    AVcc [mV] = 1,126,400 / ADC
```

### Description of Techniques

1. ***Temperature Conversion***

   Because -40&deg;C = -40&deg;F, the equations to convert degrees Celsius to/from degrees Farenheit can be stated as follows;
```
   C = (F + 40) * 5/9 - 40
   F = (C + 40) * 9/5 - 40
```
   This leads to the following very simple temperature conversion function;
```
   #define C_TO_F true
   #define F_TO_C false
   
   template <typename T>
   T ConvTemp(T temp, bool dir) {
       return ((temp + 40) * (dir ? 81 : 25)) / 45 - 40;
   }
```
1. ***Description 2***
1. ***Description 3***

### Author

John Fitter B.E., Eagle Air Australia Pty. Ltd.

### License

All of the provided material in this archive is licensed under the terms of the GNU Lesser General Public License as published by the Free Software Foundation. See the GNU Lesser General Public License for more details at <http://www.gnu.org/copyleft/gpl.html>

