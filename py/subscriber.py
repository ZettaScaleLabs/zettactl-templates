import sys
import time
import zenoh
from zenoh import Sample, Reliability

print("Opening session...")
session = zenoh.open(zenoh.Config())

key = "demo/example/**"

print("Declaring Subscriber on '{}'...".format(key))

def listener(sample: Sample):
    print(f">> [Subscriber] Received {sample.kind} ('{sample.key_expr}': '{sample.payload.decode('utf-8')}')")
    
# WARNING, you MUST store the return value in order for the subscription to work!!
# This is because if you don't, the reference counter will reach 0 and the subscription
# will be immediately undeclared.
sub = session.declare_subscriber(key, listener, reliability=Reliability.RELIABLE())

print("Enter 'q' to quit...")
c = '\0'
while c != 'q':
    c = sys.stdin.read(1)
    if c == '':
        time.sleep(1)

# Cleanup: note that even if you forget it, cleanup will happen automatically when 
# the reference counter reaches 0
sub.undeclare()
session.close()