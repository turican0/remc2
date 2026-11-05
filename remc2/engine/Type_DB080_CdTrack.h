#pragma once
#include <cstdint>

#pragma pack(push, 1)
typedef struct {//lenght 4
	int32_t startPos_0;
	int32_t length_2;
}
Type_DB080_TrackSegment;

typedef struct {//lenght 42
	int8_t TrackIdx_0;
	//int8_t stub;
	Type_DB080_TrackSegment TrackSegments_DB080[10];
}
Type_DB080_CdTrack;
#pragma pack(pop)

