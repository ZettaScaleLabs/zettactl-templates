import sys
import time
import zenoh
from zenoh import config, Sample, Value

def queryable_callback(query):
    print(f">> [Queryable ] Received Query '{query.selector}'" + (f" with value: {query.value.payload}" if query.value is not None else ""))
    query.reply(Sample(key, value))


print("Opening session...")
session = zenoh.open(zenoh.Config())

key = "demo/example/zenoh-python-queryable"

print("Declaring Queryable on '{}'...".format(key))
value = "Queryable from Python!"
queryable = session.declare_queryable(key, queryable_callback, False)

print("Enter 'q' to quit...")
c = '\0'
while c != 'q':
    c = sys.stdin.read(1)
    if c == '':
        time.sleep(1)

queryable.undeclare()
session.close()