#include <iostream>
//#include "assignments/dg/graph.h"
#include "graph.h"

// ---------------------- Constructors ----------------------
template<typename N, typename E>
gdwg::Graph<N, E>::Graph()= default;

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<N>::const_iterator begin, typename std::vector<N>::const_iterator end){
  for (auto i = begin;i != end;++i) {
    InsertNode(*i);
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin, typename std::vector<std::tuple<N, N, E>>::const_iterator end){
  for (auto i = begin;i != end;++i) {
    auto t = *i;
    N start = std::get<0>(t);
    N dest = std::get<1>(t);

    InsertNode(start);
    InsertNode(dest);
    InsertEdge(start, dest, std::get<2>(t));
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> list){
  for (auto i = list.begin();i != list.end();i++){
    InsertNode(*i);
  }
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E>& toCopy) noexcept {
  nodes_ = toCopy.nodes_;
}

template<typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& toMove) noexcept : nodes_{std::move(toMove.nodes_)}{
}


template<typename N, typename E>
gdwg::Graph<N, E>::~Graph()= default;

// ---------------------- Operations ----------------------

template<typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N,E>::operator=(const Graph<N,E>& toCopy){
    nodes_ = toCopy.nodes_;
}

template<typename N, typename E>
gdwg::Graph<N, E>& gdwg::Graph<N,E>::operator=(Graph<N,E>&& toMove) noexcept {
    nodes_ = std::move(toMove.nodes_);
}

// ---------------------- Methods ----------------------
template<typename N, typename E>
bool gdwg::Graph<N,E>::InsertNode(const N& val){
  if(IsNode(val)) {
    return false;
  }
  auto valCpy = std::make_shared<N>(val);
  auto newN = Node(valCpy);
  nodes_.insert(std::make_pair(*valCpy, std::make_shared<Node>(newN)));
  return true;
}

template<typename N, typename E>
bool gdwg::Graph<N,E>::InsertEdge(const N& src, const N& dst, const E& w){  auto srcNode = nodes_.at(src);
  auto dstNode = nodes_.at(dst);
  auto wCpy = std::make_shared<E>(w);
  Edge ed = Edge{srcNode->val_, dstNode->val_, wCpy};
  auto newEdge = std::make_shared<Edge>(ed);
  srcNode->outGoing_.push_back(newEdge);
  dstNode->inGoing_.push_back(newEdge);
  return true;
}

template<typename N, typename E>
bool gdwg::Graph<N,E>::DeleteNode(const N& del){
  auto& nodeRmv = nodes_.at(del);
  for (auto edge : nodeRmv->inGoing_) {
    std::shared_ptr<N>& src = edge->src_;
    std::vector<std::shared_ptr<Edge>>& srcEdges = nodes_.at(*src)->outGoing_;
    for( auto iter = srcEdges.begin(); iter != srcEdges.end(); ++iter ) {
      if( *iter == edge ) {
        srcEdges.erase( iter );
        break;
      }
    }
  }
  nodes_.erase(del);
  return true;
}

// TODO:: Replace without deleting and inserting again
template<typename N, typename E>
bool gdwg::Graph<N,E>::Replace(const N& oldData, const N& newData){
  if (!IsNode(oldData)){
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  }
  auto pair = *(nodes_.find(oldData));
  auto node = pair.second;
  auto key = pair.first;
  key = newData;
  *(node->val_)= newData;
  nodes_.erase(oldData);
  nodes_.insert(std::make_pair(key, node));
  return true;
}

template<typename N, typename E>
void gdwg::Graph<N,E>::MergeReplace(const N& oldData, const N& newData){}

template<typename N, typename E>
void gdwg::Graph<N,E>::Clear(){
  nodes_.clear();
}

template<typename N, typename E>
bool gdwg::Graph<N,E>::IsNode(const N& val){
  return nodes_.count(val);
}

template<typename N, typename E>
bool gdwg::Graph<N,E>::IsConnected(const N& src, const N& dst){
  auto& nSrc = nodes_.at(src);
  auto& nDst = nodes_.at(dst);
  if(!nSrc || !nDst) {
    throw std::runtime_error("Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }
  for(auto& edge : nSrc->outGoing_) {
    if(edge->src_.lock() == nSrc && edge->dst_.lock() == nDst) return true;
  }
  return false;
}

template<typename N, typename E>
std::vector<N> gdwg::Graph<N,E>::GetNodes() const {
  std::vector<N> vec;
  for(auto it = nodes_.begin(); it != nodes_.end(); ++it) {
    vec.push_back(it->first);
  }
  return vec;
}

template<typename N, typename E>
std::vector<N> gdwg::Graph<N,E>::GetConnected(const N& src){
  auto& nSrc = nodes_.at(src);
  if (!src) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
  std::vector<N> vec(nSrc->outGoing.size());
  int i = 0;
  for (auto& edge : nSrc->outGoing_) {
    auto& dsts = edge->dst_.lock()->inGoing_;
    vec[i] = dsts;
    i++;
  }
}

template<typename N, typename E>
std::vector<E> gdwg::Graph<N,E>::GetWeights(const N& src, const N& dst){
  std::vector<E> vec;
  auto& nSrc = nodes_.at(src);
  auto& nDst = nodes_.at(dst);
  if(!nSrc || !nDst) {
    throw std::out_of_range("Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }
  for(auto& edge : nSrc->outGoing_) {
    if(edge->src_.lock() == nSrc && edge->dst_.lock() == nDst) {
      vec.push_back(*edge->weight_);
    }
  }
  return vec;
}
/*
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
*/
