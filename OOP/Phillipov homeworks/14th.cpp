// 14thPhillipov.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
//#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_utility.hpp>
#include <limits>
#include <boost/graph/dijkstra_shortest_paths.hpp>
//#include <ctime>


using namespace std;


int main() {
    //typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
    typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;

    //typedef boost::adjacency_matrix<boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;

    int cities_number;
    cout << "Enter number of cities below:\n";
    cin >> cities_number;

    Graph g(cities_number);

    for (int i = 0; i < (int) cities_number / 3; i++) {
        for (int j = max(i + 1, (int) cities_number / 3); j < cities_number; j++) {
            if (i == cities_number - j) continue;
            boost::add_edge(i, j, rand() % 200, g);
        }
    }

    boost::print_graph(g);

    std::vector<int> distances(boost::num_vertices(g));
    std::vector<int> predecessors(boost::num_vertices(g));

    typedef boost::property_map<Graph, boost::vertex_index_t>::type IndexMap;
    typedef boost::iterator_property_map<std::vector<int>::iterator, IndexMap> PredecessorMap;
    typedef boost::iterator_property_map<std::vector<int>::iterator, IndexMap> DistanceMap;

    Graph::vertex_iterator vi, vi_end;
    for (boost::tie(vi, vi_end) = boost::vertices(g); vi != vi_end; ++vi) {
        boost::dijkstra_shortest_paths(
            g, *vi,
            boost::predecessor_map(PredecessorMap(predecessors.begin(), get(boost::vertex_index, g)))
            .distance_map(DistanceMap(distances.begin(), get(boost::vertex_index, g))));

        for (int target_vertex = 0; target_vertex < cities_number; ++target_vertex) {
            if (target_vertex != *vi) {
                std::cout << "Shortest path from " << *vi << " to " << target_vertex << ": ";
                int current_vertex = target_vertex;
                while (current_vertex != *vi) {
                    std::cout << current_vertex << " <- ";
                    current_vertex = predecessors[current_vertex];
                }
                std::cout << *vi << " (Distance: " << distances[target_vertex] << ")" << std::endl;
            }
        }
    }

    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
