/* 
 * Copyright 2018 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "unordered_map"
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iostream>
#include "graphics.h"
#include "m1_function_declaration.h"

#include <set>

//global variables

//structure to store street segments travel time
struct segLen {
    double time;
};

//vector of segments structure
std::vector<segLen> segLength;
//hash table for streets: use name as key, vector(store street id) as data
std::unordered_map<std::string, std::vector<unsigned>> streetHT;
//vector for intersections: use index match intersection id, and storing vector of segments id connect to the intersection
std::vector<std::vector<unsigned>> intersectionSeg;
//vector to store vector of seg id connect to each road, index=street id
std::vector<std::vector<unsigned>> streetSeg;
//vector to store position of intersections
std::vector<LatLon> positionInt;
//vector to store position of interests
std::vector<LatLon> positionPOI;
//vector to store endpoints of street
std::vector<std::unordered_set<unsigned>> streetInt;
double cosine(double x);
//
std::unordered_map<std::string, std::vector<unsigned>> POIname;

std::vector<std::pair<long double, unsigned>> area_of_index;
std::vector<unsigned> openfeatures;
std::vector<FeatureType> feature_type;
std::vector<StreetSegmentInfo> Seg_Info;

std::vector<unsigned> avail_seg_to_itsc; 

//double AvgLat1=0;


//load map

bool Feature_close1(unsigned FeatureID) {
    bool close = false;
    LatLon Start, End;
    int number_of_point = getFeaturePointCount(FeatureID);
    
    Start = getFeaturePoint(FeatureID, 0);
    End = getFeaturePoint(FeatureID, number_of_point - 1);
    
    if (Start.lat() == End.lat() && Start.lon() == End.lon()) close = true;
    return close;
}
//calculate area of closed feature
long double area_of_feature(unsigned feature_index);

long double area_of_feature(unsigned feature_index){
    unsigned    num_of_point=getFeaturePointCount(feature_index);
    long double area=0.0;
    double LatAvg=0;
    for(unsigned i=0; i<num_of_point-1;i++){
        LatLon temp=getFeaturePoint(feature_index, i);
        LatAvg+=temp.lat();
    }
    LatAvg=LatAvg/num_of_point;
    for(unsigned i=0; i<num_of_point-1; i++){
        long double x1,y1,x2,y2, plus;
            LatLon temp1=getFeaturePoint(feature_index, i);
            LatLon temp2=getFeaturePoint(feature_index, i+1);
            x1=temp1.lon()* cos(LatAvg*DEG_TO_RAD);
            y1=temp1.lat(); 
            x2=temp2.lon()* cos(LatAvg*DEG_TO_RAD);
            y2=temp2.lat();
            plus=(x1*y2-x2*y1);
            area=area+plus;
    }
    return abs(area/2.0);
}

//loading the data in structure
bool load_map(std::string map_path) {
    bool load_successful = false; //Indicates whether the map has loaded 
    //successfully
    load_successful = loadStreetsDatabaseBIN(map_path);

    if (!load_successful) return false;
    else    {
        //resize all the structures
        streetSeg.resize(getNumberOfStreets());
        intersectionSeg.resize(getNumberOfIntersections());
        positionInt.resize(getNumberOfIntersections());
        streetInt.resize(getNumberOfStreets());
        positionPOI.resize(getNumberOfPointsOfInterest());
        Seg_Info.resize(getNumberOfStreetSegments());
        
        for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {
            unsigned from, to, streetID;
            StreetSegmentInfo temp=getStreetSegmentInfo(i);
            from = temp.from;
            to = temp.to;
            streetID = temp.streetID;

            //adding data into structure
            intersectionSeg[from].push_back(i);
            intersectionSeg[to].push_back(i);
            streetSeg[streetID].push_back(i);
            segLength.push_back(segLen());
            segLength[i].time = find_street_segment_length(i) / getStreetSegmentInfo(i).speedLimit * 3.6;
            
            streetInt[streetID].insert(from);
            streetInt[streetID].insert(to);
            
            Seg_Info[i]=temp;
            
            
            

        }

        //loading structure "streetHT"
        for (unsigned i = 0; i < getNumberOfStreets(); i++) {
            std::vector<unsigned> temp{i};
            std::string name = getStreetName(i);
            //std::transform(name.begin(), name.end(), name.begin(), ::tolower);
            //check if the name has been existed, 
            //if yes, then adding the street id under it. if not, create one
            if (streetHT.find(name) == streetHT.end()) streetHT.emplace(name, temp);
            else if (std::find(streetHT[name].begin(), streetHT[name].end(), i) == streetHT[name].end()) 
                streetHT[name].insert(streetHT[name].end(), i);
        }

        
        
//        double minlon=getIntersectionPosition(0).lon();
//        double maxlon=getIntersectionPosition(0).lon();
//        double minlat=getIntersectionPosition(0).lat();
//        double maxlat=getIntersectionPosition(0).lat();
        //loading the structure "positionInt"
        for (unsigned i = 0; i < getNumberOfIntersections(); i++) {
            positionInt[i] = getIntersectionPosition(i);
            //if(positionInt[i].lat()<minlat)minlat=positionInt[i].lat();
            //if(positionInt[i].lat()>maxlat)maxlat=positionInt[i].lat();
        }
        
        
        
        for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
            positionPOI[i] = getPointOfInterestPosition(i);
            //if(positionPOI[i].lat()<minlat)minlat=positionPOI[i].lat();
            //if(positionPOI[i].lat()>maxlat)maxlat=positionPOI[i].lat();
            
            
            
            
            
            std::vector<unsigned> temp{i};
            std::string name = getPointOfInterestName(i);
            //check if the name has been existed, 
            //if yes, then adding the street id under it. if not, create one
            if (POIname.find(name) == POIname.end()) POIname.emplace(name, temp);
            else 
                POIname[name].push_back(i);
        }
        //AvgLat1=0.5*(minlat+maxlat);
        //-------------------------------------------feature-----------------------------------------------
        feature_type.resize(getNumberOfFeatures());
        for(unsigned i=0; i<getNumberOfFeatures(); i++){
            FeatureType type=getFeatureType(i);
            feature_type[i]=type;
            /*
            if(type==Park||type==Greenspace||type==Golfcourse) FeaturetypeHT["Green"].push_back(i);
            else if(type==Unknown||type==Beach||type==Building) FeaturetypeHT["Yellow"].push_back(i);
            else if (type==Island) FeaturetypeHT["island"].push_back(i);
            else FeaturetypeHT["Blue"].push_back(i);
            */
            
            if(Feature_close1(i)){
            long double    area=area_of_feature(i);
                std::pair<long double, unsigned> area_index(area, i);
                
                area_of_index.push_back(area_index);
            }
            else {
                openfeatures.push_back(i);
            }
            
        }
    }
    std::sort(area_of_index.begin(),area_of_index.end());
    //finish loading
    load_successful = true; //Make sure this is updated to reflect whether
    
    //loading the map succeeded or failed
    return load_successful;
}

void close_map() {
    //Clean-up your map related data structures here
    segLength.clear();
    streetHT.clear();
    intersectionSeg.clear();
    streetSeg.clear();
    streetInt.clear();
    positionInt.clear();
    positionPOI.clear();
    area_of_index.clear();
    openfeatures.clear();
    feature_type.clear();
    POIname.clear();
    Seg_Info.clear();
    
    closeStreetDatabase();
    area_of_index.clear();
    
}


//Returns street id(s) for the given street name
//If no street with this name exists, returns a 0-length vector.
std::vector<unsigned> find_street_ids_from_name(std::string street_name) {
    return streetHT[street_name];
}

//Returns the street segments for the given intersection 
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id) {
    return intersectionSeg[intersection_id];
}

//Returns the street names at the given intersection (includes duplicate street 
//names in returned vector)
std::vector<std::string> find_intersection_street_names(unsigned intersection_id) {
    std::vector<unsigned> found_intersection_segment_id = intersectionSeg[intersection_id];
    std::vector<std::string> street_id_found;
    
    //loop through the vector of segment id which is under the input intersection id
    //according to the segment id, finding the street id, and then use it to get street name
    for (unsigned i = 0; i < found_intersection_segment_id.size(); i++) {
        unsigned temp = found_intersection_segment_id[i];
        unsigned streetID = getStreetSegmentInfo(temp).streetID;
        street_id_found.push_back(getStreetName(streetID));
    }
    return street_id_found;
}

//Returns true if you can get from intersection1 to intersection2 using a single 
//street segment (hint: check for 1-way streets too)
//corner case: an intersection is considered to be connected to itself
bool are_directly_connected(unsigned intersection_id1, unsigned intersection_id2) {
    //check if the two intersections are the same one
    if (intersection_id1 == intersection_id2)
        return true;

    //loop through the segment id that under the intersection id1
    std::vector<unsigned> seg_of_intersection = intersectionSeg[intersection_id1];
    for (unsigned i = 0; i < seg_of_intersection.size(); i++) {
        //check if the id1 is "from" of the segment, if so then check if id2 is "to"
        //if yes, then they are directly connected
        if (intersection_id1 == getStreetSegmentInfo(seg_of_intersection[i]).from) {
            if (intersection_id2 == getStreetSegmentInfo(seg_of_intersection[i]).to)
                return true;
        }
        //if id1 is "to", id2 is "from", and its two-way segment
        else if (!getStreetSegmentInfo(seg_of_intersection[i]).oneWay && 
                intersection_id2 == getStreetSegmentInfo(seg_of_intersection[i]).from)
            return true;
    }

    return false;

}

//Returns all intersections reachable by traveling down one street segment 
//from given intersection (hint: you can't travel the wrong way on a 1-way street)
//the returned vector should NOT contain duplicate intersections
std::vector<unsigned> find_adjacent_intersections(unsigned intersection_id) {
    std::vector<unsigned> segs = intersectionSeg[intersection_id];
   
    std::vector<unsigned> adj;
    
    //loop through the segments that connect to the intersection id
    for (unsigned i = 0; i < segs.size(); i++) {
        StreetSegmentInfo temp = getStreetSegmentInfo(segs[i]);
        //insert the element if this segment is "from" this id
        if (temp.from == intersection_id) {
            adj.push_back(temp.to);
        //if intersection_id is not "from", check if this segment is one-way
        //if is one-way, then insert this element
        } else if (!temp.oneWay) {
            adj.push_back(temp.from); 
        }
    }

    

    return adj;
}

//Returns all street segments for the given street
//Search every segments and find relevant segments by streetID
std::vector<unsigned> find_street_street_segments(unsigned street_id) {
    return streetSeg[street_id];
}

//Returns all intersections along the a given street
std::vector<unsigned> find_all_street_intersections(unsigned street_id) {

    std::vector<unsigned> intersect;
    for (const auto& elem : streetInt[street_id]) {
        intersect.push_back(elem);
    }
    return intersect;
}

//Return all intersection ids for two intersecting streets
//This function will typically return one intersection id.
//However street names are not guarenteed to be unique, so more than 1 intersection id
//may exist
std::vector<unsigned> find_intersection_ids_from_street_names(std::string street_name1,
        std::string street_name2) {
    std::vector<unsigned> streetID1 = find_street_ids_from_name(street_name1);
    std::vector<unsigned> streetID2 = find_street_ids_from_name(street_name2);
    std::vector<unsigned> intersections;
    
    //loop through the street id under name1
    for (unsigned i = 0; i < streetID1.size(); i++) {
        //loop through the street id under name2
        for (unsigned j = 0; j < streetID2.size(); j++) {
            std::vector<unsigned> itsc1 = find_all_street_intersections(streetID1[i]);
            std::vector<unsigned> itsc2 = find_all_street_intersections(streetID2[j]);
            //loop through the intersections under street id1 and find the element
            //in street id2. if find out, then push back
            for (unsigned k = 0; k < itsc1.size(); k++) {
                if (std::find(itsc2.begin(), itsc2.end(), itsc1[k]) != itsc2.end()) {
                    intersections.push_back(itsc1[k]);

                }
            }
        }
    }
    return intersections;
}

//use Taylor Series to approximate cosine
double cosine(double x) {
    double ans = 0;
    double coeff = 1.0;
    double factor = 1.0;
    for (int i = 0; i < 5; i++) {
        ans = ans + coeff*factor;
        coeff = coeff * (-1.0 / ((i * 2 + 1)*(2 * i + 2)));
        factor = x * x*factor;
    }
    return ans;
}

//Returns the distance between two coordinates in meters
double find_distance_between_two_points(LatLon point1, LatLon point2) {
    double latavg = cos((point1.lat() + point2.lat()) * DEG_TO_RAD / 2.0);
    double x1 = point1.lon() * latavg*DEG_TO_RAD;
    double y1 = point1.lat() * DEG_TO_RAD;
    double x2 = point2.lon() * latavg*DEG_TO_RAD;
    double y2 = point2.lat() * DEG_TO_RAD;
    return EARTH_RADIUS_IN_METERS * sqrt(pow((y2 - y1), 2.0) + pow((x2 - x1), 2.0)); //calculate by formula
}


//Returns the length of the given street segment in meters
double find_street_segment_length(unsigned street_segment_id) {
    unsigned curve_point_number = getStreetSegmentInfo(street_segment_id).curvePointCount;
    unsigned start_point_ID = getStreetSegmentInfo(street_segment_id).from;
    unsigned end_point_ID = getStreetSegmentInfo(street_segment_id).to;
    if (curve_point_number == 0) {//case: no curve point
        return find_distance_between_two_points(getIntersectionPosition(start_point_ID), getIntersectionPosition(end_point_ID));
    } else {//case: one or more curve points
        //calculate distance between start point and first curve point
        double whole_length = find_distance_between_two_points(getIntersectionPosition(start_point_ID), getStreetSegmentCurvePoint(street_segment_id, 0));
        for (unsigned i = 0; i < curve_point_number - 1; i++) {//loop to calculate distance between curve points
            whole_length = whole_length + find_distance_between_two_points(getStreetSegmentCurvePoint(street_segment_id, i), getStreetSegmentCurvePoint(street_segment_id, i + 1));
        }
        //calculate distance between end point and last curve point
        whole_length = whole_length + find_distance_between_two_points(getStreetSegmentCurvePoint(street_segment_id, curve_point_number - 1), getIntersectionPosition(end_point_ID));
        return whole_length;
    }
}

//Returns the length of the specified street in meters
double find_street_length(unsigned street_id) {
    std::vector<unsigned> seg_list = streetSeg[street_id];
    double street_length = 0.0;
    for (unsigned i = 0; i < seg_list.size(); i++) {//loop to add all segments together
        street_length = street_length + find_street_segment_length(seg_list[i]);
    }
    return street_length;
}

//Returns the travel time to drive a street segment in seconds 
//(time = distance/speed_limit)
double find_street_segment_travel_time(unsigned street_segment_id) {
    return segLength[street_segment_id].time; //calculate the time in load_map to save time
}

//quick calculate distance, for comparison only
double find_distance_between_two_point(LatLon point1, LatLon point2) {
    double latavg = cosine((point1.lat()) * DEG_TO_RAD);
    double x1 = point1.lon() * latavg;
    double y1 = point1.lat();
    double x2 = point2.lon() * latavg;
    double y2 = point2.lat();
    return (y2 - y1)* (y2 - y1)+ (x2 - x1)*(x2 - x1);
}

//Returns the nearest point of interest to the given position
unsigned find_closest_point_of_interest(LatLon my_position) {
    double distance = find_distance_between_two_point(positionPOI[0], my_position);
    unsigned index = 0;
    for (unsigned i = 1; i < positionPOI.size(); i++) {//loop to find closest position
        double temp = find_distance_between_two_point(positionPOI[i], my_position);
        if (temp < distance) {//compare and find minimum distance
            distance = temp;
            index = i;
        }
    }

    return index;
}

//Returns the the nearest intersection to the given position
unsigned find_closest_intersection(LatLon my_position) {
    double distance = find_distance_between_two_point(positionInt[0], my_position);
    unsigned index = 0;
    for (unsigned i = 1; i < positionInt.size(); i++) {//loop to find closest position
        double temp = find_distance_between_two_point(positionInt[i], my_position);
        if (temp < distance) {//compare and find minimum distance
            distance = temp;
            index = i;
        }
    }

    return index;
}

