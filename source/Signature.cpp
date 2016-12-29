#include <git2.h>
#include "Signature.h"
#include "utility.h"
#include "User.h"

namespace repoman {
  Signature::Signature(const Repository &repo) {
//    check_error(git_signature_now(&id, repo.get_name().c_str(), repo.get_email().c_str()), "create signature");
    check_error(git_signature_default(&id, repo.get_id()), "get configured signature");
  }

  Signature::~Signature() {
    git_signature_free(id);
  }

}
