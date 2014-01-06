#include "rwlock.hpp"
#include <utility>

namespace nbrwl {

    rwlock::rwlock(rwlockable * const lockable, bool const exclusive)
        : lockable_(lockable)
        , lockType_(exclusive ? lock_type::write : lock_type::read)
    {}

    rwlock::rwlock()
        : lockable_(nullptr)
        , lockType_(lock_type::none)
    {}


    rwlock::rwlock(rwlock&& rhs) {
        lockable_ = rhs.lockable_;
        lockType_ = rhs.lockType_;
        rhs.lockable_ = nullptr;
        rhs.lockType_ = lock_type::none;
    }

    rwlock& rwlock::operator =(rwlock&& rhs) {
        std::swap(lockable_, rhs.lockable_);
        std::swap(lockType_, rhs.lockType_);
        return *this;
    }


    rwlock::~rwlock() {
        if (lockable_ == nullptr) {
            return;
        }
        switch (lockType_) {
            case lock_type::read:
                lockable_->unlockRead();
                break;
            case lock_type::write:
                lockable_->unlockWrite();
                break;
            default:
                // do nothing
                break;
        }
    }

    void rwlock::unlock() {
        (*this) = rwlock();
        // emptyLock's dtor will now activate
    }

    rwlock::lock_type rwlock::getLockType() const {
        return lockType_;
    }

}
