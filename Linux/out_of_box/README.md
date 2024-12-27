AIPU LINUX DRIVER RELEASE OUT-Of-BOX README
===========================================
This README is for Zhouyi driver release out-of-box package.

In this package, we provide a simple user application demo which
allow you to run an inference task on x86-Linux simulator as target platform.  

The demo application source code is also an example of the programming model.  
You may update it based on the specification in AIPU Software Programming Guide
if you need to support more features.  

Before running this demo, you should ensure follows:
1. Change `<YOUR_WORKSPACE>` to actural path in `out-of-box-test.sh` accordingly, and it will recover after running automatically
2. Move corresponding AIPU target benchmark folder to 'out_of_box/benchmarks', and ensure it has aipu.bin,input0.bin,output.bin files

To build & run this demo application, please follow these steps:

1. Compile and execute the applicateion with provided benchmark resnet_50 by:
```bash
    $ ./out-of-box-test.sh -a X3_1304MP2 -c resnet_50_x3 # for X3
    $ ./out-of-box-test.sh -a X2_1204MP3 -c resnet_50_x2 # for X2
    $ ./out-of-box-test.sh -s X1 -c resnet_50_x1         # for X1
```

2. A log "[TEST INFO] Test Result Check PASS!" will be printed
    which means that the execution is successful.

note:
1. this OOB is only for simulator platform
2. you can only execute the application with provided benchmark resnet_50 after corresponding target build, by:  
- for aipu v1/v2(1 core)  
```bash
$ ./run.sh -c resnet_50_x1 -s X1
```
- for aipu v3(3 cores)  
```bash
$ ./run.sh -a X2_1204MP3 -c resnet_50_x2
```
- for aipu v3.1(3 cores)  
```bash
$ ./run.sh -a X3_1304MP2 -c resnet_50_x3
```
