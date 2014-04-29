#include <nnxx/error.h>
#include <nnxx/nn.h>

using namespace std;

namespace nnxx {

  const char *term_error::what() const noexcept
  { return strerror(ETERM); }

  timeout_error::timeout_error():
    std::system_error(std::make_error_code(std::errc(ETIMEDOUT)), strerror(ETIMEDOUT))
  { }

  signal_error::signal_error():
    std::system_error(std::make_error_code(std::errc(EINTR)), strerror(EINTR))
  { }

  namespace this_thread {

  int get_errno() noexcept
  { return nn_errno(); }

  errc get_errc() noexcept
  { return errc(get_errno()); }

  }

  const char *strerror() noexcept
  { return strerror(this_thread::get_errno()); }

  const char *strerror(int code) noexcept
  { return nn_strerror(code); }

  void throw_error()
  { throw_error(this_thread::get_errno()); }

  void throw_error(int code)
  {
    switch (code) {
    case ENOMEM:
      throw bad_alloc{ };

    case ETERM:
      throw term_error{ };

    case ETIMEDOUT:
      throw timeout_error{ };

    case EINTR:
      throw signal_error{ };

    default:
      throw system_error{ make_error_code(errc(code)), strerror() };
    }
  }

}
