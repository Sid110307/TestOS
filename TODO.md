# TODO

## Roadmap

| **Feature**             | **Description**                                                                   |
|:------------------------|:----------------------------------------------------------------------------------|
| **Keyboard Input**      | Implement keyboard input handling for user interaction.                           |
| **File System**         | Implement a basic file system for storing and accessing files on storage devices. |
| **Basic Shell**         | Create a simple command-line interface (shell) for user interaction.              |
| **Process Management**  | Implement a basic process scheduler and management system.                        |
| **Multitasking**        | Enable multitasking to run multiple processes concurrently.                       |
| **User Mode**           | Differentiate between kernel mode and user mode, with privilege levels.           |
| **Device Drivers**      | Develop drivers for essential hardware components (e.g., keyboard, screen, disk). |
| **System Calls**        | Implement system calls to allow user programs to interact with the kernel.        |
| **Interrupt Timer**     | Set up a timer interrupt for scheduling and system timekeeping.                   |
| **Signals and IPC**     | Implement inter-process communication (IPC) mechanisms and signals.               |
| **Thread Management**   | Add support for multithreading and thread synchronization.                        |
| **User Programs**       | Develop user-space programs and libraries for your OS.                            |
| **Networking**          | Implement basic networking functionality (e.g., sockets, TCP/IP stack).           |
| **User Authentication** | Add user authentication and permissions handling.                                 |
| **Error Handling**      | Implement robust error handling throughout the OS.                                |
| **Graphics Support**    | Add graphics support if necessary (e.g., for GUI applications).                   |
| **Power Management**    | Include power management features, like sleep and hibernation.                    |
| **Debugging Tools**     | Create debugging tools and logs for troubleshooting.                              |
| **System Information**  | Develop functions to retrieve system information and statistics.                  |
| **Security**            | Add security mechanisms to protect against common threats.                        |
| **The Sacred Ritual**   | Make it run DOOM.                                                                 |

## Programs to port

| **Program** | **Description**                                                                          |
|:------------|:-----------------------------------------------------------------------------------------|
| arch        | Print machine architecture.                                                              |
| ash         | A lightweight shell.                                                                     |
| b2sum       | Compute and check BLAKE2 message digest.                                                 |
| base32      | Base32 encode or decode FILE, or standard input, to standard output.                     |
| base64      | Base64 encode or decode FILE, or standard input, to standard output.                     |
| basename    | Strip directory and suffix from filenames.                                               |
| basenc      | Base16, Base32, Base58, Base64, Base85 data encoding and decoding.                       |
| cat         | Concatenate FILE(s), or standard input, to standard output.                              |
| chcon       | Change SELinux security context of file FILE to CONTEXT.                                 |
| chgrp       | Change the group ownership of files.                                                     |
| chmod       | Change the mode of each FILE to MODE.                                                    |
| chown       | Change the owner and/or group of each FILE to OWNER and/or GROUP.                        |
| chroot      | Run COMMAND with root directory set to NEWROOT.                                          |
| cksum       | Print CRC checksum and byte counts.                                                      |
| comm        | Compare two sorted files line by line.                                                   |
| cp          | Copy SOURCE to DEST, or multiple SOURCE(s) to DIRECTORY.                                 |
| csplit      | Split a file into sections determined by context lines.                                  |
| cut         | Print selected parts of lines from each FILE to standard output.                         |
| date        | Print or set the system date and time.                                                   |
| dd          | Copy a file, converting and formatting according to the operands.                        | 
| df          | Report file system disk space usage.                                                     |
| dir         | List directory contents.                                                                 |
| dircolors   | Color setup for ls.                                                                      |
| dirname     | Strip last component from file name.                                                     |
| du          | Estimate file space usage.                                                               |
| echo        | Write arguments to the standard output.                                                  |
| env         | Run a program in a modified environment.                                                 |
| expand      | Convert tabs to spaces.                                                                  |
| expr        | Evaluate expressions.                                                                    |
| factor      | Print prime factors.                                                                     |
| false       | Do nothing, unsuccessfully.                                                              |
| fmt         | Reformat paragraph text.                                                                 |
| fold        | Wrap each input line to fit in specified width.                                          |
| groups      | Print the groups a user is in.                                                           |
| head        | Output the first part of files.                                                          |
| hostid      | Print the numeric identifier for the current host.                                       |
| hostname    | Print or set system name.                                                                |
| id          | Print user and group IDs.                                                                |
| install     | Copy files and set attributes.                                                           |
| join        | Join lines of two files on a common field.                                               |
| kill        | Send a signal to a process.                                                              |
| link        | Make links between files.                                                                |
| ln          | Make links between files.                                                                |
| logname     | Print current login name.                                                                |
| ls          | List directory contents.                                                                 |
| md5sum      | Print or check MD5 (128-bit) checksums.                                                  |
| mkdir       | Create directories.                                                                      |
| mkfifo      | Make FIFOs (named pipes).                                                                |
| mknod       | Make block or character special files.                                                   |
| mktemp      | Create a temporary file or directory.                                                    |
| mv          | Move (rename) files.                                                                     |
| nice        | Run a program with modified scheduling priority.                                         |
| nl          | Number lines and write files.                                                            |
| nohup       | Run a command immune to hangups, with output to a non-tty.                               |
| nproc       | Print the number of processing units available.                                          |
| numfmt      | Reformat numbers.                                                                        |
| od          | Write an unambiguous representation, octal bytes by default, of FILE to standard output. |
| paste       | Merge lines of files.                                                                    |
| pathchk     | Check whether file names are valid or portable.                                          |
| pinky       | Lightweight `finger`. Used to print user information.                                    |
| pr          | Convert text files for printing.                                                         |
| printenv    | Print all or some environment variables.                                                 |
| printf      | Format and print data.                                                                   |
| ptx         | Produce a permuted index of file contents.                                               |
| pwd         | Print working directory.                                                                 |
| readlink    | Print resolved symbolic links or canonical file names.                                   |
| realpath    | Print the resolved path.                                                                 |
| rm          | Remove files or directories.                                                             |
| rmdir       | Remove empty directories.                                                                |
| runcon      | Run COMMAND with a specified security context.                                           |
| seq         | Print a sequence of numbers.                                                             |
| sha1sum     | Print or check SHA1 (160-bit) checksums.                                                 |
| sha224sum   | Print or check SHA224 (224-bit) checksums.                                               |
| sha256sum   | Print or check SHA256 (256-bit) checksums.                                               |
| sha384sum   | Print or check SHA384 (384-bit) checksums.                                               |
| sha512sum   | Print or check SHA512 (512-bit) checksums.                                               |
| shred       | Overwrite a file to hide its contents, and optionally delete it.                         |
| shuf        | Shuffle text.                                                                            |
| sleep       | Delay for a specified amount of time.                                                    |
| sort        | Sort lines of text files.                                                                |
| split       | Split a file into pieces.                                                                |
| stat        | Display file or file system status.                                                      |
| stdbuf      | Run COMMAND, with modified buffering operations for its standard streams.                |
| stty        | Print or change terminal characteristics.                                                |
| sum         | Print checksum and block counts for each FILE.                                           |
| sync        | Synchronize data on disk with memory.                                                    |
| tac         | Concatenate and print files in reverse.                                                  |
| tail        | Output the last part of files.                                                           |
| tee         | Read from standard input and write to standard output and files.                         |
| test        | Check file types and compare values.                                                     |
| timeout     | Run a command with a time limit.                                                         |
| touch       | Change file timestamps, or create a file if it does not exist.                           |
| tr          | Translate, squeeze, and/or delete characters.                                            |
| true        | Do nothing, successfully.                                                                |
| truncate    | Shrink or extend the size of a file to the specified size.                               |
| tsort       | Topological sort.                                                                        |
| tty         | Print the file name of the terminal connected to standard input.                         |
| uname       | Print system information.                                                                |
| unexpand    | Convert spaces to tabs.                                                                  |
| uniq        | Omit repeated lines from input.                                                          |
| unlink      | Remove files.                                                                            |
| uptime      | Print system uptime and load.                                                            |
| users       | Print the user names of users currently logged in to the current host.                   |
| vdir        | List directory contents.                                                                 |
| wc          | Print newline, word, and byte counts for each FILE.                                      |
| who         | Print who is currently logged in.                                                        |
| whoami      | Print effective userid.                                                                  |
| yes         | Print a string until interrupted.                                                        |
