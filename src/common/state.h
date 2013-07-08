#ifndef __MARKOV_COMMON_STATE_H__
#define __MARKOV_COMMON_STATE_H__

#include <vector>
#include <boost/utility.hpp>
#include <boost/scoped_ptr.hpp>
#include "common/types.h"

template<typename Item>
class State : boost::noncopyable
{
public:
    typedef State<Item>       this_type;
    typedef std::vector<Item> data_type;

    State(ChainOrder order);

    void clear();
    void push(Item const&);
    bool complete() const;

    bool operator<(this_type const&) const;

    template<typename Archive>
    void serialize(Archive &ar, unsigned int const version)
    {
        ar & _data;
    }

    data_type const& data() const;

private:
    ChainOrder _count;
    data_type _data;
};

#endif /* __MARKOV_COMMON_STATE_H__ */
