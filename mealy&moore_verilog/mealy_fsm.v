`timescale 1ns/1ns
module mealy_fsm(input clck,ares,a,output w);
    reg [2:0]d;
  wire  [2:0]dd;
  assign dd[2]=d[1]&d[0]&a;
  assign dd[1]=((~d[1])&d[0]&a)+(d[1]&(~d[0])) + (d[2]&(~d[1])&(~a));
  assign dd[0]=((~d[2])&(~d[1])&(~d[0])&a)+(d[1]&(~d[0])&(~a));
  assign w=d[2]&(~a);
  always @(posedge clck,posedge ares) begin
  if(ares)
    d=3'b000;
 else if(clck)
      d=dd;
    end
  endmodule
