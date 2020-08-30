#coding:utf-8

#TKEngine仕様をUsualEngine仕様に変換!
#多分もう使わない

import glob
import os
import chardet


def encode(p):
    sc = None
    enc = None
    with open(p,"rb") as f:
        enc =chardet.detect(f.read())["encoding"]
        print(enc)
    if "Windows" in enc:
        enc = "Shift-JIS"
        
    with open(p,"r",encoding=enc) as f:
        sc=f.read()
        
    sc = sc.replace("stdafx","PreCompile")
    sc = sc.replace("TkEngine","UsualEngine")
    sc = sc.replace("tkEngine","UsualEngine")
    print(sc)
    with open(p,"w",encoding="utf-8") as f:
        f.write(sc)

path = input("folder<<< ")
#path = os.path.dirname(__file__)
files = glob.glob(path+"\\*.h")
files += glob.glob(path+"\\*.cpp")

print(files)
for file in files:
    encode(file)

