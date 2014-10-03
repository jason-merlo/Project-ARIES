#Project ARIES
###Background
Project ARIES was originally designed with the intent of creating a scale car capable of autonomously and intelligently identify obstacles approaching on its course and stop or alter course before a collision occurs.  I started this project as a side project the summer before my Senior year of high school and continued it throughout the year as an independent study project.

###Top Level Controls Layout
![Controls Flow Diagram](https://raw.githubusercontent.com/SuperUser320/Project-ARIES/master/Flow%20Diagrams/ARIES%20Presentation.key/Data/Autonomous%20Flow%20Diagram-89.png)
On the highest level the project consists of two components, the dashboard which is run from a laptop, and the vehicle itself.  All low power and control processes are run onboard either on the microcontroller (Arduino Pro) or the ARM based SBC (BeagleBoard-XM) running Arch Linux.  

###Onboard Processes Breakdown
######BeagleBoard-XM - C++ & Bash
The onboard BeagleBoard has the sole task of transmitting the two simultaneous video streams in 320x240 resolution using _mjpg-streamer_, an open source streaming program developed for Linux.  The BeagleBoard communicates sensor readings and transmits video via WiFi to a router on the same network as the dashboard.  Originally the Arduino also communicated with the BeagleBoard via a standard USB port BeagleBoard, however due to software lag a separate FM radio communication channel interfacing directly with the dashboard was implemented for a reduced latency form of control. 

######Arduino Pro - C
The Arduino is tasked with all the low level I/O on the car, this includes sampling battery voltages, reading the turning potentiometer, and _(in the future)_ reading the ground speed sensor.  The Arduino also controls the two onboard motor controllers, one for turning and one for propulsion.  To increase accuracy of turning a PID control loop was implemented using feedback from the turn potentiometer.

> A PID loop, for those who do not know, is a control feedback loop used to minimize accumulated error by using the three part formula composed of three characteristics; a _proportional_ response characteristic, an _integrated error_ characteristic, and a _derived prediction of overshoot_ characteristic.  By balancing the tree of these functions an aggressive and accurate output can be created.  More information on PID loops can be found on [Wikipedia](http://en.wikipedia.org/wiki/PID_controller)

