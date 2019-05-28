

#include "LPFM_LogicalProgramFlow_CFG.h"
#include "LPFM_LogicalProgramFlow_INT.h"


#define LPFM_SuspendGlobalInterrupts()
#define LPFM_ResumeGlobalInterrupts()
#define SET_ERROR_WRONG_ENVIROMENT                   printf("ERROR_WRONG_ENVIROMENT"); while(1){}
#define SET_ERROR_MAXIMUM_NO_OF_PARALLEL_SEQ_REACHED printf("ERROR_MAXIMUM_NO_OF_PARALLEL_SEQ_REACHED"); while(1){}
#define SET_ERROR_WRONG_CHECKPOINT                   printf("ERROR_WRONG_CHECKPOINT"); while(1){}



static Std_ReturnType LPFM_CheckInverse(uint8 val, uint8 invVal)
{
    Std_ReturnType ret = E_NOT_OK;

    LPFM_SuspendGlobalInterrupts();
    
    if ((val ^ invVal) == (uint8) 0xFF)
    {
        ret = E_OK;
    }

    LPFM_ResumeGlobalInterrupts();

    return ret;
}

static Std_ReturnType LPFM_firstEntry(uint8 checkpointID)
{
    uint8 retVal = E_NOT_OK;

    if (checkpointID == 0)
    {
        retVal = E_OK;
    }

    return retVal;
}

static void LPFM_CheckEnviroment(void)
{
    uint8 seq_no, par_seq_no;
    Std_ReturnType retVal = E_OK;
    for (seq_no = 0; seq_no < LPFM_NoOfSequences; seq_no++)
    {
        for (par_seq_no = 0; par_seq_no < LPFM_NO_OF_PARALLEL_SEQ; par_seq_no++)
        {
            if(LPFM_CheckInverse(LPFM_curCheckpoint[seq_no][par_seq_no], LPFM_invCurCheckpoint[seq_no][par_seq_no]) != E_OK)
            {
                retVal = E_NOT_OK;
            }
        }
    }

    if(retVal != E_OK)
    {
        SET_ERROR_WRONG_ENVIROMENT;
    }
}

static uint8 LPFM_GetNextCheckpointID(uint8 sequenceID)
{
    uint8 nextPos, curParallelSeq;
    
    curParallelSeq = LPFM_ParallelSequences[sequenceID];
    nextPos = (LPFM_curCheckpoint[sequenceID][curParallelSeq] + 1) % LPFM_SequenceSize[sequenceID];

    return nextPos;
}

static void LPFM_GotoNextCheckpoint(uint8 sequenceID)
{
    uint8 curParallelSeq;
    curParallelSeq = LPFM_ParallelSequences[sequenceID];
    LPFM_SuspendGlobalInterrupts();
    LPFM_invCurCheckpoint[sequenceID][curParallelSeq] = (uint8) ~(((LPFM_curCheckpoint[sequenceID][curParallelSeq] + 1) % LPFM_SequenceSize[sequenceID]));
    LPFM_curCheckpoint[sequenceID][curParallelSeq]    = ((LPFM_curCheckpoint[sequenceID][curParallelSeq] + 1) % LPFM_SequenceSize[sequenceID]);
    LPFM_ResumeGlobalInterrupts();
}

static void LPFM_StartNewParallelSequence(uint8 sequenceID)
{
    uint8 curParallelSeq;
    curParallelSeq = LPFM_ParallelSequences[sequenceID];
    LPFM_SuspendGlobalInterrupts();
    LPFM_invCurCheckpoint[sequenceID][curParallelSeq] = (uint8) ~0x00;
    LPFM_curCheckpoint[sequenceID][curParallelSeq]    = 0x00;
    LPFM_ResumeGlobalInterrupts();
}

static void LPFM_LogicalCheckpointReached(uint8 checkpointID, uint8 sequenceID)
{
    uint8 nextCheckpoint;
    nextCheckpoint = LPFM_GetNextCheckpointID(sequenceID);
    
    LPFM_CheckEnviroment();
    
    if(checkpointID == nextCheckpoint)
    {
        LPFM_GotoNextCheckpoint(sequenceID);
    }
    else
    {
#if LPFM_NO_OF_PARALLEL_SEQ > 1
        if (checkpointID == LPFM_firstEntry(sequenceID))
        {
            LPFM_ParallelSequences[sequenceID]++;

            if (LPFM_ParallelSequences[sequenceID] >= LPFM_NO_OF_PARALLEL_SEQ)
            {
                SET_ERROR_MAXIMUM_NO_OF_PARALLEL_SEQ_REACHED;
            }
            else
            {
                LPFM_StartNewParallelSequence(sequenceID);
            }
            
        }
        else
        {
            SET_ERROR_WRONG_CHECKPOINT;
        }
        
#else /* LPFM_NO_OF_PARALLEL_SEQ > 1 */

    SET_ERROR_WRONG_CHECKPOINT;

#endif /* LPFM_NO_OF_PARALLEL_SEQ > 1 */
    }

}

static void LPFM_init(void)
{
    uint8 seq_no, par_seq_no, initialCheckpoint, initialCheckpointInverted;

	for (seq_no = 0; seq_no < LPFM_NoOfSequences; seq_no++)
	{
		LPFM_ParallelSequences[seq_no] = 0;

		initialCheckpoint = LPFM_SequenceSize[seq_no] - 1;
        initialCheckpointInverted = ~(LPFM_SequenceSize[seq_no] - 1);


		for (par_seq_no = 0; par_seq_no < LPFM_NO_OF_PARALLEL_SEQ; par_seq_no++)
		{
			LPFM_curCheckpoint[seq_no][par_seq_no] = initialCheckpoint;
			LPFM_invCurCheckpoint[seq_no][par_seq_no] = initialCheckpointInverted;
		}
	}
}

int main(void) /* PRQA S 0850 */ /* MD_MSR_19.8 */
{
/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << Start of runnable implementation >>             DO NOT CHANGE THIS COMMENT!
 * Symbol: LPFM_InitLPF
 *********************************************************************************************************************/
    LPFM_init();

    LPFM_LogicalCheckpointReached(0,0);
    LPFM_LogicalCheckpointReached(1,0);
    LPFM_LogicalCheckpointReached(0,0);
    LPFM_LogicalCheckpointReached(1,0);
    LPFM_LogicalCheckpointReached(2,0);
    LPFM_LogicalCheckpointReached(3,0);
    LPFM_LogicalCheckpointReached(0,0);
    LPFM_LogicalCheckpointReached(1,0);
    LPFM_LogicalCheckpointReached(0,0);
    LPFM_LogicalCheckpointReached(1,0);
    LPFM_LogicalCheckpointReached(0,0);
    LPFM_LogicalCheckpointReached(1,0);
    LPFM_LogicalCheckpointReached(0,0);


/**********************************************************************************************************************
 * DO NOT CHANGE THIS COMMENT!           << End of runnable implementation >>               DO NOT CHANGE THIS COMMENT!
 *********************************************************************************************************************/
 return 0;
}

