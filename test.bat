@echo off
cl /O2 /LD /I "%JAVA_HOME%/include" -I "%JAVA_HOME%/include/win32" jthreadmagic.c
javac -d bin/ src/rip/mem/jthreadmagic/JThreadMagic.java src/rip/mem/jthreadmagic/JThreadMagicTest.java
java -cp bin/ rip.mem.jthreadmagic.JThreadMagicTest