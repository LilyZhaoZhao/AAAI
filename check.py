#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
cmdArgv = sys.argv


if __name__=="__main__":
    s1 = cmdArgv[1] #../categoryValidation/unaccuracy
    s2 = cmdArgv[2] #sampling_wifi_poi_sampingv2_0316_Neighbor

    fw = open(s2+"_neighborNum",'w')
    fr = open(s1,'r')
    macList = []
    for line in fr.readlines():
        line = line.strip('\n')
        data = line.split(',')
        macList.append(data[0])
    fr.close()

    fr = open(s2,'r')

    count = 0
    for line in fr.readlines():
        line = line.strip('\n')
        data = line.split(',')
        sum = 0
        mac = data[0]
        if mac in macList:
            for i in range(2,14):
                sum += int(data[i])
            fw.write(mac +','+str(sum)+'\n')#mac, 邻域ap个数（包含自己）
            if sum == 1:
                count += 1
    fr.close()
    fw.close()

    print count
    print float(count)/354
    #print float(count)/11969
