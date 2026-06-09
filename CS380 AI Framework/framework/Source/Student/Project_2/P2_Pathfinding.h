#pragma once
#include "Misc/PathfindingDetails.hpp"

class AStarPather
{
public:
    /* 
        The class should be default constructible, so you may need to define a constructor.
        If needed, you can modify the framework where the class is constructed in the
        initialize functions of ProjectTwo and ProjectThree.
    */

    /* ************************************************** */
    // DO NOT MODIFY THESE SIGNATURES
    bool initialize();
    void shutdown();
    PathResult compute_path(PathRequest &request);
    /* ************************************************** */

    /*
        You should create whatever functions, variables, or classes you need.
        It doesn't all need to be in this header and cpp, structure it whatever way
        makes sense to you.
    */

    enum class NodeStatues {
		INACTIVE,
        OPEN,
        CLOSED
    };

    struct Node {
    public:
        GridPos pos; //position of the node
        Node* parent = nullptr; //pointer to the parent node
		float cost = -1.0f; //cost from start to this node
		float heuristic = -1.0f; //estimated cost from this node to goal
		NodeStatues status = NodeStatues::INACTIVE;
        uint8_t     neighbors = 0; //neighbours

    };
    //Matrix that holds all the nodes in the map
    std::array< std::array<Node, 40>, 40> nodes;
    //List of nodes in the openlist to be checked
	std::vector<Node*> openList;

    //Node List helper functions
	// reset ad initialize the list
    void ResetNodes();

    //my codes
	void precomputeNeighbors();
	float computeHeuristic(const GridPos& a, const GridPos& b, Heuristic type) const;
	void buildPath(Node* goalNode, PathRequest& request) const;
	void rubberband(WaypointList& path) const;
	void smooth(WaypointList& path) const;
	void addPointsIfNeeded(WaypointList& path) const;
    void onMapChange();

};