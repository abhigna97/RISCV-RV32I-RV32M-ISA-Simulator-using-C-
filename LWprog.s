lui x3, 1048575
lui x2, 65532
srli x3, x3, 12
srli x2, x2, 12
sh x3, 0(x2)
sh x3, 2(x2)
lw x4, 0(x2)
lw x5, 2(x2)
addi x0,x0,0