/*
 * (C) Copyright 1996-2015 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Tiago Quintino
/// @author Simon Smart
/// @date Dec 2015

#include "multio/DataSink.h"

#include "eckit/thread/AutoLock.h"
#include "eckit/thread/Mutex.h"
#include "eckit/exception/Exceptions.h"

using namespace eckit;

namespace multio {

//----------------------------------------------------------------------------------------------------------------------

static eckit::Mutex *local_mutex = 0;
static std::map<std::string, DataSinkFactory*> *m = 0;
static pthread_once_t once = PTHREAD_ONCE_INIT;
static void init() {
    local_mutex = new eckit::Mutex();
    m = new std::map<std::string, DataSinkFactory*>();
}

DataSinkFactory::DataSinkFactory(const std::string &name) :
    name_(name) {

    pthread_once(&once, init);

    eckit::AutoLock<eckit::Mutex> lock(local_mutex);

    ASSERT(m->find(name) == m->end());
    (*m)[name] = this;
}


DataSinkFactory::~DataSinkFactory() {
    eckit::AutoLock<eckit::Mutex> lock(local_mutex);
    m->erase(name_);
}


void DataSinkFactory::list(std::ostream& out) {

    pthread_once(&once, init);

    eckit::AutoLock<eckit::Mutex> lock(local_mutex);

    const char* sep = "";
    for (std::map<std::string, DataSinkFactory*>::const_iterator j = m->begin() ; j != m->end() ; ++j) {
        out << sep << (*j).first;
        sep = ", ";
    }
}


DataSink* DataSinkFactory::build(const std::string &name, const eckit::Configuration& config) {

    pthread_once(&once, init);

    eckit::AutoLock<eckit::Mutex> lock(local_mutex);

    eckit::Log::info() << "Looking for DataSinkFactory [" << name << "]" << std::endl;

    std::map<std::string, DataSinkFactory *>::const_iterator j = m->find(name);
    if (j == m->end()) {
        eckit::Log::error() << "No DataSinkFactory for [" << name << "]" << std::endl;
        eckit::Log::error() << "DataSinkFactories are:" << std::endl;
        for (j = m->begin() ; j != m->end() ; ++j)
            eckit::Log::error() << "   " << (*j).first << std::endl;
        throw eckit::SeriousBug(std::string("No DataSinkFactory called ") + name);
    }

    return (*j).second->make(config);
}

//----------------------------------------------------------------------------------------------------------------------

DataSink::DataSink(const eckit::Configuration& config) :
    Journal(config),
    failOnError_( config.getBool("failOnError",true) ),
    journaled_( config.getBool("journaled",false) ),
    // By default, use self as the journal, unless externally overridden.
    journal_(this) {
}

DataSink::~DataSink() {}

//----------------------------------------------------------------------------------------------------------------------

void DataSink::open() {

    journal_->open();

    // Call the implementation specific open routine.
    open_();
}

void DataSink::write(const void* buffer, const Length& length, JournalRecord * const parent_record, Metadata* metadata) {

    ScopedPtr<JournalRecord> new_record;

    if (!parent_record) {
        new_record.reset(new JournalRecord(JournalRecord::WriteEntry));
    }

    JournalRecord& journal_record(parent_record ? *parent_record : *new_record);

    write_(buffer, length, journal_record);

    // If we are the creator of the journal record, we are responsible for ensuring that it
    // gets written if it is populated.
    if (!parent_record && journal_record.utilised()) {
        journal_->write_record(journal_record);
    }

}


void DataSink::record_write_journal_entry(JournalRecord& journal_record,
                                          const void * buffer, const eckit::Length& length) {

    // Ensure that the JournalEntry has a copy of the data. Note that this may
    // already have been done by another DataSink (in which case this is a NOP).
    journal_record.addData(buffer, length);

    // Add the entry here. By default there is no additional (DataSink-specific)
    // information, so the payload length is zero
    journal_record.addJournalEntry(JournalRecord::JournalEntry::Write);

}


int DataSink::iopenfdb(const char *name, const char *mode, int name_len, int mode_len) {
    return 0;
}

int DataSink::iclosefdb() {
    return 0;
}

int DataSink::iinitfdb() {
    return 0;
}

int DataSink::isetcommfdb(int *rank) {
    return 0;
}

int DataSink::isetrankfdb(int *rank) {
    return 0;
}

int DataSink::iset_fdb_root(const char *name, int name_len) {
    return 0;
}

int DataSink::iflushfdb() {
    return 0;
}

int DataSink::isetfieldcountfdb(int *all_ranks, int *this_rank) {
    return 0;
}

int DataSink::isetvalfdb(const char *name, const char *value, int name_len, int value_len) {
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------

}  // namespace multio

