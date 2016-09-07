#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
cmdArgv = sys.argv

import numpy as np

def metrics(pre1,pre2,classN=12):
    k1=np.zeros([classN,])
    k2=np.zeros([classN,])
    kx=np.zeros([classN,])
    n=np.size(pre1)
    for i in range(n):
        p1=pre1[i]
        p2=pre2[i]
        k1[p1]=k1[p1]+1
        k2[p2]=k2[p2]+1
        if p1==p2:
            kx[p1]= kx[p1]+1

    pe=np.sum(k1*k2)/n/n
    pa=np.sum(kx)/n
    kappa=(pa-pe)/(1-pe)
    return kappa

# 自己写的计算kappa值
def calKappa(p,r,classN = 12):
    cList = [[0]*3 for i in range(classN)] #存储内容为： 预测为该类别的个数，实际为该类别的个数，预测为该类别正确的个数
    n = len(p)
    for i in range(n):
        cList[int(p[i])-1][0] += 1
        cList[int(r[i])-1][1] += 1
        if p[i]==r[i]:
            cList[int(r[i])-1][2] += 1
#    return cList

#def calAccuracy(cList, classN = 16):
    po = 0
    sum1 = 0
    sum2 = 0
    #print '每一类的预测正确的个数，每一类实际的ap个数: '
    rightAp = []
    realAp = []
    for i in range(classN):
        sum1 += cList[i][2]
        sum2 += (cList[i][0] * cList[i][1])
        rightAp.append(cList[i][2])
        realAp.append(cList[i][1]) #每一类的预测正确的个数，每一类实际的ap个数

    po = float(sum1)/n #就是准确率
    pe = float(sum2)/(n**2)
    kappa = float(po - pe)/(1 - pe)
    print "kappa is: "+str(kappa)
    print "accuracy is: "+str(po)


if __name__=="__main__":
    s = cmdArgv[1] #split_neighborSimilar2
    fr = open(s,'r')
    r = []
    p = []
    for line in fr.readlines():
        line = line.strip('\n')
        data = line.split(',')
        r.append(int(data[1]))
        p.append(int(data[2]))
    fr.close()

    #metrics(r,p,12)
    calKappa(r,p,12)
