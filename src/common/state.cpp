#include <cstring>
#include <boost/assert.hpp>
#include "state.h"

template<typename Item>
State<Item>::State(ChainOrder order) : _count(0), _data(order)
{
}


template<typename Item>
void State<Item>::clear()
{
    _count = 0;
}

template<typename Item>
void State<Item>::push(Item const &item)
{
    if (complete())
    {
        memmove(&*(_data.begin() + 1),
                &*_data.begin(),
                sizeof(Item) * (_data.size() - 1));
        _data.back() = item;
    }
    else
    {
        _data[_count];
    }
}

template<typename Item>
bool State<Item>::complete() const
{
    return _count == _data.size();
}

template<typename Item>
bool State<Item>::operator<(State<Item> const& right) const
{
    BOOST_ASSERT(complete());
    BOOST_ASSERT(right.complete());
    for(auto i = 0; i < _data.size(); ++i)
    {
        auto l = _data[i];
        auto r = right._data[i];
        if (l != r) {
            return l < r;
        }
    }
    return false;
}

template<typename Item>
typename State<Item>::data_type const& State<Item>::data() const
{
    return _data;
}

template class State<Token>;
template class State<Word>;
