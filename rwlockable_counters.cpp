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

    rwlockable::counters rwlockable::counters::wantToRead() const {
        counters c = *this;
        c.read_ += 1;
        return c;
    }

    rwlockable::counters rwlockable::counters::wantToWrite() const {
        counters c = *this;
        c.writeClaim_ += 1;
        return c;
    }

    rwlockable::counters rwlockable::counters::doneReading() const {
        counters c = *this;
        c.read_ -= 1;
        return c;
    }

    rwlockable::counters rwlockable::counters::doneWriting() const {
        counters c = *this;
        c.writeDone_ += 1;
        return c;
    }

}
