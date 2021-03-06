/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <set>

#include "m1.h"
#include "m3.h"
#include "StreetsDatabaseAPI.h"
#include <vector>
#include <algorithm>
#include "unordered_map"
#include "m3_function_declaration.h"
#include <queue>
#include "m1_function_declaration.h"
#include "node.h"
#include <ctime>// include this header

extern std::vector<std::vector<unsigned>> intersectionSeg;
extern std::unordered_map<std::string, std::vector<unsigned>> POIname;
extern std::vector<LatLon> positionPOI;
extern std::vector<LatLon> positionInt;
extern std::vector<StreetSegmentInfo> Seg_Info;

double compute_path_travel_time(const std::vector<unsigned>& path, 
                                const double turn_penalty){
    if(path.size()==0) return -1.0;
    else {
        double tot_time=0;
    
        for(unsigned i=0; i<path.size()-1; i++){
            unsigned Seg_index1=path[i];
            unsigned Seg_index2=path[i+1];
            double time=find_street_segment_travel_time(Seg_index1);
            
            tot_time+=time;
            StreetSegmentInfo temp1=getStreetSegmentInfo(Seg_index1);
            StreetSegmentInfo temp2=getStreetSegmentInfo(Seg_index2);
            if(temp1.streetID!=temp2.streetID) tot_time+=turn_penalty;
        }
        tot_time=tot_time+find_street_segment_travel_time(path[path.size()-1]);
        return tot_time;
    }
}




std::vector<unsigned> available_segs(unsigned intersection_id,const std::unordered_map<unsigned,node>& visited_list) {
    std::vector<unsigned> segs = intersectionSeg[intersection_id];
   
    std::vector<unsigned> result;
    
    //loop through the segments that connect to the intersection id
    for (unsigned i = 0; i < segs.size(); i++) {
        StreetSegmentInfo temp = Seg_Info[segs[i]];
        //insert the element if this segment is "from" this id
        if (temp.from == intersection_id) {
            if(visited_list.find(temp.to)==visited_list.end())
            result.push_back(segs[i]);
        //if intersection_id is not "from", check if this segment is one-way
        //if is one-way, then insert this element
        } else if (!temp.oneWay) {
            if(visited_list.find(temp.from)==visited_list.end())
            result.push_back(segs[i]); 
        }
    }

    

    return result;
}



std::vector<unsigned> find_path_between_intersections(const unsigned intersect_id_start, 
                                                      const unsigned intersect_id_end,
                                                      const double turn_penalty){
    if(intersect_id_start==intersect_id_end) return{};
    
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
    
    
    if(Queue.empty()){return{};}
        else current=((Queue.top()).second); 
    //prepare for the next loop 
      //int start_s=clock();  
    while(visited_list[current]!=foo){
        Queue.pop();
        if(Queue.empty()){return{};}
        else current=((Queue.top()).second); //start with first in queue 
    }
    //int stop_s=clock();
      // std::cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << std::endl;
    }
    
    
    if(current==intersect_id_end){
        //int start_s=clock();

        std::vector<unsigned> final_path;
        //trace back 
         
        final_path.push_back( node_list[intersect_id_end].from_segID);
        for(unsigned i=node_list[intersect_id_end].previous_node; i!=intersect_id_start; i=visited_list[i].previous_node){
            unsigned last_seg=visited_list[i].from_segID;
            final_path.push_back( last_seg);
        }
        std::reverse(final_path.begin(),final_path.end());
        //int stop_s=clock();
       //std::cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << std::endl;
        
        return final_path;
    }
    
    else return {};
    
    
}



std::unordered_map<unsigned, unsigned> find_cloest_int_to_POI(std::vector<unsigned>POI_name_vec){
    std::unordered_map<unsigned, unsigned> result;
    for(unsigned i=0; i<POI_name_vec.size();i++){
        unsigned POIindex=POI_name_vec[i];
        LatLon POI_position=positionPOI[POIindex];
        unsigned Int=find_closest_intersection(POI_position);
        //LatLon  Int_position=positionInt[Int];
        result.emplace(Int,1);
        
        
        
    }
    
    return result;




}




std::vector<unsigned> find_path_to_point_of_interest(const unsigned intersect_id_start, 
                                               const std::string point_of_interest_name,
                                               const double turn_penalty){
    std::vector<unsigned> result;
    std::priority_queue<std::pair<double, unsigned>, std::vector<std::pair<double, unsigned>>, std::greater<std::pair<double,unsigned>>> Queue; //   weight|intersection_index
    
    std::unordered_map<unsigned, node> visited_list;     //   key:intersection index
    std::unordered_map<unsigned, node> node_list;
    
    unsigned current=intersect_id_start;            //initialize
    unsigned unusedInt=getNumberOfIntersections();  
    unsigned unusedstr=getNumberOfStreetSegments();
    
    std::pair<double, unsigned> first(0, current);
    Queue.emplace(first); 
    
    node start(current,unusedstr,unusedInt,0);
    node_list.emplace(current, start);
              std::vector<unsigned> POI_name_vec=POIname[point_of_interest_name]; //getPOI index
    std::unordered_map<unsigned, unsigned> closest_int_to_POI=find_cloest_int_to_POI(POI_name_vec);
    
    if(closest_int_to_POI.find(intersect_id_start)!=closest_int_to_POI.end()) return {};
    
    
    while(closest_int_to_POI.find(current)==closest_int_to_POI.end()&&Queue.size()!=0){       
        
        
        double previous_weight=node_list[current].weight;
        
       // std::vector<unsigned> previous_node=(Queue.top()).second; //record before deleting
       
        node current_node=node_list[current];
        node_list.erase(current);
        visited_list.emplace(current, current_node);
        Queue.pop();
        //std::vector<unsigned> avail_int=available_intersections(current, visited_list);  //check the flag
        std::vector<unsigned> next_seg_vec=available_segs(current, visited_list);
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
            if(node_list.find(other_end)!=node_list.end()&&node_list[other_end].weight<=next_node.weight){}
            else {
                node_list.erase(other_end);node_list.emplace(other_end,next_node);
                std::pair<double, unsigned> weighted_node(weight, other_end);     //put new node to queue
                Queue.emplace(weighted_node);
            
            }
            
            
            
            
            
    
        }
    
    
    
    //prepare for the next loop 
        if(Queue.empty()){return{};}
        else current=((Queue.top()).second); //start with first in queue 
    while(visited_list.find(current)!=visited_list.end()){
        Queue.pop();
        if(Queue.empty()){return{};}
        else current=((Queue.top()).second); //start with first in queue 
    } 
    
    }
    
    if(closest_int_to_POI.find(current)!=closest_int_to_POI.end()){
        
        std::vector<unsigned> final_path;
        if (visited_list.size() <= 0){
            return final_path;
        }
        //trace back 
        final_path.push_back(node_list[current].from_segID);
        for(unsigned i=node_list[current].previous_node; i!=intersect_id_start; i=visited_list[i].previous_node){
            unsigned last_seg=visited_list[i].from_segID;
            final_path.push_back(last_seg);
        }
        std::reverse(final_path.begin(),final_path.end());
        return final_path;
    }
    else return {};
    
    
    
    
    
    
    
    
    
    
}


