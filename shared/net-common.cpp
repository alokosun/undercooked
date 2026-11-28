#include "net-common.h"


const char* to_string(UdpCheck c) {
  switch (c) {
    case UdpCheck::Ok: return "Ok";
    case UdpCheck::TooSmall: return "TooSmall";
    case UdpCheck::TooLarge: return "TooLarge";
    case UdpCheck::BadProtocolId: return "BadProtocolId";
    case UdpCheck::UnknownType: return "UnknownType";
  }
  return "?";
}