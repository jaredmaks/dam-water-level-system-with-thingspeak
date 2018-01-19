/** mbded library for driving the PMAXIM DS3231 Real Time Clock
* datasheet link : http://datasheets.maximintegrated.com/en/ds/DS3231.pdf
* breakout       : MACETECH ChronoDot V2.1 High Precision RTC
* remi cormier 2012
* WARNING : sda and sdl should be pulled up with 2.2k resistor
*/

/** Example code
* @code
// DS3231 Library test program
// remi cormier 2012

#include "mbed.h"
#include "DS3231.h"

Serial pc(USBTX, USBRX);

int hour;
int minute;
int second;

int dayOfWeek;
int date;
int month;
int year;  
   
DS3231 RTC(p28,p27);


int main()
    {printf("\r\n\nDS3231 Library test program\r\nremi cormier 2012\r\n\n");
    
     RTC.setI2Cfrequency(400000);
    
     //RTC.writeRegister(DS3231_Aging_Offset,0); // uncomment to set Aging Offset 1LSB = approx. 0.1 ppm according from datasheet = 0.05 ppm @ 21 °C from my measurments
     
     RTC.convertTemperature();
      
     int reg=RTC.readRegister(DS3231_Aging_Offset);
     if (reg>127)
        {reg=reg-256;}
     pc.printf("Aging offset : %i\r\n",reg);
         
     pc.printf("OSF flag : %i",RTC.OSF());
     pc.printf("\r\n");
     
     RTC.readDate(&date,&month,&year);
     pc.printf("date : %02i-%02i-%02i",date,month,year);
     pc.printf("\r\n");
     
     //RTC.setTime(19,48,45); // uncomment to set time
     
     RTC.readTime(&hour,&minute,&second);
     pc.printf("time : %02i:%02i:%02i",hour,minute,second);
     pc.printf("\r\n");
     
     //RTC.setDate(6,22,12,2012); // uncomment to set date
     
     RTC.readDateTime(&dayOfWeek,&date,&month,&year,&hour,&minute,&second);
     pc.printf("date time : %i / %02i-%02i-%02i %02i:%02i:%02i",dayOfWeek,date,month,year,hour,minute,second);
     pc.printf("\r\n");
     
     pc.printf("temperature :%6.2f",RTC.readTemp());
     pc.printf("\r\n");
    }
* @endcode
*/


#include "mbed.h"

#ifndef MBED_DS3231_H
#define MBED_DS3231_H

//DS3231 8 bit adress
#define DS3231_Address          0xD0

//DS3231 registers
#define DS3231_Seconds          0x00
#define DS3231_Minutes          0x01
#define DS3231_Hours            0x02
// DS3231 Hours bits
#define DS3231_bit_AM_PM        0x20
#define DS3231_bit_12_24        0x40

#define DS3231_Day              0x03
#define DS3231_Date             0x04
#define DS3231_Month_Century    0x05
#define DS3231_Year             0x06
#define DS3231_Alarm1_Seconds   0x07
#define DS3231_Alarm1_Minutes   0x08
#define DS3231_Alarm1_Hours     0x09
#define DS3231_Alarm1_Day_Date  0x0A
#define DS3231_Alarm2_Minutes   0x0B
#define DS3231_Alarm2_Hours     0x0C
#define DS3231_Alarm_2_Day_Date 0x0D

#define DS3231_Control          0x0E
// DS3231 Control bits
#define DS3231_bit_A1IE        1
#define DS3231_bit_A2IE        2
#define DS3231_bit_INTCN       4
#define DS3231_bit_SQW_1Hz     0
#define DS3231_bit_SQW_1024Hz  8
#define DS3231_bit_SQW_4096Hz 16
#define DS3231_bit_SQW_8192Hz 24
#define DS3231_bit_CONV       32
#define DS3231_bit_BBSQW      64
#define DS3231_bit_EOSCb     128


#define DS3231_Control_Status   0x0F
// DS3231 Control/Status bits
#define DS3231_bit_BSY     0x04
#define DS3231_bit_EN32kHz 0x08
#define DS3231_bit_OSF     0x80

#define DS3231_Aging_Offset     0x10
#define DS3231_MSB_Temp         0x11
#define DS3231_LSB_Temp         0x12

/* Interface to MAXIM DS3231 RTC */
class DS3231
    {public :
     /** Create an instance of the DS3231 connected to specfied I2C pins
     *
     * @param sda The I2C data pin
     * @param scl The I2C clock pin
     */
     DS3231(PinName sda, PinName scl);
     
     /** set I2C bus speed
     * @param frequency : I2C clocl frequenct (Hz)
     */
     void setI2Cfrequency(int frequency);
     
     /** Read the temperature
     *
     * @return The temperature
     */
     float readTemp();
     
     /** Read the time registers
     * @param hours
     * @param minutes
     * @param seconds
     */
     void readTime(int *hours, int *minutes, int *seconds);
     
     /** force temperature conversion
     * 
     */
     void convertTemperature();
     
     /** Set the time registers
     * @param hours
     * @param minutes
     * @param seconds
     */
     void setTime(int hours, int minutes, int seconds);
     
     /** Read the date registers
     * @param date
     * @param month
     * @param year
     */
     void readDate(int *date, int *month, int *year);
     
     /** Set the date registers
     * @param dayOfWeek : day of week
     * @param date
     * @param month
     * @param year
     */
     void setDate(int dayOfWeek, int date, int month, int year);
     
     /** Read the date and time registers
     * @param dayOfWeek : day of week
     * @param date
     * @param month
     * @param year
     * @param hours
     * @param minutes
     * @param seconds
     */
     void readDateTime(int *dayOfWeek, int *date, int *month, int *year, int *hours, int *minutes, int *seconds);
     
     /** Read a register
     * @param reg : register address
     * @return The register content
     */     
     int readRegister(char reg);
     
     /** Write to a register
     * @param reg : register address
     * @param The register content
     */       
     void writeRegister(int reg,char byte);
     
     /** set OSF (Oscillator Stop Flag) bit to 0 in Control Status register
     * should be done just after power up DS3231
     * OSF bit is automaticaly set to 1 when on power up or when the DS3231 oscillator stops
     */
     void eraseOSF();
     
     /** Return OSF bit. If true the oscillator stopped or the DS3231 just powered up
     * @return The OSF bit
     */
     bool OSF();
     
     bool error;
     
     private :
     I2C i2c;
     int bcd2dec(int k); // bcd to decimal conversion
     int dec2bcd(int k); // decimal to bcd conversion
     void decodeTime(int regHours, int regMinutes, int regSeconds,int *Hours, int *Minutes, int *Seconds);
     void decodeDate(int regDate,int regMonth, int regYear, int *date, int *month, int *year);
    };


#endif