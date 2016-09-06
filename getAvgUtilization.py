#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
cmdArgv = sys.argv

numCatg = 12

#根据已有的每个ap的utilization，计算出每种category的平均utilization（这里是百分比）
def func_Utilization(s1, s2, s3):
    fr1 = open(s1, 'r')#safe_wifi_poi_catg12v2_0316_Utilization
    fr2 = open(s2, 'r')#safe_wifi_poi_catg12v2_0316_intCatagory
    fw = open(s3,'w')

    macCtgy = {}
    for line in fr2.readlines():
        line = line.strip('\n')
        data = line.split('|')
        mac = data[1]
        catg = int(data[7])
        macCtgy[mac] = catg
    fr2.close()

    avg = {}
    countList = [0]*numCatg
    for line in fr1.readlines():
        line = line.strip('\n')
        data = line.split(',')
        sumU = 0
        mac = data[0]
        catg = macCtgy[mac]
        countList[catg-1] += 1
        length = len(data)
        for i in range(1,length):
            sumU += int(data[i])
        if catg not in avg.keys():
            avg[catg] = [0]*(length-1)
        for i in range(1,length):
            if sumU != 0:
                avg[catg][i-1] += float(data[i])/sumU
    fr1.close()

    for c in range(1,numCatg+1):
        fw.write(str(c))
        for i in range(24):
            avg[c][i] /= float(countList[c-1])
            fw.write(','+str(avg[c][i]))
        fw.write('\n')
    fw.close()

def func_neighbor(s1, s3):
    fr1 = open(s1, 'r')#safe_wifi_poi_catg12v2_0316_Neighbor
    fw = open(s3,'w')

    avg = {}
    countList = [0]*numCatg
    for line in fr1.readlines():
        line = line.strip('\n')
        data = line.split(',')
        sumU = 0
        catg = int(data[0])
        countList[catg-1] += 1
        length = len(data)
        for i in range(1,length):
            sumU += int(data[i])
        if catg not in avg.keys():
            avg[catg] = [0]*(length-1)
        for i in range(1,length):
            if sumU != 0:
                avg[catg][i-1] += float(data[i])/sumU
    fr1.close()

    for c in range(1,numCatg+1):
        fw.write(str(c))
        for i in range(12):
            avg[c][i] /= float(countList[c-1])
            fw.write(','+str(avg[c][i]))
        fw.write('\n')
    fw.close()

if __name__=="__main__":

    s = cmdArgv[1] #safe_wifi_poi_catg12v2_0316

#    func_Utilization(s+'_Utilization',s+'_intCatagory',s+'_avgUtilization')
    func_neighbor(s+'_Neighbor',s+'_avgNeighbor') #从mostSimilar.cpp中可以得出
