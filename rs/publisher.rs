use std::thread;
use std::time::Duration;
use zenoh::config::Config;
use zenoh::prelude::sync::SyncResolve;
use zenoh::prelude::KeyExpr;

fn main() {
    println!("Opening session...");
    let session = zenoh::open(Config::default()).res_sync().unwrap();

    let key_expr = KeyExpr::try_from("demo/example/zenoh-rs-pub").unwrap();
    let value = "Pub from Rust!".to_string();

    println!("Declaring Publisher on '{key_expr}'...");
    let publisher = session.declare_publisher(&key_expr).res_sync().unwrap();

    for idx in 0..u32::MAX {
        let buf = format!("[{idx:4}] {value}");
        println!("Putting Data ('{}': '{}')...", &key_expr, buf);
        publisher.put(buf).res_sync().unwrap();
        thread::sleep(Duration::from_secs(1));
    }
}
