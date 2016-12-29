#include <git2.h>
#include "Repository.h"
#include "Git_Manager.h"
#include "utility.h"
#include "User.h"
#include "Signature.h"
#include "Index.h"

using namespace std;

namespace repoman {

  typedef struct { /* … */ } match_data;

  int matcher(const char *path, const char *spec, void *payload) {
    match_data *d = (match_data *) payload;
    /*
     * return 0 to add/remove this path,
     * a positive number to skip this path,
     * or a negative number to abort the operation.
     */
    return 0;
  }

  Repository::Repository(const std::string &path, const std::string &url) :
    path(path), url(url) {
    Git_Manager::ensure_initialized();
    check_error(git_repository_init(&id, path.c_str(), false), "initializing repo");
  }

  Repository::~Repository() {
    git_repository_free(id);
  }

  void Repository::add_all() {
//    git_index *index = nullptr;

//    check_error(git_index_add_bypath(index, "README.md"), "add all files to index");

    auto index = Index(*this);
    const char *paths[] = {"."};
    git_strarray git_paths = {const_cast<char **>(paths), 1};

    match_data data;
    check_error(git_index_add_all(index.get_id(), &git_paths, GIT_INDEX_ADD_DEFAULT, matcher, &data),
                "add all files to index");

    check_error(git_index_update_all(index.get_id(), &git_paths, matcher, &data), "add all files to index");
    check_error(git_index_write(index.get_id()), "save index");
    current_tree = unique_ptr<Tree>(new Tree(index));
  }

  void Repository::commit(const std::string &message) {
    auto signature = Signature(*this);
    auto signature_id = signature.get_id();

//    const git_commit *parents[] = {parent1, parent2};

    git_oid new_commit_id = {0};
    check_error(git_commit_create(
      &new_commit_id,
      id,
      "HEAD",                      /* name of ref to update */
      signature_id,                          /* author */
      signature_id,                          /* committer */
      "UTF-8",                     /* message encoding */
      message.c_str(),  /* message */
      current_tree->get_id(),                        /* root tree */
      0,                           /* parent count */
      nullptr), "create commit");                    /* parents */
  }

}