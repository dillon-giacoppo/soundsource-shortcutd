// Copyright 2024 Dillon Giacoppo
// SPDX-License-Identifier: MIT

#include <CoreAudio/CoreAudio.h>
#include <getopt.h>
#include <iostream>

using namespace std;

static constexpr auto defaultOutputDeviceAddress = AudioObjectPropertyAddress{
    kAudioHardwarePropertyDefaultOutputDevice,
    kAudioObjectPropertyScopeGlobal,
    kAudioObjectPropertyElementMain
};

static constexpr void argparse(int argc, char *const* &argv);

static string argShortcutName;

[[noreturn]]
int main(const int argc, char *const *argv) {
    argparse(argc, argv);

    const auto listenRet = AudioObjectAddPropertyListener(
        kAudioObjectSystemObject,
        &defaultOutputDeviceAddress,
        [](AudioObjectID, UInt32, const AudioObjectPropertyAddress *, void *) -> OSStatus {
            if (const auto sysRet = system(("shortcuts run \"" + argShortcutName + "\"").c_str()); sysRet != 0) {
                cerr << "Failed to exec shortcuts. code=" << sysRet << endl;
                return sysRet;
            }
            return noErr;
        },
        nullptr
    );

    if (listenRet != noErr) {
        cerr << "Failed to listen on audio object. OSStatus=" << listenRet << endl;
        exit(listenRet);
    }

    sigset_t mask;
    sigemptyset(&mask);
    while (true) {
        sigsuspend(&mask);
    }
}

static constexpr void argparse(const int argc, char *const * &argv) {
    static constexpr option longOpts[] = {
        {"name", required_argument, nullptr, 0},
        {nullptr, 0, nullptr, 0}
    };

    int opt, longIndex;
    while ((opt = getopt_long(argc, argv, "", longOpts, &longIndex)) != -1) {
        if (opt == 0 && longIndex == 0) {
            argShortcutName = optarg;
        } else {
            exit(1);
        }
    }

    if (optind < argc) {
        cerr << "Unrecognized arguments: --";
        while (optind < argc) cerr << ' ' << argv[optind++];
        cerr << endl;
        exit(1);
    }
}
