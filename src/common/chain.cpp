#include <iostream>
#include <boost/format.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>

#include "chain.h"

template<typename Item>
Chain<Item>::Chain(ChainOrder order) : _order(order)
{
}

template<typename Item>
void Chain<Item>::learn(State<Item> const& state, Item const& token)
{
    auto &tree_with_stat = _data[state];
    auto &stat = tree_with_stat.first;
    auto &tree = tree_with_stat.second;
    ++stat;
    ++tree[token];
}

template class Chain<Token>;
template class Chain<Word>;
