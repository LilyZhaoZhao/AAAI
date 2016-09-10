/*
仅根据平均邻域分布来预测mac类别。
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
map<string, int> macCtgy;//迭代更新，最终包含了预测类别
map<string, int> macCtgyReal;//全部的mac真实类别
map<string, int> macCtgyTest;//所有未知类型的mac
typedef map<string, vector<int> > distri;
distri macCtgyDistribution;

double minDistance = 0.001; //0.003; ap间的距离在这个范围内(经纬度都相差0.001)时，列为邻居
int ctgyNum = 12; //其中未知ap的类别为0
int k = 3;

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


void getMostSimilar1(){

    vector< pair<string, float> > macCosine;

    string mac1, mac2;
    int ctgy = 0;
    float similr=0;

    map<string, int>::iterator iter1;
    distri::iterator iter2;

    for(iter1=macCtgyTest.begin(); iter1!=macCtgyTest.end(); ++iter1){
      mac1 = iter1->first;
      if(macCtgy[mac1] == 0){
      macCosine.clear();
      for(iter2=macCtgyDistribution.begin(); iter2!=macCtgyDistribution.end(); ++iter2){
          mac2 = iter2->first;
          macCosine.push_back(make_pair(mac2, cos(macCtgyDistribution[mac1], iter2->second)));
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
      macCtgy[mac1] = macCtgy[mac2]; //更新macCtgy
      //ofs<<mac1<<','<<macCtgy[mac1]<<','<<macCtgy[mac2]<<','<<similr<<endl;
      }
    }

}


//在getMostSimilar1的基础上更改，在得到macCosine后，进行top k选择，选取最多的类别为预测类别。
void getMostSimilar1_v2(){

    vector< pair<string, float> > macCosine;

    string mac1, mac2;
    int ctgy = 0;
    float similr=0;

    map<string, int>::iterator iter1;
    distri::iterator iter2;
    vector<int>::iterator biggest;

    for(iter1=macCtgyTest.begin(); iter1!=macCtgyTest.end(); ++iter1){
      mac1 = iter1->first;
      if(macCtgy[mac1] == 0){
      macCosine.clear();
      for(iter2=macCtgyDistribution.begin(); iter2!=macCtgyDistribution.end(); ++iter2){
          mac2 = iter2->first;
          macCosine.push_back(make_pair(mac2, cos(macCtgyDistribution[mac1], iter2->second)));
      }
      sort(macCosine.begin(), macCosine.end(), cmp_by_value);

      vector<int> ctgyVec(12,0); //每一轮都重新定义
      for(int i=0; i<k; i++){
          mac2 = macCosine[i].first;
          ctgy = macCtgy[mac2];
          if(ctgy!=0)
            ctgyVec[ctgy-1]++;
      }

      biggest = max_element(std::begin(ctgyVec), std::end(ctgyVec));
      macCtgy[mac1] = distance(std::begin(ctgyVec), biggest) + 1; //更新macCtgy
      //ofs<<mac1<<','<<macCtgy[mac1]<<','<<macCtgy[mac2]<<','<<similr<<endl;
      }
    }

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
              if(ctgy != 0){
                macCtgyDistribution[mac1][ctgy-1] += 1;
              }
            }
        }
    }
}


void validation(string s){
    string s1 = s+"_intCatagory_";
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

   int countIter = 0;
   map<string, int>::iterator iter;
   string mac;
   int flag = 1; //终止判断条件：当值为1时，说明还存在未知类别；值为0时，则不存在未知类别，可以终止。
   //while(flag == 1){
   while(countIter <= 10){
      //首先得到当前的所有mac的邻域类型分布。
      getNearstDistDistribution();
      //然后，比较位置类别中，最相似的类型。并更新macCtgy。
      getMostSimilar1();
      //getMostSimilar1_v2();

      //判断终止条件
      flag = 0;
      for(iter=macCtgyTest.begin(); iter!=macCtgyTest.end(); ++iter){
        mac = iter->first;
        if(macCtgy[mac] == 0){
          flag = 1;
          break;
        }
      }
      countIter++ ;
    //  cout<<"iterator "<<countIter++ <<"complelted!"<<endl;

   }


   string s4 = s+"_neighborSimilar1_10"; //使用getMostSimilar1
   //string s4 = s+"_neighborSimilar1_v2_10"; //使用getMostSimilar1_v2
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
