use async_std::task::sleep;
use std::time::Duration;
use zenoh::config::Config;
use zenoh::prelude::r#async::*;

#[async_std::main]
async fn main() {
    println!("Opening session...");
    let session = zenoh::open(Config::default()).res().await.unwrap();

    let key_expr = KeyExpr::try_from("demo/example/zenoh-rs-pub").unwrap();
    let value = "Pub from Rust!".to_string();

    println!("Declaring Publisher on '{key_expr}'...");
    let publisher = session.declare_publisher(&key_expr).res().await.unwrap();

    for idx in 0..u32::MAX {
        sleep(Duration::from_secs(1)).await;
        let buf = format!("[{idx:4}] {value}");
        println!("Putting Data ('{}': '{}')...", &key_expr, buf);
        publisher.put(buf).res().await.unwrap();
    }
}
