/*
*  File:        ptree.h
*  Description: Definition of a partitioning tree class for CPSC 221 PA3
*  Date:        2022-02-28 00:08
* 
*               DO NOT MODIFY THIS FILE!
*               PRIVATE FUNCTION DEFINITIONS SHOULD GO IN ptree-private.h
*/

#ifndef _PTREE_H_
#define _PTREE_H_

#include "cs221util/PNG.h"
#include "cs221util/HSLAPixel.h"

using namespace std;
using namespace cs221util;

class Node {
  public:
    // Node attributes
    pair<unsigned int, unsigned int> upperleft; // upper-left coordinate of the Node's image region
    unsigned int width;  // horizontal dimension of the Node's image region
    unsigned int height; // vertical dimension of the Node's image region
    HSLAPixel avg;       // Average colour of the Node's image region
    Node* A;             // pointer to first child (left or upper, depending on direction of child region tiling)
    Node* B;             // pointer to second child (right or lower, depending on direction of child region tiling)

    // Node constructor
    Node(pair<unsigned int, unsigned int> ul = make_pair(0, 0), unsigned int w = 0, unsigned int h = 0, HSLAPixel avgc = HSLAPixel(0, 0, 0), Node* childA = nullptr, Node* childB = nullptr) : upperleft(ul), width(w), height(h), avg(avgc), A(childA), B(childB) {}
};

/*
*  PTree: This is a binary tree whose children divide an image into two vertically-
*         or horizontally-tiled non-overlapping regions which fully cover the node's
*         own image region. Each node contains information about the image region it
*         covers, including the boundaries and dimensions of the region and its
*         average HSLA colour.
*/
class PTree {

    //////////////////////////////////
    // PTree private member attributes
    //////////////////////////////////

  private:

    Node* root; // entry point to the tree

    /////////////////////////////////
    // PTree private member functions
    /////////////////////////////////

    /*
    *  Destroys all dynamically allocated memory associated with the current PTree object.
    *  You may want to add a recursive helper function for this!
    *  POST: all nodes allocated into the heap have been released.
    */
    void Clear();

    /*
    *  Copies the parameter other PTree into the current PTree.
    *  Does not free any memory. Should be called by copy constructor and operator=.
    *  You may want a recursive helper function for this!
    *  PARAM: other - the PTree which will be copied
    *  PRE:   There is no dynamic memory associated with this PTree.
    *  POST:  This PTree is a physically separate copy of the other PTree.
    */
    void Copy(const PTree& other);

    /*
    *  Private helper function for the constructor. Recursively builds the tree
    *  according to the specification of the constructor.
    *  You *may* change this if you like, but we have provided here what we
    *  believe will be sufficient to use as-is.
    *  PARAM:  im - full reference image used for construction
    *  PARAM:  ul - upper-left image coordinate of the currently building Node's image region
    *  PARAM:  w - width of the currently building Node's image region
    *  PARAM:  h - height of the currently building Node's image region
    *  RETURN: pointer to the fully constructed Node
    */
    Node* BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h);

    ////////////////////////////////
    // PTree public member functions
    ////////////////////////////////

  public:

    /*
    *  Constructor that builds the PTree using the provided PNG.
    * 
    *  The PTree represents the sub-image (actually the entire image) from (0,0) to (w-1, h-1) where
    *  w-1 and h-1 are the largest valid image coordinates of the original PNG.
    *  Each node corresponds to a rectangle of pixels in the original PNG, represented by
    *  an (x,y) pair for the upper-left corner of the rectangle, and two unsigned integers for the
    *  number of pixels on the width and height dimensions of the rectangular sub-image region the
    *  node defines.
    * 
    *  A node's two children correspond to a partition of the node's rectangular region into two
    *  equal (or approximately equal) size regions which are either tiled horizontally or vertically.
    * 
    *  If the rectangular region of a node is taller than it is wide, then its two children will divide
    *  the region into vertically-tiled sub-regions of equal height:
    *  +-------+
    *  |   A   |
    *  |       |
    *  +-------+
    *  |   B   |
    *  |       |
    *  +-------+
    * 
    *  If the rectangular region of a node is wider than it is tall, OR if the region is exactly square,
    *  then its two children will divide the region into horizontally-tiled sub-regions of equal width:
    *  +-------+-------+
    *  |   A   |   B   |
    *  |       |       |
    *  +-------+-------+
    * 
    *  If any region cannot be divided exactly evenly (e.g. a horizontal division of odd width), then
    *  child B will receive the larger half of the two subregions.
    * 
    *  When the tree is fully constructed, each leaf corresponds to a single pixel in the PNG image.
    * 
    *  For the average colour, this MUST be computed separately over the node's rectangular region.
    *  Do NOT simply compute this as a weighted average of the children's averages.
    *  The functions defined in hue_utils.h and implemented in hue_utils.cpp will be very useful.
    *  Computing the average over many overlapping rectangular regions sounds like it will be
    *  inefficient, but as an exercise in theory, think about the asymptotic upper bound on the
    *  number of times any given pixel is included in an average calculation.
    * 
    *  PARAM: im - reference image which will provide pixel data for the constructed tree's leaves
    *  POST:  The newly constructed tree contains the PNG's pixel data in each leaf node.
    */
    PTree(PNG& im);

    /*
    *  Copy constructor
    *  Builds a new tree as a copy of another tree.
    *  
    *  PARAM: other - an existing PTree to be copied
    *  POST:  This tree is constructed as a physically separate copy of other tree.
    */
    PTree(const PTree& other);

    /*
    *  Assignment operator
    *  Rebuilds this tree as a copy of another tree.
    * 
    *  PARAM: other - an existing PTree to be copied
    *  POST:  If other is a physically different tree in memory, all pre-existing dynamic
    *           memory in this tree is deallocated and this tree is reconstructed as a
    *           physically separate copy of other tree.
    *         Otherwise, there is no change to this tree.
    */
    PTree& operator=(const PTree& other);

    /*
    *  Destructor
    *  Deallocates all dynamic memory associated with the tree and destroys this PTree object.
    */
    ~PTree();

    /*
    *  Traverses the tree and puts the leaf nodes' color data into the nodes'
    *  defined image regions on the output PNG.
    *  For non-pruned trees, each leaf node corresponds to a single pixel that will be coloured.
    *  For pruned trees, each leaf node may cover a larger rectangular region that will be
    *  entirely coloured using the node's average colour attribute.
    * 
    *  You may want to add a recursive helper function for this!
    * 
    *  RETURN: A PNG image of appropriate dimensions and coloured using the tree's leaf node colour data
    */
    PNG Render() const;

    /*
    *  Trims subtrees as high as possible in the tree. A subtree is pruned
    *  (its children are cleared/deallocated) if ALL of its leaves have colour
    *  within tolerance of the subtree root's average colour.
    *  Pruning criteria should be evaluated on the original tree, and never on a pruned
    *  tree (i.e. we expect that Prune would be called on any tree at most once).
    *  When processing a subtree, you should determine if the subtree should be pruned,
    *  and prune it if possible before determining if it has subtrees that can be pruned.
    * 
    *  You may want to add (a) recursive helper function(s) for this!
    *
    *  PRE:  This tree has not been previously pruned (and is not copied/assigned from a tree that has been pruned)
    *  POST: Any subtrees (as close to the root as possible) whose leaves all have colour
    *        within tolerance from the subtree's root colour will have their children deallocated;
    *        Each pruned subtree's root becomes a leaf node.
    */
    void Prune(double tolerance);

    /*
    *  Returns the total number of nodes in the tree.
    *  This function should run in time linearly proportional to the size of the tree.
    * 
    *  You may want to add a recursive helper function for this!
    */
    int Size() const;

    /*
    *  Returns the total number of leaf nodes in the tree.
    *  This function should run in time linearly proportional to the size of the tree.
    * 
    *  You may want to add a recursive helper function for this!
    */
    int NumLeaves() const;

    /*
    *  Rearranges the nodes in the tree, such that a rendered PNG will be flipped horizontally
    *  (i.e. mirrored over a vertical axis).
    *  This can be achieved by manipulation of the nodes' member attribute(s).
    *  Note that this may possibly be executed on a pruned tree.
    *  This function should run in time linearly proportional to the size of the tree.
    * 
    *  You may want to add a recursive helper function for this!
    *  
    *  POST: Tree has been modified so that a rendered PNG will be flipped horizontally.
    */
    void FlipHorizontal();

    /*
    *  Like the function above, rearranges the nodes in the tree, such that a rendered PNG
    *  will be flipped vertically (i.e. mirrored over a horizontal axis).
    *  This can be achieved by manipulation of the nodes' member attribute(s).
    *  Note that this may possibly be executed on a pruned tree.
    *  This function should run in time linearly proportional to the size of the tree.
    * 
    *  You may want to add a recursive helper function for this!
    *  
    *  POST: Tree has been modified so that a rendered PNG will be flipped vertically.
    */
    void FlipVertical();

    /*
    *  Provides access to the root of the tree.
    *  Dangerous in practice! This is only used for testing.
    */
    Node* GetRoot();

  private:
    
    #include "ptree-private.h"
};

#endif