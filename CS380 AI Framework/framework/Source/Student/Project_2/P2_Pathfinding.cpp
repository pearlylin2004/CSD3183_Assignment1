#include <pch.h>
#include "Projects/ProjectTwo.h"
#include "P2_Pathfinding.h"

#pragma region Extra Credit
bool ProjectTwo::implemented_floyd_warshall()
{
    return false;
}

bool ProjectTwo::implemented_goal_bounding()
{
    return false;
}

bool ProjectTwo::implemented_jps_plus()
{
    return false;
}
#pragma endregion

namespace Heuristics {
    float manhattan(const GridPos a, const GridPos b) {
        return std::abs(a.row - b.row) + std::abs(a.col - b.col);
    }
}

//constants
static constexpr int   MAP_SIZE = 40;
static constexpr float SQRT2 = 1.41421356f;
static constexpr float CARD_COST = 1.0f;
static constexpr float DIAG_COST = 1.41421356f;

//8 neighbour offsets
static const int DR[8] = { -1, -1,  0,  1,  1,  1,  0, -1 };
static const int DC[8] = { 0,  1,  1,  1,  0, -1, -1, -1 };

bool AStarPather::initialize()
{
    // handle any one-time setup requirements you have

    /*
        If you want to do any map-preprocessing, you'll need to listen
        for the map change message.  It'll look something like this:

        Callback cb = std::bind(&AStarPather::your_function_name, this);
        Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

        There are other alternatives to using std::bind, so feel free to mix it up.
        Callback is just a typedef for std::function<void(void)>, so any std::invoke'able
        object that std::function can wrap will suffice.
    */
    for (int r = 0; r < MAP_SIZE; ++r)
        for (int c = 0; c < MAP_SIZE; ++c)
            nodes[r][c].pos = GridPos{ r, c };

    Callback cb = std::bind(&AStarPather::onMapChange, this);
    Messenger::listen_for_message(Messages::MAP_CHANGE, cb);

    return true; // return false if any errors actually occur, to stop engine initialization
}

void AStarPather::shutdown()
{
    /*
        Free any dynamically allocated memory or any other general house-
        keeping you need to do during shutdown.
    */
}

void AStarPather::onMapChange() {
    precomputeNeighbors();
}

void AStarPather::precomputeNeighbors() {
    int rows = terrain->get_map_height();
    int cols = terrain->get_map_width();

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            nodes[r][c].neighbors = 0;

            if (terrain->is_wall(r, c)) continue;

            for (int d = 0; d < 8; ++d) {
                int nr = r + DR[d];
                int nc = c + DC[d];

                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) continue;
                if (terrain->is_wall(nr, nc)) continue;

                bool isDiag = (d % 2 == 1);
                if (isDiag) {
                    int cardA = (d - 1 + 8) % 8;
                    int cardB = (d + 1) % 8;
                    int raA = r + DR[cardA], caA = c + DC[cardA];
                    int raB = r + DR[cardB], caB = c + DC[cardB];

                    bool openA = (raA >= 0 && raA < rows && caA >= 0 && caA < cols && !terrain->is_wall(raA, caA));
                    bool openB = (raB >= 0 && raB < rows && caB >= 0 && caB < cols && !terrain->is_wall(raB, caB));

                    if (!openA || !openB) continue;
                }
                nodes[r][c].neighbors |= (1 << d);
            }
        }
    }
}

void AStarPather::ResetNodes() {
    //reset all the nodes in the node matrix to their default values
    for (int i = 0; i < nodes.size(); i++) {
        for (int j = 0; j < nodes[i].size(); j++) {
            nodes[i][j].parent = nullptr;
            nodes[i][j].cost = -1.0f;
            nodes[i][j].heuristic = -1.0f;
            nodes[i][j].status = NodeStatues::INACTIVE;
        }
    }
}

float AStarPather::computeHeuristic(const GridPos& a, const GridPos& b, Heuristic type) const {
    float dx = static_cast<float>(std::abs(a.row - b.row));
    float dy = static_cast<float>(std::abs(a.col - b.col));

    switch (type) {
    case Heuristic::OCTILE:
        return std::min(dx, dy) * SQRT2 + std::abs(dx - dy);
    case Heuristic::CHEBYSHEV:
        return std::max(dx, dy);
    case Heuristic::MANHATTAN:
        return dx + dy;
    case Heuristic::EUCLIDEAN:
        return std::sqrt(dx * dx + dy * dy);
    case Heuristic::INCONSISTENT:
        if ((a.row + a.col) % 2 != 0)
            return std::sqrt(dx * dx + dy * dy);
        return 0.f;
    default:
        return std::min(dx, dy) * SQRT2 + std::abs(dx - dy);
    }
}

void AStarPather::buildPath(Node* goalNode, PathRequest& request) const {
    std::vector<Vec3> reverse;
    Node* cur = goalNode;
    while (cur != nullptr) {
        reverse.push_back(terrain->get_world_position(cur->pos));
        cur = cur->parent;
    }
    for (int i = static_cast<int>(reverse.size()) - 1; i >= 0; --i) {
        request.path.push_back(reverse[i]);
    }
}

void AStarPather::rubberband(WaypointList& path) const {
    if (path.size() < 3) return;

    bool changed = true;
    while (changed) {
        changed = false;
        auto it = path.begin();
        while (it != path.end()) {
            auto itB = std::next(it);
            if (itB == path.end()) break;
            auto itC = std::next(itB);
            if (itC == path.end()) break;

            GridPos a = terrain->get_grid_position(*it);
            GridPos c = terrain->get_grid_position(*itC);

            int rMin = std::min(a.row, c.row);
            int rMax = std::max(a.row, c.row);
            int cMin = std::min(a.col, c.col);
            int cMax = std::max(a.col, c.col);

            bool clear = true;
            for (int r = rMin; r <= rMax && clear; ++r) {
                for (int cc = cMin; cc <= cMax && clear; ++cc) {
                    if (terrain->is_wall(r, cc)) {
                        clear = false;
                    }
                }
            }
            if (clear) {
                path.erase(itB);
                changed = true;
            }
            else {
                ++it;
            }
        }
    }
}

void AStarPather::addPointsIfNeeded(WaypointList& path) const {
    if (path.size() < 2) return;

    bool changed = true;
    while (changed) {
        changed = false;
        auto it = path.begin();
        while (it != path.end()) {
            auto next = std::next(it);
            if (next == path.end()) break;

            GridPos a = terrain->get_grid_position(*it);
            GridPos b = terrain->get_grid_position(*next);
            float dx = static_cast<float>(a.row - b.row);
            float dy = static_cast<float>(a.col - b.col);
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist > 1.5f) {
                Vec3 mid;
                mid.x = (it->x + next->x) * 0.5f;
                mid.y = (it->y + next->y) * 0.5f;
                mid.z = (it->z + next->z) * 0.5f;
                path.insert(next, mid);
                changed = true;
            }
            else {
                ++it;
            }
        }
    }
}

static Vec3 catmullRom(const Vec3& p0, const Vec3& p1, const Vec3& p2, const Vec3& p3, float s) {
    float s2 = s * s;
    float s3 = s2 * s;
    Vec3 out;
    out.x = 0.5f * ((-p0.x + 3 * p1.x - 3 * p2.x + p3.x) * s3 + (2 * p0.x - 5 * p1.x + 4 * p2.x - p3.x) * s2 + (-p0.x + p2.x) * s + 2 * p1.x);
    out.y = 0.5f * ((-p0.y + 3 * p1.y - 3 * p2.y + p3.y) * s3 + (2 * p0.y - 5 * p1.y + 4 * p2.y - p3.y) * s2 + (-p0.y + p2.y) * s + 2 * p1.y);
    out.z = 0.5f * ((-p0.z + 3 * p1.z - 3 * p2.z + p3.z) * s3 + (2 * p0.z - 5 * p1.z + 4 * p2.z - p3.z) * s2 + (-p0.z + p2.z) * s + 2 * p1.z);
    return out;
}

void AStarPather::smooth(WaypointList& path) const {
    if (path.size() < 2) return;

    std::vector<Vec3> pts(path.begin(), path.end());
    int n = static_cast<int>(pts.size());

    WaypointList smoothed;
    for (int i = 0; i < n - 1; ++i) {
        const Vec3& p0 = (i == 0) ? pts[0] : pts[i - 1];
        const Vec3& p1 = pts[i];
        const Vec3& p2 = pts[i + 1];
        const Vec3& p3 = (i + 2 >= n) ? pts[n - 1] : pts[i + 2];

        smoothed.push_back(p1);
        smoothed.push_back(catmullRom(p0, p1, p2, p3, 0.25f));
        smoothed.push_back(catmullRom(p0, p1, p2, p3, 0.50f));
        smoothed.push_back(catmullRom(p0, p1, p2, p3, 0.75f));
    }
    smoothed.push_back(pts.back());
    path = smoothed;
}

PathResult AStarPather::compute_path(PathRequest& request)
{
    /*
        This is where you handle pathing requests, each request has several fields:

        start/goal - start and goal world positions
        path - where you will build the path upon completion, path should be
            start to goal, not goal to start
        heuristic - which heuristic calculation to use
        weight - the heuristic weight to be applied
        newRequest - whether this is the first request for this path, should generally
            be true, unless single step is on

        smoothing - whether to apply smoothing to the path
        rubberBanding - whether to apply rubber banding
        singleStep - whether to perform only a single A* step
        debugColoring - whether to color the grid based on the A* state:
            closed list nodes - yellow
            open list nodes - blue

            use terrain->set_color(row, col, Colors::YourColor);
            also it can be helpful to temporarily use other colors for specific states
            when you are testing your algorithms

        method - which algorithm to use: A*, Floyd-Warshall, JPS+, or goal bounding,
            will be A* generally, unless you implement extra credit features

        The return values are:
            PROCESSING - a path hasn't been found yet, should only be returned in
                single step mode until a path is found
            COMPLETE - a path to the goal was found and has been built in request.path
            IMPOSSIBLE - a path from start to goal does not exist, do not add start position to path
    */

    // WRITE YOUR CODE HERE

    //find the start node and init the search
    GridPos start = terrain->get_grid_position(request.start);
    GridPos goal = terrain->get_grid_position(request.goal);

    if (request.newRequest) {

        //initialize the lists
        ResetNodes();
        openList = {};
        request.newRequest = false;

        //find starting node
        GridPos start = terrain->get_grid_position(request.start);
        Node* startNode = &nodes[start.row][start.col];
        startNode->status = NodeStatues::OPEN;
        startNode->cost = 0.0f;
        startNode->heuristic = 0.0f;
        openList.push_back(startNode);
        //paint the node in blue if requested
        if (request.settings.debugColoring) {
            terrain->set_color(start, Colors::Blue);
        }
    }

    //While (Open List is not empty) {
    while (openList.size() > 0) {
        //Pop cheapest node off Open List(parent node)
        //get the list sorted (there are many ways)
        int bestIdx = 0;
        float bestF = openList[0]->cost + openList[0]->heuristic * request.settings.weight;
        for (int k = 1; k < static_cast<int>(openList.size()); ++k) {
            float f = openList[k]->cost + openList[k]->heuristic * request.settings.weight;
            if (f < bestF) {
                bestF = f; bestIdx = k;
            }
        }
        //get the cheapest element
        Node* currentNode = openList[bestIdx];
        //remove the referenced node from the openList
        openList[bestIdx] = openList.back();
        openList.pop_back();
        //check if node is already on the closed list
        if (currentNode->status == NodeStatues::CLOSED) {
            continue;
        }

        //If node is the Goal Node, then path found(RETURN  found)
        if (currentNode->pos == goal) {
            // post process the path if needed
            buildPath(currentNode, request);
            if (request.settings.rubberBanding) {
                rubberband(request.path);
            }
            if (request.settings.smoothing) {
                if (request.settings.rubberBanding) {
                    addPointsIfNeeded(request.path);
                }
                smooth(request.path);
            }
            //return the completed path
            return PathResult::COMPLETE;
        }
        //For (all neighboring child nodes) {
        //check neighbors
        std::array<GridPos, 8> neighborOffset = { GridPos{-1,0}, GridPos{-1,1}, GridPos{0, 1}, GridPos{1, 1}, GridPos{1, 0}, GridPos{1, -1}, GridPos{0, -1}, GridPos{-1, -1} };
        for (size_t i = 0; i < neighborOffset.size(); i++) {
            //Compute its cost, f(x) = g(x) + h(x)
            //If child node isn t on Open or Closed list, put it on Open List.
            //If child node is on Open or Closed List, AND this new one is cheaper,
            //then take the old expensive one off both lists and put this new cheaper one on the Open List.

             // skip if this direction is blocked (wall or corner cut)
            if (!(currentNode->neighbors & (1 << i))) continue;

            int nr = currentNode->pos.row + DR[i];
            int nc = currentNode->pos.col + DC[i];
            Node* child = &nodes[nr][nc];

            float moveCost = (i % 2 == 0) ? CARD_COST : DIAG_COST;
            float newG = currentNode->cost + moveCost;
            float newH = computeHeuristic(child->pos, goal, request.settings.heuristic);
            float newF = newG + (newH * request.settings.weight);

            if (child->status == NodeStatues::INACTIVE) {
                child->parent = currentNode;
                child->cost = newG;
                child->heuristic = newH;
                child->status = NodeStatues::OPEN;
                openList.push_back(child);

                if (request.settings.debugColoring) {
                    terrain->set_color(child->pos, Colors::Blue);
                }
            }
            else if (newF < child->cost + child->heuristic * request.settings.weight) {
                child->parent = currentNode;
                child->cost = newG;
                child->heuristic = newH;
                child->status = NodeStatues::OPEN;
                openList.push_back(child);
                if (request.settings.debugColoring) {
                    terrain->set_color(child->pos, Colors::Blue);
                }
            }
        }

        //Place parent node on the Closed List (we re done with it)
        currentNode->status = NodeStatues::CLOSED;
        //update color to yellow after set it to closed
        //paint the node in yellow if requested
        if (request.settings.debugColoring) {
            terrain->set_color(currentNode->pos, Colors::Yellow);
        }

        //Node* neig_N = &nodes[currentNode->pos.row][currentNode->pos.col + 1];
        //Node* neig_S = &nodes[currentNode->pos.row][currentNode->pos.col - 1];
        //Node* neig_E = &nodes[currentNode->pos.row + 1][currentNode->pos.col];
        //Node* neig_W = &nodes[currentNode->pos.row - 1][currentNode->pos.col];


        //If taken too much time this frame (or in single step mode), abort search for now and resume next frame (RETURN  working )
        if (request.settings.singleStep) {
            return PathResult::PROCESSING;
        }
        //Open List empty, thus no path possible (RETURN  fail )

    }

    return PathResult::IMPOSSIBLE;

    // Just sample code, safe to delete
    /*
    GridPos goal = terrain->get_grid_position(request.goal);
    terrain->set_color(start, Colors::Orange);
    terrain->set_color(goal, Colors::Orange);
    request.path.push_back(request.start);
    request.path.push_back(request.goal);
    return PathResult::COMPLETE;
    */
}
