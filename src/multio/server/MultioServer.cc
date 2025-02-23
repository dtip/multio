
#include "MultioServer.h"

#include "eckit/config/YAMLConfiguration.h"
#include "eckit/filesystem/PathName.h"

#include "multio/LibMultio.h"
#include "multio/server/Transport.h"

namespace multio {
namespace server {

MultioServer::MultioServer(const eckit::Configuration& config) :
    transport_{TransportFactory::instance().build(config.getString("transport"), config)},
    listener_{config, *transport_} {
    LOG_DEBUG_LIB(multio::LibMultio) << "Server config: " << config << std::endl;
    listener_.start();
}

MultioServer::~MultioServer() = default;

}  // namespace server
}  // namespace multio
