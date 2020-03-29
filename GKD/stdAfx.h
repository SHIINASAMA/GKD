#pragma once
#include "base/base.h"
#include "duilib/uilib.h"
#include <Windows.h>
#include <Commdlg.h>
#include <fstream>
#include "resource.h"

enum ThreadId {
    kThreadUI,
    kWorkerThread
};