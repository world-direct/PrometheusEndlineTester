#include "stlinkdeviceflasher.h"

#include<cstdlib>
#include<iostream>

namespace worlddirect {

  StLinkDeviceFlasher::StLinkDeviceFlasher(const std::string &pathtoCmd)
    :_pathtoCmd(pathtoCmd)
  {

  }

  int StLinkDeviceFlasher::write(const std::string &writeArgs, const std::string& pathToBinary) const
  {
    std::string full_cmd="\""+_pathtoCmd+"\" "+writeArgs+" "+pathToBinary;
    std::cout<<full_cmd<<std::endl;
    return std::system(full_cmd.c_str());
  }

  int StLinkDeviceFlasher::erase(const std::string &eraseArgs) const
  {
    std::string full_cmd="\""+_pathtoCmd+"\" "+eraseArgs;
    std::cout<<full_cmd<<std::endl;
    return std::system(full_cmd.c_str());
  }

  int StLinkDeviceFlasher::reset(const std::string &resetArgs) const
  {
    std::string full_cmd="\""+_pathtoCmd+"\" "+resetArgs;
    std::cout<<full_cmd<<std::endl;
    return std::system(full_cmd.c_str());
  }


} // namespace worlddirect
