#include <cstddef>
#include <cstdint>

#include "RliStreamProtocol/RliStreamProtocol_v2.h"

#include "../mock_handlers.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    SsrHandler_v1 ssrHandler_v1;
    AmcsHandler_v1 amcsHandler_v1;
    AdsbHandler_v1 adsbHandler_v1;

    SsrHandler_v2 ssrHandler_v2;
    AmcsHandler_v2 amcsHandler_v2;
    AdsbHandler_v2 adsbHandler_v2;
    AdsbAmcsHandler_v2 adsbAmcsHandler_v2;

    rli_stream::version_1::ssr::parse(std::vector<uint8_t>(data, data + size), ssrHandler_v1);
    rli_stream::version_1::ssr::amcs::parse(std::vector<uint8_t>(data, data + size), amcsHandler_v1);
    rli_stream::version_1::adsb::parse(std::vector<uint8_t>(data, data + size), adsbHandler_v1);

    rli_stream::version_2::ssr::parse(std::vector<uint8_t>(data, data + size), true, ssrHandler_v2);
    rli_stream::version_2::ssr::amcs::parse(std::vector<uint8_t>(data, data + size), true, amcsHandler_v2);
    rli_stream::version_2::adsb::parse(std::vector<uint8_t>(data, data + size), true, adsbHandler_v2);
    rli_stream::version_2::adsb::amcs::parse(std::vector<uint8_t>(data, data + size), true, adsbAmcsHandler_v2);
    return 0;
}
