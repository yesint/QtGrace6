# This is an example of a script that fits two sine-waves and a straight line onto every selected set
# Only data points outside region 0 are used. Please select the same sets for Source and Destination.
# 
# At first the selected set is copied to set G0.S6 and afterwards the restriction is applied in order
# not to use data points inside region 0 (we just want to remove a background signal, not the
# interesting signals inside the rectangle). 
# Please note, that G0.S6 is used as a temporary set during the calculation and it will be 
# deleted at the end of the script. Therefore you should use a set-id that does not exist here.
# You have to use complete set-Ids in the script (like G0.S0, not just S0). The $ at the end 
# of the set-Id tells QtGrace not to alter this id during script processing. Only the button
# "Replay replacing set-Ids" understands this option. The "Replay" button does not understand this.
# Any set-Id that does not end with $ is replaced with the set-Id(s) selected above, regardless 
# of what Id is written in the script.
copy g0.s3 to g0.s6$
restrict(g0.s6$,r0,on)
# Now we initiate a fit to the background signal (set starting values, the formula and so on).
with G0
fit formula "y=a0+a1*x+a2*sin(2*PI*a3*x+a4)+a5*sin(2*PI*a6*x+a7)"
fit with 8 parameters
fit prec 0.010000
a0 = 0.249945
a0 constraints off
a1 = 0.153922
a1 constraints off
a2 = 0.199480
a2 constraints off
a3 = 50.307000
a3 constraints off
a4 = 2.735180
a4 constraints off
a5 = 0.129247
a5 constraints off
a6 = 149.004000
a6 constraints off
a7 = 0.794684
a7 constraints off
# Do the fit as stated above onto set G0.S6 (always the same set) with 50 iterations.
nonlfit (g0.s6$, 50)
# The fit does not generate a new set, but remembers the formula for Y with the best fit.
# To remove the background sine-waves we justed fitted, we just have to substract the
# fit-formula from the current set.
#QTGRACE_SPECIAL MINUS LAST_FORMULA
# The following line is just a 500 Hz-Low-Pass-Filter on the set to reduce white noise.
#QTGRACE_SPECIAL FILTER_SET 1,1{0,4}{0,4}{0;1;4;2;1;0;0;0;-1;0;500.0;100000.0;10.0;1}
# To clean up, we remove the temporary set.
kill g0.s6$
# Finished. (Imagine doing this on 100 or more sets with just one click on a button...)
