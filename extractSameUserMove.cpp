/*
  1、从5天的共同用户中，提取出这些用户的每天的conn记录。
  2、从5天的共同mac中，提取出这些mac的每天的conn记录。
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
using namespace std;

map<string, pair<float, float> > macLonlat;

//从共同user提取move
/*
int main(int argc, char* argv[]){
    if(argc<4){
        cout<<"More Parameters Required"<<endl;
        return 1;
    }
    ifstream ifs1(argv[1]);// user1617181920
    ifstream ifs2(argv[2]);// moveSZ_0316_intCatagory
    ofstream ofs(argv[3]);// newmoveSZ_0316_intCatagory

    set<string> userSet;

    string line, user;
	  while(getline(ifs1, line)){
      istringstream iss(line);
      iss>> user;
      userSet.insert(user);
    }


    string guid, token;
    time_t t;
    int start_hour = 0, count = 0;
    while(getline(ifs2, line)){
		    istringstream iss(line);
        count = 0;
		    while(getline(iss, token, ',')){
          count ++;
			    if(count == 1){
          guid = token;
          break;
          }
		    }
        if(userSet.find(guid) != userSet.end()){
          ofs << line<<endl;
        }
	  }

    ifs1.close();
    ifs2.close();
    ofs.close();
    return 0;
}
*/


int main(int argc, char* argv[]){
    if(argc<5){
        cout<<"More Parameters Required"<<endl;
        return 1;
    }
    ifstream ifs1(argv[1]);// mac1617181920
    ifstream ifs2(argv[2]);// newmoveSZ_0316_intCatagory
    ifstream ifs3(argv[3]);// ../safe_wifi_poi_shenzhen
    ofstream ofs(argv[4]);// moveSZ_0316_intCatagory

//这一部分是：根据mac得到含有经纬度的记录
    set<string> macSet;

    string line, mac;
	  while(getline(ifs1, line)){
      istringstream iss(line);
      iss>> mac;
      macSet.insert(mac);
    }


        int count = 0;
        string token, category;
        float lon=0, lat=0;
        while(getline(ifs3,line)){
          istringstream iss(line);
          count = 0;
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
              case 15:
                category = token;
                break;
            }
          }
          macLonlat[mac] = make_pair(lon, lat);
          if(macSet.find(mac) != macSet.end()){
            macSet.erase(mac);
            ofs<< "null" <<'|' <<mac <<'|'<<macLonlat[mac].first<<'|'<<macLonlat[mac].second<<'|'<<"null"<<"|null|"<<category<<endl;
          }
        }

//这一部分是根据safe_wifi_poi_shenzhen,  向newmoveSZ_0316_intCatagory中添加经纬度
/*
    int count = 0;
    string token;
    float lon=0, lat=0;
    while(getline(ifs3,line)){
      istringstream iss(line);
      count = 0;
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
      }
      macLonlat[mac] = make_pair(lon, lat);
    }


    string guid, start_hour, category, intCtgy;
    while(getline(ifs2, line)){
		    istringstream iss(line);
        count = 0;
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
              start_hour = token;
              break;
            case 4:
              category = token;
              break;
            case 5:
              intCtgy = token;
              break;
          }
		    }
        if(macSet.find(mac) != macSet.end()){
            ofs<< guid <<'|' <<mac <<'|'<<macLonlat[mac].first<<'|'<<macLonlat[mac].second<<'|'<<start_hour<<"|null|"<<category<<'|'<<intCtgy<<endl;
        }
	  }
*/
    ifs1.close();
    ifs2.close();
    ifs3.close();
    ofs.close();
    return 0;
}
