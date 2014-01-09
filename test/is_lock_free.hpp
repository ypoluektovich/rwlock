#include <cxxtest/TestSuite.h>
#include "rwlock.hpp"

class IsLockFreeSuite : public CxxTest::TestSuite {
    public:
        void testIsLockFree() {
            nbrwl::rwlockable l;
            TS_ASSERT(l.isLockFree());
        }
};
