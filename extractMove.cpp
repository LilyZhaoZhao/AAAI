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

map<string, string> filter;
map<string, set<string> > userLog;
map<string, set<string> > userMac;
map<string, set<int> > userTime;

typedef pair<string, set<string> > PAIR;

bool cmp_by_value(const PAIR& lhs, const PAIR& rhs){
    return lhs.second.size() > rhs.second.size();
}

int main(int argc, char* argv[]){
    if(argc<4){
        cout<<"More Parameters Required"<<endl;
        return 1;
    }
    ifstream ifs1(argv[1]);//../safe_wifi_poi_shenzhen
    ifstream ifs2(argv[2]);// ../../TecentData/wifi_conn/20150316/safe_wifi_connect_sample_export
    ofstream ofs(argv[3]);

    string line;
	  string token;
    string mac, tmp;
	  while(getline(ifs1, line)){
    istringstream iss(line);
    int count=0;
    while(getline(iss, token, '|')){
      count ++;
            switch(count){
            case 2:
                mac = token;
                break;
            case 15:
                tmp = token;
                break;
            }
    }
    filter[mac] = tmp;

  }


    //cout<< filter[mac]<<','<< mac<<endl;

    string gotime, guid, ssid, bssid, connect_time;
    time_t t;
    int start_hour = 0;
    while(getline(ifs2, line)){
		istringstream iss(line);
		int count=0;
		while(getline(iss, token, '|')){
			count ++;
            switch(count){
            case 1:
                gotime = token;
                break;
            case 2:
                guid = token;
                break;
            case 4:
                bssid = token;
                break;
            case 6:
                connect_time = token.substr(0,10);
                t  = atoi(connect_time.c_str());
                struct tm* tm = localtime(&t);
                start_hour = int(tm->tm_hour);
                std::stringstream ss;
                ss << start_hour;
                ss >> connect_time;
                //connect_time = string(start_hour);
                break;
            }
		}
        //&& userMac[guid].find(bssid) == userMac[guid].end()&&
        if(filter.find(bssid)!=filter.end() && userTime[guid].find(t/900) == userTime[guid].end()){
          //  cout<< "yes"<<endl;
            userLog[guid].insert(bssid+','+connect_time+','+filter[bssid]);
            userMac[guid].insert(bssid);
            userTime[guid].insert(t/900);
        }
	}

    vector<PAIR > cPair(userLog.begin(),userLog.end());
    sort(cPair.begin(), cPair.end(), cmp_by_value);

    //if(cPair.size() < 10){
    for(int i=0; i<cPair.size(); ++i){
        if(cPair[i].second.size() < 100){
            for(set<string>::const_iterator it2=cPair[i].second.begin();
                it2!=cPair[i].second.end(); ++it2){
                ofs<<cPair[i].first<<','<<*it2<<endl;
            }
        }
    }
   /* }
    else{
        for(int i=0; i<10; ++i){
            ofs<<cPair[i].first<<' '<<cPair[i].second.size()<<endl;
            for(set<string>::const_iterator it2=cPair[i].second.begin();
                it2!=cPair[i].second.end(); ++it2)
                ofs<<*it2<<endl;
            ofs<<endl<<endl;
        }
    }*/
    ifs1.close();
    ifs2.close();
    ofs.close();
    return 0;
}
