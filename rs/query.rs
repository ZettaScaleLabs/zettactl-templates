use async_std::task::sleep;
use futures::prelude::*;
use futures::select;
use std::time::Duration;
use zenoh::config::Config;
use zenoh::prelude::r#async::*;

#[async_std::main]
async fn main() {
    println!("Opening session...");
    let session = zenoh::open(Config::default()).res().await.unwrap();

    let key_expr = KeyExpr::try_from("demo/example/zenoh-rs-queryable").unwrap();

    println!("Declaring Queryable on '{key_expr}'...");
    let queryable = session
        .declare_queryable(&key_expr)
        .complete(true)
        .res()
        .await
        .unwrap();

    let value = "Queryable from Rust!".to_string();

    println!("Enter 'q' to quit...");
    let mut stdin = async_std::io::stdin();
    let mut input = [0_u8];
    loop {
        select!(
            query = queryable.recv_async() => {
                let query = query.unwrap();
                match query.value() {
                    None => println!(">> [Queryable ] Received Query '{}'", query.selector()),
                    Some(value) => println!(">> [Queryable ] Received Query '{}' with value '{}'", query.selector(), value),
                }
                query
                    .reply(Ok(Sample::new(key_expr.clone(), value.clone())))
                    .res()
                    .await
                    .unwrap_or_else(|e| println!(">> [Queryable ] Error sending reply: {e}"));
            },

            _ = stdin.read_exact(&mut input).fuse() => {
                match input[0] {
                    b'q' => break,
                    0 => sleep(Duration::from_secs(1)).await,
                    _ => (),
                }
            }
        );
    }
}
