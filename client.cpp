#include <iostream>
#include <string>

#include "assignments/dg/graph.h"

// Note: At the moment, there is no client.sampleout. Please do your own testing

int main() {
  gdwg::Graph<std::string, int> g;
  g.InsertNode("hello");
  g.InsertNode("how");
  g.InsertNode("are");
  g.InsertNode("you?");

  g.InsertEdge("hello", "how", 5);
  g.InsertEdge("hello", "are", 8);
  g.InsertEdge("hello", "are", 2);

  g.InsertEdge("how", "you?", 1);
  g.InsertEdge("how", "hello", 4);

  g.InsertEdge("are", "you?", 3);

  std::cout << g << '\n';

  std::vector<std::string> a = g.GetNodes();
  for(auto i : a) {
    std::cout << i << "\n";
  }

  gdwg::Graph<std::string, int> g2{g};

  std::cout << g2 << "\n";

  std::cout << (g == g2) << "\n";

  g2.DeleteNode("how");

  std::cout << g2 << "\n";

  std::cout << (g == g2) << "\n";

  // This is a structured binding.
  // https://en.cppreference.com/w/cpp/language/structured_binding
  // It allows you to unpack your tuple.

  for (gdwg::Graph<std::string,int>::const_iterator it = g.cbegin(); it != g.cend(); it++) {
    const auto& [from, to, weight] = *it;
    std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  }

  /*
  std::cout << g;
  for (const auto& [from, to, weight] : g) {
    std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  }
  */

  /*
  std::cout << g;
  for (const auto& [from, to, weight] : g) {
    std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  }*/




  for (gdwg::Graph<std::string,int>::const_reverse_iterator it = g.rbegin(); it != g.rend(); it++) {
    const auto& [from, to, weight] = *it;
    std::cout << from << " -> " << to << " (weight " << weight << ")\n";
  }
}
