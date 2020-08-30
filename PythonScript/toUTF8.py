#coding:utf-8

#ファイルの文字コードをutf-8に変換!

import glob
import os
import chardet

def encode(p):
    sc = None
    enc = None
    with open(p,"rb") as f:
        enc =chardet.detect(f.read())["encoding"]
        print(enc)
    if enc == "utf-8":
        print("it is UTF-8!!!")
        return
    elif "Windows" in enc:
        enc = "Shift-JIS"
        
    with open(p,"r",encoding=enc) as f:
        sc=f.read()
    with open(p,"r",encoding="utf-8") as f:
        f.write(sc);


path = input("file or folder\n<<< ")

if os.path.isdir(path):
    ps = glob.glob(path+"\\*.h")
    ps += glob.glob(path+"\\*.cpp")
    for file in ps:
        encode(file)
else:
    encode(path)
