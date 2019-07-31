#include <iostream>
#include "assignments/dg/graph.h"
/*
// ---------------------- Constructors ----------------------
template<typename N, typename E>
gdwg::Graph<N, E>::Graph()= default;

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator, typename std::vector<N>::const_iterator){}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator, typename std::vector<std::tuple<N, N, E>>::const_iterator){}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N>){}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>&) noexcept = default;

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&&) noexcept {}

template<typename N, typename E>
gdwg::Graph<N, E>::~Graph()= default;

// ---------------------- Operations ----------------------
template<typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N,E>::operator=(const Graph<N,E>&){}

template<typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N,E>::operator=(const Graph<N,E>&&){}
*/
// ---------------------- Methods ----------------------
template<typename N, typename E>
bool gdwg::Graph<N,E>::InsertNode(const N& val){
  auto valCpy = std::make_shared<N>(val);
  auto newN = Node(valCpy);
  nodes_.insert(std::make_pair(*valCpy, std::make_shared<Node>(newN)));
  return true;
}

template<typename N, typename E>
bool gdwg::Graph<N,E>::InsertEdge(const N& src, const N& dst, const E& w){  auto srcNode = nodes_.at(src);
  auto dstNode = nodes_.at(dst);
  auto wCpy = std::make_shared<E>(w);
  Edge ed = Edge{srcNode, dstNode, wCpy};
  auto newEdge = std::make_shared<Edge>(ed);
  srcNode->outGoing_.push_back(newEdge);
  dstNode->inGoing_.push_back(newEdge);
  return true;
}

template<typename N, typename E>
bool gdwg::Graph<N,E>::DeleteNode(const N& del){
  auto nodeRmv = nodes_.at(del);
  return true;
}

/*
template<typename N, typename E>
bool gdwg::Graph<N,E>::Replace(const N& oldData, const N& newData){}

template<typename N, typename E>
void gdwg::Graph<N,E>::MergeReplace(const N& oldData, const N& newData){}

template<typename N, typename E>
void gdwg::Graph<N,E>::Clear(){}

template<typename N, typename E>
bool gdwg::Graph<N,E>::IsNode(const N& val){}

template<typename N, typename E>
bool gdwg::Graph<N,E>::IsConnected(const N& src, const N& dst){}

template<typename N, typename E>
std::vector<N> gdwg::Graph<N,E>::GetNodes(){}

template<typename N, typename E>
std::vector<N> gdwg::Graph<N,E>::GetConnected(const N& src){}

template<typename N, typename E>
std::vector<E> gdwg::Graph<N,E>::GetWeights(const N& src, const N& dst){}

template<typename N, typename E>
const_iterator gdwg::Graph<N,E>::find(const N&, const N&, const E&){}

template<typename N, typename E>
bool gdwg::Graph<N,E>::erase(const N& src, const N& dst, const E& w){}

template<typename N, typename E>
const_iterator gdwg::Graph<N,E>::erase(const_iterator it){}

template<typename N, typename E>
const_iterator gdwg::Graph<N,E>::cbegin(){}

template<typename N, typename E>
const_iterator gdwg::Graph<N,E>::cend(){}

template<typename N, typename E>
const_reverse_iterator gdwg::Graph<N,E>::crbegin(){}

template<typename N, typename E>
const_iterator gdwg::Graph<N,E>::crend(){}

template<typename N, typename E>
const_iterator gdwg::Graph<N,E>::begin(){}

template<typename N, typename E>
const_iterator gdwg::Graph<N,E>::end(){}

template<typename N, typename E>
const_reverse_iterator gdwg::Graph<N,E>::rbegin(){}

template<typename N, typename E>
const_reverse_iterator gdwg::Graph<N,E>::rend(){}

// ---------------------- Friends ----------------------
template<typename N, typename E>
bool operator==(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&){}
template<typename N, typename E>
bool operator!=(const gdwg::Graph<N, E>&, const gdwg::Graph<N, E>&){}
 */
