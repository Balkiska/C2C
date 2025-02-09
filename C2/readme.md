C2 Server - Instructions
------------------------

This C2 server:
- Listens for incoming connections from the malware.
- Implements a port knocking mechanism to unlock the port for specific IP addresses.
- Receives and stores stolen credentials in a local database (e.g., `credentials.bin`).
- Provides a simple terminal user interface (TUI) to manage and view stored data.

--------------------------------------------------------
Compilation
--------------------------------------------------------
Within the C2 server project directory, run:

    ```make```

This will generate the C2 server executable (e.g., `c2_server`).

--------------------------------------------------------
Execution
--------------------------------------------------------
Run the C2 server with root privileges if you want to bind privileged ports (e.g., port 22). Otherwise, you can modify the code to use a higher port number.

    ```sudo ./c2_server```

By default, the server will:
- Initialize the port knocking logic.
- Listen on the specified port (e.g., 22 or 4444).
- Provide an optional user interface (e.g., a menu) to list, search, or delete credentials.

--------------------------------------------------------
Verify if the C2 is Listening
--------------------------------------------------------
Use the following command to check if the port is open and listening:

    ```sudo netstat -tulnp | grep <PORT_NUMBER>```

For example:

    ```sudo netstat -tulnp | grep 22```

If the server is running properly, you should see a line indicating it is bound to the desired port.

--------------------------------------------------------
Testing the Port Knocking Sequence
--------------------------------------------------------
Before the malware can send credentials, it must complete the port knocking sequence. The server tracks each knock in a specified order. If the sequence is correct, the server unlocks the port for that IP.

From the malware side or another client, try connecting to each port in the sequence. Once completed, the server output should indicate:

    ```Correct sequence received from x.x.x.xxx```
    ```SSH port is unlocked for x.x.x.xxx```

At this point, the server will accept the final connection and potentially capture any data sent by the malware.

--------------------------------------------------------
Receiving Credentials
--------------------------------------------------------
When the malware completes port knocking and connects, the server will:

1. Accept the new connection.
2. Read any transmitted data (e.g., stolen credentials).
3. Print it in the console and/or store it in a local file or database (e.g., `credentials.bin`).

You may see output like:

    [Client] Received: username=alice; password=secret;
    New credentials saved with ID=5

--------------------------------------------------------
Using the Server User Interface (Optional)
--------------------------------------------------------
If your server has a menu-based interface (running in another thread or integrated in `main.c`), you can:

- Press `1` to list all stored credentials.
- Press `2` to search by username.
- Press `3` to delete a credential by ID.
- Press `4` to quit the C2 server.

These options may vary depending on your implementation.

--------------------------------------------------------
Stopping the C2 Server
--------------------------------------------------------
To stop the server, press `4` in the menu (if available) or terminate the process via the command line:

    ```sudo pkill -f c2_server```

Alternatively, if it’s running in the foreground, press `CTRL + C`.

--------------------------------------------------------
Troubleshooting
--------------------------------------------------------
- **Port Binding**: If you cannot bind to a privileged port (e.g., 22), change it to a higher port (e.g., 4444) in the source code or run with `sudo`.
- **Port Knocking**: Ensure the knocking sequence on the server matches what the malware sends. Any discrepancy in port numbers or order will cause unlocking to fail.
- **Credentials File**: Check for the presence of `credentials.bin` (or whichever file is configured in `database.h`). Ensure the server has write permissions in the current directory.
- **Firewall Issues**: If you do not see incoming connections, confirm your host firewall (or network policy) allows traffic on the specified ports.
- **Verify Listener**: Use `netstat -tulnp` or `ss -tulnp` to ensure the server is listening on the correct port.


