From :
Youtube :  https://www.youtube.com/watch?v=sZ8GJ1TiMdk
Github : https://github.com/tsoding/memalloc/


Search for the issue in the current state : 
```shell
❯ ./heap.elf
Start of the (fake) heap : 0xaaaad4a14020
####### BEFORE #####
Chunks alloced (2):
  start : 0xaaaad4a14020, size : 1
  start : 0xaaaad4a14023, size : 3
Chunks freed (3):
  start : 0xaaaad4a14021, size : 2
  start : 0xaaaad4a14026, size : 4
  start : 0xaaaad4a1402a, size : 639990
####### AFTER #####
Chunks alloced (7):
  start : 0xaaaad4a14020, size : 1
  start : 0xaaaad4a14021, size : 1
  start : 0xaaaad4a14023, size : 3
  start : 0xaaaad4a14026, size : 2
  start : 0xaaaad4a1402a, size : 3
  start : 0xaaaad4a1402d, size : 4
  start : 0xaaaad4a14031, size : 3
Chunks freed (3):
  start : 0xaaaad4a14022, size : 1
  start : 0xaaaad4a14028, size : 2
  start : 0xaaaad4a14034, size : 639980
```