#include "LPFM_types.h"

typedef enum
{
	LPFM_Seq1_Checkpoint1 = 0,
	LPFM_Seq1_Checkpoint2,
	LPFM_Seq1_Checkpoint3,
	LPFM_Seq1_Checkpoint4,
	
	LPFM_EndOfSeq1
	
} LPFM_Sequence1;

typedef enum
{
	LPFM_Seq2_Checkpoint1 = 0,
	LPFM_Seq2_Checkpoint2,
	LPFM_Seq2_Checkpoint3,
	LPFM_Seq2_Checkpoint4,
	LPFM_Seq2_Checkpoint5,
	LPFM_Seq2_Checkpoint6,
	
	LPFM_EndOfSeq2
	
} LPFM_Sequence2;

typedef enum
{
	LPFM_Sequence1en = 0,
	LPFM_Sequence2en,

	LPFM_NoOfSequences
} LPFM_Sequence;

static const uint8 LPFM_SequenceSize[LPFM_NoOfSequences] =
{
    LPFM_EndOfSeq1,
    LPFM_EndOfSeq2
};