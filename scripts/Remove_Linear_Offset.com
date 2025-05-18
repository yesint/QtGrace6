COPY G0.S0 TO S1N
#Points inside region 0
RESTRICT(S1N,R0,OFF)
#Points outside region 0
#RESTRICT(S1N,R0,ON)
with G0
fit formula "y=a0+a1*x"
fit with 2 parameters
fit prec 0.010000
a0 = 1.000000
a0 constraints off
a1 = 1.000000
a1 constraints off
nonlfit (S1N, 5)
#QTGRACE_SPECIAL MINUS LAST_FORMULA
KILL S1N
