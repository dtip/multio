/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Domokos Sarmany
/// @author Simon Smart
/// @author Tiago Quintino

/// @date Jan 2019

#ifndef multio_server_MpiTransport_H
#define multio_server_MpiTransport_H

#include <queue>

#include "eckit/io/Buffer.h"
#include "eckit/log/Statistics.h"
#include "eckit/mpi/Comm.h"
#include "eckit/serialisation/ResizableMemoryStream.h"

#include "multio/server/Transport.h"
#include "multio/server/StreamPool.h"
#include "multio/server/StreamQueue.h"

namespace multio {
namespace server {

class MpiTransport final : public Transport {
public:
    MpiTransport(const eckit::Configuration& config);
    ~MpiTransport();

private:
    void openConnections() override;
    void closeConnections() override;

    Message receive() override;

    void send(const Message& msg) override;

    void bufferedSend(const Message& msg) override;

    void print(std::ostream& os) const override;

    Peer localPeer() const override;

    void listen() override;

    PeerList createServerPeers() override;

    const eckit::mpi::Comm& comm() const;

    eckit::mpi::Status probe();
    size_t blockingReceive(eckit::mpi::Status& status, MpiBuffer& buffer);

    void encodeMessage(eckit::Stream& strm, const Message& msg);

    MpiPeer local_;

    StreamPool pool_;

    StreamQueue streamQueue_;
    std::queue<Message> msgPack_;

    std::mutex mutex_;
};

}  // namespace server
}  // namespace multio

#endif
