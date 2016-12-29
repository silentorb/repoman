#pragma once

#include <string>

namespace repoman {

  class User {
      std::string &name;
      std::string &email;

  public:
      User(std::string &name, std::string &email);

      std::string &get_name() const {
        return name;
      }

      std::string &get_email() const {
        return email;
      }
  };
}