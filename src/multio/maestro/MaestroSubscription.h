/// @author Domokos Sarmany
/// @date   Dec 2020

#ifndef multio_MaestroSubscription_H
#define multio_MaestroSubscription_H

#include "MaestroEvent.h"
#include <ctime>
#include "eckit/exception/Exceptions.h"
extern "C" {
#include <maestro.h>
}

namespace multio {

class MaestroSubscription {
public:
    MaestroSubscription(mstro_cdo_selector selector, mstro_pool_event_kind events,
                        mstro_subscription_opts flags);
    ~MaestroSubscription();

    // Only define move construction
    MaestroSubscription(MaestroSubscription&&) noexcept;

    MaestroSubscription(const MaestroSubscription&) = delete;
    MaestroSubscription& operator=(const MaestroSubscription&) = delete;
    MaestroSubscription& operator=(MaestroSubscription&&) noexcept = delete;

    MaestroEvent poll();
    MaestroEvent wait();
    MaestroEvent timedwait(const timespec* abstime);
    bool ack(MaestroEvent& event);
private:
    mstro_subscription subscription_ = nullptr;
};

inline mstro_pool_event_kind operator|(mstro_pool_event_kind a, mstro_pool_event_kind b)
{
    return static_cast<mstro_pool_event_kind>(static_cast<int>(a) | static_cast<int>(b));
}

inline mstro_subscription_opts operator|(mstro_subscription_opts a, mstro_subscription_opts b) {
    return static_cast<mstro_subscription_opts>(static_cast<int>(a) | static_cast<int>(b));
}

}

#endif // multio_MaestroSubscription_H
