#include <cerrno>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/algorithm/string/replace.hpp>

#include "downloader.h"

typedef std::string Argument;
typedef std::vector< Argument > ArgumentList;

using namespace boost;

process::child p_launch(Url const& url, Timeout const& timeout, bool verbose)
{
  ArgumentList al;
  al.push_back((format("--connect-timeout %1%") % timeout).str());
  // do not show progress bar
  al.push_back("-s");
  // but show error message
  al.push_back("-S");
  al.push_back(url);
  process::context context;
  context.environment = process::self::get_environment();
  context.stdout_behavior = process::capture_stream();
  if (verbose) {
    context.stderr_behavior = process::capture_stream();
    std::cerr << format("starting download %1%...\n") % url << std::flush;
  }

  return process::launch(std::string("/usr/bin/curl"), al, context);
}

std::string p_stderr(boost::process::child& process)
{
  typedef std::istreambuf_iterator<char> isi;
  isi eos;
  std::string result(isi(process.get_stderr()), eos);
  replace_all(result, "\n", "\\n");
  return result;
}

Downloader::Downloader(Url const& url, Timeout const& timeout, bool verbose) : 
  _verbose(verbose),
  _url(url),
  _work(true),
  _child(p_launch(url, timeout, verbose))
{
}

Downloader::~Downloader()
{
  join();
}

std::istream& Downloader::data()
{
  return _child.get_stdout();
}

void Downloader::join()
{
  if (!_work) {
    return;
  }
  _work = false;

  auto s = _child.wait();

  if (_verbose) {
    format m; // message
    int code = s.exit_status();
    if (0 == code) {
      m = format("download of '%1%': succesfully\n") % _url;
    } else {
      m = format("download of '%1%': error, exit code is %2%, stderr is '%3%'\n")
	% _url % code % p_stderr(_child);
    }
    std::cerr << m << std::flush;
  }
}

Url const& Downloader::url() const
{
  return _url;
}
