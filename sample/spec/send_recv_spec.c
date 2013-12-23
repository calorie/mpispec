#include "mpispec/mpispec.h"
#include "../src/send_recv.h"

mpispec_def(send_recv_spec)
  describe("send_recvは")
    it("rank0とrank1で通信する")
      int from = 0;
      int to   = 1;
      int tag  = 0;
      double timeout = 0.11;
      expect_to_send_recv(send_recv, from, to, tag, timeout)
    end
  end
end
