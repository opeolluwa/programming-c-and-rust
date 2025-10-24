fn main() {
    let age = 45;
    by_reference(&age);

    by_value(age);
}

fn by_reference(x: &i32) {
    println!("{}", x); // immutable borrow
}

fn by_value(x: &mut i32) {
    *x += 1; // mutable borrow
}
