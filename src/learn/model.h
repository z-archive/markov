#ifndef __MARKOV_LEARN_MODEL_H__
#define __MARKOV_LEARN_MODEL_H__

#include <iostream>
#include <map>
#include <boost/utility.hpp>

#include "common/types.h"
#include "common/state.h"

class Model : boost::noncopyable
{
private:
    typedef std::map<Token, Frequency> Tree;
    typedef std::map<State, Tree> Data;

public:
    Model(ChainOrder order);

    void add(State const&, Token const&);
    template<typename Archive>
    void serialize(Archive &ar, unsigned int const version)
    {
        ar & _order;
        ar & _data;
    }

private:
    ChainOrder _order;
    Data _data;
};

#endif /* __MARKOV_LEARN_MODEL_H__ */
