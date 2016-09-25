/*
  1、check 0316和0323中每个（u,t）访问的ap中有重复类型的(u,t)的占比。
  2、check 0316和0323中每个ap被访问的(u,t)重复的占比。
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


map< pair<string, int>, set<int> > utCtgy16;
map< pair<string, int>, set<int> > utCtgy23;

map< pair<string, int>, set<string> > utMac16;
map< pair<string, int>, set<string> > utMac23;

map< pair<string, int>, set<int> > getMapCtgy(string s){ //newmoveSZ_0316_inCatagory

  map< pair<string, int>, set<int> > utCtgy;

  ifstream ifs1(s.c_str());
  string guid, mac, token, line;
  int start_hour=0, catagory = 0;
  int count = 0;
  while(getline(ifs1, line)){
      istringstream iss(line);
      count=0;
      while(getline(iss, token, ',')){
          count ++;
          switch(count){
            case 1:
              guid = token;
              break;
            case 2:
              mac = token;
              break;
            case 3:
              start_hour = atoi(token.c_str());
              break;
            case 5:
              catagory = atoi(token.c_str());
              break;

          }
      }
      utCtgy[make_pair(guid, start_hour)].insert(catagory);
   }
   ifs1.close();
   return utCtgy;

}


map< pair<string, int>, set<string> > getMapMac(string s){ //newmoveSZ_0316_inCatagory

  map< pair<string, int>, set<string> > utMac;

  ifstream ifs1(s.c_str());
  string guid, mac, token, line;
  int start_hour=0, catagory = 0;
  int count = 0;
  while(getline(ifs1, line)){
      istringstream iss(line);
      count=0;
      while(getline(iss, token, ',')){
          count ++;
          switch(count){
            case 1:
              guid = token;
              break;
            case 2:
              mac = token;
              break;
            case 3:
              start_hour = atoi(token.c_str());
              break;
            case 5:
              catagory = atoi(token.c_str());
              break;

          }
      }
      utMac[make_pair(guid, start_hour)].insert(mac);
   }
   ifs1.close();
   return utMac;

}


void getNum(){
    int count = 0;//统计有相同类别的(u,t)的个数
    map< pair<string, int>, set<int> >::iterator iter1;
    set<int> set1, set2, set3;
    pair<string, int> utpair;
    string guid;
    int hour;
    cout<<"coming1"<<endl;

    for(iter1=utCtgy16.begin(); iter1 != utCtgy16.end(); ++iter1){
        set3.clear();
        utpair = iter1->first;
        guid = utpair.first;
        hour = utpair.second;

        set1 = iter1->second;
        set2 = utCtgy23[utpair];
        set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(set3, set3.begin()));
        if(set3.size() > 0){
          count ++;
          //cout<< set3.size()<<endl;
        }

        set3.clear();
        set2 = utCtgy23[make_pair(guid, hour-1)];
        set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(set3, set3.begin()));
        if(set3.size() > 0){
          count ++;
          //cout<< set3.size()<<endl;
        }
        set3.clear();
        set2 = utCtgy23[make_pair(guid, hour+1)];
        set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(set3, set3.begin()));
        if(set3.size() > 0){
          count ++;
          //cout<< set3.size()<<endl;
        }
    }
    cout<<count<<endl;
    double percentage = double(count)/(utCtgy16.size());
    cout<< percentage <<endl;
}

//输出0316一天中，每个(u,t)内的ap的个数，以及ctgy的个数。
void getNum2(){
    int count = 0;//统计有相同类别的(u,t)的个数
    map< pair<string, int>, set<int> >::iterator iter1;
    set<int> set1, set2, set3;
    pair<string, int> utpair;
    string guid;
    int hour=0, i=0, num1=0, num2=0, num3=0;

    ofstream ofs("numOfCtgyandAp0316");

    for(iter1=utCtgy16.begin(); iter1 != utCtgy16.end(); ++iter1){
        set2.clear();
        set3.clear();
        utpair = iter1->first;


        guid = utpair.first;
        hour = utpair.second;
        num1 = (iter1->second).size();


        if(hour > 0)
        {
          if(utCtgy16.find(make_pair(guid, hour-1))!=utCtgy16.end())
            num2 = utCtgy16[make_pair(guid, hour-1)].size();
          else
            num2 = 0;
          //set2 = utCtgy16[make_pair(guid, hour-1)];
          //num2 = set2.size();
        }
        else
          num2 = 0;

        if(hour < 23){
          if(utCtgy16.find(make_pair(guid, hour+1))!=utCtgy16.end()) //注意判断是否是空值，不然会引起内存紊乱。
            num3 = utCtgy16[make_pair(guid, hour+1)].size();
          else
            num3 = 0;
        }
        else
          num3 = 0;

        ofs<< num1 <<','<<num2<<','<<num3<<','<<(num1+num2+num3);

        num1 = utMac16[utpair].size();
        if(hour > 0)
        {
          if(utMac16.find(make_pair(guid, hour-1))!=utMac16.end())
            num2 = utMac16[make_pair(guid, hour-1)].size();
          else
            num2 = 0;
          //set2 = utCtgy16[make_pair(guid, hour-1)];
          //num2 = set2.size();
        }
        else
          num2 = 0;

        if(hour < 23){
          if(utMac16.find(make_pair(guid, hour+1))!=utMac16.end()) //注意判断是否是空值，不然会引起内存紊乱。
            num3 = utMac16[make_pair(guid, hour+1)].size();
          else
            num3 = 0;
        }
        else
          num3 = 0;

        ofs<<','<<(num1+num2+num3)<<endl;
    }
    ofs.close();
}


//输出0316一天中，每个(u,t)内的ap的个数，以及ctgy的个数。
void getNum3(){
    int count = 0;//统计有相同类别的(u,t)的个数
    map< pair<string, int>, set<int> >::iterator iter1;
    set<int> set1, set2, set3;
    pair<string, int> utpair;
    string guid;
    int hour, i=0, set1Num, set2Num, set3Num;
    vector<int> ivec(12,0);

    ofstream ofs("numOfCtgyandAp0316");

    for(iter1=utCtgy16.begin(); iter1 != utCtgy16.end(); ++iter1){
        //set3.clear();
        utpair = iter1->first;
        guid = utpair.first;
        hour = utpair.second;
        set1Num = (iter1->second).size();

        cout<< set1Num<<',';//输出该(u,t)的ctgy个数
        set1Num = utMac16[make_pair(guid, hour)].size();


      //  ofs<< set1Num <<endl;//ap的个数

    }
    ofs.close();
}

int main(int argc, char* argv[]){
    string s1 = argv[1];//newmoveSZ_0316_inCatagory
    string s2 = argv[2];//newmoveSZ_0323_intCatagory


    utCtgy16 = getMapCtgy(s1);
    utMac16 = getMapMac(s1);
    cout<<"getMap ok"<<endl;
    //cout<<utCtgy16.size()<<endl;
    //mypair = getMap(s2);
    //utCtgy23 = mypair.first;
    //utMac23 = mypair.second;

    getNum2();

     return 0;

}



/*
map<string, set<pair<string, int> > > conn16;
map<string, set<pair<string, int> > > conn23;


map<string, set<pair<string, int> > > getConn(string s1){ //newmoveSZ_0316_inCatagory

  map<string, set<pair<string, int> > > conn;
  conn.clear();
  ifstream ifs1(s1.c_str());
  string guid, mac, token, line;
  int start_hour=0, catagory = 0;
  int count = 0;

  while(getline(ifs1, line)){
      istringstream iss(line);
      count=0;
      while(getline(iss, token, ',')){
          count ++;
          switch(count){
            case 1:
              guid = token;
              break;
            case 2:
              mac = token;
              break;
            case 3:
              start_hour = atoi(token.c_str());
              break;
            case 5:
              catagory = atoi(token.c_str());
              break;

          }
      }
      conn[mac].insert(make_pair(guid, start_hour));
   }
   ifs1.close();
   return conn;

}

void getNum(){
    int count = 0;//统计有相同类别的(u,t)的个数
    map< pair<string, int>, set<int> >::iterator iter1;
    set<int> set1, set2, set3;
    pair<string, int> utpair;
    for(iter1=utCtgy16.begin(); iter1 != utCtgy16.end(); ++iter1){
        set3.clear();
        utpair = iter1->first;
        set1 = iter1->second;
        set2 = utCtgy23[utpair];
        set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(set3, set3.begin()));

        if(set3.size() > 0){
          count ++;
          cout<< set3.size()<<endl;
        }
    }
    cout<<count<<endl;
    double percentage = double(count)/(utCtgy16.size());
    cout<< percentage <<endl;
}


int main(int argc, char* argv[]){
    string s1 = argv[1];//newmoveSZ_0316_inCatagory
    string s2 = argv[2];//newmoveSZ_0323_intCatagory

    utCtgy16 = getConn(s1);
    utCtgy23 = getConn(s2);

    getNum();

     return 0;

}
*/
