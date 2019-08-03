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
    // TODO:: Duplicated Edge/Node
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
  SECTION("A node doesn't exist"){
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
  SECTION("Either Node doesn't exist"){
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
    REQUIRE(g.GetNodes().size() == 0);
  }
}