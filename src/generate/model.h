#ifndef __MARKOV_GENERATE_MODEL_H__
#define __MARKOV_GENERATE_MODEL_H__

#include <boost/utility.hpp>

#include "common/types.h"

class Model : boost::noncopyable
{
public:
    Model(ChainOrder order, RandomSeed seed);

    Token get(State const&);
    void load(std::istream&);
};

#endif /* __MARKOV_GENERATE_MODEL_H__ */
