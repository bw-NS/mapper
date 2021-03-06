/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <thread>
#include "m4.h"
#include "intersection.h"
#include "unordered_map"
#include "StreetsDatabaseAPI.h"
#include "node.h"
#include <queue>
#include <algorithm>
#include "m1.h"
#include "m3_function_declaration.h"
#include "m3.h"
#include "node.h"
#include <ctime>
#define TIME_LIMIT 30

extern std::vector<StreetSegmentInfo> Seg_Info;
extern std::vector<std::vector<unsigned>> intersectionSeg;
extern std::vector<LatLon> positionInt;


double find_path_to_available_intersections(const unsigned intersect_id_start, 
                                               const std::vector<unsigned>& closest_int_to_POI,
                                               const double turn_penalty, unsigned & next_inter);

void find_weight_between_intersections(const unsigned intersect_id_start, 
                                                      const unsigned intersect_id_end,
                                                      const double turn_penalty, double& a, std::pair<unsigned, double>& b);
void greedy(unsigned chosen_depot, std::vector<unsigned>& pickup_list, std::vector<unsigned> depots, double turn_penalty
                        , double& finalweight,std::vector<std::pair<unsigned,double>>& weight);
std::vector<intersection> intersection_list;
std::vector<unsigned> traveling_courier(const std::vector<DeliveryInfo>& deliveries, 
                                        const std::vector<unsigned>& depots, 
                                        const float turn_penalty){
    
    //get all the intersections from the inputs
    clock_t starttime=clock();
    
    intersection_list.resize(getNumberOfIntersections());
    intersection foo;
    std::vector<unsigned> pickup_list; //record all pickups that are not drop off
    
    
    for(unsigned i=0; i<deliveries.size(); i++){
        unsigned pickup=deliveries[i].pickUp;
        unsigned dropoff=deliveries[i].dropOff;
        if(std::find(pickup_list.begin(),pickup_list.end(), pickup)==pickup_list.end())pickup_list.push_back(pickup);
        if(intersection_list[pickup]!=foo){
                                    
                intersection temp=intersection_list[pickup];
                temp.pickup=1;
                temp.Codropoff.push_back(dropoff);
                intersection_list[pickup]=temp;
                
        }
        else {
            
            intersection temp(1,0, {dropoff}, {});
            intersection_list[pickup]=temp;           
            
        }
        
        if(intersection_list[dropoff]!=foo){
                                    
                intersection temp=intersection_list[dropoff];
                temp.dropoff=1;
                temp.Copickup.push_back(pickup);
                intersection_list[dropoff]=temp;
                
        }
        else {
            
            intersection temp(0,1, {}, {pickup});
            intersection_list[dropoff]=temp;           
            
        }                       
    }
    unsigned erased=0;
    for(unsigned i=0; i<pickup_list.size();i++){
        
        if(intersection_list[pickup_list[i-erased]].dropoff){
            pickup_list.erase(pickup_list.begin()+i-erased);
            erased++;
        }
        
    }
    
    
    //start of greedy 
    
    //sort the depot closest to one 
    
    //choose a depot
    
 
    
    std::priority_queue<std::pair<double, unsigned>, std::vector<std::pair<double, unsigned>>, std::greater<std::pair<double,unsigned>>> depot_list;
    unsigned next;  
    for(unsigned i=0; i<depots.size(); i++){
        
        double temp=find_path_to_available_intersections(depots[i],  pickup_list, turn_penalty,  next);
        if(temp==-1){}
        else depot_list.emplace(temp, depots[i]);
        
    }
    
    unsigned chosen_depot = depot_list.top().second;  //will be change later
    //find the depot closest to pickup with most drop off
    std::vector<std::pair<unsigned,double>> weight;
    if(depot_list.size()==1){
    
    
    std::vector<unsigned> available_list=pickup_list;
    //chose the closest available intersection to the depot
    unsigned next_inter;
    unsigned current=chosen_depot;
    
    std::pair<unsigned, double > start(chosen_depot, 0);
    weight.push_back(start);
    while(available_list.size()!=0){
    
        double temp=find_path_to_available_intersections(current,  available_list, turn_penalty,  next_inter);

        if(temp==-1) return {};
        
        current=next_inter;
        available_list.erase(std::find(available_list.begin(), available_list.end(), current));
        
        std::vector<unsigned> drops=intersection_list[current].Codropoff;
        for(unsigned i=0; i<drops.size();i++){
            
            if(intersection_list[drops[i]].Copickup.size()==1) available_list.push_back(drops[i]);
            else {
                 
                 intersection itsc=intersection_list[drops[i]];
                 itsc.Copickup.pop_back();
                 intersection_list[drops[i]]=itsc;
                 
            }
                
        }

        std::pair<unsigned, double> nod(current, temp);
        weight.push_back(nod);

    }
    double temp=find_path_to_available_intersections(weight[weight.size()-1].first,  depots, turn_penalty,  next_inter);
    std::pair<unsigned, double> nod(next_inter, temp);
    weight.push_back(nod);
    }
    else if (depot_list.size()==2){
        
        std::thread th1, th2;
        double finalweight1, finalweight2;
        std::vector<std::pair<unsigned, double>>weight1, weight2;
        unsigned chosen_depot1=chosen_depot;
        depot_list.pop();
        unsigned chosen_depot2=chosen_depot = depot_list.top().second;
        th1=std::thread(greedy, chosen_depot1, std::ref(pickup_list), std::ref(depots), turn_penalty, std::ref(finalweight1), std::ref(weight1));
        th2=std::thread(greedy, chosen_depot2, std::ref(pickup_list), std::ref(depots), turn_penalty, std::ref(finalweight2), std::ref(weight2));
        th1.join();
        th2.join();
        
        if(finalweight1<finalweight2) weight=weight1;
        else weight=weight2;
        
    }
    else if (depot_list.size()>=3){
        
        std::thread th1, th2, th3;
        double finalweight1, finalweight2,finalweight3;
        std::vector<std::pair<unsigned, double>>weight1, weight2, weight3;
        unsigned chosen_depot1=chosen_depot;
        depot_list.pop();
        unsigned chosen_depot2=depot_list.top().second;
        depot_list.pop();
        unsigned chosen_depot3=depot_list.top().second;
        th1=std::thread(greedy, chosen_depot1, std::ref(pickup_list), std::ref(depots), turn_penalty, std::ref(finalweight1), std::ref(weight1));
        th2=std::thread(greedy, chosen_depot2, std::ref(pickup_list), std::ref(depots), turn_penalty, std::ref(finalweight2), std::ref(weight2));
        th3=std::thread(greedy, chosen_depot3, std::ref(pickup_list), std::ref(depots), turn_penalty, std::ref(finalweight3), std::ref(weight3));
        th1.join();
        th2.join();
        th3.join();
        if(finalweight1<finalweight2&&finalweight1<finalweight3) weight=weight1;
        else if(finalweight2<finalweight3)weight=weight2;
        else weight=weight3;
        
        
        
    }
    
    //end of greedy
    
    
    // start of 2opt
    
    bool changed=1;
    unsigned test=0;
    //while(test!=2){
        changed=0;
    for(unsigned i=1; i<weight.size()-2;i++){
        for(unsigned k=i+1; k<=i+1/*weight.size()-1*/; k++){
            //if two path intersect do following? 
            bool changable=1;
            
            
            
            
            //reverse the middle part, check if is reversable        
            
            for(unsigned j=i; j<k&&changable; j++){
                
                std::vector<unsigned>drops;
                
                if(intersection_list[weight[j].first].pickup) {                       
                        drops=intersection_list[weight[j].first].Codropoff;
                
                  
                for(unsigned q=j+1; q<=k; q++){                    
                    
                        if (!intersection_list[weight[q].first].dropoff) {}                       
                        else if(std::find(drops.begin(),drops.end(), weight[q].first)!=drops.end()) {
                            changable=0;
                            break;
                        
                    }                    
                }
            }
            }
            
            
            if(changable) {
                
                double old_weight=0;
                for(unsigned z=i; z<=k+1; z++)                   
                    old_weight+=weight[z].second;
                
                
                //use multi thread
                double new_weight=0;
                std::thread th1, th2, th3;
                
                std::pair<unsigned, double> tmp_pair1, tmp_pair2,tmp_pair3;
                double weight1, weight2, weight3;
                th1=std::thread(find_weight_between_intersections, weight[i-1].first ,weight[k].first,turn_penalty, std::ref(weight1), std::ref(tmp_pair1));
                th2=std::thread(find_weight_between_intersections, weight[k].first ,weight[i].first,turn_penalty, std::ref(weight2), std::ref(tmp_pair2));
                th3=std::thread(find_weight_between_intersections, weight[i].first ,weight[k+1].first,turn_penalty, std::ref(weight3), std::ref(tmp_pair3));
                th1.join();
                th2.join();
                th3.join();
                new_weight= weight1+weight2+weight3;
                
                
                
                
                    
              
                //i to k+1 
                //function to calculate new_weight (i-1, k) (reverse) (i,k+1)
                
                
                if(new_weight<old_weight){
                    changed=1;
                    
                     weight[i]=tmp_pair1;
                    weight[i+1]=tmp_pair2;
                    weight[i+2]=tmp_pair3;
                    
                    
                }
   
            }
            
            
        }
    }
        test++;
    //}
    
    //end of 2-opt
    //then try greedy on next depot 
       // depot_list.pop(); 
        //chosen_depot=depot_list.top().second;
        
        
        
         for(unsigned i=1; i<weight.size()-3;i++){
        for(unsigned k=i+2; k<=i+2/*weight.size()-1*/; k++){
            //if two path intersect do following? 
            bool changable=1;
            
            
            
            
            //reverse the middle part, check if is reversable        
            
            for(unsigned j=i; j<k&&changable; j++){
                
                std::vector<unsigned>drops;
                
                if(intersection_list[weight[j].first].pickup) {                       
                        drops=intersection_list[weight[j].first].Codropoff;
                
                  
                for(unsigned q=j+1; q<=k; q++){                    
                    
                        if (!intersection_list[weight[q].first].dropoff) {}                       
                        else if(std::find(drops.begin(),drops.end(), weight[q].first)!=drops.end()) {
                            changable=0;
                            break;
                        
                    }                    
                }
            }
            }
            
            
            if(changable) {
                
                double old_weight=0;
                for(unsigned z=i; z<=k+1; z++)                   
                    old_weight+=weight[z].second;
                
                double new_weight=0;
                std::thread th1, th2, th3, th4;
                
                double weight1, weight2, weight3, weight4;
                std::pair<unsigned, double> tmp_pair1, tmp_pair2,tmp_pair3, tmp_pair4;
                th1=std::thread(find_weight_between_intersections, weight[i-1].first ,weight[i+2].first,turn_penalty, std::ref(weight1), std::ref(tmp_pair1));
                th2=std::thread(find_weight_between_intersections, weight[i+2].first ,weight[i+1].first,turn_penalty, std::ref(weight2), std::ref(tmp_pair2));
                th3=std::thread(find_weight_between_intersections, weight[i+1].first ,weight[i].first,turn_penalty, std::ref(weight3), std::ref(tmp_pair3));
                th4=std::thread(find_weight_between_intersections, weight[i].first ,weight[i+3].first,turn_penalty, std::ref(weight4), std::ref(tmp_pair4));
                th1.join();
                th2.join();
                th3.join();
                th4.join();
                
                
                new_weight= weight1+weight2+weight3+weight4;
                
                
                
                
                //i to k+1 
                //function to calculate new_weight (i-1, k) (reverse) (i,k+1)
                
                
                if(new_weight<old_weight){
                    changed=1;
                    weight[i]=tmp_pair1;
                    weight[i+1]=tmp_pair2;
                    weight[i+2]=tmp_pair3;
                    weight[i+3]=tmp_pair4;
                    
                    
                    
                }
   
            }
            
            
        }
    }
        
        for(unsigned i=1; i<weight.size()-4;i++){
        for(unsigned k=i+3; k<=i+3/*weight.size()-1*/; k++){
            //if two path intersect do following? 
            bool changable=1;
            
            
            
            
            //reverse the middle part, check if is reversable        
            
            for(unsigned j=i; j<k&&changable; j++){
                
                std::vector<unsigned>drops;
                
                if(intersection_list[weight[j].first].pickup) {                       
                        drops=intersection_list[weight[j].first].Codropoff;
                
                  
                for(unsigned q=j+1; q<=k; q++){                    
                    
                        if (!intersection_list[weight[q].first].dropoff) {}                       
                        else if(std::find(drops.begin(),drops.end(), weight[q].first)!=drops.end()) {
                            changable=0;
                            break;
                        
                    }                    
                }
            }
            }
            
            
            if(changable) {
                
                double old_weight=0;
                for(unsigned z=i; z<=k+1; z++)                   
                    old_weight+=weight[z].second;
                
                double new_weight=0;
                std::thread th1, th2, th3, th4, th5;
                
                double weight1, weight2, weight3, weight4, weight5;
                std::pair<unsigned, double> tmp_pair1, tmp_pair2,tmp_pair3, tmp_pair4, tmp_pair5;
                th1=std::thread(find_weight_between_intersections, weight[i-1].first ,weight[i+3].first,turn_penalty, std::ref(weight1), std::ref(tmp_pair1));
                th2=std::thread(find_weight_between_intersections, weight[i+3].first ,weight[i+2].first,turn_penalty, std::ref(weight2), std::ref(tmp_pair2));
                th3=std::thread(find_weight_between_intersections, weight[i+2].first ,weight[i+1].first,turn_penalty, std::ref(weight3), std::ref(tmp_pair3));
                th4=std::thread(find_weight_between_intersections, weight[i+1].first ,weight[i].first,turn_penalty, std::ref(weight4), std::ref(tmp_pair4));
                th5=std::thread(find_weight_between_intersections, weight[i].first ,weight[i+4].first,turn_penalty, std::ref(weight5), std::ref(tmp_pair5));
                th1.join();
                th2.join();
                th3.join();
                th4.join();
                th5.join();
                
                
                new_weight= weight1+weight2+weight3+weight4+weight5;
                
                
                
                
                //i to k+1 
                //function to calculate new_weight (i-1, k) (reverse) (i,k+1)
                
                
                if(new_weight<old_weight){
                    changed=1;
                    weight[i]=tmp_pair1;
                    weight[i+1]=tmp_pair2;
                    weight[i+2]=tmp_pair3;
                    weight[i+3]=tmp_pair4;
                    weight[i+4]=tmp_pair5;
                    
                    
                    
                }
   
            }
            
            
        }
    }
        
        clock_t currenttime=clock();
        if((double)(currenttime-starttime)/1000<0.78*TIME_LIMIT){
        for(unsigned i=1; i<weight.size()-2;i++){
        for(unsigned k=i+1; k<=i+1/*weight.size()-1*/; k++){
            //if two path intersect do following? 
            bool changable=1;
            
            
            
            
            //reverse the middle part, check if is reversable        
            
            for(unsigned j=i; j<k&&changable; j++){
                
                std::vector<unsigned>drops;
                
                if(intersection_list[weight[j].first].pickup) {                       
                        drops=intersection_list[weight[j].first].Codropoff;
                
                  
                for(unsigned q=j+1; q<=k; q++){                    
                    
                        if (!intersection_list[weight[q].first].dropoff) {}                       
                        else if(std::find(drops.begin(),drops.end(), weight[q].first)!=drops.end()) {
                            changable=0;
                            break;
                        
                    }                    
                }
            }
            }
            
            
            if(changable) {
                
                double old_weight=0;
                for(unsigned z=i; z<=k+1; z++)                   
                    old_weight+=weight[z].second;
                
                
                //use multi thread
                double new_weight=0;
                std::thread th1, th2, th3;
                
                std::pair<unsigned, double> tmp_pair1, tmp_pair2,tmp_pair3;
                double weight1, weight2, weight3;
                th1=std::thread(find_weight_between_intersections, weight[i-1].first ,weight[k].first,turn_penalty, std::ref(weight1), std::ref(tmp_pair1));
                th2=std::thread(find_weight_between_intersections, weight[k].first ,weight[i].first,turn_penalty, std::ref(weight2), std::ref(tmp_pair2));
                th3=std::thread(find_weight_between_intersections, weight[i].first ,weight[k+1].first,turn_penalty, std::ref(weight3), std::ref(tmp_pair3));
                th1.join();
                th2.join();
                th3.join();
                new_weight= weight1+weight2+weight3;
                
                
                
                
                
                    
              
                //i to k+1 
                //function to calculate new_weight (i-1, k) (reverse) (i,k+1)
                
                
                if(new_weight<old_weight){
                    changed=1;
                    weight[i]=tmp_pair1;
                    weight[i+1]=tmp_pair2;
                    weight[i+2]=tmp_pair3;
                    
                }
   
            }
            
            
        }
    }   
        
        
        }
        currenttime=clock();
        if((double)(currenttime-starttime)/1000<0.78*TIME_LIMIT){
         for(unsigned i=1; i<weight.size()-3;i++){
        for(unsigned k=i+2; k<=i+2/*weight.size()-1*/; k++){
            //if two path intersect do following? 
            bool changable=1;
            
            
            
            
            //reverse the middle part, check if is reversable        
            
            for(unsigned j=i; j<k&&changable; j++){
                
                std::vector<unsigned>drops;
                
                if(intersection_list[weight[j].first].pickup) {                       
                        drops=intersection_list[weight[j].first].Codropoff;
                
                  
                for(unsigned q=j+1; q<=k; q++){                    
                    
                        if (!intersection_list[weight[q].first].dropoff) {}                       
                        else if(std::find(drops.begin(),drops.end(), weight[q].first)!=drops.end()) {
                            changable=0;
                            break;
                        
                    }                    
                }
            }
            }
            
            
            if(changable) {
                
                double old_weight=0;
                for(unsigned z=i; z<=k+1; z++)                   
                    old_weight+=weight[z].second;
                
                double new_weight=0;
                std::thread th1, th2, th3, th4;
                
                double weight1, weight2, weight3, weight4;
                std::pair<unsigned, double> tmp_pair1, tmp_pair2,tmp_pair3, tmp_pair4;
                th1=std::thread(find_weight_between_intersections, weight[i-1].first ,weight[i+2].first,turn_penalty, std::ref(weight1), std::ref(tmp_pair1));
                th2=std::thread(find_weight_between_intersections, weight[i+2].first ,weight[i+1].first,turn_penalty, std::ref(weight2), std::ref(tmp_pair2));
                th3=std::thread(find_weight_between_intersections, weight[i+1].first ,weight[i].first,turn_penalty, std::ref(weight3), std::ref(tmp_pair3));
                th4=std::thread(find_weight_between_intersections, weight[i].first ,weight[i+3].first,turn_penalty, std::ref(weight4), std::ref(tmp_pair4));
                th1.join();
                th2.join();
                th3.join();
                th4.join();
                
                
                new_weight= weight1+weight2+weight3+weight4;
                
                
                
                
                //i to k+1 
                //function to calculate new_weight (i-1, k) (reverse) (i,k+1)
                
                
                if(new_weight<old_weight){
                    changed=1;
                    weight[i]=tmp_pair1;
                    weight[i+1]=tmp_pair2;
                    weight[i+2]=tmp_pair3;
                    weight[i+3]=tmp_pair4;
                    
                    
                    
                }
   
            }
            
            
        }
    }
        
        
        }
        
        currenttime=clock();
        if((double)(currenttime-starttime)/1000<0.78*TIME_LIMIT){
        for(unsigned i=1; i<weight.size()-2;i++){
        for(unsigned k=i+1; k<=i+1/*weight.size()-1*/; k++){
            //if two path intersect do following? 
            bool changable=1;
            
            
            
            
            //reverse the middle part, check if is reversable        
            
            for(unsigned j=i; j<k&&changable; j++){
                
                std::vector<unsigned>drops;
                
                if(intersection_list[weight[j].first].pickup) {                       
                        drops=intersection_list[weight[j].first].Codropoff;
                
                  
                for(unsigned q=j+1; q<=k; q++){                    
                    
                        if (!intersection_list[weight[q].first].dropoff) {}                       
                        else if(std::find(drops.begin(),drops.end(), weight[q].first)!=drops.end()) {
                            changable=0;
                            break;
                        
                    }                    
                }
            }
            }
            
            
            if(changable) {
                
                double old_weight=0;
                for(unsigned z=i; z<=k+1; z++)                   
                    old_weight+=weight[z].second;
                
                
                //use multi thread
                double new_weight=0;
                std::thread th1, th2, th3;
                
                std::pair<unsigned, double> tmp_pair1, tmp_pair2,tmp_pair3;
                double weight1, weight2, weight3;
                th1=std::thread(find_weight_between_intersections, weight[i-1].first ,weight[k].first,turn_penalty, std::ref(weight1), std::ref(tmp_pair1));
                th2=std::thread(find_weight_between_intersections, weight[k].first ,weight[i].first,turn_penalty, std::ref(weight2), std::ref(tmp_pair2));
                th3=std::thread(find_weight_between_intersections, weight[i].first ,weight[k+1].first,turn_penalty, std::ref(weight3), std::ref(tmp_pair3));
                th1.join();
                th2.join();
                th3.join();
                new_weight= weight1+weight2+weight3;
                
                
                
                
                
                    
              
                //i to k+1 
                //function to calculate new_weight (i-1, k) (reverse) (i,k+1)
                
                
                if(new_weight<old_weight){
                    changed=1;
                    weight[i]=tmp_pair1;
                    weight[i+1]=tmp_pair2;
                    weight[i+2]=tmp_pair3;
                    
                }
   
            }
            
            
        }
    }   
        
        
        }
        
    //final path
    
    std::vector<unsigned> final_path;
    for(unsigned i=0; i<weight.size()-1;i++){
        
        std::vector<unsigned> a=find_path_between_intersections(weight[i].first,  weight[i+1].first, turn_penalty); 
        final_path.insert(final_path.end(), a.begin(), a.end());
        
        
    }
    return final_path;
    
}

void find_weight_between_intersections(const unsigned intersect_id_start, 
                                                      const unsigned intersect_id_end,
                                                      const double turn_penalty, double& a, std::pair<unsigned, double>& b){
    if(intersect_id_start==intersect_id_end) {a=0; std::pair<unsigned, double> tmp(intersect_id_end, a);b=tmp; return ;}
    
    std::vector<unsigned> result;
    std::priority_queue<std::pair<double, unsigned>, std::vector<std::pair<double, unsigned>>, std::greater<std::pair<double,unsigned>>> Queue; //   weight|intersection_index
    std::vector<node> visited_list;
    std::vector<node> node_list;
    unsigned unusedInt=getNumberOfIntersections();
    visited_list.resize(unusedInt);
    node_list.resize(unusedInt);
    node foo;
    
    unsigned current=intersect_id_start;            //initialize
    
    unsigned unusedstr=getNumberOfStreetSegments();
    
    std::pair<double, unsigned> first(0, current);
    Queue.emplace(first); 
    
    node start(current,unusedstr,unusedInt,0);
    node_list[current]= start;
           
  
    while(current!=intersect_id_end&&Queue.size()!=0){       
        
        
        double previous_weight=node_list[current].weight;
        
       
       
        node current_node=node_list[current];
        node_list[current]=foo;
        visited_list[current]= current_node;
        Queue.pop();
        //std::vector<unsigned> avail_int=available_intersections(current, visited_list);  //check the flag
        std::vector<unsigned> segs = intersectionSeg[current];
   
        std::vector<unsigned> next_seg_vec;
    
    //loop through the segments that connect to the intersection id
        for (unsigned i = 0; i < segs.size(); i++) {
            StreetSegmentInfo temp = Seg_Info[segs[i]];
        //insert the element if this segment is "from" this id
        if (temp.from == current) {
            if(visited_list[temp.to]==foo)
            next_seg_vec.push_back(segs[i]);
        //if intersection_id is not "from", check if this segment is one-way
        //if is one-way, then insert this element
        } else if (!temp.oneWay) {
            if(visited_list[temp.from]==foo)
            next_seg_vec.push_back(segs[i]); 
        }
    }
        
        
        unsigned   num_Seg = next_seg_vec.size();   //need to get the available segs form previous line
        //go through all the segs of the current intersection; insert neighbours in queue
        
        for(unsigned i=0; i<num_Seg; i++){
            
            unsigned seg_index=next_seg_vec[i]; 
            unsigned other_end;
            if(Seg_Info[seg_index].from==current) other_end=Seg_Info[seg_index].to;
            else other_end=Seg_Info[seg_index].from;
            double weight=find_street_segment_travel_time(seg_index);  //travel time of seg
            weight=weight+previous_weight;                              //plus the previous
            StreetSegmentInfo info1=Seg_Info[seg_index];
            unsigned n_street_id=info1.streetID;
            
           
            
            if(current_node.from_segID!=unusedstr){
                
                if(Seg_Info[current_node.from_segID].streetID!=n_street_id) weight=weight+turn_penalty;          //add turn penalty
                
            }
            
            
            node next_node(other_end,seg_index,current,weight);
            if(node_list[other_end]!=foo&&node_list[other_end].weight<next_node.weight){}
            else {node_list[other_end]=next_node;double distance=find_distance_between_two_point(positionInt[intersect_id_end],positionInt[other_end] );
            
            weight=weight+10*distance;
            
            
            //std::pair<double, unsigned> weighted_node(weight, other_end);     //put new node to queue
            Queue.emplace(weight, other_end);
            }
            
            
            
    
        }
    
    
    if(Queue.empty()){a=-1; return ;}
        else current=((Queue.top()).second); 
    //prepare for the next loop 
      //int start_s=clock();  
    while(visited_list[current]!=foo){
        Queue.pop();
        if(Queue.empty()){a=-1; return ;}
        else current=((Queue.top()).second); //start with first in queue 
    }
    //int stop_s=clock();
      // std::cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << std::endl;
    }
    
    
    if(current==intersect_id_end){
        //int start_s=clock();

        
        //trace back 
         
         a=node_list[intersect_id_end].weight;
         std::pair<unsigned, double> tmp(current, a);
         b=tmp;
                 
       return;
        
    }
    
    else {a=-1; return ;}
    
    
}


double find_path_to_available_intersections(const unsigned intersect_id_start, 
                                               const std::vector<unsigned>& closest_int_to_POI,
                                               const double turn_penalty, unsigned & next_inter){
    std::vector<unsigned> result;
    std::priority_queue<std::pair<double, unsigned>, std::vector<std::pair<double, unsigned>>, std::greater<std::pair<double,unsigned>>> Queue; //   weight|intersection_index
    
    std::vector<node> visited_list;     //   key:intersection index
    std::vector<node> node_list;
    
    unsigned current=intersect_id_start;            //initialize
    unsigned unusedInt=getNumberOfIntersections();  
    unsigned unusedstr=getNumberOfStreetSegments();
    visited_list.resize(unusedInt);
    node_list.resize(unusedInt);
    node foo;
    
    std::pair<double, unsigned> first(0, current);
    Queue.emplace(first); 
    
    node start(current,unusedstr,unusedInt,0);
    node_list[current]=start;
             
    
    
    if(std::find(closest_int_to_POI.begin(), closest_int_to_POI.end(), intersect_id_start)!=closest_int_to_POI.end()) return 0;
    
    
    while(std::find(closest_int_to_POI.begin(), closest_int_to_POI.end(), current)==closest_int_to_POI.end()&&Queue.size()!=0){       
        
        
        double previous_weight=node_list[current].weight;
        
       // std::vector<unsigned> previous_node=(Queue.top()).second; //record before deleting
       
        node current_node=node_list[current];
        node_list[current]=foo;
        visited_list[current]=current_node;
        Queue.pop();
        std::vector<unsigned> segs = intersectionSeg[current];
   
        std::vector<unsigned> next_seg_vec;
    
    //loop through the segments that connect to the intersection id
        for (unsigned i = 0; i < segs.size(); i++) {
            StreetSegmentInfo temp = Seg_Info[segs[i]];
        //insert the element if this segment is "from" this id
        if (temp.from == current) {
            if(visited_list[temp.to]==foo)
            next_seg_vec.push_back(segs[i]);
        //if intersection_id is not "from", check if this segment is one-way
        //if is one-way, then insert this element
        } else if (!temp.oneWay) {
            if(visited_list[temp.from]==foo)
            next_seg_vec.push_back(segs[i]); 
        }
    }
        
        
        unsigned   num_Seg = next_seg_vec.size();   //need to get the available segs form previous line
        //go through all the segs of the current intersection; insert neighbours in queue
        
        for(unsigned i=0; i<num_Seg; i++){
    
            unsigned seg_index=next_seg_vec[i]; 
            unsigned other_end;
            if(Seg_Info[seg_index].from==current) other_end=Seg_Info[seg_index].to;
            else other_end=Seg_Info[seg_index].from;
            double weight=find_street_segment_travel_time(seg_index);  //travel time of seg
            weight=weight+previous_weight;                              //plus the previous
            StreetSegmentInfo info1=Seg_Info[seg_index];
            unsigned n_street_id=info1.streetID;
            
           
            
            if(current_node.from_segID!=unusedstr){
                
                if(Seg_Info[current_node.from_segID].streetID!=n_street_id) weight=weight+turn_penalty;          //add turn penalty
                
            }
            
            
            node next_node(other_end,seg_index,current,weight);
            if(node_list[other_end]!=foo&&node_list[other_end].weight<next_node.weight){}
            else {node_list[other_end]=next_node;
           
           
            
            
            //std::pair<double, unsigned> weighted_node(weight, other_end);     //put new node to queue
            Queue.emplace(weight, other_end);
            }
            
            
            
            
            
            
            
            
            
            
            
            
    
        }
    
    
    
    //prepare for the next loop 
        if(Queue.empty()){return -1;}
        else current=((Queue.top()).second); //start with first in queue 
    while(visited_list[current]!=foo){
        Queue.pop();
        if(Queue.empty()){return -1;}
        else current=((Queue.top()).second); //start with first in queue 
    } 
    
    }
    
    if(std::find(closest_int_to_POI.begin(), closest_int_to_POI.end(), current)!=closest_int_to_POI.end()){
        
        
        if (visited_list.size() <= 0){
            return 0;
        }
        //trace back 
        next_inter=current;
        return node_list[current].weight;
        
    }
    else return -1;
}


void greedy(unsigned chosen_depot, std::vector<unsigned>& pickup_list, std::vector<unsigned> depots, double turn_penalty
                        , double& finalweight,std::vector<std::pair<unsigned,double>>& weight){
    
    
     std::vector<unsigned> available_list=pickup_list;
    //chose the closest available intersection to the depot
    unsigned next_inter;
    unsigned current=chosen_depot;
    finalweight=0;
    std::pair<unsigned, double > start(chosen_depot, 0);
    std::vector<unsigned>dropoff_list(getNumberOfIntersections(),0);
    weight.push_back(start);
    
    while(available_list.size()!=0){
    
        double temp=find_path_to_available_intersections(current,  available_list, turn_penalty,  next_inter);

        if(temp==-1) return ;
        
        current=next_inter;
        available_list.erase(std::find(available_list.begin(), available_list.end(), current));
        
        std::vector<unsigned> drops=intersection_list[current].Codropoff;
        for(unsigned i=0; i<drops.size();i++){
            
            if(intersection_list[drops[i]].Copickup.size()==1||dropoff_list[drops[i]]==1) available_list.push_back(drops[i]);
            else {
                if(dropoff_list[drops[i]]==0) 
                dropoff_list[drops[i]]=intersection_list[drops[i]].Copickup.size()-1;
                else dropoff_list[drops[i]]=dropoff_list[drops[i]]-1;
                 
            }
                
        }
        finalweight+=temp;
        std::pair<unsigned, double> nod(current, temp);
        weight.push_back(nod);

    }
    
    double temp=find_path_to_available_intersections(weight[weight.size()-1].first,  depots, turn_penalty,  next_inter);
    std::pair<unsigned, double> nod(next_inter, temp);
    weight.push_back(nod);
    finalweight+=temp;
    return ;
}