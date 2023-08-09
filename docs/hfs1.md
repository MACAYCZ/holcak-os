# HFS1 (Holcak File System 1)
HFS1 is file system suitable for 32 bit systems. All integers are stored in the little endian format.

## HFS1 Header
HFS1 Header is located at the address 0x02 (after short jump) in the bootsector.
| Size (byte) | Description |
| :---------: | :---------- |
| 0x04        | Magic number "HFS1" (0x31534648) |
| 0x04        | LBA pointing to the beginning of the [table](#hfs1-table) |
| 0x04        | LBA pointing to the beginning of the [free table](#hfs1-free-table) |

## HFS1 Table
HFS1 Table stores all [cells](#hfs1-cell) of the file system with their unique id where some of them are reserved (root = 0x00, system = 0x01, stage2 = 0x02, stage3 = 0x03, kernel = 0x04).
| Size (byte) | Description |
| :---------: | :---------- |
| 0x04        | Number of [cells](#hfs1-cell) |
| Cell*N      | Array of [cells](#hfs1-cell) each with it's unique id |

## HFS1 Cell
Each HFS1 Cell has it's unique id and stores information about file's data. The cell is NULL when the LBA is zero.
| Size (byte) | Description |
| :---------: | :---------- |
| 0x04        | LBA pointing to the beginning of node data |
| 0x04        | Size of node data in sectors |
| 0x02        | Size of EOF in bytes |
| 0x02        | [Flags](#hfs1-cell-flags) consists of type, permissions, ... |
| 0x04        | Last modified ([Unix time](https://en.wikipedia.org/wiki/Unix_time)) |
| 0x04        | Last accessed ([Unix time](https://en.wikipedia.org/wiki/Unix_time)) |

## HFS1 Cell Flags
| Size (bit)  | Description |
| :---------: | :---------- |
| 0x02        | Type (File = 0x00, Folder = 0x01, Link = 0x02) |
| 0x04        | Permissions (Access = 0x01, Modify = 0x02, Execute = 0x08) where root privileges are required when the corresponding bit is unset |

## HFS1 Folder
Folder is stored as regular file containing sorted array of [entries](#hfs1-folder-entry) by name.
| Size (byte) | Description |
| :---------: | :---------- |
| 0x04        | Number of [entries](#hfs1-folder-entry) |
| Entry*N     | Array of [entries](#hfs1-folder-entry) |

## HFS1 Folder Entry
| Size (byte) | Description |
| :---------: | :---------- |
| 0x04        | Unique id of entry |
| 0x01        | Size of entry name |
| 0x01*N      | Name of entry stored in ASCII |

## HFS1 Link
Link is stored as regular file containing path to the target in ASCII.
| Size (byte) | Description |
| :---------: | :---------- |
| 0x01*N      | Path stored in ASCII |

## HFS1 Free Table
HFS1 Free Table is containing array of [cells](#hfs1-free-cell) which indicates where the free space is.
| Size (byte) | Description |
| :---------: | :---------- |
| 0x04        | Number of [cells](#hfs1-free-cell) |
| Free Cell*N | Array of cells |

## HFS1 Free Cell
| Size (byte) | Description |
| :---------: | :---------- |
| 0x04        | Start of free space |
| 0x04        | End of free space |
