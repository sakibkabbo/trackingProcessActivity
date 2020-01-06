# trackingProcessActivity

- A program to track the clock cyles within inactive periods while a program runs.

- Inactive periods are determined as context switches that shift the processor away from executing the program

- find_inactive.c takes two parameters:
  
  1) The number of inactive periods that are to be recorded
  2) The threshold that determines that an inactive period has begun

- find_threshold.c is used to determine the threshold