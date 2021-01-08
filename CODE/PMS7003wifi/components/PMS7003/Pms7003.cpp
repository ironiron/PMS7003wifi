/*
 * Pms7003.cpp
 *
 *  Created on: 23 mar 2020
 *      Author: Rafa³
 */

#include "Pms7003.h"
#include "Pms7003_uart.h"
#include <functional>
#include <array>
#include <numeric>

#include <iostream>//TODO cleanup
using namespace std;

Pms7003::Pms7003()
{
  // TODO Auto-generated constructor stub

}

Pms7003::~Pms7003()
{
  // TODO Auto-generated destructor stub
}

Pms7003::error_code Pms7003::ProcessData(void)
{
  unsigned int index=0;
  while(index<_max_index)
    {
      if(_uart_buf[index]==0x42 && _uart_buf[++index]==0x4d)
        {
          break;
        }
      index++;
    }
  if(index>=_max_index)
    {
      return error_code::NODATA;
    }
  unsigned int start_ind=index-1;

  // calculate crc
  unsigned int crc=uint8_t(_uart_buf[start_ind+30])<<8|uint8_t(_uart_buf[start_ind+31]);
  unsigned int crc2=0;
  for (index=start_ind;index<start_ind+30;index++)
  {
      crc2=crc2+uint8_t(_uart_buf[index]);
  }

//  cout << "start_ind:" <<start_ind<< endl;
//  cout << "crc:" <<crc<< endl;
//  cout << "crc2:" <<crc2<< endl;
//  cout << "_uart_buf[start_ind+30]:" <<_uart_buf[start_ind+30]<< endl;
//  cout << "_uart_buf[start_ind+31]:" <<_uart_buf[start_ind+31]<< endl;
//  cout << "_uart_buf[start_ind+4]:" <<+_uart_buf[start_ind+4]<< endl;
//  cout << "_uart_buf[start_ind+5]:" <<+_uart_buf[start_ind+5]<< endl;
//  cout << "_uart_buf[start_ind+6]:" <<+_uart_buf[start_ind+6]<< endl;
//  cout << "_uart_buf[start_ind+7]:" <<+_uart_buf[start_ind+7]<< endl;
//  cout << "uint8_t(_uart_buf[start_ind+4])<<8|uint8_t(_uart_buf[start_ind+5])" <<+(uint8_t(_uart_buf[start_ind+4])<<8|uint8_t(_uart_buf[start_ind+5]))<< endl;

    if(crc2!=crc)
    {
        return error_code::BADCRC;
    }

  //calculate actual data:
    _latest_data.pm1=uint8_t(_uart_buf[start_ind+4])<<8|uint8_t(_uart_buf[start_ind+5]);
    _latest_data.pm2_5=uint8_t(_uart_buf[start_ind+6])<<8|uint8_t(_uart_buf[start_ind+7]);
    _latest_data.pm10=uint8_t(_uart_buf[start_ind+8])<<8|uint8_t(_uart_buf[start_ind+9]);
    _latest_data.atm_pm1=uint8_t(_uart_buf[start_ind+10])<<8|uint8_t(_uart_buf[start_ind+11]);
    _latest_data.atm_pm2_5=uint8_t(_uart_buf[start_ind+12])<<8|uint8_t(_uart_buf[start_ind+13]);
    _latest_data.atm_pm10=uint8_t(_uart_buf[start_ind+14])<<8|uint8_t(_uart_buf[start_ind+15]);
    _latest_data.beyond0_3=uint8_t(_uart_buf[start_ind+16])<<8|uint8_t(_uart_buf[start_ind+17]);
    _latest_data.beyond0_5=uint8_t(_uart_buf[start_ind+18])<<8|uint8_t(_uart_buf[start_ind+19]);
    _latest_data.beyond1=uint8_t(_uart_buf[start_ind+20])<<8|uint8_t(_uart_buf[start_ind+21]);
    _latest_data.beyond2_5=uint8_t(_uart_buf[start_ind+22])<<8|uint8_t(_uart_buf[start_ind+23]);
    _latest_data.beyond5=uint8_t(_uart_buf[start_ind+24])<<8|uint8_t(_uart_buf[start_ind+25]);
    _latest_data.beyond10=uint8_t(_uart_buf[start_ind+26])<<8|uint8_t(_uart_buf[start_ind+27]);

    _avg_buffer[_buffer_index]=_latest_data;
    _buffer_index++;
    if (_buffer_index>=8)
      {
        _buffer_index=0;
      }

  return error_code::OK;
}

Pms7003::data Pms7003::GetLatestData(void)
{
  return _latest_data;
}

Pms7003::data Pms7003::GetAverageData(void)
{
  cout << "_avg_buffer[0].pm1=" <<+_avg_buffer[0].pm1<< endl;
  cout << "_avg_buffer[1].pm1=" <<+_avg_buffer[1].pm1<< endl;
  cout << "_avg_buffer[2].pm1=" <<+_avg_buffer[2].pm1<< endl;
  cout << "_avg_buffer[3].pm1=" <<+_avg_buffer[3].pm1<< endl;
  cout << "_avg_buffer[4].pm1=" <<+_avg_buffer[4].pm1<< endl;
  cout << "_avg_buffer[5].pm1=" <<+_avg_buffer[5].pm1<< endl;
  cout << "_avg_buffer[6].pm1=" <<+_avg_buffer[6].pm1<< endl;
  cout << "_avg_buffer[7].pm1=" <<+_avg_buffer[7].pm1<< endl;

  std::accumulate(_avg_buffer.begin(), _avg_buffer.end(), 0,
      [](unsigned int sum, const Pms7003::data& curr) { return sum + curr.pm1;});
  return _latest_data;
}

uint8_t* Pms7003::GetBuffer(void)//addr
{
  //TODO check for better solutions
  return _uart_buf;
}


//uint8_t Pms7003::GetBuffer(void)
//{
//  unsigned int index=0;
//
//
//  return true;
//}
