#include "model.h"

Model::Model(ChainOrder order) : _order(order)
{
}

void Model::add(State const& state, Token const& token)
{
    _data[state][token] += 1;
}
