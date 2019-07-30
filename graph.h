#ifndef ASSIGNMENTS_DG_GRAPH_H_
#define ASSIGNMENTS_DG_GRAPH_H_

#include <vector>
#include <iterator>

namespace gdwg {

template <typename N, typename E>
class Graph {
  public:
    class const_iterator {};
    // ---------------------- Constructors ----------------------
    Graph<N, E>();
    Graph<N, E>(typename std::vector<N>::const_iterator, typename std::vector<N>::const_iterator);
    Graph<N, E>(typename std::vector<std::tuple<N, N, E>>::const_iterator, typename std::vector<std::tuple<N, N, E>>::const_iterator);
    Graph<N, E>(std::initializer_list<N>);
    Graph<N, E>(const gdwg::Graph<N, E>&) noexcept ;
    Graph<N, E>(gdwg::Graph<N, E>&&) noexcept ;
    ~Graph<N, E>();

    // ---------------------- Operations ----------------------
    Graph<N, E>& operator=(const Graph&);
    Graph<N, E>& operator=(const Graph&&);

    // ---------------------- Methods ----------------------
    bool InsertNode(const N& val);
    bool InsertEdge(const N& src, const N& dst, const E& w);
    bool DeleteNode(const N&);
    bool Replace(const N& oldData, const N& newData);
    void MergeReplace(const N& oldData, const N& newData);
    void Clear();
    bool IsNode(const N& val);
    bool IsConnected(const N& src, const N& dst);
    std::vector<N> GetNodes();
    std::vector<N> GetConnected(const N& src);
    std::vector<E> GetWeights(const N& src, const N& dst);
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

    // ---------------------- Friends ----------------------
    friend bool operator==(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&);
    friend bool operator!=(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&);
    friend std::ostream& operator<<(std::ostream&, const gdwg::Graph<N, E>&);

  private:
};



}  // namespace gdwg

#include "graph.tpp"

#endif  // ASSIGNMENTS_DG_GRAPH_H_
