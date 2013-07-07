#include <fstream>
#include <boost/optional.hpp>
#include <boost/utility/typed_in_place_factory.hpp>

#include "file.h"

boost::optional<std::ifstream> fin;
boost::optional<std::ofstream> fout;

template<typename S, typename F>
S& resolve_stream(S& default_stream,
                  boost::optional<F>& file,
                  OptionalString const& path)
{
    if (!path) {
        return default_stream;
    }
    file = boost::in_place(path.get());
    return file.get();
}

std::istream& getInput(OptionalString const& path)
{
    return resolve_stream(std::cin, fin, path);
}

std::ostream& getOutput(OptionalString const& path)
{
    resolve_stream(std::cout, fout, path);
}
