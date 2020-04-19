#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef	struct tagVertexColor
	{
		_vec3			vPos;
		_ulong			dwColor;

	}VTXCOL;

	const _ulong		FVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE;

	typedef	struct tagVertexTexture
	{
		_vec3			vPos;
		_vec2		 	vTexUV;

	}VTXTEX;

	const _ulong		FVF_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef	struct tagVertexCubeTexture
	{
		_vec3			vPos;
		_vec3		 	vTex;

	}VTXCUBE;

	const _ulong		FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);

	typedef struct tagScreenVertex
	{
		_vec4				vPos;
		_vec2				vTexUV;

	}VTXSCREEN;

	const _ulong		FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef	struct tagIndex16
	{
		_ushort	_0, _1, _2;

	}INDEX16;

	typedef	struct tagIndex32
	{
		_ulong	_0, _1, _2;

	}INDEX32;

	typedef struct tagTextureInfo
	{
		IDirect3DBaseTexture9*	pTexture;
		D3DXIMAGE_INFO			tImgInfo;
	}TEX_INFO, *PTEX_INFO;

	typedef struct tagLine
	{
		tagLine()
			: a(0.f), b(0.f), c(0.f)
		{

		}
		//	ax + by + c = 0
		_float a, b, c;
	}LINE, *PLINE;

	typedef struct tagLine3D
	{
		tagLine3D()
			: dx(0.f), dy(0.f), dz(0.f)
			, px(0.f), py(0.f), pz(0.f)
		{

		}
		union
		{
			_vec3 vDir;
			struct
			{
				_float dx, dy, dz;
			};
			struct
			{
				_float a, b, c;
			};
		};

		union 
		{
			_vec3 vPos;
			struct
			{
				_float px, py, pz;
			};
		};
	}LINE3D, *PLINE3D;

	typedef struct tagSphere
	{
	public:
		tagSphere()
			: vPos(0.f, 0.f, 0.f)
			, fRadius(0.f)
		{

		}

		union 
		{
			_vec3	vPos;
			struct { _float	x, y, z; };
		};
		_float	fRadius;
	}SPHERE, *PSPHERE;

	typedef struct tagCube
	{
		union 
		{
			_vec3	vPos;
			_float	x, y, z;
		};
		union 
		{
			_vec3	vSize;
			_float	Width, Height, Depth;
		};
		union 
		{
			_vec3	vAngle;
			_float	alpha,	Beta,	Gamma;
		};
	}MY_CUBE, *PMY_CUBE;
}

#endif // Engine_Struct_h__
