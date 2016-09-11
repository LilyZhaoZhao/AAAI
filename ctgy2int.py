#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
cmdArgv = sys.argv


#将poi数据中的catagory用数字代替
#隐藏作用： 不属于这12个类别的记录则不会被输出，即自动过滤了。
def catagory2int(s1):
    #with open(s1,'r') as f: # safe_wifi_poi_newCatg
    #    datas = f.readlines()
    #print 'yes'
    #f = open(s1,'r')
    fw = open(s1+'_intCatagory','w')# moveSZ_0316_intCatagory

    #catagoryDict1 = {"美食":4,"酒店宾馆":5,"运动健身":6,"旅游景点":8}#一级category
    catagoryDict1 = {"美食":11,"酒店宾馆":12}#一级category
    #catagoryDict2 = {"综合商场":1,"综合医院":2,"大学":3,"咖啡厅":4,"KTV":7,"夜总会":7,"洗浴推拿足疗":7,"酒吧":7}#二级category
    catagoryDict2 = {"KTV":1,"咖啡厅":2,"电影院":3,"住宅区":4,"商务楼宇":5,"大学":6,"小学":7,"便利店":8,"综合商场":9,"综合医院":10}#二级category
    #for line in open(s1,'r'):
    with open(s1) as infile:
        for line in infile:
    #for line in datas:
            line = line.strip('\n')
            data = line.split('|')
            if(len(data)==7):
                ctgy = data[6]
                ctgy = ctgy.split(';')
            #print ctgy[1]
                if ctgy[0] in catagoryDict1.keys():
                    catagory = catagoryDict1[ctgy[0]]
                    fw.write(line+'|'+str(catagory)+'\n')
                elif ctgy[1] in catagoryDict2.keys():
                    catagory = catagoryDict2[ctgy[1]]
                    fw.write(line+'|'+str(catagory)+'\n')
            #if ctgy[1] in catagoryDict2.keys():
            #    catagory = catagoryDict2[ctgy[1]]
            #    fw.write(line+'|'+str(catagory)+'\n')
    fw.close()
    infile.close()

if __name__=="__main__":

    catagory2int(cmdArgv[1]) #moveSZ_0316

    #sortByLonLat(cmdArgv[1],cmdArgv[2])
