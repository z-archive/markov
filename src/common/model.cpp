#include <iostream>
#include <boost/format.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/map.hpp>

#include "model.h"

bool save_header(ArchiveOutput &archive,
                 settings::Common const &settings)
{
    archive << settings.order;
    archive << settings.max_word_length;
    archive << settings.compress;
}

bool load_header(ArchiveInput &archive,
                 settings::Common &settings)
{
    archive >> settings.order;
    archive >> settings.max_word_length;
    archive >> settings.compress;
}

bool WordModel::save(ArchiveOutput& archive,
                     settings::Common const& settings,
                     WordModel::learn_converter_type const& converter,
                     WordModel::chain_type const& chain)
{
    save_header(archive, settings);
    archive << converter;
    archive << chain;
}

bool WordModel::load(ArchiveInput& archive,
                     settings::Common const& settings,
                     WordModel::generate_converter_type& converter,
                     WordModel::chain_type& chain)
{
    BOOST_ASSERT(false == settings.compress);
    archive >> converter;
    archive >> chain;
}

bool TokenModel::save(ArchiveOutput& archive,
                     settings::Common const& settings,
                     TokenModel::learn_converter_type const& converter,
                     TokenModel::chain_type const& chain)
{
    save_header(archive, settings);
    archive << converter;
    archive << chain;
}

bool TokenModel::load(ArchiveInput& archive,
                     settings::Common const& settings,
                     TokenModel::generate_converter_type& converter,
                     TokenModel::chain_type& chain)
{
    BOOST_ASSERT(true == settings.compress);
    archive >> converter;
    archive >> chain;
}
