/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Tiago Quintino
/// @author Domokos Sarmany
/// @date   Dec 2015

#include "multio/fdb5/FDB5Sink.h"

#include "eckit/exception/Exceptions.h"
#include "fdb5/config/Config.h"

#include "multio/LibMultio.h"

namespace multio {

namespace {
fdb5::Config fdb5_configuration(const eckit::Configuration& cfg) {
    auto fdb_configuration = cfg.getSubConfiguration("config");

    eckit::LocalConfiguration userConfig;
    if (fdb_configuration.has("userConfig")) {
        userConfig = fdb_configuration.getSubConfiguration("userConfig");
    }
    else {
        userConfig.set("useSubToc", true);
    }

    fdb5::Config fdb_config(fdb_configuration, userConfig);

    LOG_DEBUG_LIB(LibMultio) << "FDB5 Config = " << fdb_config << std::endl;

    return fdb_config;
}
}  // namespace

FDB5Sink::FDB5Sink(const eckit::Configuration& config) :
    DataSink(config), fdb_{fdb5_configuration(config)} {
    LOG_DEBUG_LIB(LibMultio) << "Config = " << config << std::endl;
}

void FDB5Sink::write(eckit::message::Message msg) {
    LOG_DEBUG_LIB(LibMultio) << "FDB5Sink::write()" << std::endl;

    fdb_.archive(msg);
}

void FDB5Sink::flush() {
    LOG_DEBUG_LIB(LibMultio) << "FDB5Sink::flush()" << std::endl;

    fdb_.flush();
}

void FDB5Sink::print(std::ostream& os) const {
    os << "FDB5Sink()";
}

static DataSinkBuilder<FDB5Sink> FDB5SinkBuilder("fdb5");

}  // namespace multio
