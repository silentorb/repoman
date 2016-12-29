#include "Tree.h"
#include <git2.h>
#include "Index.h"
#include "utility.h"
#include "Repository.h"

namespace repoman {

  Tree::Tree(Index &index) {
    git_oid tree_id;
    check_error(git_index_write_tree(&tree_id, index.get_id()), "write index to tree");
    check_error(git_tree_lookup(&id, index.get_repo().get_id(), &tree_id), "lookup tree");
  }

  Tree::~Tree() {
    git_tree_free(id);
  }
}