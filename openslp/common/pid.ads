with Interfaces.C;

package Pid is
   -- todo: use common u32
   type U32 is mod 2**32;
   function SLPPidGet return U32
     with
       Export => True,
       Convention => C,
       External_Name => "SLPPidGet";
end Pid;
