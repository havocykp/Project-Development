# coding:utf-8
from CServe import *
from CDataBase import *

HOST = '127.0.0.1'  #服务器IP地址
PORT = 1234         #服务器端口


def test():
    server = CServeSocket(HOST,PORT)
    server.MyAccept()
    # 下面的代码是和数据库交互用到的
    sql = CSqlForChat()
    while True:
        print("1.查询 2.添加")
        index = int(input())
        if index == 1:
            result = sql.query(input("请输入查询语句"))
            print(result[0])
        elif index == 2:
            result = sql.insert(input("请输入插入语句"))
            if result == 1:
                print("添加成功")
            pass


if __name__ == '__main__':
    test()