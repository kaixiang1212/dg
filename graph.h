#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <vector>
#include <iterator>
#include <map>
#include <memory>

namespace gdwg {

template <typename N, typename E>
class Graph {
  public:
    class const_iterator {};
    // ---------------------- Constructors ----------------------

    Graph<N, E>();
    /*
    Graph<N, E>(typename std::vector<N>::const_iterator, typename std::vector<N>::const_iterator);
    Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator, typename std::vector<std::tuple<N, N, E>>::const_iterator);
    Graph<N, E>(std::initializer_list<N>);
    */
    Graph<N, E>(const gdwg::Graph<N, E>&) noexcept ;
    /*
    Graph<N, E>(gdwg::Graph<N, E>&&) noexcept ;

    ~Graph<N, E>();
    */
    // ---------------------- Operations ----------------------
    /*
    Graph<N, E>& operator=(const Graph&);
    Graph<N, E>& operator=(const Graph&&);

    // ---------------------- Methods ----------------------
     */
    bool InsertNode(const N& val);
    bool InsertEdge(const N& src, const N& dst, const E& w);
    bool DeleteNode(const N&);
    /*
    bool Replace(const N& oldData, const N& newData);
    void MergeReplace(const N& oldData, const N& newData);
    void Clear();
     */
    bool IsNode(const N& val);
    bool IsConnected(const N& src, const N& dst);
    std::vector<N> GetNodes();
    std::vector<N> GetConnected(const N& src);
    std::vector<E> GetWeights(const N& src, const N& dst);
    /*
    const_iterator find(const N&, const N&, const E&);
    bool erase(const N& src, const N& dst, const E& w);
    const_iterator erase(const_iterator it);
    const_iterator cbegin();
    const_iterator cend();
    const_reverse_iterator crbegin();
    const_iterator crend();
    const_iterator begin();
    const_iterator end();
    const_reverse_iterator rbegin();
    const_reverse_iterator rend();
    */
    // ---------------------- Friends ----------------------
    friend bool operator==(const gdwg::Graph<N, E>& g1, const gdwg::Graph<N, E>& g2) {
      return !(g1 != g2);
    }
    friend bool operator!=(const gdwg::Graph<N, E>& g1, const gdwg::Graph<N, E>& g2) {
      auto g1Nodes = g1.GetNodes();
      auto g2Nodes = g2.GetNodes();
      if (g1Nodes.size() != g2Nodes.size()) {
        return true;
      }
      for(int i = 0; i < g2Nodes.size(); i++) {
        if(g1Nodes[i] != g2Nodes[i]) {
          return true;
        }
      }
      return false;
     };
    friend std::ostream& operator<<(std::ostream& os, const gdwg::Graph<N, E>& g){
      for(auto it = g.nodes_.begin(); it != g.nodes_.end(); ++it){
        auto curNode = it->second;
        os << *curNode->val_ << " (\n";
        for(auto& e : curNode->outGoing_) {
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
      explicit Node(std::shared_ptr<N>& val) : val_{val}{};
      friend bool operator==(const Node& lhs, const Node& rhs)
      {
        if(*lhs.val != *rhs.val || !(*lhs.inGoing_ == *rhs.inGoing_) || !(*lhs.outGoing_ != *rhs.outGoing_)) {
          return  false;
        }
        return true;
      }
    };

    struct Edge {
      std::shared_ptr<N> src_;
      std::shared_ptr<N> dst_;
      std::shared_ptr<E> weight_;
      Edge(std::shared_ptr<N> src, std::shared_ptr<N> dst,std::shared_ptr<E> weight){
        src_ = src;
        dst_ = dst;
        weight_ = weight;
      };
      friend bool operator==(const Edge& lhs, const Edge& rhs) {
        if(*lhs.dst_ != *rhs.dst_ || *lhs.src != *rhs.src_ || lhs.weight_ != rhs.weight_) {
          return  false;
        }
        return true;
      }
    };
};



}  // namespace gdwg

#include "graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
