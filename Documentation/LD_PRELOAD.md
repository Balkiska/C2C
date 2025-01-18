# `ld.so` and `ld-linux.so` | Dynamic Linker/Loader

### **Detailed Summary of `ld.so` and `ld-linux.so`**

### **Introduction**

`ld.so` and `ld-linux.so` are dynamic linkers/loaders that manage shared object (libraries) dependencies for dynamically linked programs. They load required libraries, prepare the program to execute, and handle runtime linking. These linkers work with:

- **Legacy `a.out` binaries** (`ld.so`)
- **Modern ELF binaries** (`ld-linux.so.*`)

The linker can be invoked:

- **Indirectly**, by running a dynamically linked program, where it automatically executes based on the binary's `.interp` section.
- **Directly**, with the command:
    
    ```
    /lib/ld-linux.so.* [OPTIONS] [PROGRAM [ARGUMENTS]]
    ```
    

### **Main Functions**

Shared Object Resolution: The linker resolves dependencies for shared libraries using a prioritized search order:

- Path specified in `DT_RPATH` (unless overridden by `DT_RUNPATH`).
- Directories listed in the `LD_LIBRARY_PATH` environment variable.
- `DT_RUNPATH` entries (applied to direct dependencies only).
- Compiled cache (`/etc/ld.so.cache`).
- Default directories: `/lib` and `/usr/lib` (or `/lib64` and `/usr/lib64` for 64-bit systems).

Dynamic String Tokens: Tokens in paths and environment variables are expanded:

- **`$ORIGIN`**: Directory of the binary/shared object.
- **`$LIB`**: Expands to `lib` or `lib64` based on architecture.
- **`$PLATFORM`**: Host CPU type (e.g., `x86_64`).

Dynamic Linking: Handles runtime symbol resolution, enabling shared libraries to be loaded and used by programs.

---

### **Options**

The linker provides a variety of options for control and debugging:

- `-argv0 <string>`: Sets `argv[0]` for the program.
- `-audit <list>`: Specifies shared objects to act as link-auditors.
- `-library-path <path>`: Overrides the `LD_LIBRARY_PATH` variable.
- `-preload <list>`: Preloads shared objects specified in the list.
- `-list`: Displays all dependencies and their resolution paths.
- `-verify`: Verifies if a program can be dynamically linked.
- `-inhibit-cache`: Disables usage of `/etc/ld.so.cache`.
- `-list-diagnostics`: Outputs diagnostics, including auxiliary vectors and environment variables.

---

### **Environment Variables**

Several environment variables influence the linker's behavior:

General Variables:

- `LD_LIBRARY_PATH`: Specifies directories for searching shared libraries.
- `LD_PRELOAD`: Forces preloading of specified shared objects.
- `LD_BIND_NOW`: Resolves all symbols at startup instead of deferring resolution.

Debugging & Auditing:

- `LD_DEBUG`: Outputs detailed debugging information (e.g., bindings, library paths).
- `LD_AUDIT`: Lists ELF shared objects for auditing.

Compatibility and Performance:

- `LD_ASSUME_KERNEL`: Forces the linker to assume a specific kernel version.
- `LD_DYNAMIC_WEAK`: Allows overriding weak symbols with strong ones in different libraries.
- `LD_POINTER_GUARD`: Enables or disables pointer mangling for security.

Secure-Execution Mode:

- Automatically triggered when a binary runs with set-user-ID/set-group-ID permissions or confers capabilities.
- Strips or ignores certain environment variables (e.g., `LD_LIBRARY_PATH`, `LD_PRELOAD`) for security.

---

### **Secure Execution**

Secure mode enforces stricter behavior:

- **Condition**: Enabled when `AT_SECURE` in the auxiliary vector is non-zero.
- **Impact**: Strips or restricts environment variables to prevent unintended behavior in privileged binaries.

---

### **Hardware Capabilities**

Legacy Hardware Capabilities:

- Supported up to glibc 2.37.
- Directories (e.g., `/usr/lib/sse2`) allow loading hardware-optimized libraries.
- Drawback: Exponential growth of search paths with added capabilities.

Modern Scheme (glibc 2.33+):

- Introduced CPU architecture levels (e.g., `x86-64-v2`, `x86-64-v3`).
- Simplifies search paths by avoiding combinatorial growth.

---

### **Files**

Executable Files:

- `/lib/ld.so`: Dynamic linker for `a.out` binaries.
- `/lib/ld-linux.so.{1,2}`: Dynamic linker for ELF binaries.

Configuration Files:

- `/etc/ld.so.cache`: Compiled cache of library paths.
- `/etc/ld.so.preload`: System-wide list of preloaded shared objects.

Shared Objects:

- `lib*.so*`: Shared library files.

---

### **Debugging and Diagnostics**

Output Options:

- Use `LD_DEBUG` to enable detailed logs about the linker’s activity.
- `-list-diagnostics` provides detailed system diagnostics.

Diagnostics Format:

- Includes auxiliary vector details (`AT_PLATFORM`, `AT_SECURE`), environment variables, and symbol binding information.

---

### **Notes on Compatibility**

Older glibc versions had unique behaviors (e.g., weak symbol overrides) that differ from modern, standardized implementations. glibc versions 2.33 and later introduced new capabilities for improved performance and security.

---

### Reference

https://man7.org/linux/man-pages/man8/ld.so.8.html