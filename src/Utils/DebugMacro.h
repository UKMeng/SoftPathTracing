//
// Created by UKMeng on 2024/9/11.
//

#pragma once

#ifdef DEBUG
#define DEBUG_LINE(...) __VA_ARGS__;
#else
#define DEBUG_LINE(...)
#endif