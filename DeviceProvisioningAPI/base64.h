#ifndef WORLDDIRECT_BASE64_H
#define WORLDDIRECT_BASE64_H

#include <string>
#include <vector>
#include <stdint.h>

namespace worlddirect {

    class Base64 {
    public:
        Base64() = delete ;
        Base64&  operator= ( const  Base64& ) = delete;
        Base64&  operator= ( const  Base64&& ) = delete;
        Base64 ( const  Base64& ) = delete;
        Base64 ( const  Base64&& ) = delete;
        ~Base64() = delete ;

    public:
        static std::string Encode(const std::vector<uint16_t>& data);

        static std::string Encode(const std::vector<uint8_t> data);

        static std::string Decode(const std::string& input, std::vector<uint8_t>& out);

        static std::string Decode(const std::string& input, std::vector<uint16_t>& out);

    };

}


#endif // WORLDDIRECT_BASE64_H
