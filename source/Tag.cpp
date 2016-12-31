#include <git2.h>
#include "Tag.h"
#include "utility.h"
#include "Repository.h"

namespace repoman {

  Tag::Tag(Repository &repo, const git_oid *object_id) {
    check_error(git_tag_lookup(&id, repo.get_id(), object_id), "lookup tag");
  }

  Tag::~Tag() {
    if (id)
      git_tag_free(id);
  }

}