#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
cmdArgv = sys.argv

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
        data = line.split('|')
        if len(data) == 8:
            macDict[data[1]] = data[7]
    fr1.close()

    with open(s2,'r') as fr2:#mac1617181920v2_0316_Utilization
        datas = fr2.readlines()
    for line in datas:
        ulist = []
        line = line.strip('\n')
        data = line.split(',')
        mac = data[0]
        if mac in macDict.keys():
            fw.write(line+','+macDict[mac]+'\n')

    fw.close()

    fr2.close()

#从已知的move记录中筛选出mac在所需的mac集中的记录，然后获取每个mac一天内的访问次数
def macSame(s1,s2):

    #print 'yes'
    #f = open(s1,'r')
    fw = open("week/"+s2+"_intCatg",'w')#

    macDict = {}
    with open(s1,'r') as fr1: #mac1617181920v2_0316_intCatagory
        datas = fr1.readlines()
    for line in datas:
        line = line.strip('\n')
        data = line.split('|')
        if len(data) == 8:
            macDict[data[1]] = data[7]
    fr1.close()

    with open(s2,'r') as fr2:#moveSZ_0321
        datas = fr2.readlines()
    for line in datas:
        ulist = []
        line = line.strip('\n')
        data = line.split(',')
        mac = data[1]
        if mac in macDict.keys():
            fw.write(data[0]+','+mac+','+macDict[mac]+'\n')

    fw.close()

    fr2.close()

if __name__=="__main__":

    #catagory2int(cmdArgv[1], cmdArgv[2]) #szPoiba_0316_Utilization, szPoiba_0321_Utilization

    #mac2intCatg(cmdArgv[1],cmdArgv[2])

    macSame(cmdArgv[1],cmdArgv[2])
