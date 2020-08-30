#coding:utf-8

#一括でnamespaceを付けるためだけのスクリプト
#多分もういらない

import glob
import os
import chardet

nss = ["namespace UER\n","{\n"]
fs = "}\n"
tab = "\t"

inc = "#include"
prg = "#pragma"


def encode(p):
    sc = None
    enc = None
    with open(p,"rb") as f:
        enc =chardet.detect(f.read())["encoding"]
        print(enc)
    if "Windows" in enc:
        enc = "Shift-JIS"

    with open(p,"r",encoding=enc) as f:
        sc = f.readlines()

    print(sc)

    lastINC = 0
    for i,s in enumerate(sc):
        if inc in s or prg in s:
            lastINC = i+1

    for i in range(lastINC,len(sc)):
        s = list(sc[i])
        s.insert(0,tab)
        sc[i] = "".join(s)

    sc.insert(lastINC,"\n")
    sc.insert(lastINC,"\n")

    sc.insert(lastINC+2,nss[0])
    sc.insert(lastINC+3,nss[1])

    sc.append("\n\n"+fs)

    #print("".join(sc))
    with open(p,"w",encoding="utf-8") as f:
        f.writelines(sc)

        
p = input("file or folder\n<<< ")
if os.path.isdir(p):
    ps = glob.glob(p+"\\*.h")
    ps += glob.glob(p+"\\*.cpp")
    for file in ps:
        encode(file)
else:
    encode(p)
