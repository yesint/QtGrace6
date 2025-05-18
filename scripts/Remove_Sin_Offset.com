COPY G0.S0 TO S1N
#Points inside region 0
#RESTRICT(S1N,R0,OFF)
#Points outside region 0
RESTRICT(S1N,R0,ON)
with G0
fit formula "y=A0+A1*x+A2*sin(2*PI*A3*x+A4)+A5*sin(2*PI*A6*x+A7)"
fit with 8 parameters
fit prec 0.010000
a0 = 0.100161
a0 constraints off
a1 = -0.007998
a1 constraints off
a2 = 0.499980
a2 constraints off
a3 = 0.190985
a3 constraints off
a4 = 0.399650
a4 constraints off
a5 = 0.149924
a5 constraints off
a6 = 0.668451
a6 constraints off
a7 = -0.099490
a7 constraints off
nonlfit (S1N, 50)
#QTGRACE_SPECIAL MINUS LAST_FORMULA
KILL S1N
