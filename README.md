# Gamma-Correction-Multiprocessing

In this assignment, suppose you are tasked to speed up OpenCV gamma correction algorithm for imaging on a mobile device for a software company. To achieve speedup, suppose you decided to break down the image correction into four concurrent processes on your Raspberry Pi (since it has four cores). 
One way to communicate between processes is through named pipes. However, pipes have limited data capacity that cannot handle large images. Thus you decide to use shared memory. This example shows to share memory with between a parent and a child process without a backing file. If you would like to read more about IPC, this document provides three different ways to carry out IPC.

Here are the steps you will need to process the image in parallel on multiple cores using shared memory and IPC.

1. Read in the image file using OpenCV imread() and store into a Mat object. Let us call this the original image. 
2. Create a shared memory segment the size of the original image in bytes. To get the size of the original image in bytes, use the Mat object methods total() and elemSize():

```C++
unsigned size_in_bytes = image.total() * image.elemSize();
caddr_t memptr = (caddr_t) mmap (NULL,
                                 size_in_bytes,   /* how many bytes */
                                 PROT_READ | PROT_WRITE, /* access protections */
                                 MAP_SHARED | MAP_ANONYMOUS, /* mapping visible to other processes */
                                 -1,         /* no file descriptor is needed */
                                 0);         /* offset: start at 1st byte */
```
3. Create a new image in the shared memory segment with the same size and type as the original image. Use the memory pointer from step 2:
```C++
Mat new_image (image.size(), image.type(), memptr);
```
4.  Cut the original and the new image into parts such as in this example code. Note that the cut parts using the Rect call return pointers within the image. They do not create new images. 
5.  Create children processes using fork() in a loop. For the child section of the code, run the gamma correction only on the cut part for that particular child. The children will modify the image in the shared segment. 
6.  Let the parent process wait for the children to finish using wait():
```C++
for (int i = 0; i < logic_processors; i++) {    /* parent */
         // wait for children to finish.
         wait(NULL);
}
```

7.  Display the original and new image for comparison
8. Free the shared memory segment. 

Make sure to use the timer function such as in the example code in step 4. 
Note: to measure speedup, first implement gamma correction using only one process (from this example). Then, make a second version of the code that you modify to spawn four processes. Measure the processor time with the second version and take the ratio.  Remember the speedup factor:  .

## Rubric for grading:
- Spawn four child processes properly and have parent process wait for them: 10
- Divide the gamma correction of the image into four parts among the children: 10
- (Speedup > 2): 7
- (Speedup > 2.8): 3  

Total = 30 points. 

## Deliverables

Submit both your old code to process the image using a single process and your new parallel IPC using sharing memory code with CMakeLists.txt file.
