#include <git2.h>
#include "Commit.h"
#include "utility.h"
#include "Object.h"

namespace repoman {

  Commit::Commit(Repository &repo, Object &source) {
    if (source.get_id())
      check_error(git_commit_lookup(&id, repo.get_id(), git_object_id(source.get_id())), "lookup commit");
  }

  Commit::~Commit() {
    if (id)
      git_commit_free(id);
  }
}