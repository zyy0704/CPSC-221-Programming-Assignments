/*
*  File:        ptree-private.h
*  Description: Private functions for the PTree class
*  Date:        2022-03-06 03:30
*
*               DECLARE YOUR PTREE PRIVATE MEMBER FUNCTIONS HERE
*/

#ifndef _PTREE_PRIVATE_H_
#define _PTREE_PRIVATE_H_

/////////////////////////////////////////////////
// DEFINE YOUR PRIVATE MEMBER FUNCTIONS HERE
//
// Just write the function signatures.
//
// Example:
//
// Node* MyHelperFunction(int arg_a, bool arg_b);
//
/////////////////////////////////////////////////
HSLAPixel avgPixel(PNG &im, unsigned int x1,unsigned int x2,unsigned int y1, unsigned int y2);
Node* copy(Node *node);

void clear(Node *node);

int leafCount(Node *node) const;

int size(Node *node) const;

void render(PNG *png, Node *node) const;

void flipHorizontalNode(Node *node);

int countWidth(Node *node)const;

int countHeight(Node *node)const;

void flipVerticalNode(Node *root);

void pruneNode(Node *root, double tolerance);

bool checkTolerance(Node *root, double tolerance, HSLAPixel avg);

#endif