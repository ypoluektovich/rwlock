#include "rwlock.hpp"

namespace nbrwl {

    bool rwlockable::counters::isWaitingForWrite() const {
        return writeClaim_ != writeDone_;
    }

    bool rwlockable::counters::isWaitingForRead() const {
        return read_ != 0;
    }

    bool rwlockable::counters::isMyTurnToWrite(counters const & claim) const {
        return writeDone_ == claim.writeClaim_ - 1;
    }

    bool rwlockable::counters::wantToRead(rwlockable::counters * buf) const {
        if (read_ == UINT16_MAX) {
            return false;
        }
        *buf = *this;
        buf->read_ += 1;
        return true;
    }

    bool rwlockable::counters::wantToWrite(rwlockable::counters * buf) const {
        if (writeClaim_ == UINT8_MAX) {
            return false;
        }
        *buf = *this;
        buf->writeClaim_ += 1;
        return true;
    }

    rwlockable::counters rwlockable::counters::doneReading() const {
        counters c = *this;
        c.read_ -= 1;
        return c;
    }

    rwlockable::counters rwlockable::counters::doneWriting() const {
        counters c = *this;
        c.writeDone_ += 1;
        if (c.writeDone_ == UINT8_MAX) {
            c.writeClaim_ = c.writeDone_ = 0;
        }
        return c;
    }

}
