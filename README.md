# 🧠 Custom Malloc Implementation in C

My lightweight implementation of **dynamic memory allocation** in C.  
This project implements `malloc` and `free` using a **free list structure**, **first-fit allocation algorithm**,
and **coalescing of free blocks**.
If you are running the program using the _CMakesLists.txt_ make sure to remove the _tests/_ directory; feel free to create your own tests as I have not included mine here.
---
The inspiration behind this project came after reading the _Free-Space Management_ chapter from:
***Operating Systems: Three Easy Pieces.***
