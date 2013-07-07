#include <cstring>
#include <boost/assert.hpp>
#include "state.h"

template<typename Item>
State<Item>::State()
{
}


template<typename Item>
State<Item>::State(ChainOrder order)
{
    _data.reserve(order);
}

template<typename Item>
void State<Item>::clear()
{
    _data.clear();
}

template<typename Item>
void State<Item>::push(Item item)
{
    if (_data.size() < _data.capacity())
    {
        _data.push_back(item);
    }
    else
    {
        Item* to   = &_data[0];
        Item* from = to + 1;
        auto bytes = sizeof(Item) * (_data.size() - 1);
        memmove(to, from, bytes);
        _data.back() = item;
    }
}

template<typename Item>
bool State<Item>::complete() const
{
    return _data.size() == _data.capacity();
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

template class State<Token>;
template class State<Word>;
