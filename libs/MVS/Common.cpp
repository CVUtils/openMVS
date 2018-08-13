/*
* Common.cpp
*
* Copyright (c) 2014-2015 SEACAVE
*
* Author(s):
*
*      cDc <cdc.seacave@gmail.com>
*
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*
* Additional Terms:
*
*      You are required to preserve legal notices and author attributions in
*      that material or in the Appropriate Legal Notices displayed by works
*      containing it.
*/

// Source file that includes just the standard includes
// Common.pch will be the pre-compiled header
// Common.obj will contain the pre-compiled type information

#include "Common.h"

#ifdef Common_EXPORTS
namespace MVS
{
    MVS_API void Log_Open()
    {
        GET_LOG().Open();
    }
    MVS_API void Log_Close()
    {
        GET_LOG().Close();
    }
    MVS_API void Log_Register(Log::ClbkRecordMsg clbk)
    {
        GET_LOG().RegisterListener(clbk);
    }
    MVS_API void Log_Unregister(Log::ClbkRecordMsg clbk)
    {
        GET_LOG().UnregisterListener(clbk);
    }
    MVS_API void Log_Write(LPCSTR szFormat, ...)
    {
        va_list args;
        va_start(args, szFormat);
        GET_LOG().Write(szFormat, args);
    }
    MVS_API void Log_Write(UINT lt, LPCSTR szFormat, ...)
    {
        va_list args;
        va_start(args, szFormat);
        GET_LOG().Write(lt, szFormat, args);
    }
    MVS_API int Exif_FromBuf(EXIFInfo& info, const unsigned char *buf, unsigned len)
    {
        return info.parseFrom(buf, len);
    }
}
#endif
