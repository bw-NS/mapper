/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   node.h
 * Author: wubowen2
 *
 * Created on March 20, 2018, 4:29 PM
 */

#ifndef NODE_H
#define NODE_H

class node {
public:
    node();
    node(unsigned Int, unsigned seg, unsigned previous, double weight);
    virtual ~node();
    bool visited;
    unsigned intersectionID;
    unsigned from_segID;
    unsigned previous_node;
    double weight;
    bool operator==(const node& rhs);
    bool operator!=(const node& rhs);
};

#endif /* NODE_H */

