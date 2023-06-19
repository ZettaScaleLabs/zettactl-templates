import sys
import time
import zenoh
from zenoh import config, QueryTarget

print("Opening session...")
session = zenoh.open(zenoh.Config())

selector = "demo/example/**"

print("Sending Query '{}'...".format(selector))
value = "Query from Python!"
replies = session.get(selector, zenoh.Queue(), target=QueryTarget.BEST_MATCHING(), value=value)
for reply in replies.receiver:
    try:
        print(">> Received ('{}': '{}')"
              .format(reply.ok.key_expr, reply.ok.payload.decode("utf-8")))
    except:
        print(">> Received (ERROR: '{}')"
              .format(reply.err.payload.decode("utf-8")))


session.close()