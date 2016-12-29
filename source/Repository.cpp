#include <git2.h>
#include "Repository.h"
#include "Git_Manager.h"

namespace repoman {

  Repository::Repository(const std::string &path, const std::string &url) :
    path(path), url(url) {
    Git_Manager::ensure_initialized();
    int error = git_repository_init(&repo, path.c_str(), false);
  }
}