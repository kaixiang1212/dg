/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include "assignments/dg/graph.h"
#include "catch.h"


TEST_CASE("Constructors"){

  SECTION("Iterator Constructor"){
    std::vector<char> vec{'a','b','c','d'};
    gdwg::Graph<char, int> g(vec.begin(),vec.end());
    SECTION("Nodes are constructed") {
      REQUIRE(g.GetNodes().size() == vec.size());
    }
  }

  SECTION("Tuple Constructor"){
    auto e1 = std::make_tuple('a', 'b', 5);
    auto e2 = std::make_tuple('b', 'c', 7);
    auto e = std::vector<std::tuple<char,char, int>>{e1, e2};
    gdwg::Graph<char, int> g{e.begin(), e.end()};
    SECTION("Nodes are constructed"){
      REQUIRE(g.GetNodes().size() == 3);
    }
    SECTION("Edges are constructed"){
      REQUIRE(g.IsConnected('a','b'));
      REQUIRE(g.IsConnected('b','c'));
    }
  }

  SECTION("Initializer List"){
    gdwg::Graph<char, std::string> g{'a', 'b', 'x', 'y'};
    SECTION("Nodes are constructed"){
      REQUIRE(g.GetNodes().size() == 4);
    }
  }

  SECTION("Copy Constructor"){
    gdwg::Graph<std::string, int> g;
    g.InsertNode("a");
    g.InsertNode("b");
    g.InsertEdge("a","b", 1);
    g.InsertEdge("a","a", 1);
    gdwg::Graph<std::string, int> gCopy{g};
    SECTION("Edges are copied"){
      REQUIRE(gCopy.IsConnected("a","b"));
      REQUIRE(gCopy.IsConnected("a","a"));
    }
    SECTION("Any change to original"){
      g.DeleteNode("a");
      REQUIRE(gCopy.IsNode("a"));
      REQUIRE(gCopy.IsConnected("a","b"));
    }
  }

  SECTION("Move Constructor"){
    gdwg::Graph<std::string, int> a;
    a.InsertNode("a");
    a.InsertEdge("a","a",1);
    gdwg::Graph<std::string, int> aMove{std::move(a)};
    SECTION("Original is moved"){
      REQUIRE(!a.IsNode("a"));
      REQUIRE(aMove.IsNode("a"));
    }
    SECTION("Edges are moved"){
      REQUIRE(aMove.IsConnected("a","a"));
    }
  }
}


TEST_CASE("Insert Edge"){
  std::vector<char> vec{'a','b','c','d'};
  gdwg::Graph<char, int> g(vec.begin(),vec.end());
  SECTION("Same src and dest with unique weight"){
    REQUIRE(g.InsertEdge('a','b',1));
    REQUIRE(g.GetWeights('a','b').size() == 1);
    REQUIRE(g.InsertEdge('a','b',2));
    REQUIRE(g.GetWeights('a','b').size() == 2);
  }
  SECTION("Allowed to be connected to themselves"){
    REQUIRE(g.InsertEdge('a','a', 1));
    REQUIRE(g.IsConnected('a','a'));
  }
  SECTION("Edges already exist"){
    REQUIRE(g.InsertEdge('a','b',1));
    REQUIRE(!g.InsertEdge('a','b',1));
  }
  SECTION("Either Node doesnt exist throws exception"){
    REQUIRE_THROWS_AS(g.InsertEdge('a','z',1), std::runtime_error);
    REQUIRE_THROWS_AS(g.InsertEdge('z','a',1), std::runtime_error);
  }
}

TEST_CASE("Delete Node"){
  std::vector<int> vec{1,2,3,4};
  gdwg::Graph<int, int> g(vec.begin(),vec.end());
  SECTION("Delete node that doesnt exist"){
    REQUIRE(!g.DeleteNode(6));
  }
  SECTION("Delete Node with edges connected to it"){
    g.InsertEdge(1,2,1);
    REQUIRE(g.DeleteNode(1));
    REQUIRE(g.GetConnected(2).empty());
  }
}

TEST_CASE("Replace"){
  std::vector<char> vec{'a','b','c','d'};
  gdwg::Graph<char, int> g(vec.begin(),vec.end());
  g.InsertEdge('a','b',1);
  g.InsertEdge('a', 'c', 2);
  g.InsertEdge('a', 'd', 3);
  g.InsertEdge('b', 'b', 1);
  SECTION("Removed old Node"){
    REQUIRE(g.Replace('a','e'));
    REQUIRE(!g.IsNode('a'));
  }
  SECTION("Created new Node"){
    REQUIRE(g.Replace('a','e'));
    REQUIRE(g.IsNode('e'));
  }
  SECTION("Replace a node that doesn't exist throws exception"){
    REQUIRE_THROWS_AS(g.Replace('e','z'), std::runtime_error);
  }
  SECTION("New node already exist"){
    REQUIRE(!g.Replace('a','c'));
    REQUIRE(g.IsNode('a'));
    REQUIRE(g.IsNode('c'));
  }
}

TEST_CASE("MergeReplace"){
  std::vector<char> vec{'a','b','c','d'};
  gdwg::Graph<char, int> g(vec.begin(),vec.end());
  g.InsertEdge('a','b',1);
  g.InsertEdge('a', 'c', 2);
  g.InsertEdge('a', 'd', 3);
  g.InsertEdge('b', 'b', 1);
  SECTION("Removed Merged Node"){
    g.MergeReplace('a','b');
    REQUIRE(!g.IsNode('a'));
  }
  SECTION("Merged Node"){
    g.MergeReplace('a', 'b');
    std::vector<char> incoming = g.GetConnected('b');
    REQUIRE(incoming.size() == 3);
  }
  SECTION("Either Node doesn't exist throws exception"){
    REQUIRE_THROWS_AS(g.MergeReplace('z','b'), std::runtime_error);
    REQUIRE_THROWS_AS(g.MergeReplace('a','z'), std::runtime_error);
  }
}

TEST_CASE("Clear"){
  std::vector<char> vec{'a','b','c','d'};
  gdwg::Graph<char, int> g(vec.begin(),vec.end());
  SECTION("Remove all nodes"){
    REQUIRE(g.GetNodes().size() == 4);
    g.Clear();
    REQUIRE(g.GetNodes().empty());
  }
}

TEST_CASE("IsConnected"){
  gdwg::Graph<int, int> g{1, 2,3, 4};
  SECTION("If either node doesnt exist throws exception"){
    REQUIRE_THROWS_AS(g.IsConnected(5,6), std::runtime_error);
  }
}

TEST_CASE("GetNodes"){
  gdwg::Graph<int, int> g;
  g.InsertNode(3);
  g.InsertNode(1);
  g.InsertNode(4);
  g.InsertNode(2);
  SECTION("Increasing Order of Node"){
    std::vector<int> vec = g.GetNodes();
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
    REQUIRE(vec[3] == 4);
  }
}

TEST_CASE("GetConnected"){
  gdwg::Graph<int, int> g{1,2,3,4};
  g.InsertEdge(1,4,1);
  g.InsertEdge(1,1,1);
  g.InsertEdge(1,3,1);
  g.InsertEdge(1,2,1);
  SECTION("Sorted by increasing order of node"){
    std::vector<int> vec = g.GetConnected(1);
      REQUIRE(vec[0] == 1);
      REQUIRE(vec[1] == 2);
      REQUIRE(vec[2] == 3);
      REQUIRE(vec[3] == 4);
  }
  SECTION("Node not exist throws exception"){
    REQUIRE_THROWS_AS(g.GetConnected(6), std::out_of_range);
  }
}

TEST_CASE("GetWeights"){
  gdwg::Graph<int, int> g{1};
  g.InsertEdge(1,1,4);
  g.InsertEdge(1,1,2);
  g.InsertEdge(1,1,3);
  g.InsertEdge(1,1,1);
  SECTION("Sorted by increasing order of weight (edge)"){
    std::vector<int> vec = g.GetWeights(1,1);
    REQUIRE(vec[0] == 1);
    REQUIRE(vec[1] == 2);
    REQUIRE(vec[2] == 3);
    REQUIRE(vec[3] == 4);
  }
  SECTION("Either node not exist throws exception"){
    REQUIRE_THROWS_AS(g.GetWeights(6,1), std::out_of_range);
    REQUIRE_THROWS_AS(g.GetWeights(1,6), std::out_of_range);
  }
}

// TODO : Iterators
TEST_CASE("Find iterator"){
  gdwg::Graph<int, int> g{1};
  g.InsertEdge(1,1,4);
  SECTION("Return const iterator to edge"){
    auto i = g.find(1,1,4);
    REQUIRE( std::get<0>(*i) == 1);
    REQUIRE( std::get<1>(*i) == 1);
    REQUIRE( std::get<2>(*i) == 4);
  }
  SECTION("Edge not found returns cend"){
    REQUIRE(g.cend() == g.find(1,2,1));
  }
}

TEST_CASE("Erase given src, dst, weight"){
  gdwg::Graph<int, int> g{1};
  g.InsertEdge(1,1,4);
  SECTION("Erase edge"){
    REQUIRE(g.erase(1,1,4));
    REQUIRE(g.GetConnected(1).empty());
    REQUIRE(g.end() == g.find(1,1,4));
  }
  SECTION("Edge not found returns false"){
    REQUIRE(!g.erase(1,2,1));
  }
}

TEST_CASE("Erase edge given iterator"){
  gdwg::Graph<int, int> g{1,2,3,4};
  g.InsertEdge(1,1,4);
  SECTION("Erase edge"){
    auto i = g.find(1,1,4);
    g.erase(i);
    REQUIRE(g.GetConnected(1).empty());
    REQUIRE(g.end() == g.find(1,1,4));
  }
  SECTION("Invalid iterator returns cend"){
    REQUIRE(!g.erase(1,2,1));
  }
  SECTION("Erasing one element returns next") {
    g.InsertEdge(1,1,2);
    auto i = g.find(1,1,2);
    REQUIRE(g.erase(i) == g.find(1,1,4));
  }
  SECTION("Erasing the only element returns cend?"){
    // TODO:
    auto i = g.find(1,1,4);
    REQUIRE(g.begin() == g.end());
    REQUIRE(g.end() == g.erase(i));
  }
  SECTION("Erasing all edge via loop"){
    g.InsertEdge(1,2,2);
    g.InsertEdge(2,1,5);
    g.InsertEdge(3,2,2);
    g.InsertEdge(2,4,2);
    g.InsertEdge(3,4,2);
    for(auto i = g.begin();i!= g.end();i++){
      g.erase(i);
    }
  }
}

TEST_CASE("Iterators"){
  gdwg::Graph<int, int> g{1};
  g.InsertEdge(1,1,4);
  g.InsertEdge(1,1,2);
  SECTION("Move forward"){
    auto i = g.find(1,1,2);
    i++;
    REQUIRE(i == g.find(1,1,4));
  }
  SECTION("++ on last element"){
    auto i = g.find(1,1,4);
    i++;
    REQUIRE(g.end() == i);
  }
  SECTION("Move backward"){
    // TODO:
    auto i = g.find(1,1,4);
    i--;
    REQUIRE(i == g.find(1,1,2));
  }
}

TEST_CASE("Operator == & !="){
  gdwg::Graph<int, int> g{1};
  g.InsertEdge(1,1,4);
  g.InsertEdge(1,1,2);
  gdwg::Graph<int, int> gCopy(g);
  SECTION("Equal"){
    REQUIRE(g == gCopy);
  }
  SECTION("Not equal after removing node"){
    g.DeleteNode(1);
    REQUIRE(g != gCopy);
  }
  SECTION("Not equal after inserting node"){
    g.InsertNode(2);
    REQUIRE(g != gCopy);
  }
}