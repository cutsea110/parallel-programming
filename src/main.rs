use std::thread::spawn;

fn hello() {
    println!("Hello World!");
}

fn main() {
    spawn(hello).join();

    let h = || println!("Hello World!");
    spawn(h).join();
}
