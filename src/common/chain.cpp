#include <iostream>
#include <random>
#include <boost/format.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>

#include "dictionary.h"
#include "chain.h"

template<typename Item>
Chain<Item>::Chain(ChainOrder order) : _order(order)
{
}

template<typename Item>
void Chain<Item>::learn(typename Chain<Item>::input_state_type const &state,
                        typename Chain<Item>::item_type  const &item)
{
    auto &tree        = _data[state.data()];
    auto &stat_branch = tree.first;
    auto &branch      = tree.second;
    auto &stat_item   = branch[item];
    ++stat_branch;
    ++stat_item;
}

template<typename Item>
bool Chain<Item>::generate(typename Chain<Item>::input_state_type &state,
                           typename Chain<Item>::item_type &item) const
{
    auto tree_iterator = _data.find(state.data());
    if (_data.end() == tree_iterator)
    {
        return false;
    }
    auto &tree  = tree_iterator->second;
    auto &total = tree.first;
    Frequency choise = rand() % total;
    Frequency current = 0;
    for (auto pair: tree.second)
    {
        current += pair.second;
        if (choise < current)
        {
            item = pair.first;
            state.push(item);
            return true;
        }
    }
    return false;
}

template<typename Item>
template<typename Translator>
void Chain<Item>::merge(typename Chain<Item>::this_type const &other,
                        Translator& translator)
{
    for(auto const &pair: other._data)
    {
        auto const &state_other = pair.first;
        auto state_this  = translator(state_other);

        auto &tree_this  = _data[state_this];
        auto &tree_other = pair.second;

        auto &stat_branch_this  = tree_this.first;
        auto &stat_branch_other = tree_other.first;

        stat_branch_this += stat_branch_other;

        auto &branch_this  = tree_this.second;
        auto &branch_other = tree_other.second;

        for (auto const &pair: branch_other)
        {
            auto const &item_other = pair.first;
            auto item_this  = translator(item_other);
            auto const &stat_item_other = pair.second;
            auto &stat_item_this = branch_this[item_this];

            stat_item_this += stat_item_other;
        }

    }
}

template class Chain<Token>;
template class Chain<Word>;
template void Chain<Token>::merge(Chain<Token> const&, Translator&);
template void Chain<Word>::merge(Chain<Word> const&, DummyTranslator&);
