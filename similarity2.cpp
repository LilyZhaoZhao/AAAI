/*
  1、check同类ap的utilization相似度是否更大：输出同类ap间的utilization相似度
  2、check全体ap的utilization相似度分布。
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



typedef map<string, vector<int> > distri;
//distri macCtgyDistribution;
distri macUtilization;

double minDistance = 0.001; //0.003; ap间的距离在这个范围内(经纬度都相差0.001)时，列为邻居
int ctgyNum = 12; //其中未知ap的类别为0
int kvalue = 3;


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


void getSimilarity(string s, vector< vector<int> > utilization){

  ofstream ofs(s.c_str());

//  string mac1, mac2;
  double similr = 0;

  for(int i = 0; i<utilization.size(); ++i){
      for(int j = i+1; j<utilization.size(); ++j){
          similr = cos(utilization[i], utilization[j]);
          ofs<< similr <<endl;
      }
  }
  ofs.close();
}




void validation(string s){

    string s1 = "sampling_wifi_poi_sampingv2_0316_Neighbor"; //sampling_wifi_poi_sampingv2_0316_Neighbor1
    string s2, istr;
    string mac, token, line;
    int catagory = 0;
    int count = 0;

    vector< vector<int> > utilization;

    for(int i=1;i<=12;i++){
      utilization.clear(); //每次都需要清空，因为每一轮都是新的向量。
      std::stringstream ss;
      ss << i;
      ss >> istr;
      s2 = s1+istr;
      //cout<<s2<<endl;
      catagory = 0;
      count = 0;
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
         utilization.push_back(macUtilization[mac]);
     }
     ifs2.close();
     string s3 = s+istr+"_eachCatgUtilizationSimilarity";
     getSimilarity(s3, utilization);
    }

}


void getAllApUtilization(string s){

  double similr=0;

  string s4 = s+"_allApUtilizationSimilarity";

  ofstream ofs(s4.c_str());

  distri::iterator iter1, iter2;
  for(iter1=macUtilization.begin(); iter1!=macUtilization.end(); ++iter1){
    for(iter2= iter1, ++iter2; iter2!=macUtilization.end(); ++iter2){
        similr = cos(iter1->second, iter2->second);
        ofs<< similr <<endl;
    }
    }
  ofs.close();
}



int main(int argc, char* argv[]){

    string s1 = "sampling_wifi_poi_sampingv2_0316_Utilization";//sampling_wifi_poi_sampingv2_0316_Utilization

    std::stringstream os;

    string mac, token, line;
    //float lon=0.0, lat=0.0;
    int catagory = 0;
    int count = 0;

    ifstream ifs1(s1.c_str());
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

     validation(s1);

     getAllApUtilization(s1);

     return 0;

}
