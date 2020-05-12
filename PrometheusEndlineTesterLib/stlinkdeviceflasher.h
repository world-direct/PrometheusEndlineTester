#ifndef WORLDDIRECT_STLINKDEVICEFLASHER_H
#define WORLDDIRECT_STLINKDEVICEFLASHER_H

#include <string>

#include "PrometheusEndlineTesterLib_global.h"

namespace worlddirect {

  class PROMETHEUSENDLINETESTERLIB_EXPORT StLinkDeviceFlasher
  {
  public:
    StLinkDeviceFlasher(const std::string& pathtoCmd);
    StLinkDeviceFlasher&  operator= ( const  StLinkDeviceFlasher& ) = delete;
    StLinkDeviceFlasher&  operator= ( const  StLinkDeviceFlasher&& ) = delete;

    StLinkDeviceFlasher ( const  StLinkDeviceFlasher& ) = delete;
    StLinkDeviceFlasher ( const  StLinkDeviceFlasher&& ) = delete;

  public:
    int write(const std::string& writeArgs, const std::string& pathToBinary)const;
    int erase(const std::string& eraseArgs)const;
    int reset(const std::string& resetArgs)const;

  private:
    std::string _pathtoCmd;
  };

} // namespace worlddirect

#endif // WORLDDIRECT_STLINKDEVICEFLASHER_H
