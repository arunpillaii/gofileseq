#ifndef FILESEQ_CPP_GTEST_COMMON_H
#define FILESEQ_CPP_GTEST_COMMON_H

#include "../types.h"

#define TODO() FAIL() << "Not Implemented"

// Create a std::vector<long> from an long[] array
#define FRAME_ARR_TO_VEC(X) (fileseq::Frames(X, X+(sizeof(X)/sizeof(X[0]))))

#endif