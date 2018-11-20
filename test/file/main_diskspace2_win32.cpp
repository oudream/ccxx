// VolumeInfo: Displays detailed volume information for all volumes on the computer
// Copyright (C) 2010  Jeffrey Bush <jeff@coderforlife.com>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/gpl.html>.

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>

#include <stdio.h>
#include <tchar.h>

#ifndef ARRAYSIZE
#define ARRAYSIZE(a) sizeof(a)/sizeof(a[0])
#endif

// Descriptions of the flags given by GetVolumeInformation
TCHAR *flag_desc[] = {
    TEXT("Supports Case Sensitive Search"), TEXT("Supports Case Preserved Names"), TEXT("Supports File Compression"), TEXT("Supports Named Streams"),
    TEXT("Preserves ACLs"), TEXT("Read-Only"), TEXT("Supports Sequential Write Once"), TEXT("Supports Encryption"),
    TEXT("Supports Extended Attributes"), TEXT("Supports Hard Links"), TEXT("Supports Object IDs"), TEXT("Supports Open by File ID"),
    TEXT("Supports Reparse Points"), TEXT("Supports Sparse Files"), TEXT("Supports Transactions"), TEXT("Supports USN Journal"),
    TEXT("Supports Unicode on Disk"), TEXT("Is Compressed"), TEXT("Supports Disk Quotas")
};

// The values of the flags given by GetVolumeInformation
DWORD flag_values[] = {
    0x00000002, 0x00000001, 0x00000010, 0x00040000, 0x00000008, 0x00080000, 0x00100000, 0x00020000, 0x00800000, 0x00400000,
    0x00010000, 0x01000000, 0x00000080, 0x00000040, 0x00200000, 0x02000000, 0x00000004, 0x00008000, 0x00000020
};

// The types of drives given by GetDriveType
TCHAR *drive_types[] = {
    TEXT("Unknown"), TEXT("Invalid"), TEXT("Removable"), TEXT("Fixed"), TEXT("Remote"), TEXT("CD Drive"), TEXT("RAM Disk"),
};

// The units used by byte_units
TCHAR *units[] = {
    TEXT("bytes"), TEXT("KB"), TEXT("MB"), TEXT("GB"), TEXT("TB"),
};

// Prints the descriptions of the flags given by GetVolumeInformation
static void print_flags(DWORD flags) {
    DWORD i;
    for (i = 0; i < ARRAYSIZE(flag_values); ++i) {
        if (flags & flag_values[i]) {
            _tprintf(TEXT("  %s\n"), flag_desc[i]);
        }
    }
}

// Returns the size reduced to one of the units bytes, KB, MB, GB, TB
static double byte_size(ULONGLONG bytes) {
    DWORD i;
    ULONGLONG x;
    for (i = 0, x = 1; i < 4; ++i, x *= 1024)
        if (bytes < x*1024) break;
    return ((double)bytes) / x;
}

// Gets the unit for the reduced size given by byte_size
static const TCHAR *byte_unit(ULONGLONG bytes) {
    DWORD i;
    ULONGLONG x;
    for (i = 0, x = 1024; i < 4; ++i, x *= 1024)
        if (bytes < x) break;
    return units[i];
}

int _tmain(int argc, _TCHAR* argv[]) {
    HANDLE sh;
    TCHAR vname[MAX_PATH+1], dname[MAX_PATH+1], paths[1024], *path, label[MAX_PATH+1], fsname[MAX_PATH+1];
    BOOL first = TRUE;
    DWORD len, count, serial, flags, err;
    ULARGE_INTEGER free, total;

    _tprintf(TEXT("VolumeInfo Copyright (C) 2010  Jeffrey Bush <jeff@coderforlife.com>\n"));
    _tprintf(TEXT("This program comes with ABSOLUTELY NO WARRANTY;\n"));
    _tprintf(TEXT("This is free software, and you are welcome to redistribute it\n"));
    _tprintf(TEXT("under certain conditions;\n"));
    _tprintf(TEXT("See http://www.gnu.org/licenses/gpl.html for more details.\n\n"));

    // Get the first volume (handle is used to iterate)
    sh = FindFirstVolume(vname, ARRAYSIZE(vname));
    if (sh == INVALID_HANDLE_VALUE) {
        _tprintf(TEXT("FindFirstVolume failed: %d\n"), GetLastError());
        return 1;
    }

    // Iterate through all volumes
    while (first || FindNextVolume(sh, vname, ARRAYSIZE(vname))) {
        first = FALSE;

        // Make sure the volume path is valid
        len = _tcslen(vname);
        if (len < 5 || _tcsncmp(vname, TEXT("\\\\?\\"), 4) != 0 || vname[len-1] != L'\\') {
            _tprintf(TEXT("FindFirstVolume/FindNextVolume returned a bad path: %s\n"), vname);
            continue;
        }
        _tprintf(TEXT("Volume: %s\n"), vname);

        // The device name (which doesn't work with a trailing '\')
        vname[len-1] = 0;
        if (QueryDosDevice(&vname[4], dname, ARRAYSIZE(dname)))
            _tprintf(TEXT("Device: %s\n"), dname);
        else
            _tprintf(TEXT("Failed to get device name: %d\n"), GetLastError());
        vname[len-1] = L'\\';

        // The drive type
        _tprintf(TEXT("Type:   %s\n"), drive_types[GetDriveType(vname)]);

        // Get all the mount paths for a volume (typically C:\, D:\, ...)
        if (GetVolumePathNamesForVolumeName(vname, paths, ARRAYSIZE(paths), &count)) {
            _tprintf(TEXT("Paths:"));
            for (path = paths; path[0] != 0; path += _tcslen(path) + 1)
                _tprintf(TEXT("  %s"), path);
            _tprintf(TEXT("\n"));
        } else
            _tprintf(TEXT("Failed to get volume paths: %d\n"), GetLastError());

        // Get lots of information about the volume: label, serial, abilities (flags), and filesystem name
        if (GetVolumeInformation(vname, label, ARRAYSIZE(label), &serial, NULL, &flags, fsname, ARRAYSIZE(fsname))) {
            _tprintf(TEXT("Label:  %s\n"), label);
            _tprintf(TEXT("Serial: %X\n"), serial);
            _tprintf(TEXT("Format: %s\n"), fsname);
            _tprintf(TEXT("Flags:  %X\n"), flags);
            print_flags(flags);
        } else {
            _tprintf(TEXT("Failed to get volume information: %d\n"), GetLastError());
        }

        // Size information (note: this is influenced by quotas, if they are enabled for the current user)
        if (GetDiskFreeSpaceEx(vname, &free, &total, NULL)) {
            _tprintf(TEXT("Size:   Total: %13llu [%6.1f %s]\n"), total.QuadPart, byte_size(total.QuadPart), byte_unit(total.QuadPart));
            _tprintf(TEXT("        Free:  %13llu [%6.1f %s]\n"), free.QuadPart,  byte_size(free.QuadPart),  byte_unit(free.QuadPart));
        } else {
            _tprintf(TEXT("Failed to get volume size information: %d\n"), GetLastError());
        }

        _tprintf(TEXT("\n"));
    }

    // Report and errors and close the handle
    err = GetLastError();
    FindVolumeClose(sh);
    if (err != ERROR_NO_MORE_FILES) {
        _tprintf(TEXT("FindNextVolume failed: %d\n"), err);
        return 1;
    }

    int n, k;
    while (scanf("%d%d", &n, &k) != EOF) {
        printf("\niCount = %d", n);
        fflush(stdout);
    }
    return 0;
}