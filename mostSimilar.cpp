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


//map<string, string> filter;
map<string, pair<float, float> > macLonlat;
map<string, int> macCtgy;
map<string, vector<int> > macCtgyDistribution;
typedef map<string, vector<int> > distri;
//distri macCtgyDistribution;
//distri macUtilization;

double minDistance = 0.001; //0.003; ap间的距离在这个范围内(经纬度都相差0.001)时，列为邻居
int ctgyNum = 12; //其中未知ap的类别为0
int kvalue = 3;

//map<string, set<int> > userTime;

typedef pair<string, pair<float, float> > PAIR;

bool cmp_by_value(const pair<string, float> & m1, const pair<string, float> & m2) {
        return m1.second > m2.second;
}

float euclideanDistance(pair<float, float> pair1, pair<float, float> pair2){
  float d =0;
  d = (pair1.first - pair2.first)*(pair1.first - pair2.first) + (pair1.second - pair2.second)*(pair1.second - pair2.second);
  return sqrt(d);
}

float cos(vector<int> vec1, vector<int> vec2){
  float dot_product = 0.0;
  float normA = 0.0;
  float normB = 0.0;
  for(int i=0; i<vec1.size(); i++){
    dot_product += vec1[i] * vec2[i];
    normA += vec1[i] * vec1[i];
    normB += vec2[i] * vec2[i];
  }
  if(normA == 0.0 or normB == 0.0)
      return 0;
  else
      return dot_product/(sqrt(normA*normB));
}



void getNearstDistDistribution(){

    vector<PAIR > cPair(macLonlat.begin(),macLonlat.end());
    string mac1, mac2;
    float dist = 0, lon=0,lat=0, lon2=2, lat2=0;
    int ctgy = 0;
    vector<int> ivector(12,0);

    for(int i = 0; i<cPair.size(); ++i){
        mac1 = cPair[i].first;
        lon = cPair[i].second.first;
        lat = cPair[i].second.second;
        macCtgyDistribution[mac1] = ivector;
        for(int j = 0; j<cPair.size(); ++j){
            mac2 = cPair[j].first;
            //dist = euclideanDistance(cPair[i].second, cPair[j].second);
            lon2 = cPair[j].second.first;
            lat2 = cPair[j].second.second;
            //if(dist < minDistance){
            if(abs(lon-lon2)<=minDistance && abs(lat-lat2)<=minDistance){
              ctgy = macCtgy[mac2];
              macCtgyDistribution[mac1][ctgy-1] += 1;
            }
        }
    }
}


void getMostSimilar_v2(distri macCtgyDistribution_func, string s){
    vector< pair<string, float> > macCosine;

    string mac1, mac2;
    int ctgy = 0;
    float similr=0, similr2=0;

    //map<string, pair<string, float> > similarity;

    string s4 = s+"_similar";
    ofstream ofs(s4.c_str());

    distri::iterator iter1, iter2;
    for(iter1=macCtgyDistribution_func.begin(); iter1!=macCtgyDistribution_func.end(); ++iter1){
      mac1 = iter1->first;
      macCosine.clear();
      for(iter2=macCtgyDistribution_func.begin(); iter2!=macCtgyDistribution_func.end(); ++iter2){
          mac2 = iter2->first;
          macCosine.push_back(make_pair(mac2, cos(iter1->second, iter2->second)));
      }
      sort(macCosine.begin(), macCosine.end(), cmp_by_value);
      if(macCosine[0].first == mac1){
        //similarity[mac1] = macCosine[1];
        similr = macCosine[1].second;
        mac2 = macCosine[1].first;
        //cout<< macCosine[1].first<<','<<macCosine[1].second<<endl;
      }
      else{
        //similarity[mac1] = macCosine[0];
        similr = macCosine[0].second;
        mac2 = macCosine[0].first;
        //cout<< macCosine[0].first<<','<<macCosine[0].second<<endl;
      }

      similr2 = cos(macCtgyDistribution[mac1], macCtgyDistribution[mac2]);

      //cout<<macCtgy[mac1]<<','<<macCtgy[mac2]<<','<<similr<<','<<similr2<<endl;

      ofs<<macCtgy[mac1]<<','<<macCtgy[mac2]<<','<<similr<<','<<similr2<<endl;

    }
    ofs.close();
    //return similarity;
}



map<string, pair<string, float> > getMostSimilar(distri macCtgyDistribution_func){
    vector< pair<string, float> > macCosine;

    string mac1, mac2;
    int ctgy = 0;

    map<string, pair<string, float> > similarity;

    distri::iterator iter1, iter2;
    for(iter1=macCtgyDistribution_func.begin(); iter1!=macCtgyDistribution_func.end(); ++iter1){
      mac1 = iter1->first;
      macCosine.clear();
      for(iter2=macCtgyDistribution_func.begin(); iter2!=macCtgyDistribution_func.end(); ++iter2){
          mac2 = iter2->first;
          macCosine.push_back(make_pair(mac2, cos(iter1->second, iter2->second)));
      }
      sort(macCosine.begin(), macCosine.end(), cmp_by_value);
      if(macCosine[0].first == mac1){
        similarity[mac1] = macCosine[1];
        //cout<< macCosine[1].first<<','<<macCosine[1].second<<endl;
      }
      else{
        similarity[mac1] = macCosine[0];
        //cout<< macCosine[0].first<<','<<macCosine[0].second<<endl;

      }
    }

    return similarity;
}





int main(int argc, char* argv[]){

   string s = argv[1];
   string s1 = s+"_Utilization";//safe_wifi_poi_catg12v2_0316_Utilization
   string s2 = s+"_intCatagory";//safe_wifi_poi_catg12v2_0316_intCatagory

   std::stringstream os;

   string mac, token, line;
   //float lon=0.0, lat=0.0;
   int catagory = 0;
   int count = 0;

   ifstream ifs1(s1.c_str());
   distri macUtilization;

   //int a[24] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   vector<int> ivector(24,0);

   while(getline(ifs1, line)){
       istringstream iss(line);
       count=0;
       while(getline(iss, token, ',')){
           count ++;
           if(count == 1){
             mac = token;
           }
           else{
             ivector[count-2] = atoi(token.c_str()); //string to int
           }
       }
       macUtilization[mac] = ivector;
    }
    ifs1.close();

    float lon=0, lat=0;
    ifstream ifs2(s2.c_str());
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
           case 8:
               catagory = atoi(token.c_str());
               //cout<< catagory<<endl;
               break;
           }
       }
       macCtgy[mac] = catagory;
       macLonlat[mac] = make_pair(lon,lat);
   }
   ifs2.close();

   getNearstDistDistribution();
   //map<string, pair<string, float> > similarity1 = getMostSimilar(macUtilization);
   getMostSimilar_v2(macUtilization, s);

/*
   int ctgy1=0, ctgy2=0;
   string mac2;
   float similr=0;
   string s4 = s+"_similar1";
   ofstream ofs(s4.c_str());
   map<string, pair<string, float> >::iterator iter;
   for(iter=similarity1.begin(); iter!=similarity1.end(); ++iter){
     mac = iter->first;
     mac2 = (iter->second).first;
     ctgy1 = macCtgy[mac];
     ctgy2 = macCtgy[mac2];
     similr = iter->second.second;
     ofs<< ctgy1 << ',' << ctgy2 <<','<<similr<<endl;
   }
   ofs.close();
*/

}
