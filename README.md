# IIS Lab 1: Capacitive Sensing

Description:
In this lab, the goal was to explore capacitive sensing as an input to an interactive system. The interactive system I designed is a version of "Dance Dance Revolution" utilizing a left and right swipe to complete the dance moves. The system runs on the Arduino IDE and prints the instructions and results to the serial output. There is an accompanying Arduino using an ESP32 and GIPO pins, connected to copper tape, to swipe over. There are also three LED pins, red, yellow, then green tracking the success of the swipe - red for one sensor cross, yellow for two, the green if the gesture is completed within constraints. 

The system first executes when the user presses 'p' then 'return.' It then counts down to a start then displays an arrow. When the corresponding swipe to the arrow is executed a "PERFECT!" ensures the user it is correct and the next arrow is given. If the user swipes the wrong way, a "MISS!" prints. The game ends after 10 correct swipes and gives the time it took to play and offers to restart with a 'p' 'return' again. 

In the code, the gestures are detected through capacitive sensing across three sensors. The swipe only correctly executes if it is completed within 0.5 seconds and across all three sensors. A state machine tracks the direction of the swipes to differentiate right and left. When the game executes, the arrow sequence is different every time by using an array of random values that correspond to right or left. This system has a fun interactive usage of gesture swiping through gamifying the capacitive sensing and giving instant execution feedback. No AI was used.

Run instructions:
In serial monitor input field type 'p'
Press 'return'

https://github.com/user-attachments/assets/2030706b-2453-4783-b57d-4ae8e52228c4




