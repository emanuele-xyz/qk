#pragma once

#include <qk/Commons.h>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#include <wrl/client.h> // for ComPtr
namespace wrl = Microsoft::WRL;

#include <d3d11.h>

#define qk_CheckHR(hr) qk_Check(SUCCEEDED(hr))
