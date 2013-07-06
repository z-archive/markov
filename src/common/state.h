#ifndef __MARKOV_COMMON_STATE_H__
#define __MARKOV_COMMON_STATE_H__

#include <vector>
#include <boost/utility.hpp>
#include "common/types.h"

class State
{
private:
    typedef std::vector<Token> Data;

public:
    State(ChainOrder order);

    void clear();
    void add(Token);
    bool complete() const;
    bool less(State const&) const;

private:
    Data _data;
};

inline bool operator<(State const& l, State const& r)
{
    return l.less(r);
}

#endif /* __MARKOV_COMMON_STATE_H__ */
