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

// ---------------------- Methods ----------------------
template<typename N, typename E>
bool gdwg::Graph<N,E>::InsertNode(const N& val){}

template<typename N, typename E>
bool gdwg::Graph<N,E>::InsertEdge(const N& src, const N& dst, const E& w){}

template<typename N, typename E>
bool gdwg::Graph<N,E>::DeleteNode(const N&){}

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
template<typename N, typename E>
std::ostream& operator<<(std::ostream&, const gdwg::Graph<N, E>&){}
