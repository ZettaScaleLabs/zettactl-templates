use zenoh::config::Config;
use zenoh::prelude::r#async::*;

#[async_std::main]
async fn main() {
    println!("Opening session...");
    let session = zenoh::open(Config::default()).res().await.unwrap();

    let info = session.info();
    println!("zid: {}", info.zid().res().await);
    println!(
        "routers zid: {:?}",
        info.routers_zid().res().await.collect::<Vec<ZenohId>>()
    );
    println!(
        "peers zid: {:?}",
        info.peers_zid().res().await.collect::<Vec<ZenohId>>()
    );
}
