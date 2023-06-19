use std::time::Duration;
use zenoh::config::Config;
use zenoh::prelude::r#async::*;

#[async_std::main]
async fn main() {
    println!("Opening session ...");
    let session = zenoh::open(Config::default()).res().await.unwrap();

    let selector = Selector::try_from("demo/example/**").unwrap();

    println!("Requesting Query on '{selector}'...");
    let value = "Request from Rust!".to_string();
    let replies = session
        .get(&selector)
        .with_value(value)
        .target(QueryTarget::BestMatching)
        .timeout(Duration::from_millis(2000))
        .res()
        .await
        .unwrap();

    println!("Waiting reply ...");
    while let Ok(reply) = replies.recv_async().await {
        match reply.sample {
            Ok(sample) => println!(
                ">> Received ('{}': '{}')",
                sample.key_expr.as_str(),
                sample.value,
            ),
            Err(err) => println!(">> Received (ERROR: '{}')", String::try_from(&err).unwrap()),
        }
    }
}
