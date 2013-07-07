#include <iostream>
#include <boost/format.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>

#include "model.h"

template<typename Item>
Model<Item>::Model(ChainOrder order) : _order(order)
{
}

template<typename Item>
Model<Item>::Model(boost::archive::text_iarchive &ar)
{
    ar >> _order;
    ar >> _data;
}

template<typename Item>
void Model<Item>::add(State<Item> const& state, Item const& token)
{
    auto &tree_with_stat = _data[state];
    auto &stat = tree_with_stat.first;
    auto &tree = tree_with_stat.second;
    ++stat;
    ++tree[token];
}

/*
template<typename Item>
void Model<Item>::print(std::ostream &out) const
{
    using std::operator<<;
    using boost::operator<<;
    using boost::format;

    for (auto p: _data)
    {
        out << p.first;
        out << " => ";
        out << p.second.first;
        out << " = [\n";

        for (auto i_a_f: p.second.second)
        {
            out << format("\t%1% => %2%\n")
                    % i_a_f.first
                    % i_a_f.second;
        }
        out << "]\n" << std::flush;
    }
}
*/

template class Model<Token>;
template class Model<Word>;
