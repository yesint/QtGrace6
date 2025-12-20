# This is an example of a script that fits two sine-waves and a straight line onto every selected set
# Only data points outside region 0 are used. Please select the same sets for Source and Destination.
# 
# At first the selected set is copied to set G0.S0N and afterwards the restriction is applied in order
# not to use data points inside region 0 (we just want to remove a background signal, not the
# interesting signals inside the rectangle). 
# Please note, that G0.S0N is used as a temporary set during the calculation and it will be 
# deleted at the end of the script. S0N is a generic set-ID and QtGrace generates a 
# new set here with an unused ID. This is useful if you use this script for different numbers of sets.
# You have to use complete set-Ids in the script (like G0.S0, not just S0). The 'N' at the end 
# of the set-Id tells QtGrace to generate a new ID (S0N and S1N are new IDs, but different ones). 
# Only the button "Replay replacing set-Ids" understands this option.
# Any set-Id that does not end with $ is replaced with the set-Id(s) selected above, regardless 
# of what Id is written in the script.
# The "Replay" button does not understand this - it will execute every command exactly as it is written.
#
# First step: copy set into tempory set S0N and remove everything we do not want to fit onto
copy g0.s3 to g0.s0N
restrict(g0.s0N,r0,on)
# Second step: initiate a fit to the background signal (set starting values, the formula and so on).
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
# Third step: Do the fit as stated above onto temporary set G0.S0N with 50 iterations.
nonlfit (g0.s0N, 50)
# The fit itself does not generate a new set, but remembers the formula for Y with the best fit.
# To remove the background sine-waves we justed fitted, we just have to substract the
# fit-formula from the current set. (The original one, not the temporary one!)
# Fourth step: calculate "current set" - "fitted sine-waves" (i.e. last fit-result-formula)
#QTGRACE_SPECIAL MINUS LAST_FORMULA
# Fifth step: remove the noise
# The following line is just a 500 Hz-Low-Pass-Filter on the set
#QTGRACE_SPECIAL FILTER_SET 1,1{0,4}{0,4}{0;1;4;2;1;0;0;0;-1;0;500.0;100000.0;10.0;1}
# Last step: clean up - remove the temporary set G0N.
kill g0.s0N
# Finished. (Imagine doing this on 100 or more sets with just one click on a button...)
