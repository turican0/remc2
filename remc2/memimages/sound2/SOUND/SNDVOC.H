/*-----------------------------------------------

		VOC FILE PARSING STUFF

-------------------------------------------------*/

//	Voc file header structure
typedef struct _sVocHeader{
	char	filetype[20];
	uint8	offData;
	uint8	version;
	uint8	id;
} sVocHeader;

typedef uint8 VocTerminatorBlock;

struct _sVocBlockHeader {
	uint8 blockID;
	uint8 blockLen[3];
};

// VOC data
typedef struct _sVocDataBlock {
   uint8	blockID;
   uint8	blockLen[3];
   uint8	timeConstant;
   uint8	packMethod;
} sVocDataBlock;

typedef struct _sVocContinueBlock {
   uint8	blockID;
   uint8	blockLen[3];
} sVocContinueBlock;

// VOC silence Block
typedef struct _sSndSilenceBlock {
   uint8	blockID;
   uint8	blockLen[3];
   uint16	pausePeriod;
   uint8	timeConstant;
} sVocSilenceBlock;

// VOC marker block
typedef struct _sVocMarkerBlock {
   uint8	blockID;
   uint8	blockLen[3];
   int16	marker;
} sVocMarkerBlock;

// VOC ASCIIZ comment block
typedef struct _sVocAsciiBlock {
   uint8	blockID;
   uint8	blockLen[3];
   int8		string;
} sVocAsciiBlock;

// VOC repeat loop block
typedef struct _sVocLoopBlock {
   uint8	blockID;
   uint8	blockLen[3];
   uint16	repeatCount;
} sVocLoopBlock;

// VOC end-of-loop block
typedef struct _sVocEndLoopBlock {
   uint8	blockID;
   uint8	blockLen[3];
} sVocEndLoopBlock;

// VOC extended attribute block 
// (always followed by data block)
typedef struct _sVocStereoBlock {
   uint8	blockID;
   uint8	blockLen[3];
   uint16	timeConstant;
   uint8	packMethod;
   uint8	voiceMode;
} sVocStereoBlock;

// VOC extended voice block
// (replaces data and extended blocks)

typedef struct _sVocExtendedBlock {
   uint8	blockID;
   uint8	blockLen[3];
   uint32	sampleRate;
   uint8	bitsPerSample;
   uint8	channels;
   uint16	format;
   uint8	reserved[4];
} sVocExtendedBlock;

//	Voc block types
#define VOC_BLOCK_TERM		0
#define VOC_BLOCK_DATA		1
#define VOC_BLOCK_CONTINUE	2
#define VOC_BLOCK_SILENCE	3
#define VOC_BLOCK_MARKER	4
#define VOC_BLOCK_ASCII		5
#define VOC_BLOCK_LOOP		6
#define VOC_BLOCK_ENDLOOP	7
#define VOC_BLOCK_STEREO	8
#define VOC_BLOCK_EXTENDED 9

//	Voc data compression types
#define VOC_DATA_RAW8	0
#define VOC_DATA_COMP4	1
#define VOC_DATA_COMP26	2
#define VOC_DATA_COMP2	3

// unportable in that this relies on byte order
#define VOC_BLOCK_LEN(b) ((*(uint32 *)b)>>8)
