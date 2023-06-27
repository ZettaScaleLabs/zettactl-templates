use zenoh::config::Config;
use zenoh::prelude::sync::SyncResolve;
use zenoh::prelude::ZenohId;

fn main() {
    // Consult the full documentation at: https://docs.rs/zenoh/0.7.2-rc/zenoh/index.html

    println!("Opening session...");
    let session = zenoh::open(Config::default()).res_sync().unwrap();

    let info = session.info();
    println!("zid: {}", info.zid().res_sync());
    println!(
        "routers zid: {:?}",
        info.routers_zid().res_sync().collect::<Vec<ZenohId>>()
    );
    println!(
        "peers zid: {:?}",
        info.peers_zid().res_sync().collect::<Vec<ZenohId>>()
    );
}
