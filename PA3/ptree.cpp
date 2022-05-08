/*
*  File:        ptree.cpp
*  Description: Implementation of a partitioning tree class for CPSC 221 PA3
*  Date:        2022-03-03 01:53
*
*               ADD YOUR PRIVATE FUNCTION IMPLEMENTATIONS TO THE BOTTOM OF THIS FILE
*/

#include "ptree.h"
#include "hue_utils.h" // useful functions for calculating hue averages

using namespace cs221util;
using namespace std;

// The following definition may be convenient, but is not necessary to use
typedef pair<unsigned int, unsigned int> pairUI;

/////////////////////////////////
// PTree private member functions
/////////////////////////////////

/*
*  Destroys all dynamically allocated memory associated with the current PTree object.
*  You may want to add a recursive helper function for this!
*  POST: all nodes allocated into the heap have been released.
*/
void PTree::Clear() {
  clear(root);
 // delete(root);
}

/*
*  Copies the parameter other PTree into the current PTree.
*  Does not free any memory. Should be called by copy constructor and operator=.
*  You may want a recursive helper function for this!
*  PARAM: other - the PTree which will be copied
*  PRE:   There is no dynamic memory associated with this PTree.
*  POST:  This PTree is a physically separate copy of the other PTree.
*/
void PTree::Copy(const PTree& other) {
root = copy(other.root);
}

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
 Node* PTree::BuildNode(PNG& im, pair<unsigned int, unsigned int> ul, unsigned int w, unsigned int h) {
  if(w==1 && h==1){
    Node *parent = new Node(ul,w,h,avgPixel(im,ul.first,ul.first + w, ul.second,ul.second + h));
    return parent;
    //delete(parent);
  } else{
    
  Node *parent = new Node(ul,w,h,avgPixel(im,ul.first,ul.first + w, ul.second,ul.second + h));
  // parent->A = new Node(); 
  // parent->B = new Node();

  pair<unsigned int, unsigned int> ulA;
  int widthA;
  int heightA;
  HSLAPixel pixelA;

  pair<unsigned int, unsigned int> ulB;
  int widthB;
  int heightB;
  HSLAPixel pixelB;


  if(w >= h){

    ulA = make_pair(parent->upperleft.first, parent->upperleft.second);
    widthA = floor((double)w / (double)2);
    heightA = h; 
    pixelA = avgPixel(im, ulA.first, ulA.first + widthA,
                              ulA.second, ulA.second + heightA ); 

    ulB = make_pair(parent->upperleft.first + floor((double)w / (double)2), parent->upperleft.second);
    widthB = ceil((double)w / (double)2);
    heightB = h; 
    pixelB = avgPixel(im, ulB.first, ulB.first + widthB,
                              ulB.second, ulB.second + heightB );
                          
  }else if(w < h){

   ulA = make_pair(parent->upperleft.first, parent->upperleft.second);
   widthA = parent->width;
   heightA = floor((double)parent->height/(double)2); 
   pixelA = avgPixel(im, ulA.first, ulA.first + widthA,
                              ulA.second, ulA.second + heightA ); 
  

   ulB = make_pair(parent->upperleft.first, parent->upperleft.second + floor((double)parent->height / (double)2));
   widthB = parent->width;
   heightB = ceil((double)parent->height/(double)2); 
   pixelB = avgPixel(im, ulB.first, ulB.first + widthB,
                              ulB.second, ulB.second + heightB );
  }

  parent->A = BuildNode(im,ulA,widthA, heightA);
  parent->B = BuildNode(im,ulB,widthB, heightB);


return parent;
// parent = NULL; 
// delete(parent->A);
// parent->A = NULL; 
// delete(parent->B); 
// parent->B = NULL; 
// delete(parent);
// clear(root);  

    }   
}

////////////////////////////////
// PTree public member functions
////////////////////////////////

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
PTree::PTree(PNG& im) {
root = BuildNode(im,make_pair(0,0),im.width(),im.height());
   
}

/*
*  Copy constructor
*  Builds a new tree as a copy of another tree.
*
*  PARAM: other - an existing PTree to be copied
*  POST:  This tree is constructed as a physically separate copy of other tree.
*/
PTree::PTree(const PTree& other) {
  Copy(other); 
}

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
PTree& PTree::operator=(const PTree& other) {
  if(this != &other){
    Clear();
    Copy(other);
  }
  return *this;
}

/*
*  Destructor
*  Deallocates all dynamic memory associated with the tree and destroys this PTree object.
*/
PTree::~PTree() {
Clear();
root = NULL;
 
}

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
PNG PTree::Render() const {
PNG *img = new PNG(root->width,root->height);
// int height = countHeight(root);
// int width = countWidth(root);
render(img,root);

PNG png = *img;
delete(img);
img = NULL;
return png;
}

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
void PTree::Prune(double tolerance) {
  
 if (root == NULL){
   return;
 } else{
   pruneNode(root,tolerance);
 }
  
}

/*
*  Returns the total number of nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::Size() const {
  // replace the line below with your implementation
  return size(root);
}

/*
*  Returns the total number of leaf nodes in the tree.
*  This function should run in time linearly proportional to the size of the tree.
*
*  You may want to add a recursive helper function for this!
*/
int PTree::NumLeaves() const {
return leafCount(root);
}

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
void PTree::FlipHorizontal() {
  // add your implementation below
  int width = countWidth(root);
  flipHorizontalNode(root); //width);
  
}

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
void PTree::FlipVertical() {
  // add your implementation below
  int height = countHeight(root);
  flipVerticalNode(root); //,height);
  
}

/*
    *  Provides access to the root of the tree.
    *  Dangerous in practice! This is only used for testing.
    */
Node* PTree::GetRoot() {
  return root;
}

//////////////////////////////////////////////
// PERSONALLY DEFINED PRIVATE MEMBER FUNCTIONS
//////////////////////////////////////////////

HSLAPixel PTree::avgPixel(PNG &img, unsigned int minX, unsigned int maxX, unsigned int minY,unsigned int maxY){
  double x_component = 0.0;
  double y_component = 0.0;
  double s = 0.0;
  double l = 0.0;
  double a = 0.0;
  double denom = 0.0;

  if ((maxX - minX == 1 && maxY - minY == 1)) {
    HSLAPixel *pixel = img.getPixel (minX,minY);
    return *pixel;

  } else {
    for(unsigned int x = minX; x < maxX ; x++){
      for(unsigned int y = minY; y < maxY; y++){
          HSLAPixel *pixel = img.getPixel(x,y);
          x_component += Deg2X(pixel->h);
          y_component += Deg2Y(pixel->h);
          s+= pixel->s;
          l+=pixel->l;
          a+=pixel->a;
          denom += 1.0;
        }
  
     }
    x_component = x_component/denom;
    y_component = y_component/denom;
    double h = XY2Deg(x_component, y_component);
    s = s/denom;
    l = l/denom;
    a = a/denom;
    return HSLAPixel(h,s,l,a); 
  }

  }

Node* PTree:: copy(Node *node){
  if(node == NULL){
    return NULL;
  } else{
    Node *newNode = new Node(node->upperleft, node->width, node->height, node->avg, NULL, NULL);
    newNode->A = copy(node->A);
    newNode->B = copy(node->B);
    return newNode;
  }
}


void PTree::clear(Node *node){
  if(node == NULL){
    return;
  }
if(node->A){
    clear(node->A);
}
if(node->B){
    clear(node->B);
}
delete(node);
node = NULL;
}

int PTree::leafCount(Node *node)const{
  if(node == NULL){
    return 0;
  }
  if(node->A == NULL && node->B == NULL){
    return 1;
  } else{
    return leafCount(node->A) + leafCount(node->B);
  }
}

int PTree::size(Node *node) const{
  if(node == NULL){
    return 0;
  } else{
   return 1 + size(node->A) + size(node->B);
  }
}

void PTree::render(PNG *png, Node *node)const{
//two for loops for rectanlge //everything in rectangle set it to average 
if(node == NULL){
  //return *png;
  return;
}

if(!node->A && !node->B){
  for(int x = node->upperleft.first; x < node->upperleft.first + node->width; x++){
    for(int y = node->upperleft.second; y < node->upperleft.second + node->height; y++){
      // if(x + node->width >= width - 1){
      //   x = x - (width - 1);
      // }
      // if(y + node->height >= height - 1){
      //   y = y - (height - 1);
      // }
      HSLAPixel *pixel = png->getPixel(x,y);
      pixel->h = node->avg.h;
      pixel->s = node->avg.s;
      pixel->l = node->avg.l;
      pixel->a = node->avg.a;
    }
  }
}

if(node->A){
  render(png,node->A);
}

if(node->B){
  render(png,node->B);
}
}

// mirror over a horizontal axis 
void PTree::flipHorizontalNode(Node *root) { //, int width) { 

if (root == NULL) { 
    return;  
  } 

if (root->A == NULL && root->B == NULL) { 
    return; 
}
  if (root->width >= root->height) { 
    pair<unsigned int, unsigned int> ulA = make_pair(root->upperleft.first + root->B->width, root->upperleft.second); 
    pair<unsigned int, unsigned int> ulB = make_pair(root->upperleft.first, root->upperleft.second); 
    root->A->upperleft = ulA;  
    root->B->upperleft = ulB; 
  }

  if (root->height > root->width) { 
    root->A->upperleft.first = root->upperleft.first; 
    root->B->upperleft.first = root->upperleft.first;  
  } 

  flipHorizontalNode(root->A);  
  flipHorizontalNode(root->B); 
 

//  if(root == NULL){
//   return;

// } else{
//   if(root->A && root->B){

//     if(root->A->width <= root->A->height && root->B->width < root->B->height){
//       pair<unsigned int, unsigned int> ul = root->A->upperleft;
//       root->A->upperleft = root->B->upperleft;
//       root->B->upperleft = ul;

//     }else if(root->A->width > root->A->height ){
//       root->A->upperleft = make_pair(root->A->upperleft.first + root->A->width, root->A->upperleft.second);
//       root->B->upperleft = make_pair(root->B->upperleft.first + root->B->width, root->B->upperleft.second);  
//     }
//   }
// }

//   flipHorizontalNode(root->A,width);
//   flipHorizontalNode(root->B,width);

}

int PTree::countWidth(Node *root)const{
return root->width;

}

int PTree::countHeight(Node *root)const{
  return root->height;

}

// // mirror over a horizontal axis
 void PTree::flipVerticalNode(Node *root) { //, int height) { 
if (root == NULL) { 
    return;  
  } 

if (root->A == NULL && root->B == NULL) { 
    return; 
}
  if (root->height > root->width) { 
    pair<unsigned int, unsigned int> ulA = make_pair(root->upperleft.first, root->upperleft.second + root->B->height); 
    pair<unsigned int, unsigned int> ulB = make_pair(root->upperleft.first, root->upperleft.second); 
    root->A->upperleft = ulA; 
    root->B->upperleft = ulB; 
  }

  if (root->width >= root->height) { 
    root->A->upperleft.second = root->upperleft.second; 
    root->B->upperleft.second = root->upperleft.second;  
  } 

  flipVerticalNode(root->A);  
  flipVerticalNode(root->B); 

}

void PTree::pruneNode(Node *root, double tolerance){
 if (root == NULL) { 
    return; 
  }

  if(root->A == NULL && root->B == NULL){
    return;
  }
  
   if (checkTolerance(root, tolerance, root->avg)){ 
      clear(root->A); 
      clear(root->B); 
      root->A = NULL; 
      root->B = NULL; 

  } else { 
    pruneNode(root->A, tolerance); 
    pruneNode(root->B, tolerance); 
  } 


}

bool PTree::checkTolerance(Node *root, double tolerance, HSLAPixel avg) { 
 if (root == NULL) { 
    return true; 
  } 

  if (root->A == NULL && root->B == NULL) { 
    if (avg.dist(root->avg) <= tolerance) { 
      return true; 
    } else { 
      return false; 
    }
  } else { 
    return checkTolerance(root->A, tolerance, avg) && checkTolerance(root->B, tolerance, avg); 
  }

}

 