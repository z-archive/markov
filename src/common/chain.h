#ifndef __MARKOV_COMMON_CHAIN_H__
#define __MARKOV_COMMON_CHAIN_H__

#include <map>
#include <utility>
#include <boost/utility.hpp>

#include "common/types.h"
#include "common/state.h"

template<class Item>
class Chain : boost::noncopyable
{
private:
    typedef std::map<Item, Frequency> Tree;
    typedef std::pair<Frequency, Tree> TreeWithStat;
    typedef std::map<State< Item >, TreeWithStat> Data;

public:
    Chain(ChainOrder order);

    void learn(State<Item> const&, Item const&);
    //void generate(State<Item> const&);

    template<typename Archive>
    void serialize(Archive &ar, unsigned int const /*version*/)
    {
        ar & _data;
    }

private:
    ChainOrder _order;
    Data _data;
};

#endif /* __MARKOV_COMMON_CHAIN_H__ */
