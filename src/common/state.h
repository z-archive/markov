#ifndef __MARKOV_COMMON_STATE_H__
#define __MARKOV_COMMON_STATE_H__

#include <vector>
#include <boost/utility.hpp>
#include "common/types.h"

template<typename Item>
class State
{
private:
    typedef std::vector<Item> Data;

public:
    State(ChainOrder order);

    void clear();
    void add(Item);
    bool complete() const;
    bool less(State const&) const;

    template<typename Archive>
    void serialize(Archive &ar, unsigned int const version)
    {
        ar & _data;
    }

private:
    Data _data;
};

template<typename Item>
inline bool operator<(State<Item> const& l, State<Item> const& r)
{
    return l.less(r);
}

#endif /* __MARKOV_COMMON_STATE_H__ */
