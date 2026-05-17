# husk

**A rootless container runtime made for education purposes**

`husk` is a minimal container runtime that creates isolated Linux environments using kernel primitives directly — no Docker, no runc, no libcontainer.
All isolation happens without root privileges on the host: the container sees UID 0 internally while the host process retains its original UID.

I'm working on this project to understand the mechanisms that container runtimes build upon.

