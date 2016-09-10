/*
  1、check同类ap的邻域相似度是否更大：输出同类ap间的邻域相似度
  2、check全体ap大邻域相似度分布。
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


//map<string, string> filter;
map<string, pair<float, float> > macLonlat;
map<string, int> macCtgy;
//map<string, vector<int> > macCtgyDistribution;
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

bool cmp_by_value2(const pair<int, float> & m1, const pair<int, float> & m2) {
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

//调整余弦相似度算法 adjusted cosine similarity
float cos_v2(vector<int> vec1, vector<int> vec2){
  float dot_product = 0.0;
  float normA = 0.0;
  float normB = 0.0;
  float avg = 0.0;
  for(int i=0; i<vec1.size(); i++){
    avg = (vec1[i] + vec2[i])/2;
    vec1[i] = vec1[i] - avg;
    vec2[i] = vec2[i] - avg;
  }

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


void getMostSimilar(distri macCtgyDistribution_func, string s4){
    string mac1, mac2;
    int ctgy = 0;
    float similr=0;

    ofstream ofs(s4.c_str());

    distri::iterator iter1, iter2;
    for(iter1=macCtgyDistribution_func.begin(); iter1!=macCtgyDistribution_func.end(); ++iter1){
      mac1 = iter1->first;
      for(iter2= iter1, ++iter2; iter2!=macCtgyDistribution_func.end(); ++iter2){
          mac2 = iter2->first;
          similr = cos(iter1->second, iter2->second);
          ofs<< similr <<endl;
      }
      }
    ofs.close();
}


//全体ap的邻域相似度
void input1(string s1){ //s1 = sampling_wifi_poi_sampingv2_0316_Neighbor
  distri macCtgyDistribution;
  std::stringstream os;

  string mac, token, line;
  int catagory = 0;
  int count = 0;

  ifstream ifs1(s1.c_str());
  vector<int> ivector(12,0);

  while(getline(ifs1, line)){
      istringstream iss(line);
      count=0;
      while(getline(iss, token, ',')){
          count ++;
          if(count == 1){
            mac = token;
          }
          else if(count == 2){
            catagory = atoi(token.c_str());
          }
          else{
            ivector[count-3] = atoi(token.c_str()); //string to int
          }
      }
      macCtgyDistribution[mac] = ivector;
   }
   ifs1.close();

   string s2 = s1+"_allApNeighborSimilarity";
   getMostSimilar(macCtgyDistribution, s2);
}


void input2(string s1){//s1 = sampling_wifi_poi_sampingv2_0316_Neighbor
  distri macCtgyDistribution;
  string s2, istr;
  string mac, token, line;
  int catagory = 0;
  int count = 0;

  vector<int> ivector(12,0);

  for(int i=1;i<=12;i++){ //对每一类的邻域文件读入
    macCtgyDistribution.clear();//每一轮都需要重新定义
    std::stringstream ss;
    ss << i;
    ss >> istr;
    s2 = s1+istr;
    cout<<s2<<endl;
    catagory = 0;
    count = 0;
    //lon=0, lat=0;
    ifstream ifs2(s2.c_str());
    while(getline(ifs2, line)){
      istringstream iss(line);
      //ivector.clear();//ivector不能清空，不然size就为0了。
      count=0;
      while(getline(iss, token, ',')){
          count ++;
          if(count == 1){
            mac = token;
          }
          else if(count == 2){
            catagory = atoi(token.c_str());
          }
          else{
            ivector[count-3] = atoi(token.c_str()); //string to int
          }
      }
      macCtgyDistribution[mac] = ivector;
   }
   ifs2.close();
   string s3 = s2+"_eachCatgNeighborSimilarity";
   getMostSimilar(macCtgyDistribution, s3);
   }
}


int main(int argc, char* argv[]){

   string s = argv[1];//sampling_wifi_poi_sampingv2_0316_Neighbor

//   input1(s);
   input2(s);
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
