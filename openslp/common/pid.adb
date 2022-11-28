package body Pid is
   function SLPPidGet return U32 is
      function Get_Pid return U32
        with
          Import => True,
          Convention => C,
          External_Name => "GetCurrentProcessId"; -- todo: add posix support
   begin
      return Get_Pid;
   end SLPPidGet;
end Pid;
