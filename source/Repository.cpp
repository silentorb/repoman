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

  Repository::Repository(const std::string &path) :
    path(path) {
    Git_Manager::ensure_initialized();
  }

  Repository::~Repository() {
    git_repository_free(id);
  }

  void Repository::initialize() {
    check_error(git_repository_init(&id, path.c_str(), false), "initializing repo");
  }

  void Repository::clone(const std::string &url) {
    check_error(git_clone(&id, url.c_str(), path.c_str(), nullptr), "clone repo");
  }

  void Repository::open() {
    check_error(git_repository_open(&id, path.c_str()), "open repo");
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

  int status_callback(const char *path, unsigned int flags, void *payload) {
    const Status_Delegate &delegate = *static_cast<Status_Delegate *>(payload);
    delegate(path, flags);
    return 0;
  }

  void Repository::enumerate_status(const Status_Delegate &delegate) {
    check_error(git_status_foreach(id, status_callback, const_cast<Status_Delegate *>(&delegate)),
                "enumerate repo status");
  }
}