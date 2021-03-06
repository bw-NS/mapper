/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m4_mq.h
 * Author: mengqi
 *
 * Created on April 3, 2018, 11:01 PM
 */

#ifndef M4_MQ_H
#define M4_MQ_H

#pragma once
#include <vector>

struct DeliveryInfo {
    //Specifies a delivery order.
    //
    //To satisfy the order the item-to-be-delivered must have been picked-up
    //from the pickUp intersection before visiting the dropOff intersection.

    DeliveryInfo(unsigned pick_up, unsigned drop_off)
    : pickUp(pick_up), dropOff(drop_off) {
    }


    //The intersection id where the item-to-be-delivered is picked-up.
    unsigned pickUp;

    //The intersection id where the item-to-be-delivered is dropped-off.
    unsigned dropOff;
};


// This routine takes in a vector of N deliveries (pickUp, dropOff
// intersection pairs), another vector of M intersections that
// are legal start and end points for the path (depots) and a turn
// penalty in seconds (see m3.h for details on turn penalties).
//
// The first vector ’deliveries’ gives the delivery information: a set of
// pickUp/dropOff pairs of intersection ids which specify the
// deliveries to be made. A delivery can only be dropped-off after
// the associated item has been picked-up.
//
// The second vector ’depots’ gives the intersection
// ids of courier company depots containing trucks; you start at any
// one of these depots and end at any one of the depots.
//
// This routine returns a vector of street segment ids that form a
// path, where the first street segment id is connected to a depot
// intersection, and the last street segment id also connects to a
// depot intersection. The path must traverse all the delivery
// intersections in an order that allows all deliveries to be made --
// i.e. a package won’t be dropped off if you haven’t picked it up
// yet.
//
// You can assume that N is always at least one, and M is always at
// least one (i.e. both input vectors are non-empty).
//
// It is legal for the same intersection to appear multiple times in
// the pickUp or dropOff list (e.g. you might have two deliveries with
// a pickUp intersection id of #50). The same intersection can also
// appear as both a pickUp location and a dropOff location.
//
// If you have two pickUps to make at an intersection,
// traversing the intersection once is sufficient
// to pick up both packages, and similarly one traversal of an
// intersection is sufficient to drop off all the (already picked up)
// packages that need to be dropped off at that intersection.
//
// Depots will never appear as pickUp or dropOff locations for deliveries.
//
// If no path connecting all the delivery locations
// and a start and end depot exists, this routine must return an
// empty (size == 0) vector.
std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries,
        const std::vector<unsigned>& depots,
        const float turn_penalty);


#endif /* M4_MQ_H */

