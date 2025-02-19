// Display images inside a terminal
// Copyright (C) 2023  JustKidding
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
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "process.hpp"

#include <cstdlib>
#include <fmt/format.h>
#include <ios>
#include <limits>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/user.h>
#include <libprocstat.h>

constexpr auto max_size = std::numeric_limits<std::streamsize>::max();

Process::Process(int pid)
    : pid(pid)
{
    auto *procstat = procstat_open_sysctl();
    unsigned int count;
    auto *proc = procstat_getprocs(procstat, KERN_PROC_PID, this->pid, &count);
    state = proc->ki_stat;
    ppid = proc->ki_ppid;
    process_group_id = proc->ki_pgid;
    session_id = proc->ki_tsid;
    tty_nr = proc->ki_tdev;
    minor_dev = (tty_nr & ((0b11) << 9)) >> 3 | (tty_nr & 0b11111);
    pty_path = fmt::format("/dev/{}", devname(tty_nr, S_IFCHR));
}
