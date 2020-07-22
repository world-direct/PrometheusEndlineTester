#include "httpcodeutils.h"

#include <sstream>

namespace worlddirect {

    std::string code2status(long code)
    {
        if( code == HTTP_OK) {
                return "OK";
            }

        if( code == HTTP_CREATED) {
                return "CREATED";
            }

        if( code == HTTP_NO_CONTENT) {
                return "NO CONTENT";
            }

        if( code == HTTP_BAD_REQUEST) {
                return "BAD REQUEST";
            }

        if( code == HTTP_UNAUTHORIZED) {
                return "UNAUTHORIZED";
            }

        if( code == HTTP_FORBIDDEN) {
                return "FORBIDDEN";
            }
        if( code == HTTP_NOT_FOUND) {
                return "NOT FOUND";
            }
        if( code == HTTP_CONFLICT) {
                return "CONFLICT";
            }

        std::stringstream stringBuilder;
        stringBuilder << "HTTP status code: " << code;

        return stringBuilder.str();
    }


} // namespace worlddirect
