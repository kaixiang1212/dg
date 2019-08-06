// ---------------------- Constructors ----------------------
template <typename N, typename E> gdwg::Graph<N, E>::Graph() = default;

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(
    typename std::vector<N>::const_iterator begin,
    typename std::vector<N>::const_iterator end) {
  for (auto i = begin; i != end; ++i) {
    InsertNode(*i);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(typename std::vector<std::tuple<N, N, E>>::const_iterator begin,
                         typename std::vector<std::tuple<N, N, E>>::const_iterator end) {
  for (auto i = begin; i != end; ++i) {
    auto t = *i;
    N start = std::get<0>(t);
    N dest = std::get<1>(t);

    InsertNode(start);
    InsertNode(dest);
    InsertEdge(start, dest, std::get<2>(t));
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(std::initializer_list<N> list) {
  for (auto i = list.begin(); i != list.end(); i++) {
    InsertNode(*i);
  }
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(const gdwg::Graph<N, E> &toCopy) noexcept {
  nodes_ = toCopy.nodes_;
}

template <typename N, typename E>
gdwg::Graph<N, E>::Graph(gdwg::Graph<N, E> &&toMove) noexcept :
  nodes_{std::move(toMove.nodes_)} {}

template <typename N, typename E> gdwg::Graph<N, E>::~Graph() = default;

// ---------------------- Operations ----------------------

template <typename N, typename E>
gdwg::Graph<N, E> &gdwg::Graph<N, E>::operator=(const Graph<N, E>& toCopy) {
  nodes_ = toCopy.nodes_;
  return *this;
}

template <typename N, typename E>
gdwg::Graph<N, E> &gdwg::Graph<N, E>::operator=(Graph<N, E>&& toMove) noexcept {
  nodes_ = std::move(toMove.nodes_);
  return *this;
}

// ---------------------- Methods ----------------------
template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertNode(const N &val) {
  if (IsNode(val)) {
    return false;
  }
  auto valCpy = std::make_shared<N>(val);
  auto newN = Node(valCpy);
  nodes_.insert(std::make_pair(*valCpy, std::make_shared<Node>(newN)));
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::InsertEdge(const N &src, const N &dst, const E &w) {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::InsertEdge when either src or dst node does not exist");
  } else if (IsConnectedWeight(src, dst, w)) {
    return false;
  }
  auto srcNode = nodes_.at(src);
  auto dstNode = nodes_.at(dst);
  auto wCpy = std::make_shared<E>(w);
  Edge ed = Edge{srcNode->val_, dstNode->val_, wCpy};
  auto newEdge = std::make_shared<Edge>(ed);
  bool foundPos = false;
  for (auto it = srcNode->outGoing_.begin(); it != srcNode->outGoing_.end(); ++it) {
    if (*newEdge < **it) {
      srcNode->outGoing_.insert(it, 1, newEdge);
      foundPos = true;
      break;
    }
  }
  if (!foundPos) {
    srcNode->outGoing_.push_back(newEdge);
  }
  foundPos = false;
  for (auto it = dstNode->inGoing_.begin(); it != dstNode->inGoing_.end(); ++it) {
    if (*newEdge < **it) {
      dstNode->inGoing_.insert(it, 1, newEdge);
      foundPos = true;
      break;
    }
  }
  if (!foundPos) {
    dstNode->inGoing_.push_back(newEdge);
  }
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::DeleteNode(const N &del) {
  if (!IsNode(del)) {
    return false;
  }
  auto &nodeRmv = nodes_.at(del);
  for (auto edge : nodeRmv->inGoing_) {
    std::shared_ptr<N>& src = edge->src_;
    std::vector<std::shared_ptr<Edge>>& srcEdges = nodes_.at(*src)->outGoing_;
    for (auto iter = srcEdges.begin(); iter != srcEdges.end(); ++iter) {
      if (*iter == edge) {
        srcEdges.erase(iter);
        break;
      }
    }
  }
  nodes_.erase(del);
  return true;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::Replace(const N &oldData, const N &newData) {
  if (!IsNode(oldData)) {
    throw std::runtime_error("Cannot call Graph::Replace on a node that doesn't exist");
  } else if (IsNode(newData)) {
    return false;
  }
  std::vector<std::tuple<std::shared_ptr<N>, std::shared_ptr<E>>> inGoing, outGoing;
  auto &node = nodes_.at(oldData);
  for (auto edge : node->outGoing_) {
    std::shared_ptr<N> &dst = edge->dst_;
    std::shared_ptr<E> weight = edge->weight_;
    outGoing.emplace_back(dst, weight);
  }
  for (auto edge : node->inGoing_) {
    std::shared_ptr<N> &src = edge->src_;
    std::shared_ptr<E> weight = edge->weight_;
    inGoing.emplace_back(src, weight);
  }
  DeleteNode(oldData);
  InsertNode(newData);
  for (auto i : outGoing) {
    auto dst = std::get<0>(i);
    auto weight = std::get<1>(i);
    if (!IsConnectedWeight(newData, *dst, *weight)) {
      InsertEdge(newData, *dst, *weight);
    }
  }
  for (auto i : inGoing) {
    auto src = std::get<0>(i);
    auto weight = std::get<1>(i);
    if (!IsConnectedWeight(*src, newData, *weight)) {
      InsertEdge(*src, newData, *weight);
    }
  }
  return true;
}

template <typename N, typename E>
void gdwg::Graph<N, E>::MergeReplace(const N &oldData, const N &newData) {
  if (!IsNode(oldData) || !IsNode(newData)) {
    throw std::runtime_error(
        "Cannot call Graph::MergeReplace on old or new data if they don't exist in the graph");
  }
  std::vector<std::tuple<std::shared_ptr<N>, std::shared_ptr<E>>> inGoing, outGoing;
  auto &node = nodes_.at(oldData);
  for (auto edge : node->outGoing_) {
    std::shared_ptr<N> &dst = edge->dst_;
    std::shared_ptr<E> weight = edge->weight_;
    outGoing.emplace_back(dst, weight);
  }
  for (auto edge : node->inGoing_) {
    std::shared_ptr<N> &src = edge->src_;
    std::shared_ptr<E> weight = edge->weight_;
    inGoing.emplace_back(src, weight);
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
    InsertEdge(*src, newData, *weight);
  }
}

template <typename N, typename E>
void gdwg::Graph<N, E>::Clear() {
  nodes_.clear();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsNode(const N &val) {
  return nodes_.count(val);
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnected(const N &src, const N &dst) {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::runtime_error(
        "Cannot call Graph::IsConnected if src or dst node don't exist in the graph");
  }
  auto &nSrc = nodes_.at(src);
  auto &nDst = nodes_.at(dst);
  for (auto& edge : nSrc->outGoing_) {
    if (edge->src_ == nSrc->val_ && edge->dst_ == nDst->val_)
      return true;
  }
  return false;
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::IsConnectedWeight(const N &src, const N &dst, const E &wgt) {
  for (auto i : GetWeights(src, dst)) {
    if (i == wgt)
      return true;
  }
  return false;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetNodes() const {
  std::vector<N> vec;
  for (auto it = nodes_.begin(); it != nodes_.end(); ++it) {
    vec.push_back(it->first);
  }
  return vec;
}

template <typename N, typename E>
std::vector<N> gdwg::Graph<N, E>::GetConnected(const N &src) {
  if (!IsNode(src)) {
    throw std::out_of_range("Cannot call Graph::GetConnected if src doesn't exist in the graph");
  }
  auto &nSrc = nodes_.at(src);
  std::vector<N> vec(nSrc->outGoing_.size());
  int i = 0;
  for (auto &edge : nSrc->outGoing_) {
    auto &dsts = *(edge->dst_);
    vec[i] = dsts;
    i++;
  }
  return vec;
}

template <typename N, typename E>
std::vector<E> gdwg::Graph<N, E>::GetWeights(const N &src, const N &dst) {
  if (!IsNode(src) || !IsNode(dst)) {
    throw std::out_of_range(
        "Cannot call Graph::GetWeights if src or dst node don't exist in the graph");
  }
  std::vector<E> vec;
  auto &nSrc = nodes_.at(src);
  auto &nDst = nodes_.at(dst);
  for (auto &edge : nSrc->outGoing_) {
    if (edge->src_ == nSrc->val_ && edge->dst_ == nDst->val_) {
      vec.push_back(*edge->weight_);
    }
  }
  return vec;
}
template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator
gdwg::Graph<N, E>::find(const N &src, const N &dst, const E &weight) {
  auto nodeIt = nodes_.find(src);
  auto nodeItEnd = nodes_.cend();
  if (nodeIt == nodeItEnd) {
    return cend();
  }
  std::vector<std::shared_ptr<Edge>> &edV = nodeIt->second->outGoing_;
  for (auto it = edV.cbegin(); it != edV.cend(); it++) {
    const Edge &ed = **it;
    if (*ed.dst_ == dst && *ed.weight_ == weight) {
      return const_iterator(nodeIt, nodes_.cbegin(), nodeItEnd, it);
    }
  }
  return cend();
}

template <typename N, typename E>
bool gdwg::Graph<N, E>::erase(const N &src, const N &dst, const E &w) {
  const auto &it = find(src, dst, w);
  if (it != cend()) {
    erase(it);
    return true;
  }
  return false;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::erase(const_iterator it) {
  auto rm = it++;
  auto &edge = *rm.edge_;
  auto &src = *rm.node_->second;
  for (auto iter = src.outGoing_.begin(); iter != src.outGoing_.end(); ++iter) {
    if (*iter == edge) {
      src.outGoing_.erase(iter);
      break;
    }
  }
  auto &dst = nodes_.at(*edge->dst_);
  for (auto iter = dst->outGoing_.begin(); iter != dst->outGoing_.end(); ++iter) {
    if (*iter == edge) {
      dst->outGoing_.erase(iter);
      break;
    }
  }
  return it;
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cbegin() {
  auto itBeg = nodes_.cbegin();
  auto itEnd = nodes_.cend();
  typename std::vector<std::shared_ptr<Edge>>::const_iterator itEdge;
  while (itBeg != itEnd && itBeg->second->outGoing_.cbegin() == itBeg->second->outGoing_.cend()) {
    itBeg++;
  }
  if (itBeg != itEnd) {
    itEdge = itBeg->second->outGoing_.cbegin();
  }
  return const_iterator(itBeg, itBeg, itEnd, itEdge);
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::cend() {
  typename std::vector<std::shared_ptr<Edge>>::const_iterator itEdge;
  return const_iterator(nodes_.cend(),nodes_.cbegin(), nodes_.cend(), itEdge);
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crbegin() {
  auto itBeg = nodes_.crbegin();
  auto itEnd = nodes_.crend();
  typename std::vector<std::shared_ptr<Edge>>::const_reverse_iterator itEdge;
  while (itBeg != itEnd && itBeg->second->outGoing_.crbegin() == itBeg->second->outGoing_.crend()) {
    itBeg++;
  }
  if (itBeg != itEnd) {
    itEdge = itBeg->second->outGoing_.crbegin();
  }
  return const_reverse_iterator(itBeg, itBeg, itEnd, itEdge);
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::crend() {
  typename std::vector<std::shared_ptr<Edge>>::const_reverse_iterator itEdge;
  return const_reverse_iterator(nodes_.crend(), nodes_.crbegin(), nodes_.crend(), itEdge);
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::begin() {
  return cbegin();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_iterator gdwg::Graph<N, E>::end() {
  return cend();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rbegin() {
  return crbegin();
}

template <typename N, typename E>
typename gdwg::Graph<N, E>::const_reverse_iterator gdwg::Graph<N, E>::rend() {
  return crend();
}
