#include "token.h"

namespace worlddirect {

  Token::Token():
      m_tokenRaw()
  {

  }

  Token::Token(const std::string &token):
      m_tokenRaw(token)
  {

  }

  worlddirect::Token::operator const std::string &() const
  {
  return m_tokenRaw;
  }

  bool Token::valid() const
  {
     if(m_tokenRaw.empty())
         {
             return false;
         }

     return true;
  }

  std::ostream &operator<<(std::ostream &os, const Token &dt)
  {
    os << dt.m_tokenRaw;
    return os;
  }

} // namespace worlddirect
