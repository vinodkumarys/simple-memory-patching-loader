# simple-memory-patching-loader

A POC to loaded a Win32 exe and then patch its instruction in memory.

Test: 
1. Run sim.exe. It shows "This is a unregistered copy!" message.
2. Run loader.exe which starts sim.exe and patches the code which checks for regristration. You will see "This is a registered copy!" message.
