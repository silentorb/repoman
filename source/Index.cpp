#include <git2.h>
#include "Index.h"
#include "utility.h"
#include "Repository.h"

namespace repoman {

  Index::Index(Repository &repo) :
    repo(repo) {
    check_error(git_repository_index(&id, repo.get_id()), "get repo index");
  }

  Index::~Index() {
    git_index_free(id);
  }
}