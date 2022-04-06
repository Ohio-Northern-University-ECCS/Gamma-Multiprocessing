# OS-Gamma-Multiprocessing
Multiprocessing with Shared Memory for Image Gamma Correction
In this assignment, suppose you are tasked to speed up OpenCV gamma correction algorithm for imaging on a mobile device for a software company. To achieve speedup, suppose you decided to break down the image correction into four concurrent processes on your Raspberry Pi (since it has four cores). 
One way to communicate between processes is through named pipes. However, pipes have limited data capacity that cannot handle large images. Thus you decide to use shared memory. This example shows to share memory with between a parent and a child process without a backing file. If you would like to read more about IPC, this document provides three different ways to carry out IPC.
