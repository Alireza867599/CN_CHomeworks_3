#include "ospf.h"

void OSPF::addLink(int node1, int node2, int weight)
{
    adjacencyList[node1].emplace_back(node2, weight);
    adjacencyList[node2].emplace_back(node1, weight);
}

std::unordered_map<int, int> OSPF::computeShortestPaths(int startNode)
{
    std::unordered_map<int, int> distances;
    std::unordered_map<int, int> previous;
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<>> pq;

    for (const auto &[node, _] : adjacencyList)
    {
        distances[node] = std::numeric_limits<int>::max();
        previous[node] = -1;
    }
    distances[startNode] = 0;
    pq.emplace(0, startNode);

    while (!pq.empty())
    {
        int currentDistance = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (currentDistance > distances[currentNode])
            continue;

        for (const auto &[neighbor, weight] : adjacencyList[currentNode])
        {
            int newDistance = currentDistance + weight;
            if (newDistance < distances[neighbor])
            {
                distances[neighbor] = newDistance;
                previous[neighbor] = currentNode;
                pq.emplace(newDistance, neighbor);
            }
        }
    }

    return previous;
}

void OSPF::printPath(int startNode, int endNode, const std::unordered_map<int, int> &previous)
{
    std::vector<int> path;
    for (int at = endNode; at != -1; at = previous.at(at))
    {
        path.push_back(at);
    }

    std::cout << "Path from Node " << startNode << " to Node " << endNode << ": ";
    for (size_t i = 0; i < path.size(); ++i)
    {
        std::cout << path[i];
        if (i < path.size() - 1)
            std::cout << " -> ";
    }
    std::cout << "\n";
}
