#include "DS3231.h"

DS3231::DS3231(PinName sda, PinName scl) : i2c(sda, scl)
    {
    } 
    
    
// BCD to decimal conversion    
int DS3231::bcd2dec(int k)
    {return(((k&0xF0)>>4)*10+(k&0x0F));}
    
    
// decimal to BCD conversion    
int DS3231::dec2bcd(int k)
    {return((k/10)*16+(k%10));}      
    
void DS3231::setI2Cfrequency(int frequency)
    {i2c.frequency(frequency);
    }
    
// read temperature from DS3231    
float DS3231::readTemp()
    {char buf[2];
     buf[0]=DS3231_MSB_Temp;
     int w=i2c.write(DS3231_Address,buf,1);
     int r=i2c.read(DS3231_Address,buf,2);
     error=((w!=0)||(r!=0));   
     return(buf[0]+buf[1]/256.0);}  

// force temperatue conversion     
void DS3231::convertTemperature()
    {int Control=readRegister(DS3231_Control)|DS3231_bit_CONV;
     writeRegister(DS3231_Control,Control);
    }

// decode time registers     
void DS3231::decodeTime(int regHours, int regMinutes, int regSeconds,int *Hours, int *Minutes, int *Seconds)
    {*Hours=bcd2dec(regHours);
     *Minutes=bcd2dec(regMinutes);
     *Seconds=bcd2dec(regSeconds);
    }      
     
     
// read time registers dand decode them to decimal     
void DS3231::readTime(int *hours, int *minutes, int *seconds)
    {char buf[3];
     buf[0]=DS3231_Seconds;
     int w=i2c.write(DS3231_Address,buf,1);
     int r=i2c.read(DS3231_Address,buf,3);
     decodeTime(buf[2],buf[1],buf[0],hours,minutes,seconds);    
     error=((w!=0)||(r!=0));
    }  
    
// set time register    
void DS3231::setTime(int hours, int minutes, int seconds)
    {writeRegister(DS3231_Seconds,dec2bcd(seconds));
     writeRegister(DS3231_Minutes,dec2bcd(minutes));
     writeRegister(DS3231_Hours,dec2bcd(hours));
    }    
    
// decode date registers
void DS3231::decodeDate(int regDate,int regMonth, int regYear, int *date, int *month, int *year)
    {*date=bcd2dec(regDate);
     int century=regMonth&128;
     *month=bcd2dec(regMonth&127);
     *year=bcd2dec(regYear);
     if (century==0)         // only XXth and XXIst centuries allowed
        {*year=*year+1900;}
     else
        {*year=*year+2000;}
    }         
    
// read date registers    
void DS3231::readDate(int *date, int *month, int *year)
    {char buf[3];
     buf[0]=DS3231_Date;
     int w=i2c.write(DS3231_Address,buf,1);
     int r=i2c.read(DS3231_Address,buf,3);
     decodeDate(buf[0],buf[1],buf[2],date,month,year);            
     error=((w!=0)||(r!=0));         
    }     
    
// set the date registers
void DS3231::setDate(int dayOfWeek, int date, int month, int year)
    {writeRegister(DS3231_Date,dec2bcd(date));
     writeRegister(DS3231_Day,dayOfWeek);
     int century=0; // only XXth and XXIst centuries allowed
     if (year<1900){year=1900;}
     if (year>2099){year=2099;}
     if (year<2000)
        {year=year-1900;}
     else
        {year=year-2000;century=1;}   
     writeRegister(DS3231_Month_Century,dec2bcd(month)+128*century);   
     writeRegister(DS3231_Year,dec2bcd(year));
    }     
    
// read date and time registers    
void DS3231::readDateTime(int *dayOfWeek, int *date, int *month, int *year, int *hours, int *minutes, int *seconds)
    {char buf[7];
     buf[0]=DS3231_Seconds;
     int w=i2c.write(DS3231_Address,buf,1);
     int r=i2c.read(DS3231_Address,buf,7);
     decodeTime(buf[2],buf[1],buf[0],hours,minutes,seconds);
     *dayOfWeek=buf[3];
     decodeDate(buf[4],buf[5],buf[6],date,month,year);
     error=((w!=0)||(r!=0));     
    }    
    
// read a register    
int DS3231::readRegister(char reg)
    {char buf[1];
     buf[0]=reg;
     int w=i2c.write(DS3231_Address,buf,1);
     int r=i2c.read(DS3231_Address,buf,1);
     error=((w!=0)||(r!=0));  
     return(buf[0]);
    }   

// write a register    
void DS3231::writeRegister(int reg,char byte)
    {char buf[2];
     buf[0]=reg;
     buf[1]=byte;
     int w=i2c.write(DS3231_Address,buf,2); 
     error=(w!=0);   
    }      
    
void DS3231::eraseOSF()
    {int reg=readRegister(DS3231_Control_Status);
     reg=reg&0x7F;
     writeRegister(DS3231_Control_Status,reg);
    }     
    
bool DS3231::OSF()
    {int reg=readRegister(DS3231_Control_Status);
    return(reg&DS3231_bit_OSF);
    }