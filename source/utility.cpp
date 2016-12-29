#include "utility.h"
#include <stdexcept>

namespace repoman {

  void check_error(int error_code, const std::string &message) {
    if (error_code < 0) {
      throw std::runtime_error("Git Error: " + message);
    }
  }
}
