#ifndef __MARKOV_COMMON_CHAIN_H__
#define __MARKOV_COMMON_CHAIN_H__

#include <utility>
#include <map>
//#include <boost/unordered_map.hpp>
#include <boost/utility.hpp>


#include "common/types.h"
#include "common/state.h"

template<typename Item>
class Chain : boost::noncopyable
{
private:
    typedef Item                                 item_type;
    typedef Chain<Item>                          this_type;
    typedef State<item_type>                     input_state_type;
    typedef typename input_state_type::data_type state_type;
    typedef std::map<item_type, Frequency> branch_type;
    //typedef boost::unordered_map<item_type, Frequency> branch_type;
    typedef std::pair<Frequency, branch_type>    tree_type;
    typedef std::map<state_type, tree_type>     data_type;
    //typedef boost::unordered_map<state_type, tree_type>     data_type;

public:
    Chain(ChainOrder order);

    void learn(input_state_type const&, item_type const&);

    bool generate(input_state_type&, Item&) const;

    template<typename Translator>
    void merge(this_type const&, Translator&);

    template<typename Archive>
    void serialize(Archive &ar, unsigned int const /*version*/)
    {
      /*      for(auto pair: _data)
        {
          auto const &state  = pair.first;

          std::cout << "state size=" << state.size();
          for(auto i: state)
            {
              std::cout << " '" << i << "'";
            }
          std::cout << "\n";

          auto &tree  = _data[state];
          auto &stat_branch  = tree.first;
          auto &branch  = tree.second;

          std::cout << "stat branch " << stat_branch << "\n";

          for (auto pair: branch)
            {
              auto &item   = pair.first;
              auto &stat_item = branch[item];
              std::cout << "item " << item << " state " << stat_item << "\n";
            }
            }*/
        ar & _data;
    }

private:
    ChainOrder _order;
    data_type _data;
};

#endif /* __MARKOV_COMMON_CHAIN_H__ */
