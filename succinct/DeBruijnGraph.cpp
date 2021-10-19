//
// Created by daniel on 13/10/2021.
//

#include <fstream>

#include <iostream>

#include <algorithm>
#include <vector>

#include "DeBruijnGraph.h"

void replaceAll(std::string &str, const std::string &from,
                const std::string &to) {
  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
}

// For co-lex. sorting node matrix with edge label vector
typedef struct nodeWithEdge {
  nodeWithEdge(std::vector<std::string> NodeLabel, std::string EdgeLabel) {
    nodeLabel = NodeLabel;
    edgeLabel = EdgeLabel;
  }

  std::vector<std::string> nodeLabel;
  std::string edgeLabel;
} nodeWithEdge_t;

DeBruijnGraph::DeBruijnGraph(int K, const std::string &path) {
  std::vector<std::string> alphabet{"A", "T", "C", "G"};
  numReads = 0;
  numNodes = 0;
  k = K;
  w = nullptr;
  last = new BitVector<bool>;
  std::vector<std::vector<std::string>> nodes;
  std::ifstream readData(path);

  if (readData.is_open()) {
    std::vector<std::string> _w;
    std::vector<nodeWithEdge> nodesWithEdges;

    for (std::string read; getline(readData, read);) {
      // Read padding
      read.insert(0, std::string(k, '$'));
      read.insert(read.size(), "$");
      first["$" + std::to_string(numReads)] = numReads;
      alphabet.push_back("$" + std::to_string(numReads));

      // K-mer construction
      for (int i = 0; i < read.size() - (k); i++) {
        if (i == read.size() - k - 1)
          numReads++;

        std::string readSubstring = read.substr(i, k);
        std::vector<std::string> kmer;

        kmer.reserve(k);
        for (int j = 0; j < k; j++) {
          kmer.push_back(readSubstring[j] == '$'
                             ? "$" + std::to_string(numReads)
                             : std::string(1, readSubstring[j]));
        }

        std::string edgeLabel = read[k + i] == '$'
                                    ? "$" + std::to_string(numReads)
                                    : std::string(1, read[k + i]);

        std::reverse(kmer.begin(), kmer.end());
        bool exists =
            std::find_if(nodesWithEdges.begin(), nodesWithEdges.end(),
                         [&edgeLabel, &kmer](const nodeWithEdge &nwe) -> bool {
                           return nwe.nodeLabel == kmer &&
                                  nwe.edgeLabel == edgeLabel;
                         }) != nodesWithEdges.end();

        if (!exists) {
          nodesWithEdges.emplace_back(kmer, edgeLabel);
          numNodes++;
        }
      }
    }

    /*
     * Sort nodes and W tree by co-lex ordering of nodes
     * Tie-break via edge labels
     */
    std::sort(nodesWithEdges.begin(), nodesWithEdges.end(),
              [this](auto const &a, auto const &b) {
                for (int i = 0; i < k; i++) {
                  if (a.nodeLabel[i] == b.nodeLabel[i])
                    continue;
                  return a.nodeLabel[i] < b.nodeLabel[i];
                }
                return a.edgeLabel < b.edgeLabel;
              });
    for (auto &nodesWithEdge : nodesWithEdges) {
      std::reverse(nodesWithEdge.nodeLabel.begin(),
                   nodesWithEdge.nodeLabel.end());
      nodes.push_back(nodesWithEdge.nodeLabel);
      _w.push_back(nodesWithEdge.edgeLabel);
    }

    // W Tree
    w = new WaveletTree(alphabet, _w);

    // L vector
    for (int i = 0; i < nodes.size() - 1; i++) {
      bool *diff = new bool(false);
      for (int j = 0; j < k && !*diff; j++) {
        if (nodes[i][j] != nodes[i + 1][j])
          *diff = true;
      }
      last->pushBack(*diff);
      delete diff;
    }
    last->pushBack(true);

    // Flag setting
    for (int i = 0; i < nodes.size(); i++) {
      flags.emplace_back(false);
      if (i > 0) {
        for (int j = 0; j < i; j++) {

          // If ∃ j < i | w[j] = w[i]
          if (w->access(j) == w->access(i)) {
            bool *same = new bool(true);
            // See if they share suffixes
            for (int l = 1; l < k && *same; l++) {
              if (nodes[j][l] != nodes[i][l]) {
                *same = false;
              }
            }

            // If they do, then flag i's edge
            if (*same) {
              *flags[i].state = true;
              *flags[i].indexTo = j;
              *flags[j].indexFrom = i;
            }
          }
        }
        // F vector
        if (nodes[i][k - 1] != nodes[i - 1][k - 1])
          first[nodes[i][k - 1]] = i;
      }

      //            std::cout << i << " " <<  last->access(i) << "   ";
      //            for (int j = 0; j < k; j++) {
      //              std::cout << nodes[i][j] << " ";
      //            }
      //            std::cout << "  " << w->access(i) << " "
      //                      << (*flags[i].state ? "-\n" : "\n");
    }

    //        std::cout << "\n\nFirst Indexes";
    //        for (auto &it : first) {
    //          std::cout << it.first << " " << it.second << "\n";
    //        }
  } else {
    std::cout << "Could not open file " << path << ".\n";
  }
}

int DeBruijnGraph::forward(int u) {
  std::string c = w->access(u);
  int *_u = new int(*flags[u].state ? *flags[u].indexTo : u);
  int *rankC = new int(w->rank(c, *_u));

  /*
   * Since flags and edge labels are stored in separate structures
   * once W.rank(c, *_u) is calculated, traverse the interval [0, _u)
   * if W[i] == c and a flag is set at i then W[i] ∈ A⁻ ==> W[i] != c
   * so decrement rankC since it was counted in W.rank(c, *_u) originally.
   */
  for (int i = 0; i < *_u; i++) {
    if (*flags[i].state && w->access(i) == c) {
      *rankC -= 1;
    }
  }

  int startPosition = first[c];
  int rankToBase = last->rank(true, startPosition);
  int nodeIndex = last->select(true, (*rankC + rankToBase));

  delete _u;
  delete rankC;

  return nodeIndex;
}

int DeBruijnGraph::backward(int v) {
  std::string c = "$0";
  int *minIndex = new int(-1);

  for (auto &it : first) {
    if (v >= it.second && it.second >= *minIndex) {
      c = it.first;
      *minIndex = it.second;
    }
  }

  int rankToBase = last->rank(true, first[c]);
  int rankToCurrentEdge = last->rank(true, (v));
  int index = rankToCurrentEdge - rankToBase;

  /*
   * When calculating the edge index, w.select() won't take into
   * account flagged edges. We define an offset and increment for every
   * flagged c over the interval [0, w.select(c, index+ 1)).
   * This ensures that we skip past flagged edges on the final
   * select call.
   */

  int *indexOffset = new int(0);
  for (int i = 0; i < w->select(c, index + 1); i++) {
    if (*flags[i].state && w->access(i) == c) {
      *indexOffset += 1;
    }
  }

  int edge = w->select(c, index + *indexOffset);
  return edge;
}

int DeBruijnGraph::outdegree(int v) {
  return last->select(true, v) - last->select(true, v - 1);
}

// Returns index of node
int DeBruijnGraph::outgoing(int v, const std::string &c) {
  int *x = new int(w->select(c, w->rank(c, v)));
  int *nodeIndex = new int(last->select(true, v));
  int *nodeIndexBefore = new int(last->select(true, v - 1));
  bool *edgeExists = new bool(false);

  if (*x > *nodeIndexBefore && *x <= *nodeIndex)
    *edgeExists = true;
  else {
    int *_v = new int(-1);
    for (int i = (v - 1); i >= 0; i--) {
      if (*flags[i].state != *flags[v].state && w->access(i) == w->access(v)) {
        *_v = i;
        break;
      }
    }
    *x = w->select(c, w->rank(c, *_v));
    *nodeIndex = last->select(true, *_v);
    *nodeIndexBefore = last->select(true, *_v - 1);

    if (*x > *nodeIndexBefore && *x <= *nodeIndex)
      *edgeExists = true;
  }
  int edge = *x;
  bool _edgeExists = *edgeExists;

  delete x;
  delete nodeIndex;
  delete nodeIndexBefore;
  delete edgeExists;

  return _edgeExists ? forward(edge) : -1;
}

std::string DeBruijnGraph::label(int v) {
  int *minIndex = new int(-1);
  int *edgeIndex = new int(last->select(true, v));
  std::vector<std::string> label(k, " ");

  if (v >= numReads) {
    for (auto &it : first) {
      if (*edgeIndex >= it.second && it.second >= *minIndex) {
        label[0] = it.first;
        *minIndex = it.second;
      }
    }

    for (int i = 1; i < k && *edgeIndex != 0; i++) {
      if (*edgeIndex < numReads) {
        label[i] = ("$" + std::to_string(*edgeIndex));
      } else {
        *edgeIndex = backward(*edgeIndex);
        minIndex = new int(-1);
        for (auto &it : first) {
          if (*edgeIndex >= it.second && it.second >= *minIndex) {
            *minIndex = it.second;
            label[i] =
                it.first == ("$" + std::to_string(numReads)) ? "$0" : it.first;
          }
        }
      }
    }
    if (*edgeIndex == 0)
      label[k - 1] = "$0";
    std::reverse(label.begin(), label.end());
  }

  delete edgeIndex;
  if (v >= numReads) {
    std::string s;
    for (const auto &nucleotide : label)
      s += nucleotide;
    return s;
  }
  std::string padding;
  for (int i = 0; i < k; i++) {
    padding += ("$" + std::to_string(v));
  }
  return padding;
}

int DeBruijnGraph::indegree(int v) {
  int flaggedRank1 = 0;
  int flaggedRank2 = 0;
  int firstIncomingEdge = backward(last->select(true, v));

  if (v >= first["A"]) {
    std::string firstIncomingEdgeLabel = w->access(firstIncomingEdge);

    int *nonFlaggedEdge = new int(-1);
    bool *nonFlaggedEdgeFound = new bool(false);

    for (int i = firstIncomingEdge + 1; i < numNodes && !*nonFlaggedEdgeFound;
         i++) {
      if (firstIncomingEdgeLabel == w->access(i) && !*flags[i].state) {
        *nonFlaggedEdgeFound = true;
        *nonFlaggedEdge = i;
      }
    }

    for (int i = 0; i < *nonFlaggedEdge; i++) {
      if (w->access(i) == firstIncomingEdgeLabel && *flags[i].state) {
        if (i < firstIncomingEdge)
          flaggedRank1++;
        if (i < *nonFlaggedEdge)
          flaggedRank2++;
      }
    }

    return (flaggedRank2 - flaggedRank1) + 1;
  }
  return 0;
}

std::vector<std::string> DeBruijnGraph::labelV(int v) {
  int *edgeIndex = new int(last->select(true, v));
  std::vector<std::string> label(k, ("$" + std::to_string(v)));

  if (v >= numReads) {
    for (auto &it : first) {
      if (*edgeIndex >= it.second) {
        label[0] = it.first;
      }
    }

    for (int i = 1; i < k && *edgeIndex != 0; i++) {
      *edgeIndex = backward(*edgeIndex);
      for (auto &it : first) {
        if (*edgeIndex >= it.second) {
          label[i] = it.first;
        }
      }
    }
    if (*edgeIndex == 0)
      label[k - 1] = "$0";
    std::reverse(label.begin(), label.end());
  }

  return label;
}

int DeBruijnGraph::incoming(int v, const std::string &c) {
  int index = last->select(true, v);
  int rangeStart = backward(index);
  int rangeEnd = rangeStart + (indegree(v) + 1);
  std::vector<std::string> nodeLabel = labelV(v);

  if (rangeEnd > 0) {
    for (int i = rangeStart; i < rangeEnd; i++) {
      std::string edgeLabel = w->access(i);

      if (edgeLabel == nodeLabel[k - 1]) {
        int rankToI = last->rank(true, i);
        std::string _label = label(rankToI);

        if (_label.substr(0, c.size()) == c)
          return rankToI;
      }
    }
  }
  return -1;
}