
#ifndef __IQB_CLASS_3D_H__
#define __IQB_CLASS_3D_H__

////////////////////////////////////////////////////////////////////////////////

#include "avej_lite.h"
#include "gfx3d_config.h"

////////////////////////////////////////////////////////////////////////////////

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef unsigned int   UINT;
typedef int            INT;
typedef long           LONG;
typedef unsigned long  _HRESULT;
typedef void*          HANDLE;
typedef DWORD          D3DCOLOR;

////////////////////////////////////////////////////////////////////////////////

#define _SUCCEEDED(hr) (((_HRESULT)(hr)) >= 0)
#define _FAILED(hr) (((_HRESULT)(hr)) < 0)

#define TD3DVector2  D3DVECTOR2
#define TD3DVector3  D3DVECTOR3
#define TD3DVector4  D3DVECTOR4
#define TD3DMatrix   D3DMATRIX
#define TD3DLight9   D3DLIGHT9

#define D3DX_PI (3.141592f)
//?? юс╫ц
#define MAX_FVF_DECL_SIZE 100

#define D3DTS_WORLDMATRIX(index) (D3DTRANSFORMSTATETYPE)(index + 256)
#define D3DTS_WORLD  D3DTS_WORLDMATRIX(0)
#define D3DTS_WORLD1 D3DTS_WORLDMATRIX(1)
#define D3DTS_WORLD2 D3DTS_WORLDMATRIX(2)
#define D3DTS_WORLD3 D3DTS_WORLDMATRIX(3)

#define D3DCOLOR_ARGB(a,r,g,b) \
	((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a,r,g,b)
#define D3DCOLOR_XRGB(r,g,b)   D3DCOLOR_ARGB(0xff,r,g,b)

/*
 * Values for COLORARG0,1,2, ALPHAARG0,1,2, and RESULTARG texture blending
 * operations set in texture processing stage controls in D3DRENDERSTATE.
 */
#define D3DTA_SELECTMASK        0x0000000f  // mask for arg selector
#define D3DTA_DIFFUSE           0x00000000  // select diffuse color (read only)
#define D3DTA_CURRENT           0x00000001  // select stage destination register (read/write)
#define D3DTA_TEXTURE           0x00000002  // select texture color (read only)
#define D3DTA_TFACTOR           0x00000003  // select D3DRS_TEXTUREFACTOR (read only)
#define D3DTA_SPECULAR          0x00000004  // select specular color (read only)
#define D3DTA_TEMP              0x00000005  // select temporary register color (read/write)
#define D3DTA_CONSTANT          0x00000006  // select texture stage constant
#define D3DTA_COMPLEMENT        0x00000010  // take 1.0 - x (read modifier)
#define D3DTA_ALPHAREPLICATE    0x00000020  // replicate alpha to color components (read modifier)

#define D3DCLEAR_TARGET            0x00000001l  /* Clear target surface */
#define D3DCLEAR_ZBUFFER           0x00000002l  /* Clear target z buffer */
#define D3DCLEAR_STENCIL           0x00000004l  /* Clear stencil planes */

////////////////////////////////////////////////////////////////////////////////

enum // Usage
{
	D3DUSAGE_WRITEONLY = 1
};

enum D3DFORMAT
{
	D3DFMT_INDEX16
};

enum D3DPOOL
{
	D3DPOOL_MANAGED
};

enum D3DPRIMITIVETYPE
{
	D3DPT_POINTLIST     = 1,
	D3DPT_LINELIST      = 2,
	D3DPT_LINESTRIP     = 3,
	D3DPT_TRIANGLELIST  = 4,
	D3DPT_TRIANGLESTRIP = 5,
	D3DPT_TRIANGLEFAN   = 6,
	D3DPT_FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
};

enum EFVF
{
	D3DFVF_XYZ     = 1,
	D3DFVF_DIFFUSE = 2,
	D3DFVF_NORMAL  = 4,
	D3DFVF_TEX1    = 8
};

enum D3DTRANSFORMSTATETYPE
{
	D3DTS_VIEW          = 2,
	D3DTS_PROJECTION    = 3,
	D3DTS_TEXTURE0      = 16,
	D3DTS_TEXTURE1      = 17,
	D3DTS_TEXTURE2      = 18,
	D3DTS_TEXTURE3      = 19,
	D3DTS_TEXTURE4      = 20,
	D3DTS_TEXTURE5      = 21,
	D3DTS_TEXTURE6      = 22,
	D3DTS_TEXTURE7      = 23,
	D3DTS_FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
};

enum D3DRENDERSTATETYPE
{
	D3DRS_ZENABLE                   = 7,    /* D3DZBUFFERTYPE (or TRUE/FALSE for legacy) */
	D3DRS_FILLMODE                  = 8,    /* D3DFILLMODE */
	D3DRS_SHADEMODE                 = 9,    /* D3DSHADEMODE */
	D3DRS_ZWRITEENABLE              = 14,   /* TRUE to enable z writes */
	D3DRS_ALPHATESTENABLE           = 15,   /* TRUE to enable alpha tests */
	D3DRS_LASTPIXEL                 = 16,   /* TRUE for last-pixel on lines */
	D3DRS_SRCBLEND                  = 19,   /* D3DBLEND */
	D3DRS_DESTBLEND                 = 20,   /* D3DBLEND */
	D3DRS_CULLMODE                  = 22,   /* D3DCULL */
	D3DRS_ZFUNC                     = 23,   /* D3DCMPFUNC */
	D3DRS_ALPHAREF                  = 24,   /* D3DFIXED */
	D3DRS_ALPHAFUNC                 = 25,   /* D3DCMPFUNC */
	D3DRS_DITHERENABLE              = 26,   /* TRUE to enable dithering */
	D3DRS_ALPHABLENDENABLE          = 27,   /* TRUE to enable alpha blending */
	D3DRS_FOGENABLE                 = 28,   /* TRUE to enable fog blending */
	D3DRS_SPECULARENABLE            = 29,   /* TRUE to enable specular */
	D3DRS_FOGCOLOR                  = 34,   /* D3DCOLOR */
	D3DRS_FOGTABLEMODE              = 35,   /* D3DFOGMODE */
	D3DRS_FOGSTART                  = 36,   /* Fog start (for both vertex and pixel fog) */
	D3DRS_FOGEND                    = 37,   /* Fog end      */
	D3DRS_FOGDENSITY                = 38,   /* Fog density  */
	D3DRS_RANGEFOGENABLE            = 48,   /* Enables range-based fog */
	D3DRS_STENCILENABLE             = 52,   /* BOOL enable/disable stenciling */
	D3DRS_STENCILFAIL               = 53,   /* D3DSTENCILOP to do if stencil test fails */
	D3DRS_STENCILZFAIL              = 54,   /* D3DSTENCILOP to do if stencil test passes and Z test fails */
	D3DRS_STENCILPASS               = 55,   /* D3DSTENCILOP to do if both stencil and Z tests pass */
	D3DRS_STENCILFUNC               = 56,   /* D3DCMPFUNC fn.  Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	D3DRS_STENCILREF                = 57,   /* Reference value used in stencil test */
	D3DRS_STENCILMASK               = 58,   /* Mask value used in stencil test */
	D3DRS_STENCILWRITEMASK          = 59,   /* Write mask applied to values written to stencil buffer */
	D3DRS_TEXTUREFACTOR             = 60,   /* D3DCOLOR used for multi-texture blend */
	D3DRS_WRAP0                     = 128,  /* wrap for 1st texture coord. set */
	D3DRS_WRAP1                     = 129,  /* wrap for 2nd texture coord. set */
	D3DRS_WRAP2                     = 130,  /* wrap for 3rd texture coord. set */
	D3DRS_WRAP3                     = 131,  /* wrap for 4th texture coord. set */
	D3DRS_WRAP4                     = 132,  /* wrap for 5th texture coord. set */
	D3DRS_WRAP5                     = 133,  /* wrap for 6th texture coord. set */
	D3DRS_WRAP6                     = 134,  /* wrap for 7th texture coord. set */
	D3DRS_WRAP7                     = 135,  /* wrap for 8th texture coord. set */
	D3DRS_CLIPPING                  = 136,
	D3DRS_LIGHTING                  = 137,
	D3DRS_AMBIENT                   = 139,
	D3DRS_FOGVERTEXMODE             = 140,
	D3DRS_COLORVERTEX               = 141,
	D3DRS_LOCALVIEWER               = 142,
	D3DRS_NORMALIZENORMALS          = 143,
	D3DRS_DIFFUSEMATERIALSOURCE     = 145,
	D3DRS_SPECULARMATERIALSOURCE    = 146,
	D3DRS_AMBIENTMATERIALSOURCE     = 147,
	D3DRS_EMISSIVEMATERIALSOURCE    = 148,
	D3DRS_VERTEXBLEND               = 151,
	D3DRS_CLIPPLANEENABLE           = 152,
	D3DRS_POINTSIZE                 = 154,   /* float point size */
	D3DRS_POINTSIZE_MIN             = 155,   /* float point size min threshold */
	D3DRS_POINTSPRITEENABLE         = 156,   /* BOOL point texture coord control */
	D3DRS_POINTSCALEENABLE          = 157,   /* BOOL point size scale enable */
	D3DRS_POINTSCALE_A              = 158,   /* float point attenuation A value */
	D3DRS_POINTSCALE_B              = 159,   /* float point attenuation B value */
	D3DRS_POINTSCALE_C              = 160,   /* float point attenuation C value */
	D3DRS_MULTISAMPLEANTIALIAS      = 161,  // BOOL - set to do FSAA with multisample buffer
	D3DRS_MULTISAMPLEMASK           = 162,  // DWORD - per-sample enable/disable
	D3DRS_PATCHEDGESTYLE            = 163,  // Sets whether patch edges will use float style tessellation
	D3DRS_DEBUGMONITORTOKEN         = 165,  // DEBUG ONLY - token to debug monitor
	D3DRS_POINTSIZE_MAX             = 166,   /* float point size max threshold */
	D3DRS_INDEXEDVERTEXBLENDENABLE  = 167,
	D3DRS_COLORWRITEENABLE          = 168,  // per-channel write enable
	D3DRS_TWEENFACTOR               = 170,   // float tween factor
	D3DRS_BLENDOP                   = 171,   // D3DBLENDOP setting
	D3DRS_POSITIONDEGREE            = 172,   // NPatch position interpolation degree. D3DDEGREE_LINEAR or D3DDEGREE_CUBIC (default)
	D3DRS_NORMALDEGREE              = 173,   // NPatch normal interpolation degree. D3DDEGREE_LINEAR (default) or D3DDEGREE_QUADRATIC
	D3DRS_SCISSORTESTENABLE         = 174,
	D3DRS_SLOPESCALEDEPTHBIAS       = 175,
	D3DRS_ANTIALIASEDLINEENABLE     = 176,
	D3DRS_MINTESSELLATIONLEVEL      = 178,
	D3DRS_MAXTESSELLATIONLEVEL      = 179,
	D3DRS_ADAPTIVETESS_X            = 180,
	D3DRS_ADAPTIVETESS_Y            = 181,
	D3DRS_ADAPTIVETESS_Z            = 182,
	D3DRS_ADAPTIVETESS_W            = 183,
	D3DRS_ENABLEADAPTIVETESSELLATION = 184,
	D3DRS_TWOSIDEDSTENCILMODE       = 185,   /* BOOL enable/disable 2 sided stenciling */
	D3DRS_CCW_STENCILFAIL           = 186,   /* D3DSTENCILOP to do if ccw stencil test fails */
	D3DRS_CCW_STENCILZFAIL          = 187,   /* D3DSTENCILOP to do if ccw stencil test passes and Z test fails */
	D3DRS_CCW_STENCILPASS           = 188,   /* D3DSTENCILOP to do if both ccw stencil and Z tests pass */
	D3DRS_CCW_STENCILFUNC           = 189,   /* D3DCMPFUNC fn.  ccw Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
	D3DRS_COLORWRITEENABLE1         = 190,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	D3DRS_COLORWRITEENABLE2         = 191,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	D3DRS_COLORWRITEENABLE3         = 192,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
	D3DRS_BLENDFACTOR               = 193,   /* D3DCOLOR used for a constant blend factor during alpha blending for devices that support D3DPBLENDCAPS_BLENDFACTOR */
	D3DRS_SRGBWRITEENABLE           = 194,   /* Enable rendertarget writes to be DE-linearized to SRGB (for formats that expose D3DUSAGE_QUERY_SRGBWRITE) */
	D3DRS_DEPTHBIAS                 = 195,
	D3DRS_WRAP8                     = 198,   /* Additional wrap states for vs_3_0+ attributes with D3DDECLUSAGE_TEXCOORD */
	D3DRS_WRAP9                     = 199,
	D3DRS_WRAP10                    = 200,
	D3DRS_WRAP11                    = 201,
	D3DRS_WRAP12                    = 202,
	D3DRS_WRAP13                    = 203,
	D3DRS_WRAP14                    = 204,
	D3DRS_WRAP15                    = 205,
	D3DRS_SEPARATEALPHABLENDENABLE  = 206,  /* TRUE to enable a separate blending function for the alpha channel */
	D3DRS_SRCBLENDALPHA             = 207,  /* SRC blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	D3DRS_DESTBLENDALPHA            = 208,  /* DST blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
	D3DRS_BLENDOPALPHA              = 209,  /* Blending operation for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */

	D3DRS_FORCE_DWORD               = 0x7fffffff, /* force 32-bit size enum */
};

enum D3DTEXTURESTAGESTATETYPE
{
    D3DTSS_COLOROP        =  1, /* D3DTEXTUREOP - per-stage blending controls for color channels */
    D3DTSS_COLORARG1      =  2, /* D3DTA_* (texture arg) */
    D3DTSS_COLORARG2      =  3, /* D3DTA_* (texture arg) */
    D3DTSS_ALPHAOP        =  4, /* D3DTEXTUREOP - per-stage blending controls for alpha channel */
    D3DTSS_ALPHAARG1      =  5, /* D3DTA_* (texture arg) */
    D3DTSS_ALPHAARG2      =  6, /* D3DTA_* (texture arg) */
    D3DTSS_BUMPENVMAT00   =  7, /* float (bump mapping matrix) */
    D3DTSS_BUMPENVMAT01   =  8, /* float (bump mapping matrix) */
    D3DTSS_BUMPENVMAT10   =  9, /* float (bump mapping matrix) */
    D3DTSS_BUMPENVMAT11   = 10, /* float (bump mapping matrix) */
    D3DTSS_TEXCOORDINDEX  = 11, /* identifies which set of texture coordinates index this texture */
    D3DTSS_BUMPENVLSCALE  = 22, /* float scale for bump map luminance */
    D3DTSS_BUMPENVLOFFSET = 23, /* float offset for bump map luminance */
    D3DTSS_TEXTURETRANSFORMFLAGS = 24, /* D3DTEXTURETRANSFORMFLAGS controls texture transform */
    D3DTSS_COLORARG0      = 26, /* D3DTA_* third arg for triadic ops */
    D3DTSS_ALPHAARG0      = 27, /* D3DTA_* third arg for triadic ops */
    D3DTSS_RESULTARG      = 28, /* D3DTA_* arg for result (CURRENT or TEMP) */
    D3DTSS_CONSTANT       = 32, /* Per-stage constant D3DTA_CONSTANT */

    D3DTSS_FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
};

enum D3DFOGMODE
{
    D3DFOG_NONE                 = 0,
    D3DFOG_EXP                  = 1,
    D3DFOG_EXP2                 = 2,
    D3DFOG_LINEAR               = 3,
    D3DFOG_FORCE_DWORD          = 0x7fffffff, /* force 32-bit size enum */
};

enum D3DLIGHTTYPE
{
	D3DLIGHT_POINT          = 1,
	D3DLIGHT_SPOT           = 2,
	D3DLIGHT_DIRECTIONAL    = 3,
	D3DLIGHT_FORCE_DWORD    = 0x7fffffff, /* force 32-bit size enum */
};

////////////////////////////////////////////////////////////////////////////////

#define RECT D3DRECT

struct D3DRECT
{
    LONG x1;
    LONG y1;
    LONG x2;
    LONG y2;
};

struct D3DVECTOR2
{
	float x;
	float y;

	D3DVECTOR2()
		: x(0.0f), y(0.0f)
	{
	}
	
	D3DVECTOR2(float _x, float _y)
		: x(_x), y(_y)
	{
	}
};

struct D3DVECTOR3
{
	float x;
	float y;
	float z;

	D3DVECTOR3()
		: x(0.0f), y(0.0f), z(0.0f)
	{
	}

	D3DVECTOR3(float _x, float _y, float _z)
		: x(_x), y(_y), z(_z)
	{
	}
};

struct D3DVECTOR4
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			float a;
			float b;
			float g;
			float r;
		};
		float f[4];
	};
};

struct D3DMATRIX
{
	union
	{
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
		float f[16];
	};
};

struct D3DCOLORVALUE
{
	float r;
	float g;
	float b;
	float a;
};

struct D3DLIGHT9
{
	D3DLIGHTTYPE    Type;            /* Type of light source */
	D3DCOLORVALUE   Diffuse;         /* Diffuse color of light */
	D3DCOLORVALUE   Specular;        /* Specular color of light */
	D3DCOLORVALUE   Ambient;         /* Ambient color of light */
	D3DVECTOR3      Position;         /* Position in world space */
	D3DVECTOR3      Direction;        /* Direction in world space */
	float           Range;            /* Cutoff range */
	float           Falloff;          /* Falloff */
	float           Attenuation0;     /* Constant attenuation */
	float           Attenuation1;     /* Linear attenuation */
	float           Attenuation2;     /* Quadratic attenuation */
	float           Theta;            /* Inner angle of spotlight cone */
	float           Phi;              /* Outer angle of spotlight cone */
};

struct D3DSURFACE_DESC
{
	long Width;
	long Height;
};

struct D3DVERTEXELEMENT9
{
	WORD    Stream;     // Stream index
	WORD    Offset;     // Offset in the stream in bytes
	BYTE    Type;       // Data type
	BYTE    Method;     // Processing method
	BYTE    Usage;      // Semantics
	BYTE    UsageIndex; // Semantic index
};

////////////////////////////////////////////////////////////////////////////////

class IDirect3DTexture9;
class IDirect3DVertexBuffer9;
class IDirect3DIndexBuffer9;
class IDirect3DVertexDeclaration9;
class IDirect3DVertexShader9;
class IDirect3DPixelShader9;

////////////////////////////////////////////////////////////////////////////////

class _IUnknown
{
public:
	virtual ~_IUnknown()
	{
	}

	virtual void Release()
	{
		delete this;
	};
};

class IDirect3DDevice9: public _IUnknown
{
public:
	IDirect3DDevice9(avej_lite::IGfxDevice* p_gfx_device);

	_HRESULT CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle);
	_HRESULT CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle);
	_HRESULT CreateVertexDeclaration(const D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl);

	_HRESULT BeginScene(void);
    _HRESULT EndScene(void);
	_HRESULT Present(const RECT* pSourceRect, const RECT* pDestRect, void* hDestWindowOverride, const void* pDirtyRegion);

	void Clear(DWORD Count, const D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil);

	void SetTexture(DWORD stage, IDirect3DTexture9* p_texture);
	void SetStreamSource(DWORD StreamNumber, IDirect3DVertexBuffer9* pStreamData, DWORD OffsetInBytes, DWORD Stride);
	void SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl);
	void SetVertexShader(IDirect3DVertexShader9* p_shader);
	void SetPixelShader(IDirect3DPixelShader9* p_shader);
	void SetFVF(DWORD FVF);
	void SetIndices(IDirect3DIndexBuffer9* p_index_buffer);
	void SetTransform(D3DTRANSFORMSTATETYPE State, const D3DMATRIX* pMatrix);
	void GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix);
	void SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
	void SetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE state_type, DWORD value);
	void LightEnable(DWORD Index, bool Enable);
	void SetLight(DWORD Index, const D3DLIGHT9*);
	void SetVertexShaderConstantF(UINT ix_register, const float* p_constant_data, UINT num_vec4);

	void DrawPrimitive(D3DPRIMITIVETYPE primitive_type, UINT start_vertex, UINT num_primitive);
	void DrawIndexedPrimitive(D3DPRIMITIVETYPE primitive_type, INT ix_base_vertex, UINT ix_min_vertex, UINT num_vertices, UINT ix_start, UINT num_primitive);

public:
	avej_lite::IGfxDevice* m_p_gfx_device;
	struct
	{
		D3DMATRIX world;
		D3DMATRIX view;
		D3DMATRIX projection;
	} m_transform_matrix;

	enum { MAX_VERTEX_SHADER_CONSTANT = 24 };
	float m_vertex_shader_constant[MAX_VERTEX_SHADER_CONSTANT][4];

	struct TStatus
	{
		enum { MAX_STAGE = 4 };
		IDirect3DTexture9* p_current_texture[MAX_STAGE];

		DWORD fvf;
		DWORD texture_factor;

		enum { MAX_STREAM_SOURCE = 1 };
		struct TStreamSource
		{
			IDirect3DVertexBuffer9* p_stream_data;
			DWORD offset;
			DWORD stride;
		} stream_source[MAX_STREAM_SOURCE];

		IDirect3DIndexBuffer9* p_index_buffer;
	} m_status;
};

class IDirect3DVertexBuffer9: public _IUnknown
{
public:
	IDirect3DVertexBuffer9();
	~IDirect3DVertexBuffer9();

	_HRESULT Lock(UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags);
	void Unlock(void);

public:
	char* m_p_buffer;
	DWORD m_fvf;
};

class IDirect3DIndexBuffer9: public _IUnknown
{
public:
	IDirect3DIndexBuffer9();
	~IDirect3DIndexBuffer9();

	_HRESULT Lock(UINT OffsetToLock ,UINT SizeToLock, void** ppbData, DWORD Flags);
	void Unlock(void);

public:
	char* m_p_buffer;
};

class IDirect3DTexture9: public _IUnknown
{
public:
	IDirect3DTexture9();
	~IDirect3DTexture9();

	void GetLevelDesc(DWORD level, D3DSURFACE_DESC* p_desc);

public:
	avej_lite::IGfxSurface* m_p_surface;
};

class IDirect3DVertexDeclaration9: public _IUnknown
{
public:
};

class IDirect3DVertexShader9: public _IUnknown
{
public:
};

class IDirect3DPixelShader9: public _IUnknown
{
public:
};

////////////////////////////////////////////////////////////////////////////////

void D3DXCreateTextureFromFileA(IDirect3DDevice9* p_d3d_device, const char* sz_file_name, IDirect3DTexture9** ppTexture);

void D3DXMatrixIdentity(TD3DMatrix* p_out_mat);
void D3DXMatrixScaling(TD3DMatrix* p_out_mat, float x, float y, float z);
void D3DXMatrixMultiply(TD3DMatrix* p_out_mat, const TD3DMatrix* p_mat1, const TD3DMatrix* p_mat2);
void D3DXMatrixTranspose(TD3DMatrix* p_out_mat, const TD3DMatrix* p_mat);
void D3DXMatrixTranslation(TD3DMatrix* p_out_mat, float x, float y, float z);
void D3DXMatrixRotationX(TD3DMatrix* p_out_mat, float angle);
void D3DXMatrixRotationY(TD3DMatrix* p_out_mat, float angle);
void D3DXMatrixRotationZ(TD3DMatrix* p_out_mat, float angle);
void D3DXVec3Normalize(TD3DVector3* p_out_mat, const TD3DVector3* p_vec);
void D3DXVec3CrossProduct(TD3DVector3* p_out_mat, const TD3DVector3* p_vec1, const TD3DVector3* p_vec2);
void D3DXMatrixLookAtLH(TD3DMatrix* p_out_mat, const TD3DVector3* p_eye, const TD3DVector3* p_at, const TD3DVector3* p_up);
void D3DXMatrixLookAtRH(TD3DMatrix* p_out_mat, const TD3DVector3* p_eye, const TD3DVector3* p_at, const TD3DVector3* p_up);
void D3DXMatrixPerspectiveFovLH(TD3DMatrix* p_out_mat, float fovy, float aspect_ratio, float z_near, float z_far);
void D3DXMatrixPerspectiveFovRH(TD3DMatrix* p_out_mat, float fovy, float aspect_ratio, float z_near, float z_far);
void D3DXMatrixOrthoOffCenterLH(TD3DMatrix* p_out_mat, float left, float right, float bottom, float top, float z_near, float z_far);

void D3DXDeclaratorFromFVF(DWORD fvf, D3DVERTEXELEMENT9* p_out_declaration);

////////////////////////////////////////////////////////////////////////////////

namespace erio
{
	typedef float single;

	class I3dActor
	{
	public:
		virtual ~I3dActor()
		{
		}

		virtual unsigned long Process(long ref_time = 0, I3dActor* p_sender = 0) = 0;
	};
}

#endif // #ifndef __IQB_CLASS_3D_H__
