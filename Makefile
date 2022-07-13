CXX = g++
CXXFLAGS = -std=c++11 -O3 -march=native -flto -Wall -Werror -Wextra -Wpedantic
CPPFLAGS = -isystem patternavoidance/permutationalgs -isystem patternavoidance/hashtables
PATTERNAVOIDANCE_O = patternavoidance/permutationalgs/perm.o patternavoidance/permutationalgs/countavoiders.o patternavoidance/permutationalgs/permutilities.o patternavoidance/hashtables/hashdb.o patternavoidance/hashtables/hashmap.o

all:
	$(MAKE) -C patternavoidance
	$(MAKE) write_123_bijection
	$(MAKE) write_123_formula
	$(MAKE) write_1234_formula
	$(MAKE) write_algorithm_j
	$(MAKE) write_avoiders
	$(MAKE) write_avoiders_count
	$(MAKE) write_degree_one_vertices
	$(MAKE) write_even_odd_count
	$(MAKE) write_min_jumps
	$(MAKE) write_results

write_123_bijection: write_123_bijection.o minimal_jump_graph.o pattern_avoidance.o $(PATTERNAVOIDANCE_O)
	$(CXX) $(CXXFLAGS) $^ -o $@

write_123_formula: write_123_formula.o
	$(CXX) $(CXXFLAGS) $^ -o $@

write_1234_formula: write_1234_formula.o
	$(CXX) $(CXXFLAGS) $^ -o $@

write_algorithm_j: write_algorithm_j.o minimal_jump_graph.o pattern_avoidance.o $(PATTERNAVOIDANCE_O)
	$(CXX) $(CXXFLAGS) $^ -o $@

write_avoiders: write_avoiders.o pattern_avoidance.o $(PATTERNAVOIDANCE_O)
	$(CXX) $(CXXFLAGS) $^ -o $@

write_avoiders_count: write_avoiders_count.o pattern_avoidance.o $(PATTERNAVOIDANCE_O)
	$(CXX) $(CXXFLAGS) $^ -o $@

write_degree_one_vertices: write_degree_one_vertices.o minimal_jump_graph.o pattern_avoidance.o $(PATTERNAVOIDANCE_O)
	$(CXX) $(CXXFLAGS) $^ -o $@

write_even_odd_count: write_even_odd_count.o
	$(CXX) $(CXXFLAGS) $^ -o $@

write_min_jumps: write_min_jumps.o minimal_jump_graph.o pattern_avoidance.o $(PATTERNAVOIDANCE_O)
	$(CXX) $(CXXFLAGS) $^ -o $@

write_results: write_results.o minimal_jump_graph.o pattern_avoidance.o $(PATTERNAVOIDANCE_O)
	$(CXX) $(CXXFLAGS) $^ -o $@

minimal_jump_graph.o: minimal_jump_graph.h

pattern_avoidance.o: pattern_avoidance.h

clean:
	$(MAKE) clean -C patternavoidance
	rm -f write_123_bijection write_123_formula write_1234_formula write_algorithm_j write_avoiders write_avoiders_count write_degree_one_vertices write_even_odd_count write_min_jumps write_results *.o
