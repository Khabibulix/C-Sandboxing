#ifndef SNAPSHOT_H
#define SNAPSHOT_H

struct snapshot;
struct snapshot* snapshot(void *addr, size_t size);
void free_snapshot(struct snapshot *snap);
void visualize_snapshot(const struct snapshot *snap);

#endif // SNAPSHOT_H
