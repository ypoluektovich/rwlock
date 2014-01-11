#include "rwlock.hpp"
#include <utility>

namespace nbrwl {

    rwlock rwlockable::lockForRead() {
        bool written = false;
        do {
            counters exp = counters_.load(std::memory_order_acquire);
            do {
                if (exp.isWaitingForWrite()) {
                    break;
                }
                counters claim;
                if (!exp.wantToRead(&claim)) {
                    break;
                }
                written = counters_.compare_exchange_weak(
                        exp, claim,
                        std::memory_order_release,
                        std::memory_order_acquire
                );
            } while (!written);
            // todo: if (!written) progressive backoff
        } while (!written);
        return rwlock(this, false);
    }

    rwlock rwlockable::lockForWrite() {
        counters exp = counters_.load(std::memory_order_acquire);
        counters claim;
        do {
            while (!exp.wantToWrite(&claim)) {
                // todo: progressive backoff
                exp = counters_.load(std::memory_order_acquire);
            }
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
