#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "nOs/Log.h"

#ifdef NOS_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <d3d11.h>
	#include <wrl/client.h>
	#include <DirectXMath.h>
	#include <d3dcompiler.h>
#endif