#include "rwlock.hpp"
#include <utility>

namespace nbrwl {

    rwlock rwlockable::lockForRead() {
        bool written = false;
        while (!written) {
            counters exp = counters_.load(std::memory_order_acquire);
            if (!exp.isWaitingForWrite()) {
                counters des = exp.wantToRead();
                while (!(written = counters_.compare_exchange_weak(
                                exp, des,
                                std::memory_order_release,
                                std::memory_order_acquire
                ))) {
                    if (exp.isWaitingForWrite()) {
                        break;
                    }
                    des = exp.wantToRead();
                }
                if (written) {
                    break;
                }
            }
            // todo: progressive backoff
        }
        return rwlock(this, false);
    }

    rwlock rwlockable::lockForWrite() {
        counters exp = counters_.load(std::memory_order_acquire);
        counters claim;
        do {
            claim = exp.wantToWrite();
        } while (!counters_.compare_exchange_weak(
                    exp, claim,
                    std::memory_order_release,
                    std::memory_order_acquire
        ));
        while (exp.isWaitingForRead() || !exp.isMyTurnToWrite(claim)) {
            // todo: progressive backoff
            exp = counters_.load(std::memory_order_acquire);
        }
        return rwlock(this, true);
    }

    void rwlockable::unlockRead() {
        counters exp = counters_.load(std::memory_order_consume);
        counters des;
        do {
            des = exp.doneReading();
        } while (!counters_.compare_exchange_weak(
                    exp, des,
                    std::memory_order_release,
                    std::memory_order_consume
        ));
    }

    void rwlockable::unlockWrite() {
        counters exp = counters_.load(std::memory_order_consume);
        counters des;
        do {
            des = exp.doneWriting();
        } while (!counters_.compare_exchange_weak(
                    exp, des,
                    std::memory_order_release,
                    std::memory_order_consume
        ));
    }

    bool rwlockable::isLockFree() const {
        return counters_.is_lock_free();
    }

}
