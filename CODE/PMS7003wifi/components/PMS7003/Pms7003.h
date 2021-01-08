/*
 * Pms7003.hpp
 *
 *  Created on: 23 mar 2020
 *      Author: Rafa³
 */

#ifndef PMS7003_PMS7003_HPP_
#define PMS7003_PMS7003_HPP_

#include <stdint.h>
#include <array>

class Pms7003
{
  public:
    enum error_code{OK, NODATA, BADCRC};
    struct data{
        uint16_t pm1;
        uint16_t pm2_5;
        uint16_t pm10;
        uint16_t atm_pm1;
        uint16_t atm_pm2_5;
        uint16_t atm_pm10;
        uint16_t beyond0_3;
        uint16_t beyond0_5;
        uint16_t beyond1;
        uint16_t beyond2_5;
        uint16_t beyond5;
        uint16_t beyond10;
    };
    Pms7003();
    virtual ~Pms7003();
    error_code ProcessData(void);
    data GetLatestData(void);
    data GetAverageData(void);
    uint8_t* GetBuffer(void);

  private:
    const static uint_fast8_t _max_index=63;//64 bytes
    uint8_t _uart_buf[_max_index]={0};
    data _latest_data={0};
    std::array<data, 8> _avg_buffer={0};
    unsigned int _buffer_index=0;
};

#endif /* PMS7003_PMS7003_HPP_ */
