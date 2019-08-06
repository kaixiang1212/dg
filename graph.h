#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <tuple>
#include <vector>

namespace gdwg {

template <typename N, typename E> class Graph {
 public:
  class const_iterator;
  class const_reverse_iterator;
  // ---------------------- Constructors ----------------------

  Graph<N, E>();
  Graph<N, E>(typename std::vector<N>::const_iterator,
              typename std::vector<N>::const_iterator);
  Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator,
              typename std::vector<std::tuple<N, N, E>>::const_iterator);
  Graph<N, E>(std::initializer_list<N>);
  Graph<N, E>(const gdwg::Graph<N, E> &) noexcept;
  Graph<N, E>(gdwg::Graph<N, E> &&) noexcept;

  ~Graph<N, E>();
  // ---------------------- Operations ----------------------

  Graph<N, E> &operator=(const Graph &);
  Graph<N, E> &operator=(Graph &&) noexcept;

  // ---------------------- Methods ----------------------
  bool InsertNode(const N &val);
  bool InsertEdge(const N &src, const N &dst, const E &w);
  bool DeleteNode(const N &);

  bool Replace(const N &oldData, const N &newData);
  void MergeReplace(const N &oldData, const N &newData);
  void Clear();

  bool IsNode(const N &val);
  bool IsConnected(const N &src, const N &dst);
  bool IsConnectedWeight(const N &src, const N &dst, const E &);
  std::vector<N> GetNodes() const;
  std::vector<N> GetConnected(const N &src);
  std::vector<E> GetWeights(const N &src, const N &dst);

  const_iterator find(const  N&, const N &, const E &);

  bool erase(const N &src, const N &dst, const E &w);
  const_iterator erase(const_iterator it);

  const_iterator cbegin();
  const_iterator cend();
  const_reverse_iterator crbegin();
  const_reverse_iterator crend();
  const_iterator begin();
  const_iterator end();
  const_reverse_iterator rbegin();
  const_reverse_iterator rend();

  // ---------------------- Friends ----------------------
  friend bool operator==(const gdwg::Graph<N, E> &g1, const gdwg::Graph<N, E> &g2) {
    return !(g1 != g2);
  }
  friend bool operator!=(const gdwg::Graph<N, E> &g1, const gdwg::Graph<N, E> &g2) {
    auto g1Nodes = g1.GetNodes();
    auto g2Nodes = g2.GetNodes();
    if (g1Nodes.size() != g2Nodes.size()) {
      return true;
    }
    for (int i = 0; i < static_cast<int>(g2Nodes.size()); i++) {
      if (g1Nodes[i] != g2Nodes[i]) {
        return true;
      }
    }
    return false;
  }

  friend std::ostream& operator<<(std::ostream &os, const gdwg::Graph<N, E> &g) {
    for (auto it = g.nodes_.begin(); it != g.nodes_.end(); ++it) {
      auto curNode = it->second;
      os << *curNode->val_ << " (\n";
      for (auto &e : curNode->outGoing_) {
        os << "  " << *e->dst_ << " | " << *e->weight_ << "\n";
      }
      os << ")\n";
    }
    return os;
  }

 private:
  struct Edge;
  struct Node;
  std::map<N, std::shared_ptr<Node>> nodes_;

  struct Node {
    std::shared_ptr<N> val_;
    std::vector<std::shared_ptr<Edge>> inGoing_;
    std::vector<std::shared_ptr<Edge>> outGoing_;
    explicit Node(std::shared_ptr<N> &val) : val_{val} {};
    friend bool operator==(const Node &lhs, const Node &rhs) {
      if (*lhs.val != *rhs.val || !(*lhs.inGoing_ == *rhs.inGoing_) ||
          !(*lhs.outGoing_ != *rhs.outGoing_)) {
        return false;
      }
      return true;
    }
  };

  struct Edge {
    std::shared_ptr<N> src_;
    std::shared_ptr<N> dst_;
    std::shared_ptr<E> weight_;
    Edge(std::shared_ptr<N> src, std::shared_ptr<N> dst, std::shared_ptr<E> weight) {
      src_ = src;
      dst_ = dst;
      weight_ = weight;
    }
    friend bool operator==(const Edge &lhs, const Edge &rhs) {
      if (*lhs.dst_ != *rhs.dst_ || *lhs.src != *rhs.src_ || lhs.weight_ != rhs.weight_) {
        return false;
      }
      return true;
    }
    friend bool operator<(const Edge &lhs, const Edge &rhs) {
      if (*lhs.src_ < *rhs.src_) {
        return true;
      } else if (*lhs.src_ == *rhs.src_) {
        if (*lhs.dst_ < *rhs.dst_) {
          return true;
        } else if (*lhs.dst_ == *rhs.dst_) {
          if (*lhs.weight_ < *rhs.weight_) {
            return true;
          }
        }
      }
      return false;
    }
  };

 public:
  class const_iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N &, const N &, const E &>;

    reference operator*() const {
      const auto& edge = *edge_;
      return {*edge->src_, *edge->dst_, *edge->weight_};
    }
    const const_iterator& operator++() {
      ++edge_;
      if (edge_ == node_->second->outGoing_.end()) {
        do {
          ++node_;
        } while (node_ != endSentinel_ &&
                 node_->second->outGoing_.begin() == node_->second->outGoing_.end());
        if (node_ != endSentinel_) {
          edge_ = node_->second->outGoing_.begin();
        }
      }
      return *this;
    }

    const const_iterator operator++(int) {
      const auto copy{*this};
      ++(*this);
      return copy;
    }

    const_iterator& operator--() {
      --edge_;
      if (edge_ == node_->second->outGoing_.begin()) {
        do {
          --node_;
        } while (node_ != fntSentinel_ &&
                 node_->second->outGoing_.begin() == node_->second->outGoing_.end());
        if (node_ != fntSentinel_) {
          edge_ = node_->second->outGoing_.end();
        }
      }
      return *this;
    }

    const const_iterator operator--(int) {
      const auto& copy{*this};
      --(*this);
      return copy;
    }

    friend bool operator==(const const_iterator &lhs, const const_iterator &rhs) {
      return lhs.node_ == rhs.node_ && (lhs.node_ == lhs.endSentinel_ || lhs.edge_ == rhs.edge_);
    }

    friend bool operator!=(const const_iterator &lhs, const const_iterator &rhs) {
      return !(lhs == rhs);
    }

   private:
    typename std::map<N, std::shared_ptr<Node>>::const_iterator node_;
    typename std::map<N, std::shared_ptr<Node>>::const_iterator fntSentinel_;
    typename std::map<N, std::shared_ptr<Node>>::const_iterator endSentinel_;
    typename std::vector<std::shared_ptr<Edge>>::const_iterator edge_;
    const_iterator(const decltype(node_) &node,
                           const decltype(fntSentinel_) &fntSentinel,
                           const decltype(endSentinel_) &endSentinel,
                           const decltype(edge_) &edge)
        : node_{node}, fntSentinel_{fntSentinel}, endSentinel_{endSentinel}, edge_{edge} {};

    friend class Graph;
  };

  class const_reverse_iterator {
   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = std::tuple<N, N, E>;
    using reference = std::tuple<const N&, const N&, const E&>;

    reference operator*() const {
      const auto &edge = *edge_;
      return {*edge->src_, *edge->dst_, *edge->weight_};
    }

    const const_reverse_iterator &operator++() {
      ++edge_;
      if (edge_ == node_->second->outGoing_.rend()) {
        do {
          ++node_;
        } while (node_ != endSentinel_ &&
                 node_->second->outGoing_.rbegin() == node_->second->outGoing_.rend());
        if (node_ != endSentinel_) {
          edge_ = node_->second->outGoing_.rbegin();
        }
      }
      return *this;
    }

    const const_reverse_iterator operator++(int) {
      const auto copy{*this};
      ++(*this);
      return copy;
    }

    const_reverse_iterator &operator--() {
      --edge_;
      if (edge_ == node_->second->outGoing_.rbegin()) {
        do {
          --node_;
        } while (node_ != fntSentinel_ &&
                 node_->second->outGoing_.rbegin() == node_->second->outGoing_.rend());
        if (node_ != fntSentinel_) {
          edge_ = node_->second->outGoing_.rend();
        }
      }
      return *this;
    }

    const const_reverse_iterator operator--(int) {
      const auto &copy{*this};
      --(*this);
      return copy;
    }

    friend bool operator==(const const_reverse_iterator &lhs, const const_reverse_iterator &rhs) {
      return lhs.node_ == rhs.node_ && (lhs.node_ == lhs.endSentinel_ || lhs.edge_ == rhs.edge_);
    }

    friend bool operator!=(const const_reverse_iterator &lhs, const const_reverse_iterator &rhs) {
      return !(lhs == rhs);
    }

   private:
    typename std::map<N, std::shared_ptr<Node>>::const_reverse_iterator node_;
    typename std::map<N, std::shared_ptr<Node>>::const_reverse_iterator fntSentinel_;
    typename std::map<N, std::shared_ptr<Node>>::const_reverse_iterator endSentinel_;
    typename std::vector<std::shared_ptr<Edge>>::const_reverse_iterator edge_;
    const_reverse_iterator(const decltype(node_) &node,
                           const decltype(fntSentinel_) &fntSentinel,
                           const decltype(endSentinel_) &endSentinel,
                           const decltype(edge_) &edge)
      : node_{node}, fntSentinel_{fntSentinel}, endSentinel_{endSentinel}, edge_{edge} {};

    friend class Graph;
  };
};

}  // namespace gdwg

#include "graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
