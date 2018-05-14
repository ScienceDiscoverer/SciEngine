#ifndef SCIHIERARCHY_H
#define SCIHIERARCHY_H

#include "../SciMath/SciVector2.h"
#include "../SciMath/SciMatrix2x2.h"
#include "SciBoundingObjects2D.h"
#include "SciArray.h"

// Helper structs
struct Transforms
{
	const SciVector2* l_pos; // Local position
	SciVector2* w_pos; // World position
	const SciMatrix2x2* l_orient; // Local orientation
	SciMatrix2x2* w_orient; // World orientation
};

class SciHierarchy
{
public:
	// Access functions
	// Nodes must be added in order from base to farthest child
	void AddNode(int base, Transforms trs, SciAABR* brec = nullptr);
	void RemoveNode(int index) { tree.Pop(index); }

	// Core operations
	void Update();

private:
	// Internal struct
	struct Node
	{
		// Data
		int base; // Base Object (parent)
		Transforms trans;
		SciAABR* b_rec;
	};

	// Data
	SciArray<Node> tree;
};

#endif /* SCIHIERARCHY_H */