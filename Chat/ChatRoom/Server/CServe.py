# coding:utf-8
from socket import *
import threading
import struct
from enum import Enum
import time
from CDataBase import *

class EnumMessageType(Enum):
    ANONYMOUS = 1
    CHAT = 2
    ONE2ONE = 3
    REGISTER = 4
    LOGIN = 5
    ADDFRIEND = 6
    SEARCHUSER = 7
    FILETRANS = 8
    MSGRECORD = 9
    UPDATEUSER = 10

class CServeSocket():
    def __init__(self,ip,port):
        ADDR = (ip,port)
        #初始化 socket
        print ("正在启动服务器启动...")
        self.socketServer = socket(AF_INET,SOCK_STREAM)
        self.socketServer.bind(ADDR)
        self.socketServer.listen(SOMAXCONN)
        print("服务器启动成功,等待客户端连接...")
    #外部调用的accept接口
    def MyAccept(self):
        # 创建线程
        t = threading.Thread(target = self.__acceptProc__)
        t.start()
    # accept的回调函数
    def __acceptProc__(self):
        while True:
            # __accept返回的是个元祖(套接字,客户端地址)
            socketClient, addrClient = self.socketServer.accept()
            # socketClient.send("连接成功!".encode('gbk'))
            #CServerSocket.dictClient[socketClient] = None
            # 创建单独线程等待客户端消息
            # 创建线程接收消息
            t = threading.Thread(target = self.__recvProc__,args=(socketClient,))
            t.start()
    #接收消息的线程
    def __recvProc__(self,s):
        while True:
            try:
                message = s.recv(CServeSocket.BUFSIZE + 10)
                # 消息类型
                type, = struct.unpack("i",message[:4])
                CServeSocket.dictFun[type](s,message)
            except:
                name = CServeSocket.dictClient.get(s)
                if name == None:
                    return
                s.close()
                name = CServeSocket.dictClient.pop(s)
                print("客户端退出:"+name)
                CServeSocket.UpdateUser(s,False,name)
                return

    def __ChatForAnonymous__(s,msg):
        dwLen, = struct.unpack("L",msg[4:8])
        buf, = struct.unpack("%ds"%dwLen,msg[8:8 + dwLen])
        name = buf.decode("gbk")
        print(name+"加入聊天室")
        CServeSocket.dictClient[s] = name.rstrip('\0')
        for each in CServeSocket.dictClient:
            each.send(msg)
        #通知给个客户端更新在线用户列表
        CServeSocket.UpdateUser(s,True,name)

    def UpdateUser(s,bAdd,name):
        try:
            message_type = EnumMessageType.UPDATEUSER
            message = name.encode("gbk")
            message_len = len(message)
            message_send = struct.pack("lll2040s",message_type.value,bAdd,message_len,message)
            for each in CServeSocket.dictClient:
                if each == s:
                    continue
                each.send(message_send)
            if bAdd == False:
                return

            for each in CServeSocket.dictClient:
                if each == s:
                    continue
                message = CServeSocket.dictClient[each].encode("gbk")
                message_len = len(message)
                message_send = struct.pack("lll2040s",message_type.value,bAdd,message_len,message)
                s.send(message_send)
        except:
            return

    #匿名聊天
    def __ChatForChat__(s,msg):
        dwLen, = struct.unpack("L",msg[4:8])
        buf, = struct.unpack("%ds" % dwLen,msg[8:8 + dwLen])
        #解密
        buf = bytearray(buf)
        for i in range(dwLen):
            buf[i] ^= 15

        message_recv = buf.decode("gbk")
        print(message_recv)
        for each in CServeSocket.dictClient:
            if each == s:
                continue
            each.send(msg)

    # 1V1聊天 服务器要把A发来的消息 B:你好,换成A:你好
    def __ChatForOne2One__(s,msg):
        name, = struct.unpack("50s",msg[4:54])
        name = name.decode("gbk").rstrip('\0')
        for each in CServeSocket.dictClient:
            if name == CServeSocket.dictClient[each]:
                name = struct.pack("50s",CServeSocket.dictClient[s].encode('gbk'))
                each.send(msg[:4]+name+msg[54:])
                break
        # 是否保存聊天记录
        # 消息双方姓名
        msgFrom = CServeSocket.dictClient[s]
        msgTo, = struct.unpack("50s",msg[4:54])
        msgTo = msgTo.decode("gbk").rstrip('\0')
        # 消息内容
        msginfo, = struct.unpack("1024s",msg[54:54+1024])
        msginfo = msginfo.decode("gbk").rstrip('\0')
        # 把消息添加到数据库,数据库设置外键了，只会添加双方都是已注册用户的聊天信息
        CServeSocket.conn.insert("insert into msginfo(userfrom,userto,msgcontent) values(%s,%s,%s)",(msgFrom,msgTo,msginfo))


    def __ChatForLogin__(s,msg):
        name, = struct.unpack("50s",msg[4:54])
        pwd, = struct.unpack("50s",msg[54:104])
        name = name.decode('gbk').rstrip('\0')
        pwd = pwd.decode('gbk').rstrip('\0')
        # 构造查询语句
        result = CServeSocket.conn.query("SELECT * from userinfo where name=%s and pwd=%s",(name,pwd))
        #返回登录结果
        #登录失败
        message_type = EnumMessageType.LOGIN
        message_len = 50
        message = ""
        if result == None or result[1] == 0:
            message = "登录失败!".encode('gbk')
        else:
            message = "登录成功!".encode("gbk")
        message_send = struct.pack("l2048s",message_type.value,message)
        s.send(message_send)

    def __ChatForRegister__(s,msg):
        name, = struct.unpack("50s",msg[4:54])
        pwd, = struct.unpack("50s",msg[54:104])
        name = name.decode('gbk').rstrip('\0')
        pwd = pwd.decode('gbk').rstrip('\0')
        # 构造查询语句
        result = CServeSocket.conn.insert("insert into userinfo(name,pwd) VALUES(%s,%s)",(name,pwd))
        # 返回登录结果
        # 登录失败
        message_type = EnumMessageType.REGISTER
        message_len = 50
        message = ""
        if result == None:
            message = "注册失败!".encode("gbk")
        else:
            message = "注册成功!".encode("gbk")
        message_send = struct.pack("l2048s",message_type.value,message)
        s.send(message_send)

    def __ChatForAddFriend__(s,msg):
        name, = struct.unpack("50s",msg[4:54])
        frd, = struct.unpack("50s",msg[54:104])
        name = name.decode('gbk').rstrip('\0')
        frd = frd.decode('gbk').rstrip('\0')
        # 构造查询语句
        result = CServeSocket.conn.insert("insert into userfriend(name,friend) VALUES (%s,%s)",(name,frd))
        # 返回查询结果
        message_type = EnumMessageType.ADDFRIEND
        message_len = 50
        message = ""
        if result == None:
            message = "添加好友失败!".encode('gbk')
        else:
            message = "添加好友成功!".encode('gbk')
        message_send = struct.pack("l2048s",message_type.value,message)
        s.send(message_send)

    def __ChatForSearUser__(s,msg):
        name, = struct.unpack("50s",msg[4:54])
        name = name.decode('gbk').rstrip('\0')
        # 构造查询语句
        result = CServeSocket.conn.query("SELECT name from userinfo where name=%s",(name,))
        # 返回搜索结果
        message_type = EnumMessageType.SEARCHUSER
        message_len = 50
        message = ""
        if result == None or result[1] == 0:
            message = "查无此人!".encode('gbk')
        else:
            if name in CServeSocket.dictClient.values():
                message = "用户在线，双击列表内用户名1V1聊天!".encode('gbk')
            else:
                message = "用户不在线！".encode('gb2312')
            message_send = struct.pack("l2048s",message_type.value,message)
            s.send(message_send)

    def __ChatForGetMsgRecord__(s,msg):
        name = CServeSocket.dictClient[s]
        # 查询所有信息
        result = CServeSocket.conn.query("select * from msginfo where userfrom=%s or userto=%s",(name,name))
        if result == None or result[1] == 0:
            return
        message_type = EnumMessageType.MSGRECORD
        for i in range(result[1]):
            print(str(i)+":")
            print("from:"+result[0][i][0])
            print("to:"+result[0][i][1])
            print("content:"+result[0][i][2])
            # 把每条信息分别打包发送给客户端s
            msgFrom = result[0][i][0].encode('gbk')
            msgTo = result[0][i][1].encode('gbk')
            msgContent = result[0][i][2].encode('gbk')
            msgSend = struct.pack("l50s50s1948s",message_type.value,msgFrom,msgTo,msgContent)
            s.send(msgSend)
            # 最后发个END过去,告诉客户端聊天记录全部发完
            msgFrom = "~~~end~~~".encode("gbk")
            msgSend = struct.pack("l2048s",message_type.value,msgFrom)
            s.send(msgSend)

    dictFun = {
        1:__ChatForAnonymous__,
        2:__ChatForChat__,
        3:__ChatForOne2One__,
        4:__ChatForRegister__,
        5:__ChatForLogin__,
        6:__ChatForAddFriend__,
        7:__ChatForSearUser__,
        9:__ChatForGetMsgRecord__,
    }
    BUFSIZE = 2048+4
    dictClient = {}
    conn = CSqlForChat()