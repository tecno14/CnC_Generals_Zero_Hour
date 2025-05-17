/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : dx8 caps                                                     *
 *                                                                                             *
#ifdef OG
 *                     $Archive:: /VSS_Sync/ww3d2/dx8caps.cpp                                 $*
#endif
#ifdef ZH
 *                     $Archive:: /Commando/Code/ww3d2/dx8caps.cpp                            $*
#endif
 *                                                                                             *
 *              Original Author:: Hector Yee                                                   *
 *                                                                                             *
#ifdef OG
 *                      $Author:: Vss_sync                                                    $*
#endif
#ifdef ZH
 *                       Author : Kenny Mitchell                                               * 
#endif
 *                                                                                             *
#ifdef OG
 *                     $Modtime:: 8/29/01 8:16p                                               $*
#endif
#ifdef ZH
 *                     $Modtime:: 06/27/02 1:27p                                              $*
#endif
 *                                                                                             *
#ifdef OG
 *                    $Revision:: 11                                                          $*
#endif
#ifdef ZH
 *                    $Revision:: 31                                                          $*
#endif
 *                                                                                             *
#ifdef ZH
 * 06/27/02 KM Z Format support																						*
#endif
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "dx8caps.h"
#include "dx8wrapper.h"
#include "formconv.h"
#ifdef ZH
#pragma warning (disable : 4201)		// nonstandard extension - nameless struct
#include <windows.h>
#include <mmsystem.h>

static StringClass CapsWorkString;

#define DXLOG(n) CapsWorkString.Format n ; CapsLog+=CapsWorkString;
#define COMPACTLOG(n) CapsWorkString.Format n ; CompactLog+=CapsWorkString;

static const char* VendorNames[]={
	"Unknown",
	"NVidia",
	"ATI",
	"Intel",
	"S3",
	"PowerVR",
	"Matrox",
	"3Dfx",
	"3DLabs",
	"CirrusLogic",
	"Rendition"
};

DX8Caps::VendorIdType DX8Caps::Define_Vendor(unsigned vendor_id)
{
	switch (vendor_id) {
	case 0x3d3d:
	case 0x104c: return VENDOR_3DLABS;
	case 0x12D2: // STB - NVIDIA's Riva128
	case 0x14AF: // Guillemot's NVIDIA based cards
	case 0x10de: return VENDOR_NVIDIA;
	case 0x1002: return VENDOR_ATI;
	case 0x8086: return VENDOR_INTEL;
	case 0x5333: return VENDOR_S3;
	case 0x104A: return VENDOR_POWERVR;
	case 0x102B: return VENDOR_MATROX;
	case 0x1142: // Alliance based reference cards
	case 0x109D: // Macronix based reference cards
	case 0x121A: return VENDOR_3DFX;
	default:
		return VENDOR_UNKNOWN;
	}
}

static const char* DeviceNamesNVidia[]={
	"Unknown NVidia device",
	"GeForce3",
	"Quadro2 PRO",
	"GeForce2 Go",
	"GeForce2 ULTRA",
	"GeForce2 GTS",
	"Quadro",
	"GeForce DDR",
	"GeForce 256",
	"TNT2 Aladdin",
	"TNT2",
	"TNT2 ULTRA",
	"TNT2 Vanta",
	"TNT2 M64",
	"TNT",
	"RIVA 128",
	"TNT Vanta",
	"NV1",
	"GeForce2 MX"
	"GeForce4 Ti 4600",
	"GeForce4 Ti 4400",
	"GeForce4 Ti",
	"GeForce4 Ti 4200",
	"GeForce4 MX 460",
	"GeForce4 MX 440",
	"GeForce4 MX 420",
	"GeForce4",
	"GeForce4 Go 440",
	"GeForce4 Go 420",
	"GeForce4 Go 420 32M",
	"GeForce4 Go 440 64M",
	"GeForce4 Go",
	"GeForce3 Ti 500",
	"GeForce3 Ti 200",
	"GeForce2 Integrated",
	"GeForce2 Ti",
	"Quadro2 MXR//EX//GO",
	"GeFORCE2_MX 100//200",
	"GeFORCE2_MX 400",
	"Quadro DCC"
};

static const char* DeviceNamesATI[]={
	"Unknown ATI Device",
	"Rage II",
	"Rage II+",
	"Rage IIc PCI",
	"Rage IIc AGP",
	"Rage 128 Mobility",
	"Rage 128 Mobility M3",
	"Rage 128 Mobility M4",
	"Rage 128 PRO ULTRA",
	"Rage 128 4X",
	"Rage 128 PRO GL",
	"Rage 128 PRO VR",
	"Rage 128 GL",
	"Rage 128 VR",
	"Rage PRO",
	"Rage PRO Mobility",
	"Mobility Radeon",
	"Mobility Radeon VE(M6)",
	"Radeon VE",
	"Radeon DDR",
	"Radeon",
	"Mobility R7500",
	"R7500",
	"R8500"
};

static const char* DeviceNames3DLabs[]={
	"Unknown 3DLabs Device",
	"Permedia",
	"300SX",
	"500TX",
	"Delta",
	"MX",
	"Gamma",
	"Permedia2S (ST)",
	"Permedia3",
	"R3",
	"Permedia4",
	"R4",
	"G2",
	"Oxygen VX1",
	"TI P1",
	"Permedia2"
};

static const char* DeviceNames3Dfx[]={
	"Unknown 3Dfx Device",
	"Voodoo 5500 AGP",
	"Voodoo 3",
	"Banshee",
	"Voodoo 2",
	"Voodoo Graphics",
	"Voodoo Rush"
};

static const char* DeviceNamesMatrox[]={
	"Unknown Matrox Device",
	"G550",
	"G400",
	"G200 AGP",
	"G200 PCI",
	"G100 PCI",
	"G100 AGP",
	"Millennium II AGP",
	"Millennium II PCI",
	"Mystique",
	"Millennium",
	"Parhelia",
	"Parhelia AGP 8X"
};

static const char* DeviceNamesPowerVR[]={
	"Unknown PowerVR Device",
	"Kyro"
};
#endif

#ifdef OG
D3DCAPS8 DX8Caps::hwVPCaps;
D3DCAPS8 DX8Caps::swVPCaps;
bool DX8Caps::UseTnL;	
bool DX8Caps::SupportDOT3;
bool DX8Caps::SupportDXTC;
bool DX8Caps::supportGamma;
bool DX8Caps::SupportNPatches;
bool DX8Caps::SupportBumpEnvmap;
bool DX8Caps::SupportBumpEnvmapLuminance;
bool DX8Caps::SupportTextureFormat[WW3D_FORMAT_COUNT];
int DX8Caps::VertexShaderVersion;
int DX8Caps::PixelShaderVersion;
int DX8Caps::MaxSimultaneousTextures;
#endif
#ifdef ZH
static const char* DeviceNamesS3[]={
	"Unknown S3 Device",
	"Savage MX",
	"Savage 4",
	"Savage 200"
};

#endif

#ifdef OG
enum {
	VENDOR_ID_NVIDIA=0x10de,
	VENROD_ID_ATI=0x1002

#endif
#ifdef ZH
static const char* DeviceNamesIntel[]={
	"Unknown Intel Device",
	"i810",
	"i810e",
	"i815"
#endif
};
#ifdef ZH

DX8Caps::DeviceTypeATI DX8Caps::Get_ATI_Device(unsigned device_id)
{
	switch (device_id) {
	case 0x4754: return DEVICE_ATI_RAGE_II;
	case 0x4755: return DEVICE_ATI_RAGE_II_PLUS;
	case 0x5656: return DEVICE_ATI_RAGE_IIC_PCI;
	case 0x4756: return DEVICE_ATI_RAGE_IIC_PCI;
	case 0x475A: return DEVICE_ATI_RAGE_IIC_AGP;
	case 0x4759: return DEVICE_ATI_RAGE_IIC_AGP;
	case 0x4757: return DEVICE_ATI_RAGE_IIC_AGP;
	case 0x4742:
	case 0x4744:
	case 0x4749:
	case 0x4750:
	case 0x474C:
	case 0x474E:
	case 0x474D:
	case 0x474F:
	case 0x4752: return DEVICE_ATI_RAGE_PRO;

	case 0x4C4D:
	case 0x4C52:
	case 0x4C42:
	case 0x4C49:
	case 0x4C50: return DEVICE_ATI_RAGE_PRO_MOBILITY;

	case 0x4C57: return DEVICE_ATI_MOBILITY_R7500;

	case 0x4C59:
	case 0x4C5A: return DEVICE_ATI_MOBILITY_RADEON_VE_M6;

	case 0x4D46:
	case 0x4D4C: return DEVICE_ATI_RAGE_128_MOBILITY_M4;
	case 0x4C45:
	case 0x4C46: return DEVICE_ATI_RAGE_128_MOBILITY_M3;

	case 0x5041: 
	case 0x5042: 
	case 0x5043: 
	case 0x5044: 
	case 0x5045: 
	case 0x5046: return DEVICE_ATI_RAGE_128_PRO_GL;

	case 0x5047:
	case 0x5048:
	case 0x5049:
	case 0x504A:
	case 0x504B:
	case 0x504C:
	case 0x504D:
	case 0x504E:
	case 0x504F:
	case 0x5050:
	case 0x5051:
	case 0x5052:
	case 0x5053:
	case 0x5054:
	case 0x5055:
	case 0x5056:
	case 0x5057:
	case 0x5058: return DEVICE_ATI_RAGE_128_PRO_VR;

	case 0x5159:
	case 0x515A: return DEVICE_ATI_RADEON_VE;

	case 0x5144:
	case 0x5145:
	case 0x5146:
	case 0x5147: return DEVICE_ATI_RADEON_DDR;

	case 0x514c:
	case 0x514e:
	case 0x514f: return DEVICE_ATI_R8500;

	case 0x5157: return DEVICE_ATI_R7500;

	case 0x5245: 
	case 0x5246: 
	case 0x534B: 
	case 0x534C: 
	case 0x534D: return DEVICE_ATI_RAGE_128_GL;

	case 0x524B:
	case 0x524C:
	case 0x5345:
	case 0x5346:
	case 0x5347: return DEVICE_ATI_RAGE_128_VR;

	case 0x5446:
	case 0x544C:
	case 0x5452:
	case 0x5453:
	case 0x5454:
	case 0x5455: return DEVICE_ATI_RAGE_128_PRO_ULTRA;

	case 0x534E: return DEVICE_ATI_RAGE_128_4X;

	default: return DEVICE_ATI_UNKNOWN;
	}
}

DX8Caps::DeviceType3DLabs DX8Caps::Get_3DLabs_Device(unsigned device_id)
{
	switch (device_id) {
	case 0x0001: return DEVICE_3DLABS_300SX;
	case 0x0002: return DEVICE_3DLABS_500TX;
	case 0x0003: return DEVICE_3DLABS_DELTA;
	case 0x0004: return DEVICE_3DLABS_PERMEDIA;
	case 0x0006: return DEVICE_3DLABS_MX;
	case 0x0007: return DEVICE_3DLABS_PERMEDIA2;
	case 0x0008: return DEVICE_3DLABS_GAMMA;
	case 0x0009: return DEVICE_3DLABS_PERMEDIA2S_ST;
	case 0x000a: return DEVICE_3DLABS_PERMEDIA3;
	case 0x000b: return DEVICE_3DLABS_R3;
	case 0x000c: return DEVICE_3DLABS_PERMEDIA4;
	case 0x000d: return DEVICE_3DLABS_R4;
	case 0x000e: return DEVICE_3DLABS_G2;
	case 0x4C59: return DEVICE_3DLABS_OXYGEN_VX1;
	case 0x3D04: return DEVICE_3DLABS_TI_P1;
	case 0x3D07: return DEVICE_3DLABS_PERMEDIA2;
	default: return DEVICE_3DLABS_UNKNOWN;
	}
}

DX8Caps::DeviceTypeNVidia DX8Caps::Get_NVidia_Device(unsigned device_id)
{
	switch (device_id) {
	case 0x0250: return DEVICE_NVIDIA_GEFORCE4_TI_4600;
	case 0x0251: return DEVICE_NVIDIA_GEFORCE4_TI_4400;
	case 0x0252: return DEVICE_NVIDIA_GEFORCE4_TI;
	case 0x0253: return DEVICE_NVIDIA_GEFORCE4_TI_4200;
	case 0x0170: return DEVICE_NVIDIA_GEFORCE4_MX_460;
	case 0x0171: return DEVICE_NVIDIA_GEFORCE4_MX_440;
	case 0x0172: return DEVICE_NVIDIA_GEFORCE4_MX_420;
	case 0x0173: return DEVICE_NVIDIA_GEFORCE4;
	case 0x0174: return DEVICE_NVIDIA_GEFORCE4_GO_440;
	case 0x0175: return DEVICE_NVIDIA_GEFORCE4_GO_420;
	case 0x0176: return DEVICE_NVIDIA_GEFORCE4_GO_420_32M;
	case 0x0178: return DEVICE_NVIDIA_GEFORCE4_GO;
	case 0x0179: return DEVICE_NVIDIA_GEFORCE4_GO_440_64M;
	case 0x0203: return DEVICE_NVIDIA_QUADRO_DCC;
	case 0x0202: return DEVICE_NVIDIA_GEFORCE3_TI_500;
	case 0x0201: return DEVICE_NVIDIA_GEFORCE3_TI_200;
	case 0x0200: return DEVICE_NVIDIA_GEFORCE3;
	case 0x01A0: return DEVICE_NVIDIA_GEFORCE2_INTEGRATED;
	case 0x0153: return DEVICE_NVIDIA_QUADRO2_PRO;
	case 0x0152: return DEVICE_NVIDIA_GEFORCE2_ULTRA;
	case 0x0151: return DEVICE_NVIDIA_GEFORCE2_TI;
	case 0x0150: return DEVICE_NVIDIA_GEFORCE2_GTS;
	case 0x0113: return DEVICE_NVIDIA_QUADRO2_MXR_EX_GO;
	case 0x0112: return DEVICE_NVIDIA_GEFORCE2_GO;
	case 0x0111: return DEVICE_NVIDIA_GEFORCE2_MX_100_200;
	case 0x0110: return DEVICE_NVIDIA_GEFORCE2_MX_400;
	case 0x0103: return DEVICE_NVIDIA_QUADRO;
	case 0x0101: return DEVICE_NVIDIA_GEFORCE_DDR;
	case 0x0100: return DEVICE_NVIDIA_GEFORCE_256;
	case 0x00A0: return DEVICE_NVIDIA_TNT2_ALADDIN;
	case 0x0028: return DEVICE_NVIDIA_TNT2;
	case 0x0029: return DEVICE_NVIDIA_TNT2_ULTRA;
	case 0x002C: return DEVICE_NVIDIA_TNT2_VANTA;
	case 0x002D: return DEVICE_NVIDIA_TNT2_M64;
	case 0x0020: return DEVICE_NVIDIA_TNT;
	case 0x0008: return DEVICE_NVIDIA_NV1;
#endif

#ifdef ZH
	// STB cards
	case 0x0019:
	case 0x0018: return DEVICE_NVIDIA_RIVA_128;

	// Guillemot Cards
	case 0x5008: return DEVICE_NVIDIA_TNT_VANTA;		// Maxi Gamer Phoenix 2
	case 0x5810: return DEVICE_NVIDIA_TNT2;			// Maxi Gamer Xentor
	case 0x5820: return DEVICE_NVIDIA_TNT2_ULTRA;	// Maxi Gamer Xentor 32
	case 0x4d20: return DEVICE_NVIDIA_TNT2_M64;		// Maxi Gamer Cougar
	case 0x5620: return DEVICE_NVIDIA_TNT2_M64;		// Maxi Gamer Cougar Video Edition
	case 0x5020: return DEVICE_NVIDIA_GEFORCE_256;	// Maxi Gamer 3D Prophet

	default: return DEVICE_NVIDIA_UNKNOWN;
	}
}

DX8Caps::DeviceType3Dfx DX8Caps::Get_3Dfx_Device(unsigned device_id)
{
	switch (device_id) {
	case 0x0009: return DEVICE_3DFX_VOODOO_5500_AGP;
	case 0x0005: return DEVICE_3DFX_VOODOO_3;
	case 0x0003: return DEVICE_3DFX_BANSHEE;
	case 0x0002: return DEVICE_3DFX_VOODOO_2;
	case 0x0001: return DEVICE_3DFX_VOODOO_GRAPHICS;
	case 0x643d: // Alliance AT25/AT3D based reference board
	case 0x8626: // Macronix based reference board
		return DEVICE_3DFX_VOODOO_RUSH;
	default: return DEVICE_3DFX_UNKNOWN;
	}
}

DX8Caps::DeviceTypeMatrox DX8Caps::Get_Matrox_Device(unsigned device_id)
{
	switch (device_id) {
	case 0x2527: return DEVICE_MATROX_G550;
	case 0x0525: return DEVICE_MATROX_G400;
	case 0x0521: return DEVICE_MATROX_G200_AGP;
	case 0x0520: return DEVICE_MATROX_G200_PCI;
	case 0x1000: return DEVICE_MATROX_G100_PCI;
	case 0x1001: return DEVICE_MATROX_G100_AGP;
	case 0x051F: return DEVICE_MATROX_MILLENNIUM_II_AGP;
	case 0x051B: return DEVICE_MATROX_MILLENNIUM_II_PCI;
	case 0x051A: return DEVICE_MATROX_MYSTIQUE;
	case 0x0519: return DEVICE_MATROX_MILLENNIUM;
	case 0x0527: return DEVICE_MATROX_PARHELIA;
	case 0x0528: return DEVICE_MATROX_PARHELIA_AGP8X;

	default: return DEVICE_MATROX_UNKNOWN;
	}
}
 
DX8Caps::DeviceTypePowerVR DX8Caps::Get_PowerVR_Device(unsigned device_id)
{
	switch (device_id) {
	case 0x0010: return DEVICE_POWERVR_KYRO;
	default: return DEVICE_POWERVR_UNKNOWN;
	}
}

DX8Caps::DeviceTypeS3 DX8Caps::Get_S3_Device(unsigned device_id)
{
	switch (device_id) {
	case 0x8C10: return DEVICE_S3_SAVAGE_MX;
	case 0x8A22: return DEVICE_S3_SAVAGE_4;
	case 0x9102: return DEVICE_S3_SAVAGE_200;
	default: return DEVICE_S3_UNKNOWN;
	}
}

DX8Caps::DeviceTypeIntel DX8Caps::Get_Intel_Device(unsigned device_id)
{
	switch (device_id) {
	case 0x7123: return DEVICE_INTEL_810;
	case 0x7121: return DEVICE_INTEL_810E;
	case 0x1132: return DEVICE_INTEL_815;
	default: return DEVICE_INTEL_UNKNOWN;
	}
}

DX8Caps::DX8Caps(
	IDirect3D8* direct3d,
	IDirect3DDevice8* D3DDevice, 
	WW3DFormat display_format, 
	const D3DADAPTER_IDENTIFIER8& adapter_id)
	:
	Direct3D(direct3d),
	MaxDisplayWidth(0),
	MaxDisplayHeight(0)
{
	Init_Caps(D3DDevice);
	Compute_Caps(display_format, adapter_id);
}

DX8Caps::DX8Caps(
	IDirect3D8* direct3d,
	const D3DCAPS8& caps, 
	WW3DFormat display_format, 
	const D3DADAPTER_IDENTIFIER8& adapter_id)
	:
	Direct3D(direct3d),
	Caps(caps),
	MaxDisplayWidth(0),
	MaxDisplayHeight(0)
{
	if ((Caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)==D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		SupportTnL=true;
	} else {
		SupportTnL=false;			
	}

	Compute_Caps(display_format,adapter_id);
}

//Don't really need this but I added this function to free static variables so
//they don't show up in our memory manager as a leak. -MW 7-22-03
void DX8Caps::Shutdown(void)
{
	CapsWorkString.Release_Resources();
}

#endif
// ----------------------------------------------------------------------------
//
// Init the caps structure
//
// ----------------------------------------------------------------------------

void DX8Caps::Init_Caps(IDirect3DDevice8* D3DDevice)
{
	D3DDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING,TRUE);
#ifdef OG
	DX8CALL(GetDeviceCaps(&swVPCaps));
#endif
#ifdef ZH
	DX8CALL(GetDeviceCaps(&Caps));
#endif

#ifdef OG
	if ((swVPCaps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)==D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		UseTnL=true;
#endif
#ifdef ZH
	if ((Caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)==D3DDEVCAPS_HWTRANSFORMANDLIGHT) {
		SupportTnL=true;
#endif

		D3DDevice->SetRenderState(D3DRS_SOFTWAREVERTEXPROCESSING,FALSE);
#ifdef OG
		DX8CALL(GetDeviceCaps(&hwVPCaps));	
#endif
#ifdef ZH
		DX8CALL(GetDeviceCaps(&Caps));	
#endif
	} else {
#ifdef OG
		UseTnL=false;			
#endif
#ifdef ZH
		SupportTnL=false;			
#endif
	}
}

// ----------------------------------------------------------------------------
//
// Compute the caps bits
//
// ----------------------------------------------------------------------------
#ifdef ZH
void DX8Caps::Compute_Caps(WW3DFormat display_format, const D3DADAPTER_IDENTIFIER8& adapter_id)
{
//	Init_Caps(D3DDevice);
#endif

#ifdef OG
void DX8Caps::Compute_Caps(D3DFORMAT display_format, D3DFORMAT depth_stencil_format, IDirect3DDevice8* D3DDevice)
{
	const D3DADAPTER_IDENTIFIER8& adapter_id=DX8Wrapper::Get_Current_Adapter_Identifier();

#endif
#ifdef ZH
	CanDoMultiPass=true;
	IsFogAllowed=true;

	CapsLog="";
	CompactLog="";
	DXLOG(("Video Card: %s\r\n",adapter_id.Description));
	DXLOG(("Driver: %s\r\n",adapter_id.Driver));
#endif

#ifdef OG
	Init_Caps(D3DDevice);

#endif
#ifdef ZH
	DriverDLL=adapter_id.Driver;
	int Product = HIWORD(adapter_id.DriverVersion.HighPart);
	int Version = LOWORD(adapter_id.DriverVersion.HighPart);
	int SubVersion = HIWORD(adapter_id.DriverVersion.LowPart);
	DriverBuildVersion = LOWORD(adapter_id.DriverVersion.LowPart);
#endif

#ifdef OG
	const D3DCAPS8& caps=Get_Default_Caps();
#endif
#ifdef ZH
	DXLOG(("Product=%d, Version=%d, SubVersion=%d, Build=%d\r\n",Product, Version, SubVersion, DriverBuildVersion));
#endif

#ifdef OG
	if ((caps.DevCaps&D3DDEVCAPS_NPATCHES)==D3DDEVCAPS_NPATCHES) {
		SupportNPatches=true;
	} else {
		SupportNPatches=false;

#endif
#ifdef ZH
	VendorId=Define_Vendor(adapter_id.VendorId);
	// Make a guess - if driver doesn't intruduce itself and the name starts with 3, what could it possibly be?
	if (VendorId==VENDOR_UNKNOWN) {
		if (DriverDLL[0]=='3') VendorId=VENDOR_3DFX;
	}
	COMPACTLOG(("%s\t",VendorNames[VendorId]));
	DXLOG(("Video Card Chip Vendor: %s\r\n",VendorNames[VendorId]));
	DXLOG(("Type of chip: "));
	switch (VendorId) {
	default:
	case VENDOR_UNKNOWN:
		DeviceId=0;
		DXLOG(("Unknown"));
		COMPACTLOG(("Unknown"));
		break;
	case VENDOR_NVIDIA:
		DeviceId=(unsigned)Get_NVidia_Device(adapter_id.DeviceId);
		DXLOG((DeviceNamesNVidia[DeviceId]));
		COMPACTLOG((DeviceNamesNVidia[DeviceId]));
		break;
	case VENDOR_ATI:
		DeviceId=(unsigned)Get_ATI_Device(adapter_id.DeviceId);
		DXLOG((DeviceNamesATI[DeviceId]));
		COMPACTLOG((DeviceNamesATI[DeviceId]));
		break;
	case VENDOR_INTEL:
		DeviceId=(unsigned)Get_Intel_Device(adapter_id.DeviceId);
		DXLOG((DeviceNamesIntel[DeviceId]));
		COMPACTLOG((DeviceNamesIntel[DeviceId]));
		break;
	case VENDOR_S3:
		DeviceId=(unsigned)Get_S3_Device(adapter_id.DeviceId);
		DXLOG((DeviceNamesS3[DeviceId]));
		COMPACTLOG((DeviceNamesS3[DeviceId]));
		break;
	case VENDOR_POWERVR:
		DeviceId=(unsigned)Get_PowerVR_Device(adapter_id.DeviceId);
		DXLOG((DeviceNamesPowerVR[DeviceId]));
		COMPACTLOG((DeviceNamesPowerVR[DeviceId]));
		break;
	case VENDOR_MATROX:
		DeviceId=(unsigned)Get_Matrox_Device(adapter_id.DeviceId);
		DXLOG((DeviceNamesMatrox[DeviceId]));
		COMPACTLOG((DeviceNamesMatrox[DeviceId]));
		break;
	case VENDOR_3DFX:
		DeviceId=(unsigned)Get_3Dfx_Device(adapter_id.DeviceId);
		DXLOG((DeviceNames3Dfx[DeviceId]));
		COMPACTLOG((DeviceNames3Dfx[DeviceId]));
		break;
	case VENDOR_3DLABS:
		DeviceId=(unsigned)Get_3DLabs_Device(adapter_id.DeviceId);
		DXLOG((DeviceNames3DLabs[DeviceId]));
		COMPACTLOG((DeviceNames3DLabs[DeviceId]));
		break;
#endif
	}
#ifdef ZH

	COMPACTLOG(("\t%d\t",DriverBuildVersion));

	DXLOG(("\r\n"));

	DXLOG(("Vendor id: 0x%x\r\n",adapter_id.VendorId));
	DXLOG(("Device id: 0x%x\r\n",adapter_id.DeviceId));
	DXLOG(("SubSys id: 0x%x\r\n",adapter_id.SubSysId));
	DXLOG(("Revision: %d\r\n",adapter_id.Revision));

	DXLOG(("GUID = {0x%x, 0x%x, 0x%x}, {0x%2.2x, 0x%2.2x, 0x%2.2x, 0x%2.2x, 0x%2.2x, 0x%2.2x, 0x%2.2x, 0x%2.2x}\r\n",
		adapter_id.DeviceIdentifier.Data1,
		adapter_id.DeviceIdentifier.Data2,
		adapter_id.DeviceIdentifier.Data3,
		adapter_id.DeviceIdentifier.Data4[0],
		adapter_id.DeviceIdentifier.Data4[1],
		adapter_id.DeviceIdentifier.Data4[2],
		adapter_id.DeviceIdentifier.Data4[3],
		adapter_id.DeviceIdentifier.Data4[4],
		adapter_id.DeviceIdentifier.Data4[5],
		adapter_id.DeviceIdentifier.Data4[6],
		adapter_id.DeviceIdentifier.Data4[7]));

	SupportPointSprites = (Caps.MaxPointSize > 1.0f);
	SupportNPatches = ((Caps.DevCaps&D3DDEVCAPS_NPATCHES)==D3DDEVCAPS_NPATCHES);
	SupportZBias = ((Caps.RasterCaps&D3DPRASTERCAPS_ZBIAS)==D3DPRASTERCAPS_ZBIAS);
	supportGamma=((Caps.Caps2&D3DCAPS2_FULLSCREENGAMMA)==D3DCAPS2_FULLSCREENGAMMA);
	SupportModAlphaAddClr = (Caps.TextureOpCaps & D3DTEXOPCAPS_MODULATEALPHA_ADDCOLOR) == D3DTEXOPCAPS_MODULATEALPHA_ADDCOLOR;
	SupportDot3=(Caps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3) == D3DTEXOPCAPS_DOTPRODUCT3;
	SupportCubemaps=(Caps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP) == D3DPTEXTURECAPS_CUBEMAP;
	SupportAnisotropicFiltering=
		(Caps.TextureFilterCaps&D3DPTFILTERCAPS_MAGFANISOTROPIC) && (Caps.TextureFilterCaps&D3DPTFILTERCAPS_MINFANISOTROPIC);

	DXLOG(("Hardware T&L support: %s\r\n",SupportTnL ? "Yes" : "No"));
	DXLOG(("NPatch support: %s\r\n",SupportNPatches ? "Yes" : "No"));
	DXLOG(("ZBias support: %s\r\n",SupportZBias ? "Yes" : "No"));
	DXLOG(("Gamma support: %s\r\n",supportGamma ? "Yes" : "No"));
	DXLOG(("ModAlphaAddClr support: %s\r\n",SupportModAlphaAddClr ? "Yes" : "No"));
	DXLOG(("Dot3 support: %s\r\n",SupportDot3 ? "Yes" : "No"));
	DXLOG(("Anisotropic filtering support: %s\r\n",SupportAnisotropicFiltering ? "Yes" : "No"));
#endif

#ifdef OG
	if ((caps.TextureOpCaps&D3DTEXOPCAPS_DOTPRODUCT3)==D3DTEXOPCAPS_DOTPRODUCT3) {
		SupportDOT3=true;
	} else {
		SupportDOT3=false;
	}

#endif
#ifdef ZH
	Check_Texture_Format_Support(display_format,Caps);
	Check_Render_To_Texture_Support(display_format,Caps);
	Check_Depth_Stencil_Support(display_format,Caps);
	Check_Texture_Compression_Support(Caps);
	Check_Bumpmap_Support(Caps);
	Check_Shader_Support(Caps);
	Check_Driver_Version_Status();
	Check_Maximum_Texture_Support(Caps);
#endif

#ifdef OG
	supportGamma=((swVPCaps.Caps2&D3DCAPS2_FULLSCREENGAMMA)==D3DCAPS2_FULLSCREENGAMMA);
#endif
#ifdef ZH
	MaxTexturesPerPass=Caps.MaxSimultaneousTextures;
#endif

#ifdef OG
	Check_Texture_Format_Support(display_format,caps);
	Check_Texture_Compression_Support(caps);
	Check_Bumpmap_Support(caps);
	Check_Shader_Support(caps);
	Check_Maximum_Texture_Support(caps);
#endif
#ifdef ZH
	DXLOG(("Max textures per pass: %d\r\n",MaxTexturesPerPass));

#endif
	Vendor_Specific_Hacks(adapter_id);
#ifdef ZH
	CapsWorkString="";
#endif
}

// ----------------------------------------------------------------------------
//
// Check bump map texture support
//
// ----------------------------------------------------------------------------

void DX8Caps::Check_Bumpmap_Support(const D3DCAPS8& caps)
{
	SupportBumpEnvmap=!!(caps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP);
	SupportBumpEnvmapLuminance=!!(caps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAPLUMINANCE);
#ifdef ZH
	DXLOG(("Bumpmap support: %s\r\n",SupportBumpEnvmap ? "Yes" : "No"));
	DXLOG(("Bumpmap luminance support: %s\r\n",SupportBumpEnvmapLuminance ? "Yes" : "No"));
#endif
}

// ----------------------------------------------------------------------------
//
// Check compressed texture support
//
// ----------------------------------------------------------------------------

void DX8Caps::Check_Texture_Compression_Support(const D3DCAPS8& caps)
{
	SupportDXTC=SupportTextureFormat[WW3D_FORMAT_DXT1]|
		SupportTextureFormat[WW3D_FORMAT_DXT2]|
		SupportTextureFormat[WW3D_FORMAT_DXT3]|
		SupportTextureFormat[WW3D_FORMAT_DXT4]|
		SupportTextureFormat[WW3D_FORMAT_DXT5];
#ifdef ZH
	DXLOG(("Texture compression support: %s\r\n",SupportDXTC ? "Yes" : "No"));
#endif
}

#ifdef OG
void DX8Caps::Check_Texture_Format_Support(D3DFORMAT display_format,const D3DCAPS8& caps)
#endif
#ifdef ZH
void DX8Caps::Check_Texture_Format_Support(WW3DFormat display_format,const D3DCAPS8& caps)
#endif
{
#ifdef ZH
	if (display_format==WW3D_FORMAT_UNKNOWN) {
		for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
			SupportTextureFormat[i]=false;
		}
		return;
	}
	D3DFORMAT d3d_display_format=WW3DFormat_To_D3DFormat(display_format);
#endif
	for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
		if (i==WW3D_FORMAT_UNKNOWN) {
			SupportTextureFormat[i]=false;
		}
		else {
#ifdef ZH
			WW3DFormat format=(WW3DFormat)i;
#endif
			SupportTextureFormat[i]=SUCCEEDED(
#ifdef OG
				DX8Wrapper::_Get_D3D8()->CheckDeviceFormat(
#endif
#ifdef ZH
				Direct3D->CheckDeviceFormat(
#endif
					caps.AdapterOrdinal,
					caps.DeviceType,
#ifdef OG
					display_format,
#endif
#ifdef ZH
					d3d_display_format,
#endif
					0,
					D3DRTYPE_TEXTURE,
#ifdef OG
					WW3DFormat_To_D3DFormat((WW3DFormat)i)));

#endif
#ifdef ZH
					WW3DFormat_To_D3DFormat(format)));
			if (SupportTextureFormat[i]) {
				StringClass name(0,true);
				Get_WW3D_Format_Name(format,name);
				DXLOG(("Supports texture format: %s\r\n",name));
			}
		}
	}
}

void DX8Caps::Check_Render_To_Texture_Support(WW3DFormat display_format,const D3DCAPS8& caps)
{
	if (display_format==WW3D_FORMAT_UNKNOWN) {
		for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
			SupportRenderToTextureFormat[i]=false;
		}
		return;
	}
	D3DFORMAT d3d_display_format=WW3DFormat_To_D3DFormat(display_format);
	for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
		if (i==WW3D_FORMAT_UNKNOWN) {
			SupportRenderToTextureFormat[i]=false;
		}
		else {
			WW3DFormat format=(WW3DFormat)i;
			SupportRenderToTextureFormat[i]=SUCCEEDED(
				Direct3D->CheckDeviceFormat(
					caps.AdapterOrdinal,
					caps.DeviceType,
					d3d_display_format,
					D3DUSAGE_RENDERTARGET,
					D3DRTYPE_TEXTURE,
					WW3DFormat_To_D3DFormat(format)));
			if (SupportRenderToTextureFormat[i]) {
				StringClass name(0,true);
				Get_WW3D_Format_Name(format,name);
				DXLOG(("Supports render-to-texture format: %s\r\n",name));
			}
		}
	}
}

//**********************************************************************************************
//! Check Depth Stencil Format Support
/*! KJM
*/
void DX8Caps::Check_Depth_Stencil_Support(WW3DFormat display_format, const D3DCAPS8& caps)
{
	if (display_format==WW3D_FORMAT_UNKNOWN) 
	{
		for (unsigned i=0;i<WW3D_ZFORMAT_COUNT;++i) 
		{
			SupportDepthStencilFormat[i]=false;
		}
		return;
	}

	D3DFORMAT d3d_display_format=WW3DFormat_To_D3DFormat(display_format);
	
	for (unsigned i=0;i<WW3D_ZFORMAT_COUNT;++i) 
	{
		if (i==WW3D_ZFORMAT_UNKNOWN) 
		{
			SupportDepthStencilFormat[i]=false;
		}
		else 
		{
			WW3DZFormat format=(WW3DZFormat)i;
			SupportDepthStencilFormat[i]=SUCCEEDED
			(
				Direct3D->CheckDeviceFormat
				(
					caps.AdapterOrdinal,
					caps.DeviceType,
					d3d_display_format,
					D3DUSAGE_DEPTHSTENCIL,
					D3DRTYPE_TEXTURE,
					WW3DZFormat_To_D3DFormat(format)
				)
			);

			if (SupportDepthStencilFormat[i]) 
			{
				StringClass name(0,true);
				Get_WW3D_ZFormat_Name(format,name);
				DXLOG(("Supports depth stencil format: %s\r\n",name));
			}
#endif
		}
	}
}

void DX8Caps::Check_Maximum_Texture_Support(const D3DCAPS8& caps)
{
	MaxSimultaneousTextures=caps.MaxSimultaneousTextures;
}

void DX8Caps::Check_Shader_Support(const D3DCAPS8& caps)
{
	VertexShaderVersion=caps.VertexShaderVersion;
	PixelShaderVersion=caps.PixelShaderVersion;
#ifdef ZH
	DXLOG(("Vertex shader version: %d.%d, pixel shader version: %d.%d\r\n",
		(VertexShaderVersion>>8)&0xff,VertexShaderVersion&0xff,
		(PixelShaderVersion>>8)&0xff,PixelShaderVersion&0xff));
}

void DX8Caps::Check_Driver_Version_Status()
{
	DriverVersionStatus=DRIVER_STATUS_UNKNOWN;

	switch (VendorId) {
	// All 3Dfx drivers are bad
	case VENDOR_3DFX:
		DriverVersionStatus=DRIVER_STATUS_BAD;
		break;
	case VENDOR_NVIDIA:
		if (!stricmp(DriverDLL,"nv4.dll")) {
			switch (DriverBuildVersion) {
			case 327:	// 5.00.2165.327
				DriverVersionStatus=DRIVER_STATUS_BAD;
#endif
}
#ifdef ZH
		}
#endif

#ifdef ZH
		if (!stricmp(DriverDLL,"nv4_disp.dll") || !stricmp(DriverDLL,"nvdd32.dll")) {
			switch (DriverBuildVersion) {
			// 23.11 Is known to be very unstable
			case 2311:
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			// 21.83 Has occasional lock-up at start or exit
			// Darren Korman (DKORMAN2389-2K GeForce3)
			case 2183:
			case 2240:
				DriverVersionStatus=DRIVER_STATUS_OK;
				break;
			// 21.81 Has occasional lock-up at start or exit
			case 2181:
				DriverVersionStatus=DRIVER_STATUS_OK;
				break;
			case 1440:		// Denzil had problems in opening 16 bit modes, fixed by updating driver
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			case 1410:
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			case 1260:	// Byon - BYONG
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			case 1241:	// Steve Tall gets occasional blue screening with this driver version (blue screen happens in the driver dll)
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			case 1240:	// Robert Powers
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			case 1101:
				DriverVersionStatus=DRIVER_STATUS_BAD;//DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
				break;
			case 650:	// Rich Donelly - RENEGADE-JENNA2
				DriverVersionStatus=DRIVER_STATUS_BAD;//DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
				break;
			case 649:
				DriverVersionStatus=DRIVER_STATUS_BAD;//DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
				break;
			case 635:
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			case 634:	// Sean Decker - SDECKER2339-2K
				DriverVersionStatus=DRIVER_STATUS_BAD;//DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
				break;
			case 625:	// TESTIBM240
				DriverVersionStatus=DRIVER_STATUS_BAD;//DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
				break;
			case 618:
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			default:
				if (DriverBuildVersion<2000) {	// All under 20.xx versions are too old!
					DriverVersionStatus=DRIVER_STATUS_BAD;
				}
				else {
					DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
				}
			}
		}
		// Elsa OEM drivers?
		if (!stricmp(DriverDLL,"egdad.dll")) {
			// We know of version 5.9.0.312 (asked MShelling if he the drivers seem ok)
			switch (DriverBuildVersion) {
			default:
				DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
			case 312:
				DriverVersionStatus=DRIVER_STATUS_OK;
			}
		}

		// Elsa GLoria
		if (!stricmp(DriverDLL,"egliid.dll")) {
			switch (DriverBuildVersion) {
			default:
				DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
			case 172:
				DriverVersionStatus=DRIVER_STATUS_OK;
			}

		}

		// ASUS OEM drivers?
		if (!stricmp(DriverDLL,"v66_disp.dll")) {
		// TOMSS1: 5.0.2195.379
		}
		break;
	case VENDOR_ATI:
		if (!stricmp(DriverDLL,"ati2dvag.dll")) {
			switch (DriverBuildVersion) {
			case 3287:
				DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
				break;
			case 3281:
				DriverVersionStatus=DRIVER_STATUS_OK;	// Not really ok, but we have to accept something...
				break;
			case 3063:
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			case 3273:
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			case 3276:
				DriverVersionStatus=DRIVER_STATUS_BAD;
				break;
			}
		}
		if (!stricmp(DriverDLL,"atid32ae.dll")) {
			switch (DriverBuildVersion) {
			case 1010:
				DriverVersionStatus=DRIVER_STATUS_OK;
			}
		}
		if (!stricmp(DriverDLL,"ati3drai.dll")) {
			switch (DriverBuildVersion) {
			case 1119:
				DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
			}
		}
		break;
	case VENDOR_POWERVR:
		if (!stricmp(DriverDLL,"pmx2hal.dll")) {
			switch (DriverBuildVersion) {
			case 3111:	// Michael Ruppert - TESTIBM104
			default: DriverVersionStatus=DRIVER_STATUS_UNKNOWN;
			}
		}
		break;
	}

	switch (DriverVersionStatus) {
	default:
	case DRIVER_STATUS_UNKNOWN:
		DXLOG(("Driver version status: Unknown\r\n"));
		break;
	case DRIVER_STATUS_OK:
		DXLOG(("Driver version status: OK (No known problems)\r\n"));
		break;
	case DRIVER_STATUS_GOOD:
		DXLOG(("Driver version status: Good\r\n"));
		break;
	case DRIVER_STATUS_BAD:
		DXLOG(("Driver version status: Bad (Driver update recommended)\r\n"));
		break;
	}
}

bool DX8Caps::Is_Valid_Display_Format(int width, int height, WW3DFormat format)
{
	// If nothing limits the maximum resolution, accept any resolution
	if (MaxDisplayWidth==0 && MaxDisplayHeight==0) return true;

	if (width>MaxDisplayWidth || height>MaxDisplayHeight) return false;
	return true;
}

#endif
// ----------------------------------------------------------------------------
//
// Implement some vendor-specific hacks to fix certain driver bugs that can't be
// avoided otherwise.
//
// ----------------------------------------------------------------------------

void DX8Caps::Vendor_Specific_Hacks(const D3DADAPTER_IDENTIFIER8& adapter_id)
{
#ifdef OG
	if (adapter_id.VendorId==VENDOR_ID_NVIDIA) {

#endif
#ifdef ZH
	if (VendorId==VENDOR_NVIDIA) 
  {
		if (SupportNPatches) {
			DXLOG(("NVidia Driver reported N-Patch support, disabling.\r\n"));
		}
		if (SupportTextureFormat[WW3D_FORMAT_DXT1]) {
			DXLOG(("Disabling DXT1 support on NVidia hardware.\r\n"));
		}

#endif
		SupportNPatches = false;	// Driver incorrectly report N-Patch support
		SupportTextureFormat[WW3D_FORMAT_DXT1] = false;			// DXT1 is broken on NVidia hardware
		SupportDXTC=
			SupportTextureFormat[WW3D_FORMAT_DXT1]|
			SupportTextureFormat[WW3D_FORMAT_DXT2]|
			SupportTextureFormat[WW3D_FORMAT_DXT3]|
			SupportTextureFormat[WW3D_FORMAT_DXT4]|
			SupportTextureFormat[WW3D_FORMAT_DXT5];
#ifdef ZH


    if (DeviceId == DEVICE_NVIDIA_GEFORCE2_MX ||
        DeviceId == DEVICE_NVIDIA_GEFORCE2_MX_400 )
    {
		  DXLOG(("Maximum screen resolution limited to 1024 x 768 on NVidia GeForce2 mx/mx400 cards\r\n"));			
		  MaxDisplayWidth=1024;
		  MaxDisplayHeight=768;
    }

	}

	if (VendorId==VENDOR_MATROX) {
		// G400 and G550 claim support for ModAlphaAddClr but argument limitations make it unusable.
		if (DeviceId==DEVICE_MATROX_G400 ||
			DeviceId==DEVICE_MATROX_G550) {
			DXLOG(("ModAlphaAddClr disabled Matrox G400 and G550 cards (cannot put texture in 2nd arg)\r\n"));
			SupportModAlphaAddClr = false;
		}
	}

	if (VendorId==VENDOR_ATI) {
		// Rage Pro doesn't handle multitexturing well enough...
		// It also doesn't really handle render-to-texture...
		if (DeviceId==DEVICE_ATI_RAGE_PRO || DeviceId==DEVICE_ATI_RAGE_PRO_MOBILITY) {
			DXLOG(("Disabling multitexturing on ATI Rage Pro\r\n"));			
			MaxTexturesPerPass=1;
			CanDoMultiPass=false;

			DXLOG(("Disabling render-to-texture on Rage Pro\r\n"));
			for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
				SupportRenderToTextureFormat[i]=false;
			}
		}

		// Rage 128 Pro GL is used in ATI Rage Fury Maxx
		// The driver support is bad and the card seems to suffer quite a bit from
		// lock-ups and rendering errors. These ease up when multitexturing and
		// render-to-texture are disabled.
		// However, E&B requires multitexturing, so renabling it.
		if (DeviceId==DEVICE_ATI_RAGE_128_PRO_GL) {
			CanDoMultiPass=false;

			DXLOG(("Disabling render-to-texture on ATI Rage 128 Pro GL\r\n"));
			for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
				SupportRenderToTextureFormat[i]=false;
			}

		}

		if (DeviceId==DEVICE_ATI_RAGE_128_MOBILITY ||
			DeviceId==DEVICE_ATI_RAGE_128_MOBILITY_M3 ||
			DeviceId==DEVICE_ATI_RAGE_128_MOBILITY_M4 ||
			DeviceId==DEVICE_ATI_RAGE_128_PRO_ULTRA ||
			DeviceId==DEVICE_ATI_RAGE_128_4X ||
			DeviceId==DEVICE_ATI_RAGE_128_PRO_GL ||
			DeviceId==DEVICE_ATI_RAGE_128_PRO_VR ||
			DeviceId==DEVICE_ATI_RAGE_128_GL ||
			DeviceId==DEVICE_ATI_RAGE_128_VR) {
			DXLOG(("Maximum screen resolution limited to 1280 x 1024 on ATI Rage 128 cards\r\n"));			
			MaxDisplayWidth=1280;
			MaxDisplayHeight=1024;
			DXLOG(("ModAlphaAddClr disabled ATI Rage 128 cards (cannot put texture in 2nd arg)\r\n"));
			SupportModAlphaAddClr = false;
		}

		if (DeviceId==DEVICE_ATI_MOBILITY_RADEON ||
			DeviceId==DEVICE_ATI_MOBILITY_RADEON_VE_M6 ||
			DeviceId==DEVICE_ATI_RADEON_VE ||
			DeviceId==DEVICE_ATI_RADEON_DDR ||
			DeviceId==DEVICE_ATI_RADEON ||
			DeviceId==DEVICE_ATI_MOBILITY_R7500 ||
			DeviceId==DEVICE_ATI_R7500) {
			DXLOG(("Disabling render-to-texture on Radeon\r\n"));
			for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
				SupportRenderToTextureFormat[i]=false;
			}
		}

		// CAT-lab reported that selecting anisotorpic filtering on Radeon VE causes a lock up after a while
		if (DeviceId==DEVICE_ATI_RADEON_VE) {
			DXLOG(("Disabling anisotropic filtering on Radeon VE\r\n"));
			SupportAnisotropicFiltering=false;
		}

	}

	if (VendorId==VENDOR_3DFX) {
		// Voodoo2, Banshee and Voodoo3 don't handle multi-texturing with DX8 well enough.
		if (DeviceId==DEVICE_3DFX_VOODOO_3 ||
			DeviceId==DEVICE_3DFX_BANSHEE ||
			DeviceId==DEVICE_3DFX_VOODOO_2) {
			DXLOG(("Disabling multitexturing on Voodoo2/Voodoo3/Banshee\r\n"));			
			MaxTexturesPerPass=1;	// Especially important on Banshee (multitexturing crashes)!!!

			DXLOG(("Maximum screen resolution limited to 1280 x 1024 on Voodoo2/Voodoo3/Banshee\r\n"));			
			MaxDisplayWidth=1280;
			MaxDisplayHeight=1024;
#endif
	}

#ifdef OG
//	SupportDXTC=false;

#endif
#ifdef ZH
		if (DeviceId==DEVICE_3DFX_VOODOO_3) {
			DXLOG(("Disabling render-to-texture on Voodoo3\r\n"));
			for (unsigned i=0;i<WW3D_FORMAT_COUNT;++i) {
				SupportRenderToTextureFormat[i]=false;
			}
		}
	}

	if (VendorId==VENDOR_POWERVR) {
		DXLOG(("Maximum screen resolution limited to 1280 x 1024 on PowerVR Kyro cards\r\n"));			
		MaxDisplayWidth=1280;
		MaxDisplayHeight=1024;

		// No fog on scene capture cards! (They don't like fog color changes in mid-frame)
		IsFogAllowed=false;
	}

	if (VendorId==VENDOR_S3) {
		if (DeviceId==DEVICE_S3_SAVAGE_4) {
			DXLOG(("Maximum screen resolution limited to 1024 x 768 on S3 Savage 4 cards\r\n"));			
			MaxDisplayWidth=800;//1024;
			MaxDisplayHeight=600;//768;
		}

		if (DeviceId==DEVICE_S3_SAVAGE_200) {
			DXLOG(("Disabling multitexturing on S3 Savage 2000\r\n"));			
			MaxTexturesPerPass=1;
			CanDoMultiPass=false;
		}

#endif

}
#ifdef ZH
}

#endif
