#ifndef Struct_h__
#define Struct_h__

typedef struct tagAnimationFrame
{
	_float	fCurFrame;
	_float	fMaxFrame;
	_float	fFrameSpeed;
} FRAME, *PFRAME;

typedef struct tagTerrainInfo
{
	_ulong dwTileX;
	_ulong dwTileZ;
	_ulong dwItv;
}TERRAIN_INFO;

typedef struct tagTileInfo
{
	_vec3	vPos;
	_ulong	dwItv;
	_ulong	dwDrawID;
	bool	bRender;
}TILE_INFO;

typedef struct tagWallInfo
{
	_vec3	vPos;
	_ulong	dwItv;
	_ulong	dwDrawID;
	_ulong	dwIndex;
	bool	bRender;
	bool	bHasWall[WALL_END];

} WALL_INFO;

#endif // Struct_h__
