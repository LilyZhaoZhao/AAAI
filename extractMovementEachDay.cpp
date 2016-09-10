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
map<string, vector<string> > userCtgy;
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
    ifstream ifs1(argv[1]);
    ifstream ifs2(argv[2]);
    ofstream ofs(argv[3]);

    string line;
	string token;
    string mac, tmp;
	while(getline(ifs1, line)){
        istringstream iss(line);
        iss>>mac;
        getline(iss,tmp);
        filter[mac] = tmp;
    }

    //cout<< filter[mac]<<','<< mac<<endl;

    string gotime, guid, ssid, bssid, connect_time;
    time_t t;
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
                char date[50];
                strftime(date, sizeof(date), "%Y-%m-%d %T", tm);
                connect_time = string(date);
                break;
            }
		}
        if(filter.find(bssid)!=filter.end() && userTime[guid].find(t/900) == userTime[guid].end()){

            userLog[bssid].insert(gotime+','+guid+','+filter[bssid]+','+connect_time);
            userMac[bssid].insert(guid);
            userTime[guid].insert(t/900);
            //userCtgy[guid].push_back(filter[bssid]);
        }
	}

    vector<PAIR > cPair(userLog.begin(),userLog.end());
    sort(cPair.begin(), cPair.end(), cmp_by_value);

    //if(cPair.size() < 10){
    string m;
    for(int i=0; i<cPair.size(); ++i){
      //  if(cPair[i].second.size() < 100){
            m = cPair[i].first;
            ofs<< m <<' '<< filter[m]<<' '<<cPair[i].second.size()<<' ' <<userMac[m].size() <<endl;
            //int count = 0;
        //    for(set<string>::const_iterator it2=cPair[i].second.begin();
        //        it2!=cPair[i].second.end(); ++it2){
            //    count++;
        //        ofs<<cPair[i].first<<','<<*it2<<endl;
        //    }
            //ofs<<endl<<endl;
      //  }
    }

/*
    for(map<string, vector<string> >::const_iterator itr=userCtgy.begin();itr!=userCtgy.end(); ++itr){
      for(vector<string>::const_iterator itr2=itr->second.begin();itr2!=itr->second.end();++itr2){
        ofs<< *itr2 <<' ';
      }
      ofs<<'\n';
    }
*/

    ifs1.close();
    ifs2.close();
    ofs.close();
    return 0;
}
