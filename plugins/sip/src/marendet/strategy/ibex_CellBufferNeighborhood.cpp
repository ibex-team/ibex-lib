/* ============================================================================
 * I B E X - ibex_CellBufferNeighborhood.cpp
 * ============================================================================
 * Copyright   : IMT Atlantique (FRANCE)
 * License     : This program can be distributed under the terms of the GNU LGPL.
 *               See the file COPYING.LESSER.
 *
 * Author(s)   : Antoine Marendet, Gilles Chabert
 * Created     : May 4, 2018
 * ---------------------------------------------------------------------------- */
 
#include "ibex_CellBufferNeighborhood.h"

#include <limits>
#include <stack>
#include <utility>

namespace ibex {

CellBufferNeighborhood::CellBufferNeighborhood(Vector start, Vector goal, CellBufferNeighborhood::Heuristic heuristic)
    : start_(start)
    , goal_(goal)
    , heuristic_(heuristic)
{
}

CellBufferNeighborhood::~CellBufferNeighborhood()
{
    for (GraphNode* node : stack_) {
        delete node;
    }
}

void CellBufferNeighborhood::flush()
{
    for (GraphNode* node : stack_) {
        delete node->cell;
        delete node;
    }
    stack_.clear();
}

unsigned int CellBufferNeighborhood::size() const
{
    return stack_.size();
}

bool CellBufferNeighborhood::empty() const
{
    return stack_.empty();
}

void CellBufferNeighborhood::push(Cell* cell)
{
    GraphNode* newNode = new GraphNode(cell, GraphNode::UNKNOWN);
    for (GraphNode* node : stack_) {
        if (node->cell->box.intersects(cell->box)) {
            //double weight = distance(newNode->mid, node->mid);
            double weight = node->type == GraphNode::UNKNOWN ? 1 : 0;
            newNode->neighborsWeight.insert(Edge(node, weight));
            node->neighborsWeight.insert(Edge(newNode, weight));
        }
    }
    stack_.emplace(newNode);
    if (cell->box.contains(start_)) {
        start_node_ = newNode;
    }
    if (cell->box.contains(goal_)) {
        goal_node_ = newNode;
    }
}

void CellBufferNeighborhood::pushInner(Cell* cell)
{
    GraphNode* newNode = new GraphNode(cell, GraphNode::INNER);
    for (GraphNode* node : stack_) {
        if (node->cell->box.intersects(cell->box)) {
            //double weight = distance(newNode->mid, node->mid);
            double weight = node->type == GraphNode::UNKNOWN ? 1 : 0;
            newNode->neighborsWeight.insert(Edge(node, weight));
            node->neighborsWeight.insert(Edge(newNode, weight));
        }
    }
    stack_.emplace(newNode);
    if (cell->box.contains(start_)) {
        start_node_ = newNode;
    }
    if (cell->box.contains(goal_)) {
        goal_node_ = newNode;
    }
}

Cell* CellBufferNeighborhood::pop()
{
    //GraphNode* top = stack_.back();
    //stack_.pop_back();
    //GraphNode* top = topGraphNode();
    Edge top = last_top_;
    if (top.node == nullptr)
        return nullptr;
    for (auto neighbor : top.node->neighborsWeight) {
        neighbor.node->neighborsWeight.erase(top);
    }
    stack_.erase(top.node);
    Cell* cell = top.node->cell;
    delete top.node;
    return cell;
}

Cell* CellBufferNeighborhood::top() const
{
    Edge top = topGraphNode();
    last_top_ = top;
    if (top.node == nullptr) {
        return nullptr;
    } else {
        return top.node->cell;
    }
    //return stack_.back()->cell;
}

CellBufferNeighborhood::Edge CellBufferNeighborhood::topGraphNode() const
{
    // We must first prove that the starting and goal points are feasible
    if (start_node_->type == GraphNode::UNKNOWN) {
        return Edge(start_node_, 0.0);
    } else if (goal_node_->type == GraphNode::UNKNOWN) {
        return Edge(goal_node_, 0.0);
    }
    std::vector<Edge> path = shortestPath(start_node_, goal_node_);
    for (const Edge& n : path) {
        if (n.node->type == GraphNode::UNKNOWN) {
            isPathFound = false;
            return n;
        }
    }
    // PATH FOUND
    pathFound.clear();
    for (const Edge& n : path) {
        pathFound.emplace_back(n.node->cell->box);
    }
    isPathFound = true;
    return Edge{ nullptr, 0.0 };
    //return stack_.back()->cell;
}

std::set<Cell*> CellBufferNeighborhood::GraphNode::connectedComponent() const
{
    std::set<Cell*> component;
    std::stack<Edge> toExplore;
    while (!toExplore.empty()) {
        Edge edge = toExplore.top();
        toExplore.pop();
        for (Edge neighbor : edge.node->neighborsWeight) {
            if (component.find(neighbor.node->cell) == component.end()) {
                // not yet in component
                toExplore.push(neighbor);
            }
        }
        component.insert(edge.node->cell);
    }
    return component;
}

double CellBufferNeighborhood::distance(const Vector& v1, const Vector& v2) const
{
    if (heuristic_ == DIJKSTRA)
        return 0;
    else if (heuristic_ == A_STAR_DISTANCE)
        return norm(v1 - v2);
    else
        return 0;
}

std::vector<CellBufferNeighborhood::Edge> CellBufferNeighborhood::reconstructPath(
    const std::map<GraphNode*, Edge>& cameFrom, const Edge& current) const
{
    std::vector<Edge> path;
    path.emplace_back(current);
    auto it = cameFrom.find(current.node);
    while (it != cameFrom.end()) {
        path.emplace_back(it->second);
        it = cameFrom.find(it->second.node);
    }
    return path;
}

std::vector<CellBufferNeighborhood::Edge> CellBufferNeighborhood::shortestPath(GraphNode* start,
    GraphNode* goal) const
{
    std::set<GraphNode*> closedSet;
    std::set<Edge> openSet;
    Edge start_edge(start, 0.0);
    openSet.emplace(start_edge);
    std::map<GraphNode*, Edge> cameFrom;
    std::map<Edge, double> gScore;
    gScore.emplace(std::make_pair(start_edge, 0.0));
    std::map<Edge, double> fScore;
    fScore.emplace(std::make_pair(start_edge, distance(start->mid, goal->mid)));

    auto defaultMapGet = [](const std::map<Edge, double>& map, const Edge& value) {
        const auto it = map.find(value);
        if (it == map.end()) {
            return std::numeric_limits<double>::max();
        } else {
            return it->second;
        }
    };

    while (!openSet.empty()) {
        Edge current = *openSet.begin();
        double fScore_value = defaultMapGet(fScore, current);
        for (const Edge& n : openSet) {
            double tmp = defaultMapGet(fScore, n);
            if (tmp < fScore_value) {
                fScore_value = tmp;
                current = n;
            }
        }
        /*GraphNode* current = *std::min(openSet.begin(), openSet.end(), [&,fScore](GraphNode* n1, GraphNode* n2) {
		 return defaultMapGet(fScore, n1) < defaultMapGet(fScore, n2);
		 });*/
        if (current.node == goal) {
            return reconstructPath(cameFrom, current);
        }
        openSet.erase(current);
        closedSet.emplace(current.node);
        for (const Edge& neighbor : current.node->neighborsWeight) {
            if (closedSet.find(neighbor.node) != closedSet.end()) {
                continue;
            }
            openSet.emplace(neighbor);

            double tentative_gScore = defaultMapGet(gScore, current) + current.weight;
            if (tentative_gScore >= defaultMapGet(gScore, neighbor)) {
                continue;
            }
            cameFrom[neighbor.node] = current;
            gScore[neighbor] = tentative_gScore;
            fScore[neighbor] = tentative_gScore + distance(neighbor.node->mid, goal->mid);
        }
    }
    return std::vector<Edge>();
}

} // end namespace ibex
