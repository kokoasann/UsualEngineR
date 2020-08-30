#coding:utf-8

#IntelliSenseじゃ反応してくれないようなモノのリネーム等を一括で行うスクリプト
#舛添(巻き添え)に注意

import glob
import os
import chardet


def encode(p,org,rnm):
    sc = None
    enc = None
    with open(p,"rb") as f:
        enc =chardet.detect(f.read())["encoding"]
        print(enc)
    if "Windows" in enc:
        enc = "Shift-JIS"

    with open(p,"r",encoding=enc) as f:
        sc=f.read()

    if not org in sc:
        print("変更対象なし")
        return
    sc = sc.replace(org,rnm)
    print(sc)
    with open(p,"w",encoding="utf-8") as f:
        f.write(sc)


path = input("file or folder\n<<< ")
org = input("変更対象ワード\n<<< ")
rnm = input("変更後ワード\n<<< ")

if os.path.isdir(path):
    ps = glob.glob(path+"\\*.h")
    ps += glob.glob(path+"\\*.cpp")
    for file in ps:
        encode(file,org,rnm)
else:
    encode(path,org,rnm)
