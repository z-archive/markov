#include <boost/format.hpp>
#include "file.h"

using boost::format;

template<typename S, typename F>
S& resolve_stream(S& default_stream,
                  F& file,
                  OptionalString const& path)
{
    if (!path) {
        return default_stream;
    }
    file.open(path.get());
    if (!file.is_open())
    {
        auto message = format("can't open file '%1%'") % path.get();
        throw std::runtime_error(message.str());
    }
    return file;
}

std::istream& getInput(std::ifstream &fin,
                       OptionalString const& path)
{
    return resolve_stream(std::cin, fin, path);
}

std::ostream& getOutput(std::ofstream &fout,
                        OptionalString const& path)
{
    resolve_stream(std::cout, fout, path);
}
