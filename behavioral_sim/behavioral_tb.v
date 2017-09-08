
`timescale 1ns/1ns
module behavioral_tb();
  reg ares,a,clck;
  //wire [2:0]dd;
  wire mealy,moore;
 baredavazdah MUT1(mealy,clck,ares,a,moore);
 // mealy_fsm MUT2(clock,~reset,a,myw);
    //mealy_fsm MUT2(clck,ares,a,w2);
  initial begin
    #5 ares=0;
     a=0;
     clck=0;
     #2 ares=1;
     a=1;
#50 clck=1;
#50 clck=0;
#50 clck=1;
#30 a=0;
#20 clck=0;
#50 clck=1;
#40 a=1;
#10 clck=0;
#50 clck=1;
#10 a=0;
//#10 c=1;
#50 clck=0;
#50 clck=1;
#50 clck=0;
#50 clck=1;

    
    
    repeat(5) #50 clck=(~clck);
  end
endmodule



