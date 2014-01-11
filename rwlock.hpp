#pragma once
#include <climits>
#include <atomic>

namespace nbrwl {

    class rwlockable;

    class rwlock {
        friend class rwlockable;

        public:
            enum class lock_type {
                none,
                read,
                write
            };

        private:
            rwlockable * lockable_;
            lock_type lockType_;

            rwlock(rwlockable * const lockable, bool const exclusive);
            rwlock();

        public:
            rwlock(rwlock&& rhs);
            rwlock& operator =(rwlock&& rhs);
            ~rwlock();

            void unlock();
            lock_type getLockType() const;
    };

    class rwlockable {
        friend rwlock::~rwlock();

        private:
            class counters {
                private:
                    uint16_t read_;
                    uint8_t writeClaim_;
                    uint8_t writeDone_;
                public:
                    bool isWaitingForWrite() const;
                    bool isWaitingForRead() const;
                    bool isMyTurnToWrite(counters const & claim) const;

                    bool wantToRead(counters * buf) const;
                    bool wantToWrite(counters * buf) const;
                    counters doneReading() const;
                    counters doneWriting() const;
            };

            std::atomic<counters> counters_;

            void unlockRead();
            void unlockWrite();

        public:
            rwlock lockForRead();
            rwlock lockForWrite();
            bool isLockFree() const;
    };

}
