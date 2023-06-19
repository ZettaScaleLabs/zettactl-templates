import sys
import time
import zenoh

print("Opening session...")
session = zenoh.open(zenoh.Config())

key = "demo/example/zenoh-python-pub"

print(f"Declaring Publisher on '{key}'...")
value = "Pub from Python!"
pub = session.declare_publisher(key)

for idx in range(sys.maxsize):
    time.sleep(1)
    buf = f"[{idx:4d}] {value}"
    print(f"Putting Data ('{key}': '{buf}')...")
    pub.put(buf)

pub.undeclare()
session.close()