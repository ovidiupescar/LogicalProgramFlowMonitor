#include "LPFM_types.h"

#define LPFM_NO_OF_PARALLEL_SEQ 3 /* Should be 1 if there are no parallel sequences. */


static uint8 LPFM_ParallelSequences[LPFM_NoOfSequences];

static uint8 LPFM_curCheckpoint[LPFM_NoOfSequences][LPFM_NO_OF_PARALLEL_SEQ];
static uint8 LPFM_invCurCheckpoint[LPFM_NoOfSequences][LPFM_NO_OF_PARALLEL_SEQ];

static Std_ReturnType LPFM_CheckInverse(uint8 val, uint8 invVal);
static Std_ReturnType LPFM_firstEntry(uint8 checkpointID);
static void LPFM_CheckEnviroment(void);
static uint8 LPFM_GetNextCheckpointID(uint8 sequenceID);
static void LPFM_GotoNextCheckpoint(uint8 sequenceID);
static void LPFM_StartNewParallelSequence(uint8 sequenceID);
static void LPFM_LogicalCheckpointReached(uint8 checkpointID, uint8 sequenceID);
static void LPFM_init(void);
