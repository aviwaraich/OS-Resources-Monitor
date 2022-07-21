# OS-Resources-Monitor
Resource management command-line program which shows Memory Utilization, Sessions opened in System, CPU Cores Usage and System Information in Terminal.

**Example:**
![ezgif com-gif-maker](https://user-images.githubusercontent.com/56856829/180104711-95c99728-47f5-4033-a0ac-87d5da5fad29.gif)

**How to run (use) my program:**
It works like the Basic Code, Nothing Different Flags that can be used:
- “--system” - Prints : Memory, CPU and System Info
- “--user” - Prints : Session and System Info
- “--graphics” - Prints: Graphics
- “--samples=N” - N should be integer (0,1...)
- “--tdelay=T” - T should be integer (0,1....)
- “NT”: NisSampleandTisDelaybut“NT”shouldbethelastflaginthetheargument in the ./a.out
- “N” : N is Sample but “N” should be the last flag in the argument in the ./a.out
- None - It will print everything

Note: Everything can be combined other than:
- “--system” and “--user” cannot be used at same time since both are exclusive
- “N T” and “N” since one or other should be the last one both cannot be
- If “--samples=N” or “N” or “N T” are combined then one of “N” or “N T” will be taken
