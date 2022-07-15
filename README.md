# TCP socket number guessing game
## Introduction
This is my number guessing game using TCP socket and C++.
Course of this project: *11010EECS302001, Introduction to Computer Networks at NTHU*

## Description
Using TCP socket and C++, build a number guessing game for slient and user. The server should allow clients to guess a number between 0 to 999, and reply a guessing result. The client should be able to access the server, guess a number, and read the result replied by the server.

Also use Wireshark to capture the packets transmitted by the server and the client. Mainly observe TCP hand shaking packets and IP of the two characters(server and client). Related Youtube video to observe hand shaking with TCP is [here](https://www.youtube.com/watch?v=4dSaAMZsPvw&t=73s).(very useful)

See more spec details in [spec_2021.pdf](https://github.com/ccl1616/Network_project/blob/main/Document/spec_2021.pdf) 
See my project report in [project_report.pdf](https://github.com/ccl1616/Network_project/blob/main/Document/project_report.pdf)

### Compile
```
    Note: this is for compiling on linux
    gcc -o cli cli.c
    gcc -o ser ser.c
```
### Execute
```
    Note: Execute on two computer/ terminal to represent two player.
    ./cli <IP> <Port number>
    ./ser <Port number>
```
### Document
Here are some notes for files in the /Document folder
- [Socket_tutorial.pdf](https://github.com/ccl1616/Network_project/blob/main/Document/Socket_tutorial.pdf) : Environment setting and Concept for Socket programming
- [Wireshark_tutorial.pdf](https://github.com/ccl1616/Network_project/blob/main/Document/Wireshark_tutorial.pdf) : tutorial for Wireshark
- [project_report.pdf](https://github.com/ccl1616/Network_project/blob/main/Document/project_report.pdf) : my project report, include a full number guessing process
- [spec_2021.pdf](https://github.com/ccl1616/Network_project/blob/main/Document/spec_2021.pdf) : the formal specification of this project
- [windows_mingw.pdf](https://github.com/ccl1616/Network_project/blob/main/Document/windows_mingw.pdf) : tutorial of environment setup for mingw
- [wireshark_loopback_adapter.pdf](https://github.com/ccl1616/Network_project/blob/main/Document/wireshark_loopback_adapter.pdf) : tutorial of install "loopback adaptor" for wireshark on Windows.

