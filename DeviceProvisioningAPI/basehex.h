#ifndef WORLDDIRECT_BASEHEX_H
#define WORLDDIRECT_BASEHEX_H

#include <string>
#include <vector>

namespace worlddirect {

  class BaseHex
  {
    public:
        BaseHex() = delete ;
        BaseHex&  operator= ( const  BaseHex& ) = delete;
        BaseHex&  operator= ( const  BaseHex&& ) = delete;
        BaseHex ( const  BaseHex& ) = delete;
        BaseHex ( const  BaseHex&& ) = delete;
        ~BaseHex() = delete ;

    public:
        static std::string EncodeWithCrc(const std::string data);
        static std::string EncodeWithCrc(const std::vector<uint8_t> data);

        //static std::string Decode(const std::string& input, std::vector<uint8_t>& out);

  };

} // namespace worlddirect

#endif // WORLDDIRECT_BASEHEX_H
