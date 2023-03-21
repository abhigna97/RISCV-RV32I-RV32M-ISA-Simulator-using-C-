jal x2, HERE
BACK: BNE x2,x0,FINISH
HERE: addi x3,x0,4
jal x0, BACK
FINISH: addi x31,x0,1
addi x0,x0,0