# FAUSTO Kernel
_"FAUSTO hA Usurpato Simultaneamente Tre Orcolat"_ is a **free** and **open-source kernel** developed by two solo developers located in Italy. The project is currently being worked on and Work In Progress.

## Architecture
The goal of this kernel is begin able to run on older machines with Intel 80386 processors, this means that all build targets will be set for i386. The kernel can still be ran by newer processors with x86_64 architecture if the machine supports _LEGACY_ boot mode.

## Features

# Boot
The kernel currently offers full multiboot1 capabilites and comes packaged with a lightweight bootloader written in assembly. 
# Memory
The kernel supports paging and dynamic memory allocation via _kmalloc_ and _kfree_ functions built in the memory section of the kernel. **The size of the heap is not dynamic**, therefore you might have to change it manually to unleash the full potential of your target machine.
# Libc
The kernel offers a small cluster of libc implemented functions that will be expanded in the future
# Logging
The kernel also offers logging for system-level fuctions and sections of itself and checksums for all of them. This results in easy debugging.

## Why FAUSTO kernel?
The FAUSTO kernel is incredibily lightweight, it's entire creation has been designed specifically to fit on a floppy disk and to be ran on i386 processors. Lightweight also means lack of features and security, this is caused by the low capabilites of the target machines it has been made for. You aren't at risk of cyberattack tho, don't worry! We dont have networking drivers :)
