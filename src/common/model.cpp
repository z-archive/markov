#include "model.h"

template<typename Item>
Model<Item>::Model(ChainOrder order) : _order(order)
{
}

template<typename Item>
void Model<Item>::add(State<Item> const& state, Item const& token)
{
    auto tree_with_stat = _data[state];
    auto stat = tree_with_stat.first;
    auto tree = tree_with_stat.second;
    ++stat;
    ++tree[token];
}

template class Model<Token>;
template class Model<Word>;
