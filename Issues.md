# Known Issues {#issues}

### I get the following error: `error: conflicting declaration 'typedef struct HealthMetricAlert* HealthMetricAlert'`

This seems to be a problem with the aplite `pebble.h` file that doesn't exist for the other platforms.  To correct this problem either remove aplite from the `packages.json` file or edit the aplite `pebble.h` to correct the issue.

To fix the problem edit `~/.pebble-sdk/SDKs/current/sdk-core/pebble/aplite/include/pebble.h` and change one line from:

```c
typedef struct HealthMetricAlert* HealthMetricAlert;
```

to

```c
typedef struct HealthMetricAlert HealthMetricAlert;
```
