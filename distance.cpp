/*
  1、所有ap间的距离
  2、同类别ap间的距离
*/

#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstring>
#include <ctime>
#include <cmath>

using namespace std;

typedef pair<string, pair<double, double> > PAIR;

map<string, pair<double, double> > macLonlat;

double euclideanDistance(pair<double, double> pair1, pair<double, double> pair2){
  double d =0;
  d = (pair1.first - pair2.first)*(pair1.first - pair2.first) + (pair1.second - pair2.second)*(pair1.second - pair2.second);
  return sqrt(d);
}


void getDistance(string s, vector< pair<double, double> > lonlat){

    string s1 = "Distance_"+s;
    ofstream ofs(s1.c_str());

    string mac1, mac2;
    double dist = 0;

    for(int i = 0; i<lonlat.size(); ++i){
        for(int j = i+1; j<lonlat.size(); ++j){
            dist = euclideanDistance(lonlat[i], lonlat[j]) * 100000;
            ofs<< dist <<endl;
        }
    }
    ofs.close();
}


void validation(){
    //string s1 = s+"_intCatagory_";
    string s1 = "sampling_wifi_poi_sampingv2_0316_Neighbor"; //sampling_wifi_poi_sampingv2_0316_Neighbor1
    string s2, istr;
    string mac, token, line;
    int catagory = 0;
    int count = 0;
    double lon=0, lat=0;

    vector< pair<double, double> > lonlat;

    for(int i=1;i<=12;i++){
      lonlat.clear(); //每次都需要清空，因为每一轮都是新的向量。
      std::stringstream ss;
      ss << i;
      ss >> istr;
      s2 = s1+istr;
      //cout<<s2<<endl;
      catagory = 0;
      count = 0;
      lon=0, lat=0;
      ifstream ifs2(s2.c_str());
      while(getline(ifs2, line)){
         istringstream iss(line);
         count=0;
         while(getline(iss, token, ',')){
             count ++;
             switch(count){
             case 1:
                 mac = token;
                 break;
             }
         }
         lonlat.push_back(macLonlat[mac]);
     }
     ifs2.close();
     getDistance(istr, lonlat);
    }

}


void getAllApDistance(){

  vector<PAIR > cPair(macLonlat.begin(),macLonlat.end());
  double dist = 0;

  string s = "Distance_All";
  ofstream ofs(s.c_str());

  for(int i = 0; i<cPair.size(); ++i){
      for(int j = i+1; j<cPair.size(); ++j){
          dist = euclideanDistance(cPair[i].second, cPair[j].second) * 100000;
          ofs<< dist <<endl;
      }
  }

  ofs.close();
}


int main(int argc, char* argv[]){

   string s ="sampling_wifi_poi_sampingv2_0316_intCatagory";

   string mac, token, line;
   int count = 0;
   double lon=0, lat=0;

   ifstream ifs2(s.c_str());
   while(getline(ifs2, line)){
      istringstream iss(line);
      count=0;
      while(getline(iss, token, '|')){
          count ++;
          switch(count){
          case 2:
              mac = token;
              break;
          case 3:
              lon = atof(token.c_str());
              break;
          case 4:
              lat = atof(token.c_str());
              break;
      }
      macLonlat[mac] = make_pair(lon,lat);
    }
  }

   validation();

   getAllApDistance();

   //double r = 0;
   //r = euclideanDistance(make_pair(22.558601,114.12892),make_pair(22.5578,114.12899));
   //cout<< r<<endl;
}
