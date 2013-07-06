#ifndef __MARKOV_COMMON_MODEL_H__
#define __MARKOV_COMMON_MODEL_H__

#include <map>
#include <utility>
#include <boost/utility.hpp>

#include "common/types.h"
#include "common/state.h"

template<class Item>
class Model : boost::noncopyable
{
private:
    typedef std::map<Item, Frequency> Tree;
    typedef std::pair< Frequency, Tree > TreeWithStat;
    typedef std::map<State< Item >, TreeWithStat> Data;

public:
    Model(ChainOrder order);

    void add(State<Item> const&, Item const&);

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


#endif /* __MARKOV_COMMON_MODEL_H__ */
