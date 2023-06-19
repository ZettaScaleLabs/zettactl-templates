import zenoh

print("Opening session...")
session = zenoh.open(zenoh.Config())

info = session.info()
print(f"zid: {info.zid()}")
print(f"routers: {info.routers_zid()}")
print(f"peers: {info.peers_zid()}")
session.close()