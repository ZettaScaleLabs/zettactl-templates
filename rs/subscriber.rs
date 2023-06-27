use std::time::Duration;
use zenoh::config::Config;
use zenoh::prelude::sync::SyncResolve;
use zenoh::prelude::KeyExpr;

fn main() {
    // Consult the full documentation at: https://docs.rs/zenoh/0.7.2-rc/zenoh/index.html

    println!("Opening session...");
    let session = zenoh::open(Config::default()).res_sync().unwrap();

    let key_expr = KeyExpr::try_from("demo/example/**").unwrap();

    println!("Declaring Subscriber on '{}'...", &key_expr);
    let subscriber = session.declare_subscriber(&key_expr).res_sync().unwrap();

    loop {
        if let Ok(sample) = subscriber.recv_timeout(Duration::from_secs(2)) {
            println!(
                ">> [Subscriber] Received {} ('{}': '{}')",
                sample.kind,
                sample.key_expr.as_str(),
                sample.value
            );
        } else {
            println!("Waiting for message!");
        }
    }
}
