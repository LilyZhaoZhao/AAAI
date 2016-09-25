#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
cmdArgv = sys.argv

#相对于sameMac.py 添加了几个其它功能的函数而已


#将两个文件中存在相同mac的行合并，然后得到一个新的utilization——周一&周六的使用模式
def catagory2int(s1,s2):

    #print 'yes'
    #f = open(s1,'r')
    fw = open("mac1617181920v2_0316and0321_Utilizaiton",'w')

    macDict = {}
    with open(s1,'r') as fr1:
        datas = fr1.readlines()
    for line in datas:
        utilizationList = []
        line = line.strip('\n')
        data = line.split(',')
        for i in range(1,25):
            utilizationList.append(data[i])
        macDict[data[0]] = utilizationList
    fr1.close()

    with open(s2,'r') as fr2:
        datas = fr2.readlines()
    for line in datas:
        ulist = []
        line = line.strip('\n')
        data = line.split(',')
        mac = data[0]
        if mac in macDict.keys():
            fw.write(mac)
            ulist = macDict[mac]
            for i in range(24):
                fw.write(','+ulist[i])
            for i in range(1,25):
                fw.write(','+data[i])
            fw.write('\n')

    fw.close()

    fr2.close()


#给utilization文件中的mac添加上对应的category
def mac2intCatg(s1,s2):

    #print 'yes'
    #f = open(s1,'r')
    fw = open(s2+"_intCatg",'w')#mac1617181920v2_0316_Neighbor_intCatg

    macDict = {}
    with open(s1,'r') as fr1: #mac1617181920v2_0316_intCatagory
        datas = fr1.readlines()
    for line in datas:
        line = line.strip('\n')
        data = line.split(',')
        if len(data) == 3:
            macDict[data[1]] = data[2]
    fr1.close()

    with open(s2,'r') as fr2:#mac1617181920v2_0316_Utilization
        datas = fr2.readlines()
    for line in datas:
        line = line.strip('\n')
        data = line.split(',')
        mac = data[0]
        #print mac
        if mac in macDict.keys():
            fw.write(line+','+macDict[mac]+'\n')

    fw.close()

    fr2.close()

#从已知的move记录中筛选出mac在所需的mac集中的记录，用于获取每个mac一天内的访问次数
def macSame(s1,s2):

    #print 'yes'
    #f = open(s1,'r')
    fw = open(s2+"_2",'w')#

    macList = []
    with open(s1,'r') as fr1: #checkins_0322
        datas = fr1.readlines()
    for line in datas:
        line = line.strip('\n')
        data = line.split(',')
        macList.append(data[1])
    fr1.close()

    with open(s2,'r') as fr2:#checkins_0316
        datas = fr2.readlines()
    for line in datas:
        line = line.strip('\n')
        data = line.split(',')
        mac = data[1]
        if mac in macList:
            fw.write(line+'\n')

    fw.close()

    fr2.close()


def unionCheckins(s): #checkins
    macDict = {}
    fw = open(s+"_week",'w')#

    for i in range(16,23):
        s1 = s+"_03"+str(i)+"_2"
        with open(s1,'r') as fr1: #checkins_0322_2
            datas = fr1.readlines()
        for line in datas:
            line = line.strip('\n')
            data = line.split(',')
            mac = data[1]
            if mac in macDict.keys():
                macDict[mac].append(data[0])
            else:
                macDict[mac]=[]
                macDict[mac].append(data[0])
        fr1.close()

    for k,v in macDict.items():
        fw.write(k)
        for vv in v:
            fw.write(','+vv)
        fw.write('\n')
    fw.close()

if __name__=="__main__":

    #catagory2int(cmdArgv[1], cmdArgv[2]) #szPoiba_0316_Utilization, szPoiba_0321_Utilization

    mac2intCatg(cmdArgv[1],cmdArgv[2])

    #macSame(cmdArgv[1],cmdArgv[2])

    #unionCheckins(cmdArgv[1])
'''
    s = cmdArgv[1]
    fw = open(s+"_kuochong",'w')#

    macDict = {}
    with open(s,'r') as fr1: #mac1617181920v2_0316_intCatagory
        datas = fr1.readlines()
    for line in datas:
        line = line.strip('\n')
        data = line.split('|')
        fw.write(line+"|null|null|null|null|null|null|"+data[7]+'\n')
'''
