/*
 * Pms7003_test.cpp
 *
 *  Created on: 23 mar 2020
 *      Author: Rafa³
 */
#include "Pms7003.h"
#include "gtest/gtest.h"
#include "Pms7003_uart.h"

Pms7003 sensor;

void cleanup(void)
{
  uint8_t *buf=sensor.GetBuffer();
  for (int i=0; i<63; i++)
    {
      buf[i]=0;
    }
}

//TEST(PMS7003, isReceiving)
//{
//
//  ASSERT_EQ(sensor.ProcessData(), true);
//}

//TODO whole buf clean up
TEST(PMS7003, DetectStartOfTransmision)
{
  cleanup();
  uint8_t *buf=sensor.GetBuffer();

  buf[0]=0x42;
  buf[1]=0x4d;
  buf[31]=0x8f;
  ASSERT_EQ(sensor.ProcessData(), Pms7003::error_code::OK);
  buf[1]=0xff; //wrong data
  ASSERT_EQ(sensor.ProcessData(), Pms7003::error_code::NODATA);

  cleanup();
  buf[21]=0x42;
  buf[22]=0x4d;
  buf[52]=0x8f;
  ASSERT_EQ(sensor.ProcessData(), Pms7003::error_code::OK);
}

TEST(PMS7003, ComputeDataAndCrc)
{
  cleanup();
  uint8_t *buf=sensor.GetBuffer();
  buf[21]=0x42;
  buf[22]=0x4d;
  buf[21+4]=0x0f;
  buf[21+5]=0xff; //PM1->0x0fff
  buf[21+6]=0x1c;
  buf[21+7]=0x8b; //PM2.5->0x1c8b
  //crc= sum of all bytes: 244(hex) 580(dec)
  buf[21+30]=0x02;
  buf[21+31]=0x44;

  ASSERT_EQ(sensor.ProcessData(), Pms7003::error_code::OK);

  Pms7003::data data=sensor.GetLatestData();
  ASSERT_EQ(data.pm1, 0x0fff);
  ASSERT_EQ(data.pm2_5, 0x1c8b);
  ASSERT_EQ(data.pm10, 0);
}

TEST(PMS7003, ComputeDataAndCrc2)
{
  cleanup();
  uint8_t *buf=sensor.GetBuffer();

  buf[0]=0x42;//start
  buf[1]=0x4d;
  buf[2]=0;//data length
  buf[3]=28;
  buf[4]=0x12;//PM1
  buf[5]=0x34;
  buf[6]=0xaa;//PM2.5
  buf[7]=0xbb;
  buf[8]=0;//PM10
  buf[9]=0;
  buf[10]=0x46;//atm PM1
  buf[11]=0x21;
  buf[12]=0xff;// atm PM2.5
  buf[13]=0xff;
  buf[14]=0x0f;//atm PM10
  buf[15]=0xf0;
  buf[16]=0;//particles beyond 0.3
  buf[17]=0;
  buf[18]=0xaa;//beyond 0.5
  buf[19]=0xbb;
  buf[20]=0;//beyond 1
  buf[21]=0;
  buf[22]=0;//beyond 2.5
  buf[23]=0x41;
  buf[24]=0x42;//beyond 5
  buf[25]=0x4d;//<< check if it can handle start bytes within measured data
  buf[26]=0xff;// beyond 10
  buf[27]=0xff;
  buf[28]=0;//reserved
  buf[29]=0;
  buf[30]=0x09;//crc=0x09ed
  buf[31]=0xed;

  ASSERT_EQ(sensor.ProcessData(), Pms7003::error_code::OK);

  Pms7003::data data=sensor.GetLatestData();
  ASSERT_EQ(data.pm1, 0x1234);
  ASSERT_EQ(data.pm2_5, 0xaabb);
  ASSERT_EQ(data.pm10, 0);
  ASSERT_EQ(data.atm_pm1, 0x4621);
  ASSERT_EQ(data.atm_pm2_5, 0xffff);
  ASSERT_EQ(data.atm_pm10, 0x0ff0);
  ASSERT_EQ(data.beyond0_3, 0);
  ASSERT_EQ(data.beyond0_5, 0xaabb);
  ASSERT_EQ(data.beyond1, 0);
  ASSERT_EQ(data.beyond2_5, 0x0041);
  ASSERT_EQ(data.beyond5, 0x424d);
  ASSERT_EQ(data.beyond10, 0xffff);
}

TEST(PMS7003, GetAverageData)
{
  //Check just PM1, PM2.5
  cleanup();
    uint8_t *buf=sensor.GetBuffer();

    buf[0]=0x42;//start
    buf[1]=0x4d;
    buf[2]=0;//data length
    buf[3]=28;
    buf[31]=0xab;
    for(int i=0;i<8;i++)
      {
        ASSERT_EQ(sensor.ProcessData(), Pms7003::error_code::OK);
      }
    // test just 3 measurements, but internal average buffer is for 8 data sets.
    buf[4]=0;//PM1
    buf[5]=200;//!!! decimal value !!!
    buf[6]=0;//PM2.5
    buf[7]=201;
    buf[30]=0x02;//crc
    buf[31]=0x3c;
    ASSERT_EQ(sensor.ProcessData(), Pms7003::error_code::OK);
    buf[4]=0;//PM1
    buf[5]=30;
    buf[6]=0;//PM2.5
    buf[7]=202;
    buf[30]=0x01;//crc
    buf[31]=0x93;
    ASSERT_EQ(sensor.ProcessData(), Pms7003::error_code::OK);
    buf[4]=0;//PM1
    buf[5]=10;
    buf[6]=0;//PM2.5
    buf[7]=203;
    buf[30]=0x01;//crc
    buf[31]=0x80;
    ASSERT_EQ(sensor.ProcessData(), Pms7003::error_code::OK);

    Pms7003::data data=sensor.GetAverageData();
    ASSERT_EQ(data.pm1, 30);// 240/8=30
    ASSERT_EQ(data.pm2_5, 75);// 606/8=75 (rounded)
    ASSERT_EQ(data.pm10, 0);
}

TEST(PMS7003, InvalidDataCallback)
{

  ASSERT_EQ(60, 60);
}

//TEST(PMS7003, ReceivedNSamplesCallback)
//{
//
//  ASSERT_EQ(60, 60);
//}

//TEST(PMS7003, PowerOnOff)
//{
//  ASSERT_EQ(60, 60);
//}

TEST(PMS7003, HandleErrorDuringWrite)
{

  ASSERT_EQ(60, 60);
}

//TEST(PMS7003, BufferOverflowErrorCallback)
//{
//
//  ASSERT_EQ(60, 60);
//}

