
`timescale 1ns/1ns
module verilog_tb();
  reg reset,a,clock;
  //wire [2:0]dd;
  wire w1,w2;
   mealy_fsm MUT1(clock,~reset,a,w1);
  moore_fsm MUT2(a,clock,~reset,w2);
    //mealy_fsm MUT2(clck,ares,a,w2);
  initial begin
    #5 reset=0;
     a=0;
     clock=0;
     #2 reset=1;
     a=1;
#50 clock=1;
#50 clock=0;
#50 clock=1;
#30 a=0;
#20 clock=0;
#50 clock=1;
#40 a=1;
#10 clock=0;
#50 clock=1;
#10 a=0;
//#10 c=1;
#50 clock=0;
#50 clock=1;
#50 clock=0;
#50 clock=1;

    
    
    repeat(5) #50 clock=(~clock);
  end
endmodule



