# LD-PRELOAD

`LD_PRELOAD` is an environment variable in Linux that allows specifying additional shared libraries to be loaded before others. This mechanism enables dynamic interception and overriding of functions in existing shared libraries without modifying the original binaries.

---

## How LD_PRELOAD Works

The dynamic linker (`ld.so`) loads shared libraries required by a program during execution. By setting the `LD_PRELOAD` environment variable, specified libraries are loaded **before others**, enabling function or symbol overrides provided by these libraries.

---

## Use Cases

### Function Interception and Overriding

- Modify the behavior of existing functions.
- Debug or log specific function calls, such as file access or network requests.

### Testing and Debugging

- Simulate specific behaviors, e.g., inducing errors.
- Validate software resilience to library changes.

### Performance Monitoring

- Inject monitoring tools to track memory allocation, CPU usage, or file I/O.

### Temporary Fixes

- Apply quick patches or fixes to binaries without recompilation.

---

## Usage

### Syntax

```
export LD_PRELOAD=/path/to/library.so
./application
```

- `/path/to/library.so`: Full path to the shared library containing overridden functions.
- Multiple libraries can be specified, separated by spaces:

```
export LD_PRELOAD="/path/to/lib1.so /path/to/lib2.so"
```

### Example

### Overriding `printf`

**Code:**

```
#include <stdio.h>
#include <stdarg.h>

int printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int ret = vfprintf(stdout, "[Intercepted] %s", args);
    va_end(args);
    return ret;
}
```

**Compilation:**

```
gcc -shared -fPIC -o mylib.so mylib.c
```

**Execution:**

```
export LD_PRELOAD=./mylib.so
./application
```

Output from `printf` will include the prefix `[Intercepted]`.

---

## Dynamic Linker Behavior

- **Loading Order**: Libraries in `LD_PRELOAD` are loaded **before standard libraries**.
- **Scope**: Affects only the current process and its child processes.
- **Security Restrictions**: Disabled for setuid/setgid binaries to prevent privilege escalation.

---

## Advanced Usage

### Debugging with `LD_DEBUG`

Trace library loading and symbol resolution:

```
LD_DEBUG=libs ./application
```

### Combining with `LD_LIBRARY_PATH`

Specify custom library paths:

```
export LD_LIBRARY_PATH=/custom/lib/path
export LD_PRELOAD=/custom/lib/path/mylib.so
./application
```

---

## Security Considerations

- **Privilege Escalation**: Malicious use can intercept sensitive functions. Always sanitize environment variables when running untrusted programs.
- **Restricted Use**: Disabled for setuid binaries to mitigate security risks.

---

## Common Challenges

1. **Function Signature Matching**: Ensure overridden functions match the exact signature of the originals to avoid undefined behavior.
2. **Debugging Issues**: Incorrect or incompatible libraries can cause crashes.
3. **Performance Overhead**: Injected libraries may introduce latency or additional resource consumption.

---

## Conclusion

`LD_PRELOAD` is a versatile tool for dynamic library injection and function interception. It is ideal for debugging, testing, performance monitoring, and applying temporary fixes. However, it requires careful handling to avoid unintended side effects.

---

## References

1. [Linux Man Page for ld.so](https://man7.org/linux/man-pages/man8/ld.so.8.html)
2. [Medium Article: LD_PRELOAD in Linux](https://abhijit-pal.medium.com/ld-preload-in-linux-a-powerful-tool-for-dynamic-library-interception-7f681d0b6556)
