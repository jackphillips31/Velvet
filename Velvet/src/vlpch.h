#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Velvet/Core/Log.h"
#include "Velvet/Debug/Instrumentor.h"

#ifdef VL_PLATFORM_WINDOWS
	#include <Windows.h>
#endif