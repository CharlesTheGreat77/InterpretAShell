# InterpretAShell
Payload to go along with Black Hill Security's Shellcode series for bypassing EDR by simply executing shellcode within a python interpreter.


# Description
This is a method to execute shellcode within a given windows machine. This goes about checking whether python is installed and executes if so. Python interpreters can be a niche way of bypassing EDR systems when LotL and other ways seem to be sus.
This is a very basic (ugly) script to check whether python is installed and execute shellcode within such.

The python series by Black Hills Sec. can be found here can be found here: https://www.youtube.com/watch?v=ItU_V1uVaPw

# Usage
1. Create your shellcode
Example:
```bash
msfvenom -p windows/x64/exec CMD="powershell \"whoami\"" -f python | tr '"' "'" > payload 
```
* Ideally implement your own powershell payloads for the sake of bypass EDR and AV solutions
2. Paste in shellcode into **command** variable in *.cpp* file
3. compile and send to target machine.

# Considerations
* Consider encoding/encrypting the shellcode if storing such within the executable.
* Consider potentially hosting the shellcode externally for one to process, decode, and execute.
* Consider additional execution methods, or potentially injecting into other processes.

# Note
The execution method used to handle the shellcode can be found here
https://github.com/RiverGumSecurity/PythonShellcode

