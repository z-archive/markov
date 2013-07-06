#include <cstring>
#include <boost/assert.hpp>
#include "state.h"

State::State(ChainOrder order)
{
    _data.reserve(order);
}

void State::clear()
{
    _data.clear();
}

void State::add(Token token)
{
    if (_data.size() < _data.capacity())
    {
        _data.push_back(token);
    }
    else
    {
        Token* to   = &_data[0];
        Token* from = to + 1;
        auto bytes = sizeof(Token) * (_data.size() - 1);
        memmove(to, from, bytes);
        _data.back() = token;
    }
}

bool State::complete() const
{
    return _data.size() == _data.capacity();
}

bool State::less(State const& right) const
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
