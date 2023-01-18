
#include "GaiaWebStats.h"

std::atomic<uint32_t> GaiaWebStats::_cfg_uploads         = 0;
std::atomic<uint32_t> GaiaWebStats::_zip_uploads         = 0;
std::atomic<uint32_t> GaiaWebStats::_processing_requests = 0;

