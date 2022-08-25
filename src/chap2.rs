use std::thread::spawn;

fn hello() {
    println!("Hello World!");
}

fn my_func1() {
    spawn(hello).join();

    let h = || println!("Hello World!");
    spawn(h).join();
}

fn my_func2() {
    let v = 10;
    let f = move || v * 2;

    let result = spawn(f).join();
    println!("result = {:?}", result);

    match spawn(|| panic!("I'm paniced!")).join() {
        Ok(_) => {
            println!("successed");
        }
        Err(a) => {
            let s = a.downcast_ref::<&str>();
            println!("failed: {:?}", s)
        }
    }
}

fn main() {
    my_func2();
}
