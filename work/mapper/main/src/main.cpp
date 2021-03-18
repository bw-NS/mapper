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
#include <iostream>
#include <string>

#include "m1.h"
#include "m2.h"
#include "m4.h"
#include "m3.h"
//Program exit codes
constexpr int SUCCESS_EXIT_CODE = 0;        //Everyting went OK
constexpr int ERROR_EXIT_CODE = 1;          //An error occured
constexpr int BAD_ARGUMENTS_EXIT_CODE = 2;  //Invalid command-line usage

//The default map to load if none is specified
std::string default_map_path = "/cad2/ece297s/public/maps/toronto_canada.streets.bin";

int main(int argc, char** argv) {

    std::string map_path;
    if(argc == 1) {
        //Use a default map
        map_path = default_map_path;
    } else if (argc == 2) {
        //Get the map from the command line
        map_path = argv[1];
    } else {
        //Invalid arguments
        std::cerr << "Usage: " << argv[0] << " [map_file_path]\n";
        std::cerr << "  If no map_file_path is provided a default map is loaded.\n";
        return BAD_ARGUMENTS_EXIT_CODE;
    }

    //Load the map and related data structures
    bool load_success = load_map(map_path);
    if(!load_success) {
        std::cerr << "Failed to load map '" << map_path << "'\n";
        return ERROR_EXIT_CODE;
    }

    //std::cout << "Successfully loaded map '" << map_path << "'\n";
std::vector<unsigned> actual;
std::vector<DeliveryInfo> deliveries;
        std::vector<unsigned> depots;
        float turn_penalty;
        std::vector<unsigned> result_path;
      deliveries = {DeliveryInfo(102168, 46017), DeliveryInfo(59033, 3949), DeliveryInfo(52607, 70747), DeliveryInfo(62536, 100049), DeliveryInfo(87300, 100878), DeliveryInfo(61813, 23189), DeliveryInfo(103299, 82337), DeliveryInfo(87922, 6329), DeliveryInfo(106883, 61250), DeliveryInfo(74157, 81025), DeliveryInfo(107776, 82337), DeliveryInfo(24454, 83613), DeliveryInfo(89223, 26925), DeliveryInfo(22739, 82458), DeliveryInfo(96372, 26433), DeliveryInfo(56696, 21665), DeliveryInfo(94993, 64798), DeliveryInfo(103248, 35208), DeliveryInfo(68697, 105256), DeliveryInfo(105074, 73527), DeliveryInfo(14179, 3364), DeliveryInfo(20302, 23189), DeliveryInfo(97506, 81025), DeliveryInfo(51178, 62599), DeliveryInfo(94080, 80313), DeliveryInfo(39037, 63926), DeliveryInfo(45703, 90270), DeliveryInfo(108282, 64030), DeliveryInfo(7971, 96808), DeliveryInfo(53625, 82969), DeliveryInfo(62121, 7506), DeliveryInfo(38448, 83330), DeliveryInfo(89653, 73370), DeliveryInfo(70104, 23189), DeliveryInfo(103248, 22508), DeliveryInfo(36693, 53342), DeliveryInfo(106372, 81025), DeliveryInfo(103248, 56701), DeliveryInfo(48740, 82337), DeliveryInfo(48519, 22647), DeliveryInfo(106184, 12268), DeliveryInfo(107776, 105310), DeliveryInfo(107776, 87790), DeliveryInfo(50962, 1044), DeliveryInfo(103248, 59901), DeliveryInfo(41485, 2487), DeliveryInfo(54156, 35650), DeliveryInfo(103854, 788), DeliveryInfo(90686, 19604), DeliveryInfo(12849, 23966), DeliveryInfo(104727, 82337), DeliveryInfo(34239, 87043), DeliveryInfo(97438, 43353), DeliveryInfo(103887, 19163), DeliveryInfo(39005, 56987), DeliveryInfo(17558, 94519), DeliveryInfo(21142, 60659), DeliveryInfo(90026, 92605), DeliveryInfo(52852, 73529), DeliveryInfo(5207, 12090), DeliveryInfo(26171, 5120), DeliveryInfo(17800, 7514), DeliveryInfo(60805, 82874), DeliveryInfo(41711, 81630), DeliveryInfo(104428, 104847), DeliveryInfo(107744, 25649), DeliveryInfo(12111, 50400), DeliveryInfo(38362, 59881), DeliveryInfo(50843, 19784), DeliveryInfo(9088, 23189), DeliveryInfo(39544, 72147), DeliveryInfo(107776, 1708), DeliveryInfo(108614, 93653), DeliveryInfo(82136, 13562), DeliveryInfo(104671, 52031), DeliveryInfo(50283, 97305), DeliveryInfo(103298, 30594), DeliveryInfo(73397, 93338), DeliveryInfo(29944, 98197), DeliveryInfo(31562, 89513), DeliveryInfo(7644, 98949), DeliveryInfo(86296, 33118), DeliveryInfo(99187, 83233), DeliveryInfo(30362, 105801), DeliveryInfo(59268, 27793), DeliveryInfo(78113, 88982), DeliveryInfo(36306, 99377), DeliveryInfo(98705, 81149), DeliveryInfo(5162, 34639), DeliveryInfo(55265, 37355), DeliveryInfo(49864, 93912), DeliveryInfo(78527, 88186), DeliveryInfo(75827, 17720), DeliveryInfo(40339, 21829), DeliveryInfo(98173, 58391), DeliveryInfo(103248, 99952), DeliveryInfo(58155, 18617), DeliveryInfo(52214, 66081), DeliveryInfo(28847, 15478), DeliveryInfo(104461, 56342)};
        depots = {12, 46283, 736};
        turn_penalty = 15;
        result_path = traveling_courier(deliveries, depots, turn_penalty);
        
      
                //You can now do something with the map data
 //actual = find_intersection_street_segments(7);
 //draw_map();
    //Clean-up the map data and related data structures
    std::cout << "Closing map\n";
    close_map(); 

    return SUCCESS_EXIT_CODE;
}
