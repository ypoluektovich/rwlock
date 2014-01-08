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
                    unsigned int read_;
                    unsigned int writeClaim_;
                    unsigned int writeDone_;

                    static unsigned int const MAX_CONSECUTIVE_WRITES = INT_MAX;
                public:
                    bool isWaitingForWrite() const;
                    bool isWaitingForRead() const;
                    bool isMyTurnToWrite(counters const & claim) const;

                    counters wantToRead() const;
                    counters wantToWrite() const;
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
