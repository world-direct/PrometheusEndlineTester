#ifndef WORLDDIRECT_TOKEN_H
#define WORLDDIRECT_TOKEN_H

#include <string>
#include <iostream>

namespace worlddirect {

  class Token
  {
  public:
    friend std::ostream& operator<<(std::ostream& os, const Token& dt);

  public:
    Token();
    Token(const std::string& token);
    Token&  operator= ( const  Token& ) = delete;
    Token&  operator= ( const  Token&& ) = delete;

    Token ( const  Token& ) = delete;
    Token ( const  Token&& ) = delete;
    ~Token() = default;

    operator const std::string&() const;

  public:
    bool valid() const;

  private:
    std::string m_tokenRaw;
  };

  std::ostream& operator<<(std::ostream& os, const Token& dt);


} // namespace worlddirect

#endif // WORLDDIRECT_TOKEN_H
