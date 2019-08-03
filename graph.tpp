#include <iostream>
#include "assignments/dg/graph.h"
//#include "graph.h"

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
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E>&& toMove) noexcept : nodes_{std::move(toMove.nodes_)}{}


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
  if (IsNode(val)) {
    return false;
  }
  auto valCpy = std::make_shared<N>(val);
  auto newN = Node(valCpy);
  nodes_.insert(std::make_pair(*valCpy, std::make_shared<Node>(newN)));
  return true;
}

template<typename N, typename E>
bool gdwg::Graph<N,E>::InsertEdge(const N& src, const N& dst, const E& w){
  if (!IsNode(src) || !IsNode(dst)){
    throw std::runtime_error("Cannot call Graph::InsertEdge when either src or dst node does not exist");
  } else if (IsConnectedWeight(src,dst,w)){
    return false;
  }
  auto srcNode = nodes_.at(src);
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
  if (!IsNode(del)){
    return false;
  }
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

template<typename N, typename E>
bool gdwg::Graph<N,E>::Replace(const N& oldData, const N& newData){
  if (!IsNode(oldData)){
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  } else if (IsNode(newData)){
    return false;
  }
  std::vector<std::tuple<std::shared_ptr<N>,std::shared_ptr<E>>> inGoing, outGoing;
  auto& node = nodes_.at(oldData);
  for (auto edge : node->outGoing_) {
    std::shared_ptr<N>& dst = edge->dst_;
    std::shared_ptr<E> weight = edge->weight_;
    outGoing.emplace_back(dst,weight);
  }
  for (auto edge : node->inGoing_) {
    std::shared_ptr<N>& src = edge->src_;
    std::shared_ptr<E> weight = edge->weight_;
    inGoing.emplace_back(src,weight);
  }
  DeleteNode(oldData);
  InsertNode(newData);
  for (auto i : outGoing) {
    auto dst = std::get<0>(i);
    auto weight = std::get<1>(i);
    if (!IsConnectedWeight(newData,*dst,*weight)){
      InsertEdge(newData,*dst,*weight);
    }
  }
  for (auto i : inGoing) {
    auto src = std::get<0>(i);
    auto weight = std::get<1>(i);
    if (!IsConnectedWeight(*src,newData,*weight)){
      InsertEdge(*src,newData,*weight);
    }
  }
  return true;
}

template<typename N, typename E>
void gdwg::Graph<N,E>::MergeReplace(const N& oldData, const N& newData){
  if (!IsNode(oldData) || !IsNode(newData)){
    throw std::runtime_error("Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
  }
  std::vector<std::tuple<std::shared_ptr<N>,std::shared_ptr<E>>> inGoing, outGoing;
  auto& node = nodes_.at(oldData);
  for (auto edge : node->outGoing_) {
    std::shared_ptr<N>& dst = edge->dst_;
    std::shared_ptr<E> weight = edge->weight_;
    outGoing.emplace_back(dst,weight);
  }
  for (auto edge : node->inGoing_) {
    std::shared_ptr<N>& src = edge->src_;
    std::shared_ptr<E> weight = edge->weight_;
    inGoing.emplace_back(src,weight);
  }
  DeleteNode(oldData);
  InsertNode(newData);
  for (auto i : outGoing) {
    auto dst = std::get<0>(i);
    auto weight = std::get<1>(i);
    InsertEdge(newData, *dst, *weight);
  }
  for (auto i : inGoing) {
    auto src = std::get<0>(i);
    auto weight = std::get<1>(i);
    InsertEdge(*src,newData,*weight);
  }
}

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
  if(!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error("Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }
  auto& nSrc = nodes_.at(src);
  auto& nDst = nodes_.at(dst);
  for(auto& edge : nSrc->outGoing_) {
    if(edge->src_ == nSrc->val_ && edge->dst_ == nDst->val_) return true;
  }
  return false;
}

template<typename N, typename E>
bool gdwg::Graph<N,E>::IsConnectedWeight(const N& src, const N& dst, const E& wgt){
  for (auto i : GetWeights(src,dst)){
    if (i == wgt) return true;
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
  if (!IsNode(src)) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
  auto& nSrc = nodes_.at(src);
  std::vector<N> vec(nSrc->outGoing_.size());
  int i = 0;
  for (auto& edge : nSrc->outGoing_) {
    auto& dsts = *(edge->dst_);
    vec[i] = dsts;
    i++;
  }
  return vec;
}

template<typename N, typename E>
std::vector<E> gdwg::Graph<N,E>::GetWeights(const N& src, const N& dst){
  if(!IsNode(src) || !IsNode(dst)) {
    throw std::out_of_range("Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }
  std::vector<E> vec;
  auto& nSrc = nodes_.at(src);
  auto& nDst = nodes_.at(dst);
  for(auto& edge : nSrc->outGoing_) {
    if(edge->src_ == nSrc->val_ && edge->dst_ == nDst->val_) {
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