#include <git2.h>
#include "User.h"
#include "utility.h"

namespace repoman {

  User::User(std::string &name, std::string &email) :
    name(name), email(email) {}

}
