#pragma once

#include <qk/Crash.h>

#define qk_Crash(msg) throw qk::Crash{ msg }
#define qk_Check(p) do { if (!(p)) { qk_Crash("check '" #p "' failed"); } } while (false)
#define qk_Unreachable() qk_Crash("urneachable code path")
