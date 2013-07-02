#include <mpi.h>
#include <cue/cue.h>

CU_PROTOTYPE(test_pt2pt);
CU_PROTOTYPE(test_coll);
CU_PROTOTYPE(test_context);

CU_INIT;
MPI_Init(&argc, &argv);

CU_REGISTER("test pt2pt", test_pt2pt);
CU_REGISTER("test coll", test_coll);
CU_REGISTER("test context", test_context);

CU_FINALIZE;
