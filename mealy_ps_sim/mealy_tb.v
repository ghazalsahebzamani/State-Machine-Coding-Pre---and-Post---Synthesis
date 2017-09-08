
`timescale 1ns/1ns
module mealy_tb();
  reg reset,a,clock;
  //wire [2:0]dd;
  wire w3,w4,myw;
   fifteen MUT1(w3,reset,clock,a,w4);
  mealy_fsm MUT2(clock,~reset,a,myw);
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

