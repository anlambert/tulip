/**
 *
 * Copyright (C) 2021  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <iomanip>

#include <git2.h>

#include <talipot/MaterialDesignIcons.h>
#include <talipot/PluginHeaders.h>
#include <talipot/ViewSettings.h>

using namespace std;
using namespace tlp;

namespace std {
template <>
struct hash<const git_oid> {
  size_t operator()(const git_oid &oid) const {
    string sha1_git = git_oid_tostr_s(&oid);
    return stoul(sha1_git.substr(0, 7), nullptr, 16);
  }
};
}

static bool operator==(const git_oid &oid, const git_oid &oid2) {
  return git_oid_equal(&oid, &oid2);
}

static string iso8601(const git_time *gt) {
  ostringstream oss;
  oss << put_time(gmtime(reinterpret_cast<const time_t *>(&gt->time)), "%FT%TZ");
  return oss.str();
}

static constexpr string_view paramHelp[] = {
    "Path to a git repository",
};

/**
 * Talipot plugin to import the graph of git objects (commits, trees, blobs, tags)
 * from a local repository.
 *
 */
class GitImport : public ImportModule {
public:
  PLUGININFORMATION("Git", "Antoine Lambert", "20/05/2021",
                    "Imports git objects graph from a repository.", "1.0", "Software")
  GitImport(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<string>("dir::directory", paramHelp[0].data(), "");
  }
  ~GitImport() override = default;

  string icon() const override {
    return TalipotBitmapDir + "git.png";
  }

  git_oid addCommitNode(git_oid commitOid) {
    // get commit info
    git_commit *commit = nullptr;
    git_commit_lookup(&commit, repo, &commitOid);
    const git_signature *author = git_commit_author(commit);
    const git_signature *committer = git_commit_committer(commit);

    // create commit node
    node n = graph->addNode();
    oidNode[commitOid] = n;
    string sha1_git = git_oid_tostr_s(&commitOid);
    // save commit data in dedicated properties
    graph->getStringProperty("viewLabel")->setNodeValue(n, sha1_git.substr(0, 7));
    graph->getStringProperty("object_type")->setNodeValue(n, "commit");
    graph->getStringProperty("sha1_git")->setNodeValue(n, sha1_git);
    graph->getStringProperty("author_name")->setNodeValue(n, author->name);
    graph->getStringProperty("author_email")->setNodeValue(n, author->email);
    graph->getStringProperty("author_date")->setNodeValue(n, iso8601(&author->when));
    graph->getStringProperty("committer_name")->setNodeValue(n, committer->name);
    graph->getStringProperty("committer_email")->setNodeValue(n, committer->email);
    graph->getStringProperty("committer_date")->setNodeValue(n, iso8601(&committer->when));
    string message = git_commit_message(commit);
    graph->getStringProperty("message")->setNodeValue(n, message.substr(0, message.size() - 1));
    // set commit node visual attributes
    graph->getStringProperty("viewIcon")->setNodeValue(n, MaterialDesignIcons::SourceCommit);
    graph->getDoubleProperty("viewRotation")->setNodeValue(n, 90);
    graph->getColorProperty("viewColor")->setNodeValue(n, Color::Blue);
    // process commit parents, already seen as we walk on commits in reverse topological order
    vector<string> parents;
    unsigned int nbParents = git_commit_parentcount(commit);
    for (unsigned int i = 0; i < nbParents; ++i) {
      const git_oid *parentOid = git_commit_parent_id(commit, i);
      commitsSg->addNodes({oidNode[*parentOid], n});
      edge e = graph->addEdge(oidNode[*parentOid], n);
      commitsSg->addEdge(e);
      parents.push_back(git_oid_tostr_s(parentOid));
    }
    graph->getStringVectorProperty("parents")->setNodeValue(n, parents);
    git_oid treeOid = *git_commit_tree_id(commit);
    git_commit_free(commit);
    return treeOid;
  }

  void addTreeAndBlobNodes(const git_oid rootTreeOid) {
    if (const auto it = oidNode.find(rootTreeOid); it != oidNode.end()) {
      return;
    }
    node n = addTreeNode(rootTreeOid);
    currentRootTreeId = git_oid_tostr_s(&rootTreeOid);
    currentRootTreeSg = rootTreesSg->addSubGraph(currentRootTreeId);
    currentRootTreeSg->addNode(n);
    treesBlobsSg->addNode(n);
    graph->getStringProperty("viewLabel")->setNodeValue(n, "/");
    treePathToNode.clear();
    treePathToNode[""] = n;
    git_tree *tree = nullptr;
    git_tree_lookup(&tree, repo, &rootTreeOid);
    git_tree_walk(tree, GIT_TREEWALK_PRE, &GitImport::gitTreewalkCb, this);
    git_tree_free(tree);
  }

  static int gitTreewalkCb(const char *root, const git_tree_entry *entry, void *payload) {
    int ret = 0;
    GitImport *gitImport = static_cast<GitImport *>(payload);
    git_oid entryOid = *git_tree_entry_id(entry);
    node rootNode = gitImport->treePathToNode[root];
    string entryName = git_tree_entry_name(entry);
    string entryPath = root + entryName + "/";
    node n;
    if (git_tree_entry_type(entry) == GIT_OBJ_TREE) {
      if (const auto it = gitImport->oidNode.find(entryOid); it != gitImport->oidNode.end()) {
        ret = 1;
      }
      n = gitImport->addTreeNode(entryOid);
    } else if (git_tree_entry_type(entry) == GIT_OBJ_BLOB) {
      n = gitImport->addBlobNode(entryOid);
    }
    if (n.isValid()) {
      Graph *treeSg = gitImport->currentRootTreeSg;
      treeSg->addNode(n);
      edge e = gitImport->graph->addEdge(rootNode, n);
      gitImport->treesBlobsSg->addNode(n);
      gitImport->treesBlobsSg->addEdge(e);
      treeSg->addEdge(e);
      gitImport->graph->getStringProperty("name")->setNodeValue(n, entryName);
      gitImport->graph->getStringProperty("path")->setNodeValue(n, entryPath);
      gitImport->graph->getStringProperty("viewLabel")->setNodeValue(n, entryName);
      gitImport->treePathToNode[entryPath] = n;
    }
    return ret;
  }

  node addTreeNode(const git_oid treeOid) {
    if (const auto it = oidNode.find(treeOid); it != oidNode.end()) {
      return it->second;
    }
    git_tree *tree = nullptr;
    git_tree_lookup(&tree, repo, &treeOid);

    // create tree node
    node n = graph->addNode();
    oidNode[treeOid] = n;

    graph->getStringProperty("object_type")->setNodeValue(n, "tree");
    graph->getStringProperty("sha1_git")->setNodeValue(n, git_oid_tostr_s(&treeOid));

    graph->getStringProperty("viewIcon")->setNodeValue(n, MaterialDesignIcons::Folder);
    graph->getColorProperty("viewColor")->setNodeValue(n, Color::JungleGreen);
    git_tree_free(tree);
    return n;
  }

  node addBlobNode(const git_oid blobOid) {
    if (const auto it = oidNode.find(blobOid); it != oidNode.end()) {
      return it->second;
    }
    git_blob *blob = nullptr;
    git_blob_lookup(&blob, repo, &blobOid);

    // create blob node
    node n = graph->addNode();
    oidNode[blobOid] = n;

    graph->getStringProperty("object_type")->setNodeValue(n, "blob");
    graph->getStringProperty("sha1_git")->setNodeValue(n, git_oid_tostr_s(&blobOid));

    graph->getStringProperty("viewIcon")->setNodeValue(n, MaterialDesignIcons::FileDocument);
    graph->getColorProperty("viewColor")->setNodeValue(n, Color::Red);
    git_blob_free(blob);
    return n;
  }

  bool importGraph() override {

    if (!dataSet) {
      return false;
    }

    string gitRepoPath;
    dataSet->get("dir::directory", gitRepoPath);

    vector<string> pathParts = tokenize(gitRepoPath, "/");
    graph->setName(pathParts.back() + ".git");
    rootTreesSg = graph->addSubGraph("root trees");
    commitsSg = graph->addSubGraph("commits");
    treesBlobsSg = graph->addSubGraph("trees and blobs");

    graph->getIntegerProperty("viewShape")->setAllNodeValue(NodeShape::Icon);

    git_libgit2_init();

    if (git_repository_open(&repo, gitRepoPath.c_str()) < 0) {
      if (pluginProgress) {
        pluginProgress->setError(giterr_last()->message);
      }
      git_libgit2_shutdown();
      return false;
    }

    git_revwalk *walker = nullptr;
    git_revwalk_new(&walker, repo);
    git_revwalk_sorting(walker, GIT_SORT_TOPOLOGICAL | GIT_SORT_REVERSE);
    git_revwalk_push_head(walker);

    git_oid commitOid;

    unsigned int nbCommits = 0;
    while (!git_revwalk_next(&commitOid, walker)) {
      nbCommits++;
    }

    git_revwalk_reset(walker);
    git_revwalk_sorting(walker, GIT_SORT_TOPOLOGICAL | GIT_SORT_REVERSE);
    git_revwalk_push_head(walker);

    unsigned int i = 0;
    while (!git_revwalk_next(&commitOid, walker)) {
      git_oid rootTreeOid = addCommitNode(commitOid);
      addTreeAndBlobNodes(rootTreeOid);
      graph->addEdge(oidNode[commitOid], oidNode[rootTreeOid]);

      if (pluginProgress) {
        pluginProgress->progress(++i, nbCommits);
        pluginProgress->setComment(to_string(i) + " / " + to_string(nbCommits) +
                                   " commits processed");
      }
    }

    git_revwalk_free(walker);
    git_repository_free(repo);

    git_libgit2_shutdown();

    string err;
    graph->applyPropertyAlgorithm("Balloon (OGDF)", graph->getLayoutProperty("viewLayout"), err);

    return true;
  }

  git_repository *repo = nullptr;
  unordered_map<const git_oid, node> oidNode;
  unordered_map<string, node> treePathToNode;
  string currentRootTreeId;
  Graph *commitsSg = nullptr;
  Graph *treesBlobsSg = nullptr;
  Graph *rootTreesSg = nullptr;
  Graph *currentRootTreeSg = nullptr;
};

PLUGIN(GitImport)
