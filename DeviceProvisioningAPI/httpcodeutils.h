#ifndef WORLDDIRECT_HTTPCODEUTILS_H
#define WORLDDIRECT_HTTPCODEUTILS_H

#include <string>

namespace worlddirect {

  constexpr long HTTP_OK = 200;
  constexpr long HTTP_CREATED = 201;
  constexpr long HTTP_NO_CONTENT = 204;
  constexpr long HTTP_BAD_REQUEST = 400;
  constexpr long HTTP_UNAUTHORIZED = 401;
  constexpr long HTTP_FORBIDDEN = 403;
  constexpr long HTTP_NOT_FOUND = 404;
  constexpr long HTTP_CONFLICT = 409;

  std::string code2status(long code);

} // namespace worlddirect

#endif // WORLDDIRECT_HTTPCODEUTILS_H
