#ifndef __MARKOV_COMMON_MODEL_H__
#define __MARKOV_COMMON_MODEL_H__

#include "common/config.h"
#include "common/dictionary.h"
#include "common/chain.h"

namespace boost
{
namespace archive
{
class text_iarchive;
class text_oarchive;
} // namespace archive
} // namespace boost

typedef boost::archive::text_iarchive ArchiveInput;
typedef boost::archive::text_oarchive ArchiveOutput;

bool save_header(ArchiveOutput&, settings::Common&);
bool load_header(ArchiveInput&, settings::Common&);

class WordModel
{
public:
    typedef Word            item_type;
    typedef Chain<Word>     chain_type;
    typedef DummyWordDict   learn_converter_type;
    typedef DummyWordDict   generate_converter_type;
    typedef DummyTranslator translator_type;

    static bool save(ArchiveOutput&,
                     settings::Common const&,
                     learn_converter_type const&,
                     chain_type const&);

    static bool load(ArchiveInput&,
                     settings::Common const&,
                     generate_converter_type&,
                     chain_type&);
};


class TokenModel
{
public:
    typedef Token        item_type;
    typedef Chain<Token> chain_type;
    typedef WordDict     learn_converter_type;
    typedef TokenDict    generate_converter_type;
    typedef Translator   translator_type;

    static bool save(ArchiveOutput&,
                     settings::Common const&,
                     learn_converter_type const&,
                     chain_type const&);

    static bool load(ArchiveInput&,
                     settings::Common const&,
                     generate_converter_type&,
                     chain_type&);
};


#endif /* __MARKOV_COMMON_MODEL_H__ */
