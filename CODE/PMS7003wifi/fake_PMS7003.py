import serial
import time
import binascii
from random import randrange

#Simulates PMS7003 sensor

ser=serial.Serial(
port='COM25',# Change this if you need!
baudrate=9600,
parity=serial.PARITY_NONE,
stopbits=serial.STOPBITS_ONE,
bytesize=serial.EIGHTBITS,
timeout=0
)

data=bytearray(32)
data[0]=0x42
data[1]=0x4d
data[2]=0
data[3]=28
iteration=0

while True:
    pm1=randrange(0,65535)
    pm25=randrange(0,65535)
    pm10=randrange(50,100)
    iteration+=1
    
    data[4]=(pm1>>8) & 0xff   #PM1.0
    data[5]=pm1 & 0xff
    data[6]=(pm25>>8) & 0xff #PM2.5
    data[7]=pm25 & 0xff
    data[8]=(pm10>>8) & 0xff #PM10
    data[9]=pm10 & 0xff

    pm1*=2
    pm25*=2
    pm10*=2
    # Below are values for atmospheric encironment (here twice of standard)
    data[10]=(pm1>>8) & 0xff   #PM1.0
    data[11]=pm1 & 0xff
    data[12]=(pm25>>8) & 0xff #PM2.5
    data[13]=pm25 & 0xff
    data[14]=(pm10>>8) & 0xff #PM10
    data[15]=pm10 & 0xff

    data[16]=0 #particles beyond 0.3 - fixed 123
    data[17]=123
    data[18]=0x10 #particles beyond 0.5 - fixed 0x10a2=4258
    data[19]=0xa2
    data[20]=0 #particles beyond 1 - fixed 0
    data[21]=0
    data[22]=(iteration>>8) & 0xff #particles beyond 2.5 - loop counter
    data[23]=iteration & 0xff
    
    #particles beyond 5 - fixed 0x424d=16973 for checking start of transmision
    #can put something else normally
    data[24]=0x42 
    data[25]=0x4d
    
    data[26]=0 #particles beyond 10 - fixed 0
    data[27]=0
    data[28]=0 #reserved
    data[29]=0
    #CRC calculation
    data[30]=0# clear last entry
    data[31]=0
    crc=sum(data)
    data[30]=(crc>>8) & 0xff
    data[31]=crc & 0xff

    ser.write(data)
    time.sleep(1)
    if (ser.inWaiting()>0):
        data_b = ser.read(ser.inWaiting()).hex(' ') #pyhon before 3.8 does not support ' ' separator=>delete this!
        print(data_b) #print in console all received bytes
    
    
