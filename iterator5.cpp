/*
仅根据平均checkins模式来预测mac类别。
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

map<string, pair<float, float> > macLonlat;
map<string, int> macCtgy;//迭代更新，最终包含了预测类别
map<string, int> macCtgyReal;//全部的mac真实类别
map<string, int> macCtgyTest;//所有未知类型的mac
map<int, vector<float> > avgUtilization;//每一类的平均邻域分布
typedef map<string, vector<int> > distri;
distri macCtgyDistribution;
distri macUtilization;

int flag = 1; //终止判断条件：当值为1时，说明还存在未知类别；值为0时，则不存在未知类别，可以终止。

double minDistance = 0.001; //0.003; ap间的距离在这个范围内(经纬度都相差0.001)时，列为邻居
int ctgyNum = 12; //其中未知ap的类别为0

typedef pair<string, pair<float, float> > PAIR;

bool cmp_by_value(const pair<int, float> & m1, const pair<int, float> & m2) {
        return m1.second > m2.second;
}

float euclideanDistance(pair<float, float> pair1, pair<float, float> pair2){
  float d =0;
  d = (pair1.first - pair2.first)*(pair1.first - pair2.first) + (pair1.second - pair2.second)*(pair1.second - pair2.second);
  return sqrt(d);
}

float cos(vector<int> vec1, vector<float> vec2){
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



void getMostSimilar3(){

    vector< pair<int, float> > macCosine;

    string mac1;
    int ctgy2 = 0;
    float similr=0;

    map<string, int>::iterator iter1;
    map<int, vector<float> >::iterator iter2;

    flag = 0; //终止条件
    for(iter1=macCtgyTest.begin(); iter1!=macCtgyTest.end(); ++iter1){
      mac1 = iter1->first;
      macCosine.clear();
      for(iter2=avgUtilization.begin(); iter2!=avgUtilization.end(); ++iter2){
          ctgy2 = iter2->first;
          macCosine.push_back(make_pair(ctgy2, cos(macUtilization[mac1], iter2->second)));
      }
      sort(macCosine.begin(), macCosine.end(), cmp_by_value);

      similr = macCosine[0].second;
      ctgy2 = macCosine[0].first;

      if(ctgy2 != macCtgy[mac1]){ //更新
        macCtgy[mac1] = ctgy2;
        flag = 1;
      }

//      ofs<<mac1<<','<<macCtgy[mac1]<<','<<ctgy2<<','<<similr<<endl;
    }
}


void getUtilizaion(){

  string s1 = "checkins_week";
  ifstream ifs1(s1.c_str());

  string mac, token, line;
  int catagory = 0;
  int count = 0;
  vector<int> ivector(7,0);

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

}


void getAvgUtilization(){

  //string s1 = "../category12/safe_wifi_poi_catg12v2_0316_avgUtilization";
  string s1 = "checkins_week_avg2";
  ifstream ifs1(s1.c_str());

  vector<float> ivector(7,0);

  int ctgy,count;
  string token, line;
  while(getline(ifs1, line)){
      istringstream iss(line);
      count=0;
      while(getline(iss, token, ',')){
          count ++;
          if(count == 1){
            ctgy = atoi(token.c_str());
          }
          else{
            ivector[count-2] = atof(token.c_str()); //string to float
          }
      }
      avgUtilization[ctgy] = ivector;
   }
   ifs1.close();

}


void validation(string s){
    string s1 = s+"_checkins_week_"; //split_checkins_week_1
    string s2, istr;
    string mac, token, line;
    int catagory = 0;
    int count = 0;
    float lon=0, lat=0;


    for(int i=1;i<=10;i++){
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
             case 3:
                 lon = atof(token.c_str());
                 break;
             case 4:
                 lat = atof(token.c_str());
                 break;
             case 9:
                 catagory = atoi(token.c_str());
                 //cout<< catagory<<endl;
                 break;
             }
         }

         if(i!=10){ //第10个split作为测试集
           macCtgy[mac] = catagory;
           //cout<< i<<endl;
         }
         else{
           macCtgy[mac] = 0;
           macCtgyTest[mac] = 0;
         }
         macCtgyReal[mac] = catagory;//所有mac的真实类别，用于比较结果。
         macLonlat[mac] = make_pair(lon,lat);
     }
     ifs2.close();
    }
}


int main(int argc, char* argv[]){

   string s = argv[1]; //split

   //输入：按照留一验证原则
   validation(s);

   map<string, int>::iterator iter;
   string mac;

   int countIter = 0;

   while(flag == 1){
      //首先得到当前的所有mac的邻域类型分布。
      getUtilizaion();
      //得到所有mac的每一类的平均邻域分布。
      getAvgUtilization();
      //然后，比较位置类别中，最相似的类型。并更新macCtgy。
      getMostSimilar3();
      //判断终止条件
      cout<<"iterator "<<countIter++ <<" complelted!"<<endl;
   }


   string s4 = s+"_neighborSimilar5_10";
   ofstream ofs(s4.c_str());

   int ctgy = 0;
   int count1=0, count2=0;
   for(iter=macCtgyTest.begin(); iter!=macCtgyTest.end(); ++iter){
     count1++;
     mac = iter->first;
     ctgy = macCtgy[mac];
     ofs<<mac<<','<< macCtgyReal[mac] <<','<< ctgy <<endl;//mac,真实类别，预测类别

     if(ctgy == macCtgyReal[mac]){
       count2++;
     }
   }
   cout<<"Accuracy is: " <<float(count2)/count1<<endl;
   ofs.close();

}
