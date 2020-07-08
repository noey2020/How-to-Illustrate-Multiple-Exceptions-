# How-to-Illustrate-Multiple-Exceptions-

Illustrating Multiple Exceptions     July 8, 2020

I appreciate comments. Shoot me an email at noel_s_cruz@yahoo.com!

Hire me!

We try to do many tasks in a program. So we will not be surprised if several threads of
execution happen all at the same time. If there is no parallelism, then each thread is
executed one at a time. In the instance that only one region of code is executing, then
that is no problem, If more than one then one region of code has to pre-empt and makes
things more complex.

This sample program will illustrate how different regions of code execute in relation to
other regions of code. We use variables, BACKGROUND, SYSTICK, and TIM4_VAR to represent
3 code regions to exemplify exactly how and when the SysTick timer fires up the SysTick 
handler code which will clear BACKGROUND and TIM4_VAR and set SYSTICK to 1. Upon 
completion of SysTick, PC returns to background and sets BACKGROUND = 1. The other two
are cleared. When TIM4 counts up to CCR1 but hits ARR earlier, the Update Interrupt flag 
is set indicating match or overflow, whichever comes first. Our IF condition will check 
for this flag or update event generation.

The code was adapted from: https://github.com/noey2020/How-to-Write-Timer-Trigger-ADC-STM32L1.
The short story is the timer triggers the adc in external mode to start conversions. 
Timer runs periodically.

We examine visually using our logic analyzer for the rising and falling transitions in
simulator mode. You can debug using the ST-Link debugger interface and single step/in/over
and run full-sleep. You can watch the LED connected at PB.6 toggle and blink the blues.

You can use a library if you want to experiment and this would help you get started.
https://github.com/noey2020/How-to-Blink-LED-using-Standard-Peripheral-Library.

I appreciate comments. Shoot me an email at noel_s_cruz@yahoo.com!

Hire me!

Have fun and happy coding!

