#include "basehex.h"
#include <sstream>
#include  <iomanip>

#include "drivers/MbedCRC.h"

namespace worlddirect {

  std::string BaseHex::EncodeWithCrc(const std::string data)
  {
    std::vector<uint8_t> psk_vec(data.begin(), data.end());
    return EncodeWithCrc(psk_vec);
  }

  std::string BaseHex::EncodeWithCrc(const std::vector<uint8_t> data)
    {
        std::stringstream stringBuffer;
        for( auto a : data){
                stringBuffer << std::setfill('0') << std::setw(2) << std::right << std::hex << std::uppercase << +a;
            }

        mbed::MbedCRC<POLY_16BIT_CCITT,16> crc(0xFFFFFFFF,0,false,false);
        uint32_t calcCrc;
        auto _buffer = data.data();
        auto _sz = data.size();
        if(crc.compute(_buffer, _sz, &calcCrc) < 0) {
                return std::string();
            }
        stringBuffer << std::setfill('0') << std::setw(4) << std::right << std::hex << std::uppercase << calcCrc;

        return stringBuffer.str();
    }


} // namespace worlddirect
