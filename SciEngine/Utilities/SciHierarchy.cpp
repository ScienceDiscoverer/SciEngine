#include "SciHierarchy.h"

// Access functions
// Nodes must be added in order from base to farthest child
void SciHierarchy::AddNode(int base, Transforms trs, SciAABR* brec)
{
	Node tmp;
	tmp.base = base;
	tmp.trans = trs;
	tmp.b_rec = brec;
	tree.Push(tmp);
}
// End of Access functions

// Core operations
void SciHierarchy::Update()
{
	for(int i = 1; i < tree.Size(); ++i)
	{
		const int b = tree[i].base;
		SciVector2& w_pos = *tree[i].trans.w_pos;
		const SciVector2 old_w_pos = w_pos;
		// Update children's world transformations
		*tree[i].trans.w_orient = *tree[b].trans.w_orient * *tree[i].trans.l_orient;
		w_pos = *tree[b].trans.w_pos + *tree[i].trans.l_pos;
		// Update bounding object
		if(tree[i].b_rec)
		{
			tree[i].b_rec->Update(w_pos - old_w_pos);
		}
	}
}
// End of Core operations